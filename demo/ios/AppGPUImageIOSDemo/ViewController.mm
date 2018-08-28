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
    const char *urlString = [[url absoluteString] cStringUsingEncoding:NSUTF8StringEncoding];
    self.sdkTest->test_playerEngine(urlString);
}

@end
