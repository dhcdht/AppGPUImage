//
//  AGIPiplineInputFFmpegReader.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/26.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPiplineInputFFmpegReader_h
#define AGIPiplineInputFFmpegReader_h

#include "IO/input/AGIPiplineInput.h"
#include <bgfx/bgfx.h>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
#include "libavutil/error.h"
#include "libavutil/imgutils.h"
}


class AGIPiplineInputFFmpegReader : public AGIPiplineInput {
public:
    AGIPiplineInputFFmpegReader();
    ~AGIPiplineInputFFmpegReader() override;

    // begin of AGIPiplineInput
public:
    bool init(const std::string filePath) override;
    Milliseconds getDuration() override;
    int getPreferFrameRate() override;
    bool syncSeekToTime(Milliseconds time) override;
    Milliseconds getCurrentTime() override;
    // end of AGIPiplineInput

    // begin of AGIPiplineSource
public:
    int getSourceOutputCount() override { return 1; };
    std::vector<AGIImagePtr> pullOutputs() override;
    void endOneProcess() override;
    // end of AGIPiplineSource

private:
    int convertFrameToRGBFrame(AVFrame *frame, AVFrame *RGBFrame);

private:
    std::string m_filePath;
    AVFormatContext *m_avFormatContext;
    AVCodecContext *m_avCodecContext;
    AVStream *m_avStream;
    AVCodec *m_avCodec;

	AGIImagePtr m_curFrameImage;
    Milliseconds m_curFramePts;
};
typedef std::shared_ptr<AGIPiplineInputFFmpegReader> AGIPiplineInputFFmpegReaderPtr;


#endif /* AGIPiplineInputFFmpegReader_h */
