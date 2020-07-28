#include <cstdio>
#include <cstring>

#include <mach/mach.h>
#include <mach/thread_act.h>
#include <mach/mach_init.h>
#include <pthread.h>
#include <dlfcn.h>
#include <signal.h>
#include <sys/proc_info.h>
#include <libproc.h>
#include <sys/types.h>
#include <mach/error.h>
#include <mach/vm_types.h>
#include <stddef.h>

#define DLLEXPORT __attribute__((visibility("default")))

#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_12
#define PTHREAD_SET_SELF _pthread_set_self
#else
#define PTHREAD_SET_SELF __pthread_set_self
#endif

extern "C" int verify();
extern "C" void PTHREAD_SET_SELF(void*);

extern "C" void bootstrap(ptrdiff_t offset, void *param, size_t psize, void *dummy) DLLEXPORT;

void *loaderThread(void *patch_bundle)
{
    void *bundle = dlopen((char *)patch_bundle, RTLD_NOW);
    if (!bundle)
        fprintf(stderr, "Could not load patch bundle: %s\n", dlerror());
    return 0;
}

int verify() {
    printf("bootstrap laoded\n");
    return 42;
}

void bootstrap(ptrdiff_t offset, void *param, size_t psize, void *dummy)
{
    PTHREAD_SET_SELF(dummy);

    pthread_attr_t attr;
    pthread_attr_init(&attr); 
    
    int policy;
    pthread_attr_getschedpolicy(&attr, &policy);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    
    struct sched_param sched;
    sched.sched_priority = sched_get_priority_max(policy);
    pthread_attr_setschedparam(&attr, &sched);

    pthread_t thread;
    pthread_create(&thread, &attr,
            (void * (*)(void *))((long)loaderThread),
            (void *)param);
    pthread_attr_destroy(&attr);
    
    thread_suspend(mach_thread_self());
}
