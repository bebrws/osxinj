//
//  ViewController.m
//  TestIncluder
//
//  Created by Bradley Barrows on 7/23/20.
//  Copyright © 2020 Bradley Barrows. All rights reserved.
//

#import "ViewControllerBrad.h"
#import <objc/runtime.h>

#import "JGMethodSwizzler.h"

#import <objc/objc-runtime.h>

@implementation BBView
- (void)mouseDown:(NSEvent *)event {
    printf("OG CLICK HANDLER\n");
}
//- (void)rightMouseDown:(NSEvent *)event {
//    printf("OG CLICK HANDLER\n");
//}

@end

@implementation ViewControllerBrad

- (void)mouseDown:(NSEvent *)e {
    printf("HOOOKKKOKOKED \nHOOKOE\n");
}



- (void)buttonPressed {
    printf("CLICKED\n");
    self.text.stringValue = [self getString];
}

- (void)viewDidLoad {
    [super viewDidLoad];

    
    CGRect someRect = CGRectMake(0.0, 0.0, 500.0, 30.0);
    self.text = [[NSTextField alloc] initWithFrame:someRect];
    self.text.stringValue = @"default";
    
    CGRect buttonFrame = CGRectMake(100.0, 100.0, 50.0, 50.0);
    NSButton *doSomething = [[NSButton alloc] initWithFrame:buttonFrame];
    
    [doSomething setTarget:self];
    [doSomething setAction:@selector(buttonPressed)];
    
    
    [self.mainView addSubview:self.text];
    [self.mainView addSubview:doSomething];
    
    
    Class clazz = NSClassFromString(@"BBView");
    
    [clazz swizzleInstanceMethod:@selector(mouseDown:) withReplacement:JGMethodReplacementProviderBlock {
        //return a replacement block
        return JGMethodReplacement(void, NSObject *, NSEvent *e) {
            printf("SWIZZLED IT\n");
            JGOriginalImplementation(void, e);
        };
    }];
//    SEL originalSelector = @selector(mouseDown:);
//    SEL newSelector = @selector(mouseDown:);
//    Method originalMethod = class_getInstanceMethod(bbviewClass, originalSelector);
//    Method newMethod = class_getInstanceMethod(bbviewClass, newSelector);
//    method_exchangeImplementations(originalMethod, newMethod);
    
    
    // Do any additional setup after loading the view.
//    text.stringValue = [self getString];

//  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 3.0 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
//      
//      text.stringValue = [self getString];
//  
//  });
//    
//    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 6.0 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
//        
//        text.stringValue = [self getString];
//    
//    });

}



- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (NSString *) getString {
    return @"Original";
}

@end
