#ifdef CSF_STANDALONE
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include "globals.h"
#include "hook.h"
namespace {
static_assert(sizeof(void*)==4, "RTFS address ABI requires x86");
template<class T> T& at(void* p,unsigned n) { return *reinterpret_cast<T*>(static_cast<char*>(p)+n); }
CSF_GLOBAL(void*, head, 0x008BC7C8);
CSF_GLOBAL(int, registeredCount, 0x008BC7D0);
CSF_GLOBAL(int, capacity, 0x008BC7CC);
CSF_GLOBAL(void*, engine, 0x008B68C0);
static void* allocate(size_t n) {
    if(!engine) return nullptr;
    auto fn=at<void*(__cdecl*)(size_t,unsigned)>(engine,264);
    return fn?fn(n,0):nullptr;
}
static void deallocate(void* p) {
    if(p && engine) { auto fn=at<void(__cdecl*)(void*)>(engine,268); if(fn) fn(p); }
}
static void* __cdecl find_fs(const char* name) {
    if(name) for(void* p=head;p;p=at<void*>(p,0)) if(!std::strcmp(static_cast<char*>(p)+80,name)) return p;
    return nullptr;
}
static int __cdecl event(void* p,int which) {
    if(!p || (which!=1 && which!=2)) return 0;
    auto cb=at<int(__cdecl*)(const char*)>(p,which==1?32:36);
    return cb?cb(static_cast<char*>(p)+80):0;
}
static int __cdecl initialize_fs(void* p,int count,const char* name,const char* root) {
    if(!p || count<0 || !name || std::strlen(name)>7 || !root) return 0;
    // Custom FS blocks are 0x5c bytes: eight-byte name then entries pointer +0x58.
    std::strcpy(static_cast<char*>(p)+80,name); at<int>(p,4)=count; at<int>(p,8)=2;
    at<int>(p,16)=0; at<void*>(p,12)=nullptr;
    auto accessor=at<void*(__cdecl*)(void*,int)>(p,20);
    if(!accessor) return 0;
    for(int i=0;i<count;++i) { auto entry=accessor(p,i); if(!entry) return 0; at<int>(entry,32)=1; at<int>(entry,24)=0; }
    auto copy=static_cast<char*>(allocate(std::strlen(root)+1)); if(!copy) return 0;
    std::strcpy(copy,root); at<char*>(p,12)=copy; at<void*>(p,32)=at<void*>(p,36)=nullptr;
    return 1;
}
static int __cdecl register_fs(void* p) {
    if(!p || (capacity>0 && registeredCount>=capacity) || find_fs(static_cast<char*>(p)+80)) return 0;
    event(p,1); at<void*>(p,0)=head; head=p; ++registeredCount; return 1;
}
static void* __cdecl disk_entry(void* p,unsigned n) { return n<unsigned(at<int>(p,4))?static_cast<char*>(at<void*>(p,92))+96*n:nullptr; }
static int __cdecl disk_close(void* p) {
    auto h=at<HANDLE>(p,64); at<int>(p,32)=1; at<HANDLE>(p,64)=INVALID_HANDLE_VALUE;
    return h && h!=INVALID_HANDLE_VALUE ? CloseHandle(h):1;
}
static int __cdecl disk_destroy(void* p) {
    for(int i=0;i<at<int>(p,4);++i) { auto e=disk_entry(p,i); if(at<int>(e,32)!=1) disk_close(e); }
    deallocate(at<void*>(p,92)); at<void*>(p,92)=nullptr; return 1;
}
static const char* __cdecl disk_name(const char* name) { return name; }
static std::string disk_path(void* fs,const char* name) {
    // FS syntax is win:path, distinct from Windows drive syntax. The original
    // strips the first colon before joining the FS root and requested path.
    auto colon=std::strchr(name,':'); if(colon) name=colon+1;
    std::string path=at<const char*>(fs,12); path+=name;
    for(auto& c:path) if(c=='/') c='\\'; return path;
}
static int __cdecl disk_open(void* fs,void* record,const char* name,unsigned char flags) {
    if(!name || !record) return 3;
    auto path=disk_path(fs,name);
    DWORD access=(flags&1?GENERIC_READ:0)|(flags&2?GENERIC_WRITE:0);
    DWORD creation=flags&2?(flags&8?OPEN_ALWAYS:CREATE_ALWAYS):OPEN_EXISTING;
    auto h=CreateFileA(path.c_str(),access,FILE_SHARE_READ|FILE_SHARE_WRITE,nullptr,creation,FILE_ATTRIBUTE_NORMAL,nullptr);
    if(h==INVALID_HANDLE_VALUE) return 3;
    at<HANDLE>(record,64)=h; at<DWORD>(record,0)=GetFileSize(h,&at<DWORD>(record,4));
    at<DWORD>(record,8)=flags&8?SetFilePointer(h,0,nullptr,FILE_END):0;
    at<DWORD>(record,12)=0; at<int>(record,24)=0; // deliberately synchronous, even for async request
    at<int>(record,32)=2; at<int>(record,36)=1; return 1;
}
static unsigned __cdecl disk_read(void* record,void* bytes,unsigned size) {
    DWORD n=0; bool ok=ReadFile(at<HANDLE>(record,64),bytes,size,&n,nullptr)!=0;
    at<int>(record,32)=ok?2:4; uint64_t pos=uint64_t(at<DWORD>(record,8))+(uint64_t(at<DWORD>(record,12))<<32)+n;
    at<DWORD>(record,8)=DWORD(pos); at<DWORD>(record,12)=DWORD(pos>>32); return n;
}
static unsigned __cdecl disk_write(void* record,const void* bytes,unsigned size) {
    DWORD n=0; bool ok=WriteFile(at<HANDLE>(record,64),bytes,size,&n,nullptr)!=0;
    at<int>(record,32)=ok?2:4; uint64_t pos=uint64_t(at<DWORD>(record,8))+(uint64_t(at<DWORD>(record,12))<<32)+n;
    at<DWORD>(record,8)=DWORD(pos); at<DWORD>(record,12)=DWORD(pos>>32);
    at<DWORD>(record,0)=GetFileSize(at<HANDLE>(record,64),&at<DWORD>(record,4)); return n;
}
static DWORD __cdecl disk_seek(void* record,LONG distance,int origin) {
    DWORD method=origin==1?FILE_BEGIN:origin==2?FILE_CURRENT:FILE_END;
    DWORD pos=SetFilePointer(at<HANDLE>(record,64),distance,nullptr,method);
    at<DWORD>(record,8)=pos; at<DWORD>(record,12)=0; return pos;
}
static int __cdecl disk_sync(void* record,int) { return at<int>(record,32); }
static int __cdecl disk_abort(void* record) { return at<int>(record,32); }
static int __cdecl disk_eof(void* p) { return (uint64_t(at<DWORD>(p,8))+(uint64_t(at<DWORD>(p,12))<<32)) >= (uint64_t(at<DWORD>(p,0))+(uint64_t(at<DWORD>(p,4))<<32)); }
static int __cdecl disk_status(void* p) { return at<int>(p,32); }
static int __cdecl disk_exists(void* fs,const char* name) { return name && GetFileAttributesA(disk_path(fs,name).c_str())!=INVALID_FILE_ATTRIBUTES; }
static void* __cdecl disk_fs(int count,const char* root,const char* name) {
    if(count<0 || count>65536 || !root || !name || find_fs(name)) return nullptr;
    auto fs=allocate(96); if(!fs) return nullptr; std::memset(fs,0,96);
    auto records=allocate(size_t(count)*96); if(!records) { deallocate(fs); return nullptr; }
    std::memset(records,0,size_t(count)*96); at<void*>(fs,92)=records;
    at<void*(__cdecl*)(void*,unsigned)>(fs,20)=disk_entry; at<int(__cdecl*)(void*)>(fs,24)=disk_destroy;
    at<const char*(__cdecl*)(const char*)>(fs,28)=disk_name;
    at<int(__cdecl*)(void*,void*,const char*,unsigned char)>(fs,40)=disk_open;
    at<int(__cdecl*)(void*)>(fs,44)=disk_close;
    at<unsigned(__cdecl*)(void*,void*,unsigned)>(fs,48)=disk_read;
    at<unsigned(__cdecl*)(void*,const void*,unsigned)>(fs,52)=disk_write;
    at<DWORD(__cdecl*)(void*,LONG,int)>(fs,56)=disk_seek;
    at<int(__cdecl*)(void*,int)>(fs,60)=disk_sync;
    at<int(__cdecl*)(void*)>(fs,64)=disk_abort; at<int(__cdecl*)(void*)>(fs,68)=disk_eof;
    at<int(__cdecl*)(void*)>(fs,72)=disk_status; at<int(__cdecl*)(void*,const char*)>(fs,76)=disk_exists;
    for(int i=0;i<count;++i) at<void*>(static_cast<char*>(records)+96*i,56)=fs;
    if(initialize_fs(fs,count,name,root)!=1) { deallocate(records); deallocate(fs); return nullptr; }
    return fs;
}
}
HOOK(0x007C6040,find_fs,"boundary-rwa");
HOOK(0x007C6350,register_fs,"boundary-rwa");
HOOK(0x007C66D0,event,"boundary-rwa");
HOOK(0x007C67D0,initialize_fs,"boundary-rwa");
HOOK(0x007C6E90,disk_fs,"boundary-rwa");
#endif
