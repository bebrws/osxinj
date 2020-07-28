#ifndef _INJECTOR_H_
#define _INJECTOR_H_


#include <mach-o/dyld.h>


#include "mach_inject.h"

class Injector
{
public:
    Injector();
    ~Injector();

    void inject(pid_t pid, const char* lib);
    pid_t getProcessByName(const char *name);
private:
    NSModule module;
//    void *bootstrapfn;
    void (*bootstrapfn)(ptrdiff_t, void *, size_t, void *);
};

#endif
