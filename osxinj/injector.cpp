#include "injector.h"

#include <sys/mman.h>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>


#include <mach-o/dyld.h>

#include <sys/types.h>

#include <sys/mman.h>

#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#include <cstdio>
#include <cstring>

#include <stdio.h>
#include <string.h>
#include <mach-o/dyld.h>
#include <alloca.h>

#include <mach-o/loader.h>
#include <mach/mach.h>
#include <mach/thread_act.h>
#include <mach/mach_init.h>
#include <pthread.h>
#include <dlfcn.h>
#include <signal.h>
#include <sys/proc_info.h>
#include <sys/fcntl.h>
#include <libproc.h>


#include "mach_inject.h"


//void *dlopen_binary_in_memory(const void *blob, size_t len, int mode) {
//    /* Create shared-memory file descriptor */
//    // int fd = shm_open(SHM_ANON, O_RDWR, 0);
//    // int fd = shm_open(MAP_ANON, O_RDWR, 0);
//    // https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/shm_open.2.html
//    int fd = shm_open("bootstrap.dylib", O_RDWR, 0);
//    ftruncate(fd, len);
//    /* MemMap file descriptor, and load data */
//    void *mem = mmap(NULL, len, PROT_WRITE, MAP_SHARED, fd, 0);
//    memcpy(mem, blob, len);
//    munmap(mem, len);
//    /* Open Dynamic Library from SHM file descriptor */
//    void *so = fdlopen(fd,mode);
//    close(fd);
//    return so;
//}


#include "bootstrap.bundle.h"
#include "bootstrap.dylib.h"

Injector::Injector() : module(0), bootstrapfn(0)
{
    
    printf("Loading bootstrapBundle from memory. Size: %d\n", sizeof(bootstrapBundle));
    NSObjectFileImage *ofi;
    NSCreateObjectFileImageFromMemory(bootstrapBundle, sizeof(bootstrapBundle), ofi);
    
    // https://opensource.apple.com/source/dyld/dyld-433.5/include/mach-o/dyld.h.auto.html
    // NSModule NSLinkModule(NSObjectFileImage objectFileImage, const char* moduleName, uint32_t options)
    module = NSLinkModule(*ofi, "bootstrapBundle", NSLINKMODULE_OPTION_BINDNOW | NSLINKMODULE_OPTION_PRIVATE);
    //#define NSLINKMODULE_OPTION_NONE                         0x0
    //#define NSLINKMODULE_OPTION_BINDNOW                      0x1
    //#define NSLINKMODULE_OPTION_PRIVATE                      0x2
    //#define NSLINKMODULE_OPTION_RETURN_ON_ERROR              0x4
    //#define NSLINKMODULE_OPTION_DONT_CALL_MOD_INIT_ROUTINES  0x8
    //#define NSLINKMODULE_OPTION_TRAILING_PHYS_NAME          0x10
    
//    module = dlopen_binary_in_memory(bootstrap_dylib, sizeof(bootstrap_dylib), RTLD_NOW | RTLD_LOCAL);
    
    // module = dlopen("bootstrap.dylib", RTLD_NOW | RTLD_LOCAL);
    
    if (!module)
    {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        return;
    }
    
    printf("Linked module: %p\n", module);
    
    
    NSSymbol symVerify = NSLookupSymbolInModule(module, "_verify");
    if (symVerify) {
        int (*verifyfn)() = (int (*)())NSAddressOfSymbol(symVerify);
        int vr = verifyfn();
        printf("Verify function called. Value: %d\nIf value was 42 then bootstrap is loaded successfully!\n", vr);
    }
    
    NSSymbol sym = NSLookupSymbolInModule(module, "_bootstrap");
    
    if (!sym) {
        fprintf(stderr, "Error: symbol for bootstrap in loaded module is: %d\n", sym);
        return;
    }
    
    printf("Loaded bootstrap symbol: %d\n", sym);
    
//    bootstrapfn = NSAddressOfSymbol(sym);
    bootstrapfn = (void (*)(ptrdiff_t, void *, size_t, void *))NSAddressOfSymbol(sym);

//    bootstrapfn = dlsym(module, "bootstrap");
    printf("bootstrapfn: 0x%X\n", bootstrapfn);

    if (!bootstrapfn)
    {
        fprintf(stderr, "could not locate bootstrap fn\n");
        return;
    }
}

Injector::~Injector()
{
    if (module)
    {
        dlclose(module);
        module = NULL;
    }
}

void Injector::inject(pid_t pid, const char* lib)
{
    if (!module || !bootstrapfn)
    {
        fprintf(stderr, "failed to inject: module:0x%X bootstrapfn:0x%X\n", module, bootstrapfn);
        return;
    }
    printf("Calling mach_inject with bootstrapfn: %p\n", bootstrapfn);
    mach_error_t err = mach_inject((mach_inject_entry)bootstrapfn, lib, strlen(lib) + 1, pid, 0);
}

pid_t Injector::getProcessByName(const char *name)
{
    int procCnt = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
    pid_t pids[1024];
    memset(pids, 0, sizeof pids);
    proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));

    for (int i = 0; i < procCnt; i++)
    {
        if (!pids[i]) continue;
        char curPath[PROC_PIDPATHINFO_MAXSIZE];
        char curName[PROC_PIDPATHINFO_MAXSIZE];
        memset(curPath, 0, sizeof curPath);
        proc_pidpath(pids[i], curPath, sizeof curPath);
        int len = strlen(curPath);
        if (len)
        {
            int pos = len;
            while (pos && curPath[pos] != '/') --pos;
            strcpy(curName, curPath + pos + 1);
            if (!strcmp(curName, name))
            {
                return pids[i];
            }
        }
    }
    return 0;
}
