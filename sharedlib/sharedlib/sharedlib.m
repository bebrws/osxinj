//
//  sharedlib.m
//  sharedlib
//
//  Created by Bradley Barrows on 7/23/20.
//  Copyright © 2020 Bradley Barrows. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <AppKit/AppKit.h>


#import <objc/runtime.h>

#import "KZRMethodSwizzlingWithBlock.h"

#import "sharedlib.h"



@interface ViewControllerBrad : NSViewController

@property (strong) IBOutlet NSView *mainView;

- (NSString *) getString;

@end


@interface NSResponder (MouseOverAll)

- (void)rightMouseDown:(NSEvent *)event;


@end

@implementation NSResponder (MouseOverAll)

- (void)rightMouseDown:(NSEvent *)event {
    // reinterpret mouseDown + control as a right click
    if([event modifierFlags] & NSControlKeyMask) {
        printf("WAS!! a ctrl click\n");
    } else {
        printf("NOT a ctrl click\n");
    }
    
}

@end


@implementation sharedlib

NSDictionary *getClassEverything(Class objectClass) {
//    Class objectClass = [self class];
    u_int count;

    Ivar *classIvars = class_copyIvarList(objectClass, &count);

    NSMutableArray *ivarList = [NSMutableArray arrayWithCapacity:count];

    for (int i = 0; i < count; i++) {
      const char *ivarName = ivar_getName(classIvars[i]);

      [ivarList
          addObject:[NSString stringWithCString:ivarName
                                       encoding:NSUTF8StringEncoding]];
    }

    free(classIvars);

    objc_property_t *classProperties =
        class_copyPropertyList(objectClass, &count);

    NSMutableArray *propertyList =
        [NSMutableArray arrayWithCapacity:count];

    for (int i = 0; i < count; i++) {
      const char *propertyName = property_getName(classProperties[i]);

      [propertyList
          addObject:[NSString stringWithCString:propertyName
                                       encoding:NSUTF8StringEncoding]];
    }

    free(classProperties);

    Method *classMethods = class_copyMethodList(objectClass, &count);

    NSMutableArray *methodList = [NSMutableArray arrayWithCapacity:count];

    for (int i = 0; i < count; i++) {
      SEL methodNameSelector = method_getName(classMethods[i]);
      const char *methodName = sel_getName(methodNameSelector);

      [methodList
          addObject:[NSString stringWithCString:methodName
                                       encoding:NSUTF8StringEncoding]];
    }

    free(classMethods);
    NSString *logString = [NSString
            stringWithFormat:
                @"Class: %@ \n Ivars: %@ \n Properties: %@ \n Methods: %@",
                NSStringFromClass(objectClass), ivarList, propertyList,
                methodList];
    printf("Class introspection: %s\n", [logString cString]);
//    return [NSString
//        stringWithFormat:
//            @"Class: %@ \n Ivars: %@ \n Properties: %@ \n Methods: %@",
//            NSStringFromClass([self class]), ivarList, propertyList,
//            methodList];
    return @{
        @"className": NSStringFromClass(objectClass),
        @"ivars": ivarList,
        @"propertyList": propertyList,
        @"methodList": methodList
    };
}

void install(void) __attribute__((constructor));

void install() {
  printf("INITT\n");
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.2 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        printf("ABOUT TO SWIZZLE\n");
        NSMutableArray *tree = [NSMutableArray array];
        NSApplication *app = [NSApplication sharedApplication];


      
      KZRMETHOD_ADDING_("BBView", "NSView", "rightMouseDown:",
       void, call_super, sel)
      ^ (id slf, NSEvent *event){
          call_super(slf, sel, event); //call -[NSTableCellView setObjectValue:]
          printf("\n\n RIGHT CLICK METHOD!!!n\n\n");
      }_WITHBLOCK_ADD;
      
//
//      KZRMETHOD_ADDING_("BBView", "NSView", "mouseDown:",
//       void, call_super, sel)
//      ^void (NSEvent* slf, id value){
//          call_super(slf, sel, value); //call -[NSTableCellView setObjectValue:]
//          printf("\n\n Regular CLICK METHOD!!!n\n\n");
//      }_WITHBLOCK_ADD;
      KZRMETHOD_SWIZZLING_("BBView", "mouseDown:",
          void, originalMethod, originalSelector)
          ^ (id slf, NSEvent *event){  // SEL is not brought (id self, arg1, arg2...)
              printf("\n\n Reg Ccick HOOK METHOD!!!n\n\n");
              originalMethod(slf, originalSelector, event);
      }_WITHBLOCK;

      
        KZRMETHOD_SWIZZLING_("ViewControllerBrad", "getString",
            NSString *, originalGetStringMethod, originalGetStringSelector)
            ^id (id slf){  // SEL is not brought (id self, arg1, arg2...)
                printf("\n\n MADE IT INT HOOKE METHOD!!!n\n\n");
                NSString *og = originalGetStringMethod(slf, originalGetStringSelector);
              
                return [NSString stringWithFormat:@"This is HOOOOKEEED. Original: %@.", og];
        }_WITHBLOCK;
      
        sleep(0.5);
        printf("\nNo app yet\n");
      });
}

@end
