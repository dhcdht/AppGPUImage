//
//  AGIPiplineInputFFmpegReader.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/26.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIPiplineInputFFmpegReader.h"
#include "util/AGIUtil.h"


AGIPiplineInputFFmpegReader::AGIPiplineInputFFmpegReader()
    : m_filePath("")
    , m_avFormatContext(nullptr)
    , m_avCodecContext(nullptr)
    , m_avStream(nullptr)
    , m_avCodec(nullptr)
	, m_curFrameImage(nullptr)
    , m_curFramePts{0}
    , m_curFrameDuration{0}
{

}

AGIPiplineInputFFmpegReader::~AGIPiplineInputFFmpegReader()
{
    if (!m_filePath.empty()) {
        m_filePath = "";
    }
    if (m_avFormatContext != nullptr) {
        avformat_free_context(m_avFormatContext);
        m_avFormatContext = nullptr;
    }
    if (m_avCodecContext != nullptr) {
        avcodec_free_context(&m_avCodecContext);
        m_avCodecContext = nullptr;
    }
    if (m_avStream != nullptr) {
        m_avStream = nullptr;
    }
    if (m_avCodec != nullptr) {
        m_avCodec = nullptr;
    }
	m_curFrameImage = nullptr;
    m_curFramePts = Milliseconds(0);
    m_curFrameDuration = Milliseconds(0);
}

bool AGIPiplineInputFFmpegReader::init(const std::string filePath)
{
    if (filePath.empty()) {
        return false;
    }
    m_filePath = filePath;

    int ret = 0;
    ret = avformat_open_input(&m_avFormatContext, m_filePath.c_str(), nullptr, nullptr);
    if (ret < 0 || m_avFormatContext == nullptr) {
        return false;
    }
    GuardDefer deferAVFormatContext = {[&](){
        avformat_free_context(m_avFormatContext);
        m_avFormatContext = nullptr;
    }};
    ret = avformat_find_stream_info(m_avFormatContext, nullptr);
    if (ret < 0) {
        return false;
    }
    ret = av_find_best_stream(m_avFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &m_avCodec, 0);
    if (ret < 0 || m_avCodec == nullptr) {
        return false;
    }
    GuardDefer deferAVCodec = {[&](){
        m_avCodec = nullptr;
    }};
    auto bestStreamIndex = ret;
    if (bestStreamIndex >= m_avFormatContext->nb_streams) {
        return false;
    }
    m_avStream = m_avFormatContext->streams[bestStreamIndex];
    m_avCodecContext = avcodec_alloc_context3(m_avCodec);
    if (m_avCodecContext == nullptr) {
        return false;
    }
    GuardDefer deferAVCodecContext = {[&](){
        avcodec_free_context(&m_avCodecContext);
        m_avCodecContext = nullptr;
        m_avStream = nullptr;
    }};
    ret = avcodec_parameters_to_context(m_avCodecContext, m_avStream->codecpar);
    if (ret < 0) {
        return false;
    }
    ret = avcodec_open2(m_avCodecContext, m_avCodec, nullptr);
    if (ret < 0) {
        return false;
    }

    deferAVFormatContext.cancel();
    deferAVCodec.cancel();
    deferAVCodecContext.cancel();
    return true;
}

Milliseconds AGIPiplineInputFFmpegReader::getDuration()
{
    auto milliseconds = m_avFormatContext->duration / (AV_TIME_BASE / 1000);
    return Milliseconds(milliseconds);
}

int AGIPiplineInputFFmpegReader::getPreferFrameRate()
{
    auto rational = av_guess_frame_rate(m_avFormatContext, m_avStream, nullptr);
    int frameRate = round(av_q2d(rational));
    if (frameRate <= 1)
    {
        return 30;
    }

    return frameRate;
}


bool AGIPiplineInputFFmpegReader::syncSeekToTime(Milliseconds time)
{
    int ret = av_seek_frame(m_avFormatContext, m_avStream->index, time.count() * 1000, AVSEEK_FLAG_ANY);
    if (ret < 0)
    {
        return false;
    }

    return true;
}

Milliseconds AGIPiplineInputFFmpegReader::getCurrentFrameTime()
{
    return m_curFramePts;
}

