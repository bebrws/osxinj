CaptainHook usage
Basic Settings
#import <CaptainHook/CaptainHook.h> can be used, very convenient
usage
Declare a class: You must declare a class before you use CaptainHook. example:

# import  < CaptainHook/CaptainHook.h >  // Import the header file 
CHDeclareClass ( NSString ); // Declare the NSString class 
CHConstructor { // CHConstructor means that after the binary is loaded, the content of the 
  CHConstructor code block is executed immediately CHLoadLateClass ( NSString ); // Load NSString class 
}
Method hook:
First use CHMethod to declare a function, and then hook it with CHHook . example:

// Standard method hook 
# import  < CaptainHook/CaptainHook.h >  // Import header file 
CHDeclareClass ( NSString ); // Declare the class 
CHMethod ( 2 , void , NSString , writeToFile, NSString *, path, atomically, BOOL , flag) / / Declare a method to hook. Parameter 1: represents the two parameters equivalent to CHMethod2(), parameter 2: is the return type, parameter 3: which class of method is it, parameter 4: method name, parameter 5: parameter type, parameter 6: parameter type name, Parameter 7: method name of the second parameter, parameter 7: parameter type, parameter 8: parameter type name
{
    NSLog ( @" Writing string to %@ : %@ " , path, self);
     CHSuper ( 2 , NSString , writeToFile, path, atomically, flag); // Call the original implementation
}

CHConstructor //
{
    CHLoadClass ( NSString );
     CHHook ( 2 , NSString , writeToFile, atomically); // Hook method.
}

CHDeclareMethod hook

// Override, simple hook, unable to control the timing of hook, can also be used as an increase method 
# import  < CaptainHook/CaptainHook.h >
 CHDeclareClass ( NSString );
 CHDeclareMethod ( 2 , void , NSString , writeToFile, NSString *, path, atomically, BOOL , flag)
{
    NSLog(@"Writing string to %@: %@", path, self);
    CHSuper(2, NSString, writeToFile, path, atomically, flag);
}
Inject new classes at runtime:

// You can use the CHRegisterClass macro to create a new class at runtime: 
# import  < CaptainHook/CaptainHook.h >
 CHDeclareClass ( NSObject );
 CHDeclareClass (MyNewClass);
 CHMethod ( 0 , id , MyNewClass, init)
{
   if ((self = CHSuper(0, MyNewClass, init))) {
       NSLog(@"Initted MyNewClass");
   }
   return self;
}

CHConstructor
{
   CHAutoreleasePoolForScope();
   CHLoadClass(NSObject);
   CHRegisterClass(MyNewClass, NSObject) {
       CHHook(0, MyNewClass, init);
   } // Create a new class 
   [ CHAlloc (MyNewClass) autorelease ];
}
other

CHOptimizedMethod(): hook instance method

CHOptimizedClassMethod(): hook class method

CHPropertyRetainNonatomic() adds attributes

//增加属性
@interface ViewController : NSObject
   @property (nonatomic,retain)NSString *newProperty;
@end
   CHDeclareClass(ViewController)
   CHPropertyRetainNonatomic(ViewCOntroller,NSString*,newProperty,setNewProperty);

CHConstructor{
   CHLoadLateClass(ViewController);
   CHHook0(ViewController,newProperty);
   CHHook1(ViewController,setNewProperty);
}
   
Simple usage summary:
1. Import the #import <CaptainHook/CaptainHook.h> header file

2. Declare the class used: CHDeclareClass()

3. CHConstructor{} to write the loaded class (CHLoadLateClass())

4. CHMethod(), CHOptimizedMethod(), CHOptimizedClassMethod() declare new functions

5. CHConstructor () under the hook (CHHook (),)

6. CHSuper (): Call the original method to achieve.


