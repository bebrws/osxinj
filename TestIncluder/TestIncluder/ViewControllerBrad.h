//
//  ViewController.h
//  TestIncluder
//
//  Created by Bradley Barrows on 7/23/20.
//  Copyright © 2020 Bradley Barrows. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface BBView: NSView
    - (void)mouseDown:(NSEvent *)event;
    - (void)rightMouseDown:(NSEvent *)event;
@end


@interface ViewControllerBrad : NSViewController

@property (strong, atomic) NSTextField* text;
@property (strong) IBOutlet BBView *mainView;

- (NSString *) getString;

@end

