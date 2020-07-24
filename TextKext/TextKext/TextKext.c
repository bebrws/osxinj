//
//  TextKext.c
//  TextKext
//
//  Created by Bradley Barrows on 7/23/20.
//  Copyright © 2020 Bradley Barrows. All rights reserved.
//

#include <mach/mach_types.h>

#include <libkern/libkern.h>
#include <mach/mach_types.h>
#include<IOKit/IOLib.h>

kern_return_t TextKext_start(kmod_info_t * ki, void *d);
kern_return_t TextKext_stop(kmod_info_t *ki, void *d);

kern_return_t TextKext_start(kmod_info_t * ki, void *d)
{
    
    IOLog("BRAD in start\n");
    printf("BRAD in start\n");
    return KERN_SUCCESS;
}

kern_return_t TextKext_stop(kmod_info_t *ki, void *d)
{
    
    IOLog("BRAD in stop\n");
    printf("BRAD in stop\n");
    return KERN_SUCCESS;
}
