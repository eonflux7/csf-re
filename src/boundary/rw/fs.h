#pragma once
#include "life.h"

namespace csf_rw {
// From 007c67d0/006a0020/0069fc00, not SDK declarations. Callback tables
// are supplied by the game. Native tests widen pointers; x86 offsets below.
struct FileSystem;
struct File {
    uint32_t length[2], position[2], reserved16[2];
    int async, operationState, status, reserved36, operation, transferred;
    int (__cdecl *callback)(File*, int, int, int, void*);
    void* callbackData;
    FileSystem* owner;
    uint32_t reserved60;
};
struct FileSystem {
    FileSystem* next;
    int count, kind;
    char* device;
    int async;
    File* (__cdecl *getFile)(FileSystem*, uint32_t);
    void (__cdecl *destroy)(FileSystem*);
    void* reserved28;
    int (__cdecl *registered)(const char*);
    int (__cdecl *unregistered)(const char*);
    int (__cdecl *open)(FileSystem*, File*, const char*, int);
    int (__cdecl *close)(File*);
    uint32_t (__cdecl *read)(File*, void*, uint32_t);
    uint32_t (__cdecl *write)(File*, const void*, uint32_t);
    uint32_t (__cdecl *seek)(File*, int32_t, int);
    int (__cdecl *sync)(File*, int);
    int (__cdecl *abort)(File*);
    int (__cdecl *eof)(File*);
    int (__cdecl *status)(File*);
    int (__cdecl *exists)(FileSystem*, const char*);
    char name[8];
};
#if UINTPTR_MAX == 0xffffffff
static_assert(sizeof(File)==64 && offsetof(File,owner)==56 && offsetof(File,callback)==48);
static_assert(sizeof(FileSystem)==88 && offsetof(FileSystem,name)==80);
static_assert(offsetof(FileSystem,open)==40 && offsetof(FileSystem,exists)==76);
#endif
bool fileManagerActive();
// Unlike librw StreamFile, this stream keeps its dispatch choice for its entire
// lifetime (including streams opened before manager initialization).
rw::Stream* openFileStream(const char*, const char*);
}
