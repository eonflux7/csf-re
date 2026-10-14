#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "fs.h"
#include "startup.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cerrno>
#include <mutex>
#include <new>
#include <string>
#include <filesystem>
#ifdef _WIN32
#include <windows.h>
#endif

using FS=csf_rw::FileSystem;
using F=csf_rw::File;
CSF_GLOBAL(FS*, defaultFS, 0x008BC7C4);
CSF_GLOBAL(FS*, head, 0x008BC7C8);
CSF_GLOBAL(int, maxSystems, 0x008BC7CC);
CSF_GLOBAL(int, numSystems, 0x008BC7D0);
CSF_GLOBAL(int, initialized, 0x008BC7D4);
CSF_GLOBAL(int, lastError, 0x008BC7D8);
using ErrorCallback=void (__cdecl *)(int);
CSF_GLOBAL(ErrorCallback, errorCallback, 0x008BC7DC);
#ifdef _WIN32
// 0050d1a0 inlines the original manager teardown and deletes THIS section.
CSF_GLOBAL(CRITICAL_SECTION, gameFSLock, 0x008BEB40);
#endif

namespace {
std::recursive_mutex managerMutex;
rw::FileFunctions savedFiles{};
bool fileFunctionsInstalled;
void error(int n) { if(errorCallback) errorCallback(n); }
void* alloc(size_t n) {
    void* p=rw::Engine::memfuncs.rwmalloc(n, rw::MEMDUR_GLOBAL);
    if(!p && csf_rw::startupTracing()) std::fprintf(stderr,"RW startup RtFS allocation bytes=%zu -> null\n",n);
    return p;
}
void release(void* p) { if(p) rw::Engine::memfuncs.rwfree(p); }
int __stdcall capacity() { if(maxSystems<=0 || numSystems<maxSystems) return 1; error(5); return 0; }
FS* __cdecl findName(const char* name) {
    if(name) for(FS* f=head;f;f=f->next) if(!std::strcmp(f->name,name)) {
        if(csf_rw::startupTracing()) std::fprintf(stderr,"RW startup RtFS find name=\"%s\" -> %p\n",name,static_cast<void*>(f));
        return f;
    }
    if(csf_rw::startupTracing()) std::fprintf(stderr,"RW startup RtFS find name=\"%s\" -> null\n",name?name:"(null)");
    error(6); return nullptr;
}
int __cdecl nameUsed(const char* name) { if(!findName(name)) return 0; error(7); return 1; }
FS** __stdcall list() { return &head; }
bool sameDevice(const char* a,const char* b) {
    // Engine +f0 is 0063d690 (installed at 0063d832): ASCII case-folded,
    // unlike the +e8 strcmp used for filesystem names.
    if(!a || !b) return true;
    for(;;++a,++b) {
        unsigned char x=static_cast<unsigned char>(*a),y=static_cast<unsigned char>(*b);
        if(x>='A' && x<='Z') x+=32;
        if(y>='A' && y<='Z') y+=32;
        if(x!=y) return false;
        if(!x) return true;
    }
}
FS* __cdecl findDevice(const char* device) {
    if(device) for(FS* f=head;f;f=f->next) if(sameDevice(device,f->device)) return f;
    return nullptr;
}
FS* __cdecl selectFS(const char* path) {
    if(!numSystems || !path) { error(6); return nullptr; }
    for(const char* p=path;*p;++p) if(*p==':') {
        std::string prefix(path,p+1);
        if(auto* f=findDevice(prefix.c_str())) return f;
    }
    if(defaultFS) return defaultFS;
    error(6); return nullptr;
}
FS* __cdecl setDefault(FS* fs) { defaultFS=fs; if(csf_rw::startupTracing()) std::fprintf(stderr,"RW startup RtFS default -> %p\n",static_cast<void*>(fs)); return fs; }
int __cdecl event(FS* f,int which) {
    if(!f) return 0;
    auto fn=which==1?f->registered:which==2?f->unregistered:nullptr;
    return fn?fn(f->name):0;
}
int __cdecl registerFS(FS* f) {
    std::lock_guard<std::recursive_mutex> lock(managerMutex);
    if(!initialized || !f || !capacity() || findName(f->name)) {
        if(csf_rw::startupTracing()) std::fprintf(stderr,"RW startup RtFS register %p -> 0 initialized=%d count=%d max=%d\n",static_cast<void*>(f),initialized,numSystems,maxSystems);
        return 0;
    }
    if(csf_rw::startupTracing()) std::fprintf(stderr,"RW startup RtFS register name=\"%s\" device=\"%s\" -> 1\n",f->name,f->device?f->device:"(null)");
    event(f,1); f->next=head; head=f; ++numSystems; return 1;
}
int __cdecl setDevice(FS* f,const char* device) {
    if(!f || !device) return 0;
    if(findDevice(device)) return 7;
    char* copy=static_cast<char*>(alloc(std::strlen(device)+1));
    if(!copy) return 0;
    std::strcpy(copy,device); release(f->device); f->device=copy; return 1;
}
int __cdecl initFS(FS* f,int n,const char* name,const char* device) {
    auto result=[&](int r) {
        if(csf_rw::startupTracing()) std::fprintf(stderr,"RW startup RtFS init name=\"%s\" device=\"%s\" records=%d -> %d\n",name?name:"(null)",device?device:"(null)",n,r);
        return r;
    };
    if(!initialized || !f || n<0 || !name || std::strlen(name)>=sizeof(f->name) || !f->getFile) return result(0);
    if(!capacity()) return result(2);
    if(nameUsed(name)) return result(6);
    std::strcpy(f->name,name); f->count=n; f->async=0;
    for(int i=0;i<n;++i) { auto* p=f->getFile(f,i); if(!p) return result(0); p->status=1; p->async=0; }
    f->device=nullptr;
    int resultCode=setDevice(f,device);
    if(resultCode==1) { f->registered=nullptr; f->unregistered=nullptr; f->kind=2; }
    return result(resultCode);
}
F* __cdecl openOn(FS* fs,const char* path,int flags,decltype(F::callback) cb,void* data) {
    if(!fs || !path) { error(4); return nullptr; }
    F* file=nullptr;
    {
        std::lock_guard<std::recursive_mutex> lock(managerMutex);
        for(int i=0;i<fs->count;++i) {
            auto* f=fs->getFile(fs,i);
            if(f && f->status==1) { file=f; f->status=5; break; }
        }
    }
    if(!file) { lastError=8; error(8); return nullptr; }
    if((flags&16) || file->async || fs->async) {
        file->operation=2; file->callback=cb; file->callbackData=data;
        if(fs->async) flags|=16;
    }
    if(fs->open && fs->open(fs,file,path,flags)==1) { file->operationState=2; return file; }
    file->status=1; return nullptr;
}
F* __cdecl openFlags(const char* path,int flags,decltype(F::callback) cb,void* data) { return openOn(selectFS(path),path,flags,cb,data); }
F* __cdecl openMode(const char* path,const char* mode) {
    if(!mode) return nullptr;
    int flags=0; bool plus=std::strchr(mode,'+')!=nullptr;
    if(std::strchr(mode,'r')) flags=plus?11:1;
    if(std::strchr(mode,'w')) flags|=6|(plus?1:0);
    if(std::strchr(mode,'a')) flags|=10|(plus?1:0)|(std::strchr(mode,'b')?4:0);
    return openFlags(path,flags,nullptr,nullptr);
}
int __cdecl closeFile(F* f) {
    if(!f || !f->owner || !f->owner->close) return -1;
    std::lock_guard<std::recursive_mutex> lock(managerMutex);
    f->status=1; f->owner->close(f); return 0;
}
uint32_t __cdecl readFile(void* dst,uint32_t size,uint32_t count,F* f) {
    if(!size || count>UINT32_MAX/size || !f || !f->owner || !f->owner->read || (count && !dst)) return 0;
    return f->owner->read(f,dst,size*count)/size;
}
uint32_t __cdecl writeFile(const void* src,uint32_t size,uint32_t count,F* f) {
    if(!size || count>UINT32_MAX/size || !f || !f->owner || !f->owner->write || (count && !src)) return 0;
    return f->owner->write(f,src,size*count)/size;
}
int __cdecl seekFile(F* f,int32_t off,int whence) {
    if(!f || !f->owner || !f->owner->seek || whence<0 || whence>2) return -1;
    if(f->async) { f->operation=4; f->status=3; }
    f->owner->seek(f,off,whence+1); return 0;
}
int __cdecl eofFile(F* f) { return f && f->owner && f->owner->eof ? f->owner->eof(f):1; }
int __cdecl abortFile(F* f) { return f && f->owner && f->owner->abort ? f->owner->abort(f):0; }
int __cdecl exists(const char* path) { auto* fs=selectFS(path); return fs && fs->exists?fs->exists(fs,path):0; }
void __stdcall syncAll() {
    for(FS* fs=head;fs;fs=fs->next) for(int i=0;i<fs->count;++i) {
        auto* f=fs->getFile(fs,i); if(f && f->status!=1 && f->owner->sync) f->owner->sync(f,0);
    }
}
const char* __cdecl stripDevice(const char* path) { if(!path) return nullptr; auto* p=std::strchr(path,':'); return p?p+1:path; }
char* __cdecl makePath(char* dst,int capacity,const char* prefix,const char* path,char separator) {
    if(!dst || !prefix || !path || capacity<=0 || std::strlen(prefix)+std::strlen(path)>=size_t(capacity)) return nullptr;
    std::strcpy(dst,prefix); char* out=dst+std::strlen(prefix);
    do { *out++=(*path=='/' || *path=='\\')?separator:*path; } while(*path++);
    return dst;
}
int __cdecl positionEOF(F* f) {
    if(!f) return 1;
    uint64_t pos=(uint64_t(f->position[1])<<32)|f->position[0];
    uint64_t len=(uint64_t(f->length[1])<<32)|f->length[0];
    return pos==UINT64_MAX || pos>=len;
}
int __cdecl complete(F* f) {
    // 007c6780 returns the record identity when no callback is invoked.
    int result=int(reinterpret_cast<uintptr_t>(f));
    if(!f || !f->async) return result;
    int op=f->operation; auto cb=f->callback; f->operation=1;
    return op!=1 && cb?cb(f,op==4?int(f->position[0]):f->transferred,f->status,op,f->callbackData):result;
}

// Native disk filesystem: 96-byte filesystem and 96-byte file records in x86.
struct DiskFS { FS base; uint32_t sector; struct DiskFile* files; };
struct DiskFile { F base; std::FILE* handle; unsigned char overlapped[28]; };
#ifndef CSF_RW_NATIVE_TEST
static_assert(sizeof(DiskFS)==96 && offsetof(DiskFS,files)==92);
static_assert(sizeof(DiskFile)==96 && offsetof(DiskFile,handle)==64);
#endif
F* __cdecl diskGet(FS* fs,uint32_t i) { return i<uint32_t(fs->count)?&static_cast<DiskFS*>(static_cast<void*>(fs))->files[i].base:nullptr; }
DiskFile* disk(F* f) { return reinterpret_cast<DiskFile*>(f); }
struct PendingRead { uint32_t count, low, high; };
void __cdecl diskDestroy(FS* f) {
    auto* fs=reinterpret_cast<DiskFS*>(f);
    for(int i=0;i<f->count;++i) if(fs->files[i].handle) std::fclose(fs->files[i].handle);
    release(fs->files); fs->files=nullptr;
}
std::string diskPath(FS* fs,const char* path) {
    std::string result=fs->device?fs->device:"";
    result+=stripDevice(path);
#ifndef _WIN32
    for(char& c:result) if(c=='\\') c='/';
#endif
    return result;
}
int64_t tell64(std::FILE* f) {
#ifdef _WIN32
    return _ftelli64(f);
#else
    return ftello(f);
#endif
}
int seek64(std::FILE* f,int64_t off,int whence) {
#ifdef _WIN32
    return _fseeki64(f,off,whence);
#else
    return fseeko(f,off,whence);
#endif
}
void updatePosition(F* f) { auto p=tell64(disk(f)->handle); f->position[0]=uint32_t(p); f->position[1]=uint32_t(uint64_t(p)>>32); }
int __cdecl diskOpen(FS* fs,F* f,const char* path,int flags) {
    auto name=diskPath(fs,path);
    // Both original Windows open/exists join into a 256-byte buffer.
    if(name.size()>=256) return 5;
    // OPEN_ALWAYS for append (no truncation); CREATE_ALWAYS for write. RW's
    // flags allow combined read/write; do not accidentally truncate r+.
    const char* mode=(flags&2)?((flags&8)?"r+b":((flags&1)?"w+b":"wb")):"rb";
    auto* handle=std::fopen(name.c_str(),mode);
    if(!handle && (flags&2) && (flags&8)) handle=std::fopen(name.c_str(),"w+b");
    if(csf_rw::startupTracing()) std::fprintf(stderr,"RW fs open '%s' flags=%x -> %s\n",name.c_str(),flags,handle?"ok":"missing/error");
    if(!handle) { lastError=errno; return 3; }
    disk(f)->handle=handle; seek64(handle,0,SEEK_END); auto size=tell64(handle);
    if(size<0 || seek64(handle,0,(flags&8)?SEEK_END:SEEK_SET)) { std::fclose(handle); disk(f)->handle=nullptr; return 3; }
    f->length[0]=uint32_t(size); f->length[1]=uint32_t(uint64_t(size)>>32);
    f->async=(flags&16)!=0; f->status=f->async?3:2; f->reserved36=1; updatePosition(f); return 1;
}
int __cdecl diskClose(F* f) { auto* h=disk(f)->handle; disk(f)->handle=nullptr; f->status=1; return h?std::fclose(h)==0:0; }
uint32_t __cdecl diskRead(F* f,void* dst,uint32_t n) {
    auto* h=disk(f)->handle; if(!h || f->status!=2 || (n && !dst)) return 0;
    f->status=3; auto got=uint32_t(std::fread(dst,1,n,h)); f->status=std::ferror(h)?4:2;
    if(f->async) {
        auto pos=uint64_t(tell64(h)); PendingRead pending{got,uint32_t(pos),uint32_t(pos>>32)};
        std::memcpy(disk(f)->overlapped,&pending,sizeof(pending));
        f->operation=5; if(f->status==2) f->status=3; return 0;
    }
    updatePosition(f); f->transferred=int(got);
    return got;
}
uint32_t __cdecl diskWrite(F* f,const void* src,uint32_t n) {
    auto* h=disk(f)->handle; if(!h || (n && !src)) return 0;
    auto got=uint32_t(std::fwrite(src,1,n,h)); f->status=std::ferror(h)?4:2;
    updatePosition(f); f->transferred=int(got);
    if(f->async) { if(f->status==2) f->status=3; return 0; }
    return got;
}
uint32_t __cdecl diskSeek(F* f,int32_t off,int origin) {
    if(!disk(f)->handle || origin<1 || origin>3 || seek64(disk(f)->handle,off,origin==1?SEEK_SET:origin==2?SEEK_CUR:SEEK_END)) return UINT32_MAX;
    updatePosition(f); return f->position[0];
}
int __cdecl diskSync(F* f,int) {
    // Host IO completes synchronously, but publish completion only at the
    // original sync/status boundary, including the original callback fields.
    if(f->async && f->status==3) {
        f->transferred=int(f->length[0]);
        if(f->operation==5) {
            PendingRead pending{}; std::memcpy(&pending,disk(f)->overlapped,sizeof(pending));
            f->position[0]=pending.low; f->position[1]=pending.high; f->transferred=int(pending.count);
        }
        f->status=2; complete(f);
    }
    return f->status;
}
int __cdecl diskAbort(F* f) { f->status=1; return 1; }
int __cdecl diskStatus(F* f) { return f->status==1?1:diskSync(f,0); }
int __cdecl diskExists(FS* fs,const char* path) {
    auto name=diskPath(fs,path); if(name.size()>=256) return 0;
    std::error_code ec; return std::filesystem::exists(name,ec)?1:0;
}
FS* __cdecl createDisk(int count,const char* device,const char* name) {
    if(csf_rw::startupTracing()) std::fprintf(stderr,"RW startup RtFS disk-create name=\"%s\" device=\"%s\" records=%d\n",name?name:"(null)",device?device:"(null)",count);
    if(!initialized || count<0 || size_t(count)>SIZE_MAX/sizeof(DiskFile) || !name || findName(name)) return nullptr;
    auto* fs=static_cast<DiskFS*>(alloc(sizeof(DiskFS))); if(!fs) return nullptr; std::memset(fs,0,sizeof(*fs));
    fs->files=static_cast<DiskFile*>(alloc(size_t(count)*sizeof(DiskFile)));
    if(count && !fs->files) { release(fs); return nullptr; }
    if(count) std::memset(fs->files,0,size_t(count)*sizeof(DiskFile));
    auto& f=fs->base;
    f.getFile=diskGet; f.destroy=diskDestroy; f.open=diskOpen; f.close=diskClose; f.read=diskRead; f.write=diskWrite;
    f.seek=diskSeek; f.sync=diskSync; f.abort=diskAbort; f.eof=positionEOF; f.status=diskStatus; f.exists=diskExists;
    for(int i=0;i<count;++i) fs->files[i].base.owner=&f;
    if(initFS(&f,count,name,device)!=1) { release(fs->files); release(fs); return nullptr; }
#ifdef _WIN32
    char root[MAX_PATH]; std::snprintf(root,sizeof(root),"%s\\",device);
    DWORD spc,bps,free,total; if(GetDiskFreeSpaceA(root,&spc,&bps,&free,&total)) fs->sector=bps;
#endif
    return &f;
}

void* nativeOpen(const char* path,const char* mode) {
    void* f=openMode(path,mode);
    if(csf_rw::startupTracing()) std::fprintf(stderr,"RW startup RtFS open \"%s\" %s -> %p\n",path?path:"(null)",mode?mode:"",f);
    return f;
}
int nativeClose(void* f) { return closeFile(static_cast<F*>(f)); }
int nativeSeek(void* f,long off,int whence) { return off<INT32_MIN || off>INT32_MAX?-1:seekFile(static_cast<F*>(f),int32_t(off),whence); }
long nativeTell(void* p) { auto* f=static_cast<F*>(p); uint64_t pos=(uint64_t(f->position[1])<<32)|f->position[0]; return pos>uint64_t(LONG_MAX)?-1:long(pos); }
size_t nativeRead(void* dst,size_t size,size_t n,void* f) { return size>UINT32_MAX || n>UINT32_MAX?0:readFile(dst,uint32_t(size),uint32_t(n),static_cast<F*>(f)); }
size_t nativeWrite(const void* src,size_t size,size_t n,void* f) { return size>UINT32_MAX || n>UINT32_MAX?0:writeFile(src,uint32_t(size),uint32_t(n),static_cast<F*>(f)); }
int nativeEOF(void* f) { return eofFile(static_cast<F*>(f)); }
int __cdecl managerInit(int max) {
    std::lock_guard<std::recursive_mutex> lock(managerMutex);
    if(initialized) { csf_rw::startupTrace("0x007C5F60","RtFSManagerOpen -> 0 (already initialized)"); return 0; }
#ifdef _WIN32
    InitializeCriticalSection(&gameFSLock);
#endif
    maxSystems=max; numSystems=0; head=nullptr; defaultFS=nullptr; errorCallback=nullptr; initialized=1;
    if(rw::engine) { savedFiles=rw::engine->filefuncs; rw::engine->filefuncs={nativeOpen,nativeClose,nativeSeek,nativeTell,nativeRead,nativeWrite,nativeEOF}; fileFunctionsInstalled=true; }
    csf_rw::startupTrace("0x007C5F60","RtFSManagerOpen"); return 1;
}
void __cdecl managerTerm() {
    std::lock_guard<std::recursive_mutex> lock(managerMutex);
    if(!initialized) return;
    FS* fs=head;
    while(fs) {
        auto* next=fs->next;
        if(fs->destroy) fs->destroy(fs);
        release(fs->device); fs->device=nullptr; event(fs,2); release(fs); fs=next;
    }
    head=nullptr; defaultFS=nullptr; numSystems=0; initialized=0;
#ifdef _WIN32
    DeleteCriticalSection(&gameFSLock);
#endif
    if(fileFunctionsInstalled && rw::engine) rw::engine->filefuncs=savedFiles;
    fileFunctionsInstalled=false;
}
// Keep dispatch local: a pre-manager stream must not be closed through the
// newly installed manager's callbacks. No game-visible native virtual header.
class FileStream final: public rw::Stream {
    void* file=nullptr;
    rw::FileFunctions funcs;
public:
    explicit FileStream(rw::FileFunctions f):funcs(f) {}
    bool open(const char* path,const char* mode) { file=funcs.rwfopen(path,mode); return file!=nullptr; }
    ~FileStream() override { close(); }
    void close() override { if(file) { funcs.rwfclose(file); file=nullptr; } }
    uint32_t read8(void* p,uint32_t n) override { return file?uint32_t(funcs.rwfread(p,1,n,file)):0; }
    uint32_t write8(const void* p,uint32_t n) override { return file?uint32_t(funcs.rwfwrite(p,1,n,file)):0; }
    void seek(int32_t n,int32_t whence=1) override { if(file) funcs.rwfseek(file,n,whence); }
    uint32_t tell() override { return file?uint32_t(funcs.rwftell(file)):UINT32_MAX; }
    bool eof() override { return !file || funcs.rwfeof(file)!=0; }
};
}
namespace csf_rw {
bool fileManagerActive() { return initialized!=0; }
rw::Stream* openFileStream(const char* path,const char* mode) {
    if(!rw::engine || !path) return nullptr;
    auto* stream=new(std::nothrow) FileStream(rw::engine->filefuncs);
    if(!stream || !stream->open(path,mode)) { delete stream; return nullptr; }
    return stream;
}
}