Milliseconds AGIPiplineInputFFmpegReader::getCurrentFrameDuration()
{
    return m_curFrameDuration;
}

std::vector<AGIImagePtr> AGIPiplineInputFFmpegReader::pullOutputs()
{
	if (m_curFrameImage != nullptr)
	{
		return { m_curFrameImage };
	}

    AVFrame *receivedFrame = av_frame_alloc();
    if (receivedFrame == nullptr) {
        return {};
    }
    CleanDefer([&](){
        av_frame_free(&receivedFrame);
    });

    bool shouldDoAgain = false;
    do {
        shouldDoAgain = false;

        AVPacket *avPacket = av_packet_alloc();
        CleanDefer([&](){
            av_packet_free(&avPacket);
        });
        int ret;
        ret = av_read_frame(m_avFormatContext, avPacket);
        if (ret < 0) {
            if (ret == AVERROR(EAGAIN)) {
                shouldDoAgain = true;
                continue;
            }
            return {};
        }
        if (avPacket->stream_index != m_avStream->index) {
            shouldDoAgain = true;
            continue;
        }
        ret = avcodec_send_packet(m_avCodecContext, avPacket);
        if (ret < 0) {
            if (ret == AVERROR(EAGAIN)) {
                shouldDoAgain = true;
                continue;
            }
            return {};
        }
        ret = avcodec_receive_frame(m_avCodecContext, receivedFrame);
        if (ret < 0) {
            if (ret == AVERROR(EAGAIN)) {
                shouldDoAgain = true;
                continue;
            }
            return {};
        }
    } while (shouldDoAgain);
    if (receivedFrame == nullptr) {
        return {};
    }

    AVFrame *frameRGB = av_frame_alloc();
    CleanDefer([&](){
		// FIXME: why I need freep?
		av_freep(frameRGB->data);
        av_frame_free(&frameRGB);
    });
    int ret = this->convertFrameToRGBFrame(receivedFrame, frameRGB);
    if (ret) {
        return {};
    }
    auto mem = bgfx::copy(((char*)frameRGB->data[0]), (frameRGB->linesize[0] * frameRGB->height));
    auto textureHandle = bgfx::createTexture2D(frameRGB->width, frameRGB->height, false, 1, bgfx::TextureFormat::Enum::RGBA8, BGFX_TEXTURE_NONE, mem);

	m_curFrameImage = std::make_shared<AGIImage>(
		textureHandle, frameRGB->width, frameRGB->height, bgfx::TextureFormat::Enum::RGBA8
		);
    auto ptsMilliSeconds = av_rescale_q(receivedFrame->pts, m_avStream->time_base, av_make_q(1, 1000));
    m_curFramePts = Milliseconds(ptsMilliSeconds);
    auto frameDurationMilliSeconds = av_rescale_q(receivedFrame->pkt_duration, m_avStream->time_base, av_make_q(1, 1000));
    m_curFrameDuration = Milliseconds(frameDurationMilliSeconds);

	return { m_curFrameImage };
}

void AGIPiplineInputFFmpegReader::endOneProcess()
{
	m_curFrameImage = nullptr;

    AGIPiplineSource::endOneProcess();
}

int AGIPiplineInputFFmpegReader::convertFrameToRGBFrame(AVFrame *frame, AVFrame *frameRGB)
{
    int ret = 0;
    frameRGB->format = AV_PIX_FMT_RGBA;
    frameRGB->width = frame->width;
    frameRGB->height = frame->height;

	ret = av_image_alloc(frameRGB->data, frameRGB->linesize, frameRGB->width, frameRGB->height, (AVPixelFormat)frameRGB->format, 1);
    if (ret < 0) {
        return ret;
    }

    auto swsContext = sws_getContext(frame->width, frame->height, (AVPixelFormat)frame->format, frameRGB->width, frameRGB->height, (AVPixelFormat)frameRGB->format, SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
    CleanDefer([&](){
        sws_freeContext(swsContext);
    });
    ret = sws_scale(swsContext, frame->data, frame->linesize, 0, frame->height, frameRGB->data, frameRGB->linesize);
    if (ret < 0) {
        return ret;
    }

    return 0;
}

