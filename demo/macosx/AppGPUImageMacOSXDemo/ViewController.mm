//
//  ViewController.m
//  AppGPUImageMacOSX
//
//  Created by 董宏昌 on 2018/6/26.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#import "ViewController.h"
#import <AppGPUImage/AppGPUImage.h>
#import <AVFoundation/AVFoundation.h>

@interface ViewController()

@property (nonatomic) SDKTest *sdkTest;

@property (nonatomic) AVAsset *asset;
@property (nonatomic) AVAssetReader *assetReader;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
}

- (void)viewDidAppear {
    [super viewDidAppear];

    if (self.sdkTest == nil) {
        NSWindow *window = [[self view] window];
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
//    NSImage *image = [NSImage imageNamed:@"SplashScreen"];
//    NSBitmapImageRep *imageRep = (NSBitmapImageRep*)[image bestRepresentationForRect:NSZeroRect context:nil hints:nil];
//    NSData *imgData = [imageRep representationUsingType:NSPNGFileType properties:@{}];
//    self.sdkTest->test_textureFromData(imgData.bytes, (uint32_t)imgData.length);
//
//    [self test_movie];

    NSURL *url = [[NSBundle mainBundle] URLForResource:@"big_buck_bunny" withExtension:@"mp4"];
    const char *urlString = [[url absoluteString] cStringUsingEncoding:NSUTF8StringEncoding];
    self.sdkTest->test_playerEngine(urlString);
}

@end
