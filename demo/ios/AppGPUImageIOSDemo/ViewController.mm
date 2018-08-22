//
//  ViewController.m
//  AppGPUImageIOSDemo
//
//  Created by 董宏昌 on 2018/6/26.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#import "ViewController.h"
#import <AppGPUImage/AppGPUImage.h>
#import <AVFoundation/AVFoundation.h>

@interface ViewController ()

@property (nonatomic) CAMetalLayer *eaglLayer;
@property (nonatomic) SDKTest *sdkTest;

@property (nonatomic) AVAsset *asset;
@property (nonatomic) AVAssetReader *assetReader;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

    self.eaglLayer = [[CAMetalLayer alloc] init];
    [[self.view layer] addSublayer:self.eaglLayer];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];

    if (self.sdkTest == nil) {
        self.eaglLayer.frame = self.view.layer.bounds;

        CAMetalLayer *window = self.eaglLayer;
        void *voidWindow = (__bridge void*)window;
        int sizeWidth = [window frame].size.width;
        int sizeHeight = [window frame].size.height;
        self.sdkTest = new SDKTest();

        self.sdkTest->init(voidWindow, sizeWidth, sizeHeight);
    }

//    NSString *path = [[NSBundle mainBundle] pathForResource:@"parallax-d" ofType:@"png"];
//    const char *pathCStr = [path cStringUsingEncoding:NSUTF8StringEncoding];
//    self.sdkTest->test_graph(pathCStr);
//
////    [self test_blend];
//
//    UIImage *image = [UIImage imageNamed:@"SplashScreen"];
//    NSData *imgData = UIImagePNGRepresentation(image);
//    self.sdkTest->test_textureFromData(imgData.bytes, (uint32_t)imgData.length);
//
//    [self test_movie];
    
    NSURL *url = [[NSBundle mainBundle] URLForResource:@"big_buck_bunny" withExtension:@"mp4"];
    self.sdkTest->test_openMovieFile([[url absoluteString] cStringUsingEncoding:NSUTF8StringEncoding]);
//    [self test_playMovieNextFrame];
}

//- (void)test_blend {
//    NSString *path = [[NSBundle mainBundle] pathForResource:@"parallax-d" ofType:@"png"];
//    const char *pathCStr = [path cStringUsingEncoding:NSUTF8StringEncoding];
//
//    NSString *path1 = [[NSBundle mainBundle] pathForResource:@"SplashScreen" ofType:@"png"];
//    const char *pathCStr1 = [path1 cStringUsingEncoding:NSUTF8StringEncoding];
//
//    self.sdkTest->test_blend(pathCStr, pathCStr1);
//
//    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.3 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//        [self test_blend];
//    });
//}
//
//- (void)test_movie {
//    NSURL *url = [[NSBundle mainBundle] URLForResource:@"big_buck_bunny" withExtension:@"mp4"];
//    self.asset = [AVAsset assetWithURL:url];
//    NSError *error = nil;
//    self.assetReader = [AVAssetReader assetReaderWithAsset:self.asset error:&error];
//    AVAssetTrack *videoTrack = [[self.asset tracksWithMediaType:AVMediaTypeVideo] objectAtIndex:0];
//
//    NSMutableDictionary *outputSettings = [NSMutableDictionary dictionary];
//    [outputSettings setObject:@(kCVPixelFormatType_32BGRA) forKey:(id)kCVPixelBufferPixelFormatTypeKey];
//    AVAssetReaderTrackOutput *readerVideoTrackOutput = [AVAssetReaderTrackOutput assetReaderTrackOutputWithTrack:videoTrack outputSettings:outputSettings];
//    readerVideoTrackOutput.alwaysCopiesSampleData = NO;
//    [self.assetReader addOutput:readerVideoTrackOutput];
//
//    [self.assetReader startReading];
//
//    [self test_movie_next:readerVideoTrackOutput];
//}
//
//- (void)test_movie_next:(AVAssetReaderTrackOutput*)readerVideoTrackOutput {
//    if (self.assetReader.status == AVAssetReaderStatusReading
//        || self.assetReader.status == AVAssetReaderStatusCompleted) {
//        CMSampleBufferRef sampleBuffer = [readerVideoTrackOutput copyNextSampleBuffer];
//        CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
//        CVPixelBufferLockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
//
//        void *data = CVPixelBufferGetBaseAddress(imageBuffer);
//        size_t size = CVPixelBufferGetDataSize(imageBuffer);
//
//        if (data == nullptr && self.assetReader.status == AVAssetReaderStatusCompleted) {
//            return;
//        }
//
//        if (data != nullptr) {
//            size_t inputSize = size-8;
//            void *inputData = malloc(inputSize);
//            memcpy(inputData, data, inputSize);
//
//            NSString *path1 = [[NSBundle mainBundle] pathForResource:@"SplashScreen" ofType:@"png"];
//            const char *pathCStr1 = [path1 cStringUsingEncoding:NSUTF8StringEncoding];
//            self.sdkTest->test_movie(inputData, (uint32_t)inputSize, pathCStr1);
//
//            free(inputData);
//        }
//        CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
//        CFRelease(sampleBuffer);
//    }
//
//    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1/60.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//        [self test_movie_next:readerVideoTrackOutput];
//    });
//}
//
//- (void)test_playMovieNextFrame {
//    self.sdkTest->test_playMovieNextFrame();
//    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1/60.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//        [self test_playMovieNextFrame];
//    });
//}

@end
