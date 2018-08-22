//
//  AGIPlatformIOS.mm
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/3.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#import "AGIPlatformIOS.h"
#import "AGIResourceManager.h"

@implementation AGIPlatformIOS

+ (void)load {
    NSString *resourcePath = [[NSBundle bundleForClass:self] resourcePath];
    const char *bundlePath = [resourcePath cStringUsingEncoding:NSUTF8StringEncoding];
    AGIResourceManager::sharedBundle()->setBundlePath(bundlePath);
}

@end