HOOK(0x007C5F60,managerInit,"boundary-rw");
HOOK(0x007C5FA0,managerTerm,"boundary-rw");
HOOK(0x007C6010,capacity,"boundary-rw");
HOOK(0x007C6040,findName,"boundary-rw");
HOOK(0x007C6090,nameUsed,"boundary-rw");
HOOK(0x007C60C0,list,"boundary-rw");
HOOK(0x007C60D0,findDevice,"boundary-rw");
HOOK(0x007C6110,selectFS,"boundary-rw");
HOOK(0x007C61A0,setDefault,"boundary-rw");
HOOK(0x007C61B0,closeFile,"boundary-rw");
HOOK(0x007C61F0,readFile,"boundary-rw");
HOOK(0x007C6220,writeFile,"boundary-rw");
HOOK(0x007C6250,seekFile,"boundary-rw");
HOOK(0x007C62C0,eofFile,"boundary-rw");
HOOK(0x007C62D0,exists,"boundary-rw");
HOOK(0x007C62F0,abortFile,"boundary-rw");
HOOK(0x007C6300,syncAll,"boundary-rw");
HOOK(0x007C6350,registerFS,"boundary-rw");
HOOK(0x007C63C0,openOn,"boundary-rw");
HOOK(0x007C64B0,openFlags,"boundary-rw");
HOOK(0x007C64F0,openMode,"boundary-rw");
HOOK(0x007C65C0,stripDevice,"boundary-rw");
HOOK(0x007C6610,makePath,"boundary-rw");
HOOK(0x007C66A0,positionEOF,"boundary-rw");
HOOK(0x007C66D0,event,"boundary-rw");
HOOK(0x007C6710,setDevice,"boundary-rw");
HOOK(0x007C6780,complete,"boundary-rw");
HOOK(0x007C67D0,initFS,"boundary-rw");
HOOK(0x007C6880,diskGet,"boundary-rw");
HOOK(0x007C68A0,diskDestroy,"boundary-rw");
HOOK(0x007C68C0,diskClose,"boundary-rw");
HOOK(0x007C68E0,diskRead,"boundary-rw");
HOOK(0x007C6A50,diskWrite,"boundary-rw");
HOOK(0x007C6B80,diskSeek,"boundary-rw");
HOOK(0x007C6BC0,diskSync,"boundary-rw");
HOOK(0x007C6CB0,diskAbort,"boundary-rw");
HOOK(0x007C6CC0,diskStatus,"boundary-rw");
HOOK(0x007C6CE0,diskExists,"boundary-rw");
HOOK(0x007C6D40,diskOpen,"boundary-rw");
HOOK(0x007C6E90,createDisk,"boundary-rw");
#endif
