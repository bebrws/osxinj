//
//  main.m
//  TestIncluder
//
//  Created by Bradley Barrows on 7/23/20.
//  Copyright © 2020 Bradley Barrows. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <stdio.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
    }
    printf("Before NS APP Main start \n");
    return NSApplicationMain(argc, argv);
}
