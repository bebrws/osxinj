//
//  main.m
//  testframework
//
//  Created by Bradley Barrows on 7/23/20.
//  Copyright © 2020 Bradley Barrows. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <AppKit/AppKit.h>
#import "MARTNSObject.h"
#import "RTProtocol.h"
#import "RTIvar.h"
#import "RTProperty.h"
#import "RTMethod.h"
#import "RTUnregisteredClass.h"


void install(void) __attribute__ ((constructor));

void install()
{
    printf("INITT\n");
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 5 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        NSMutableArray * tree = [NSMutableArray array];
        NSApplication *app = [NSApplication sharedApplication];
//        do {
            for (NSWindow * window in app.windows) {
                printf("Window title: %s\n", [window.title cString]);
                NSArray *ivars = [[window class] rt_ivars];
                NSArray *methods = [[window class] rt_methods];
            }
            sleep(0.5);
            printf("\nNo app yet\n");
//        } while(!(app && (app.mainWindow || app.keyWindow)));
    });

}
