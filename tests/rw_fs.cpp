#include "globals.h"
#include "boundary/rw/fs.h"
#include "boundary/rw/image.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <chrono>
#include <vector>
#include <string>
using FS=csf_rw::FileSystem;
using F=csf_rw::File;
using S=csf_rw::Stream;
CSF_FUNC(int,__cdecl,init,0x00627360,(const csf_rw::MemoryFunctions*,char,int));
CSF_FUNC(int,__cdecl,open,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,close,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
CSF_FUNC(int,__cdecl,fsInit,0x007C5F60,(int));
CSF_FUNC(void,__cdecl,fsTerm,0x007C5FA0,());
CSF_FUNC(FS*,__cdecl,createDisk,0x007C6E90,(int,const char*,const char*));
CSF_FUNC(int,__cdecl,initFS,0x007C67D0,(FS*,int,const char*,const char*));
CSF_FUNC(int,__cdecl,reg,0x007C6350,(FS*));
CSF_FUNC(FS*,__cdecl,find,0x007C6040,(const char*));
CSF_FUNC(FS*,__cdecl,rtSelect,0x007C6110,(const char*));
CSF_FUNC(FS*,__cdecl,setDefault,0x007C61A0,(FS*));
CSF_FUNC(F*,__cdecl,rtFopen,0x007C64F0,(const char*,const char*));
CSF_FUNC(F*,__cdecl,fopenFlags,0x007C64B0,(const char*,int,decltype(F::callback),void*));
CSF_FUNC(int,__cdecl,rtFclose,0x007C61B0,(F*));
CSF_FUNC(uint32_t,__cdecl,rtFread,0x007C61F0,(void*,uint32_t,uint32_t,F*));
CSF_FUNC(uint32_t,__cdecl,rtFwrite,0x007C6220,(const void*,uint32_t,uint32_t,F*));
CSF_FUNC(int,__cdecl,rtFseek,0x007C6250,(F*,int32_t,int));
CSF_FUNC(int,__cdecl,rtFeof,0x007C62C0,(F*));
CSF_FUNC(int,__cdecl,exists,0x007C62D0,(const char*));
CSF_FUNC(void,__stdcall,syncAll,0x007C6300,());
CSF_FUNC(int,__cdecl,complete,0x007C6780,(F*));
CSF_FUNC(char*,__cdecl,makePath,0x007C6610,(char*,int,const char*,const char*,char));
CSF_FUNC(S*,__cdecl,streamOpen,0x006298E0,(int,int,void*));
CSF_FUNC(int,__cdecl,streamClose,0x00629810,(S*,csf_rw::Memory*));
CSF_FUNC(uint32_t,__cdecl,streamRead,0x006293E0,(S*,void*,uint32_t));
CSF_FUNC(S*,__cdecl,streamSkip,0x00629700,(S*,uint32_t));
CSF_FUNC(csf_rw::Raster*,__cdecl,readDDS,0x0063A4D0,(S*));
CSF_FUNC(int,__cdecl,destroyRaster,0x00627570,(csf_rw::Raster*));

struct Fixture {
    std::filesystem::path path=std::filesystem::temp_directory_path()/std::filesystem::path("csf-rwfs-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    Fixture() { std::filesystem::create_directories(path/"nested"); }
    ~Fixture() { std::filesystem::remove_all(path); }
};
struct VirtualFS { FS base; F file; };
static F* __cdecl virtualGet(FS* fs,uint32_t i) { return i==0?&reinterpret_cast<VirtualFS*>(fs)->file:nullptr; }
static int virtualOpens;
static std::vector<unsigned char> virtualContents{'a','r','c','h','i','v','e'};
static int __cdecl virtualOpen(FS*,F* f,const char* path,int) {
    assert(!std::strcmp(path,"archive.dds") || !std::strcmp(path,"vfs:archive.dds"));
    ++virtualOpens; f->status=2; f->length[0]=uint32_t(virtualContents.size()); f->position[0]=0; return 1;
}
static int __cdecl virtualClose(F* f) { f->status=1; return 0; }
static uint32_t __cdecl virtualRead(F* f,void* p,uint32_t n) { n=std::min(n,uint32_t(virtualContents.size())-f->position[0]); std::memcpy(p,virtualContents.data()+f->position[0],n); f->position[0]+=n; return n; }
static uint32_t __cdecl virtualSeek(F* f,int32_t n,int origin) { f->position[0]=(origin==1?0:origin==2?f->position[0]:uint32_t(virtualContents.size()))+n; return f->position[0]; }
static int __cdecl virtualEOF(F* f) { return f->position[0]>=virtualContents.size(); }
static int callbacks,callbackOp,callbackCount;
static int __cdecl callback(F*,int count,int status,int op,void* data) { assert(data==&callbacks && status==2); ++callbacks; callbackOp=op; callbackCount=count; return 1; }

static std::vector<unsigned char> dds(uint32_t w,uint32_t h,uint32_t fourcc,uint32_t levels=1) {
    std::vector<unsigned char> bytes(128,0); auto* p=reinterpret_cast<uint32_t*>(bytes.data());
    p[0]=0x20534444; ++p; p[0]=124; p[1]=0x1007|(levels>1?0x20000:0); p[2]=h; p[3]=w; p[6]=levels;
    p[18]=32; p[19]=fourcc?4:0x41; p[20]=fourcc; p[21]=fourcc?0:32;
    if(!fourcc) { p[22]=0x00FF0000; p[23]=0x0000FF00; p[24]=0x000000FF; p[25]=0xFF000000; }
    p[26]=0x1000;
    for(uint32_t i=0;i<levels;++i,w=std::max(1u,w/2),h=std::max(1u,h/2)) {
        auto n=fourcc?((w+3)/4)*((h+3)/4)*(fourcc==0x31545844?8:16):w*h*4;
        size_t at=bytes.size(); bytes.resize(at+n,0);
        for(uint32_t j=0;j<n;++j) bytes[at+j]=uint8_t(j+i);
    }
    return bytes;
}
static csf_rw::Raster* decode(std::vector<unsigned char>& bytes) {
    csf_rw::Memory m{bytes.data(),uint32_t(bytes.size())};
    auto* s=streamOpen(3,1,&m); assert(s); auto* r=readDDS(s); assert(streamClose(s,nullptr)); return r;
}
static void checkDDS() {
    for(uint32_t cc:{0u,0x31545844u,0x32545844u,0x33545844u,0x34545844u,0x35545844u}) {
        auto bytes=dds(7,5,cc,3); auto* r=decode(bytes); assert(r && r->width==7 && r->height==5);
        auto* native=csf_rw::rasterNative(r); assert(native->getNumLevels()==3);
        for(int i=0;i<3;++i) {
            auto* p=native->lock(i,rw::Raster::LOCKREAD); assert(p && p[0]==i && p[1]==i+1);
            native->unlock(i);
        }
        assert(destroyRaster(r));
        bytes.pop_back(); assert(!decode(bytes));
    }
    auto bytes=dds(4,4,0x31545844); std::memset(bytes.data()+128,0,8); bytes[132]=3;
    auto* r=decode(bytes); assert(r && (r->format&15)==1); assert(destroyRaster(r));
    auto mutate=[&](int index,uint32_t value) { auto b=dds(4,4,0); std::memcpy(b.data()+index*4,&value,4); assert(!decode(b)); };
    mutate(0,0); mutate(1,123); mutate(2,0); mutate(4,0); mutate(4,0xFFFFFFFF); mutate(19,31); mutate(28,0x200); mutate(28,0x200000);
    bytes=dds(4,4,0x30315844); assert(!decode(bytes)); // DX10
    bytes=dds(1,1,0); bytes.resize(127); assert(!decode(bytes));
    auto pixel=[&](uint32_t flags,uint32_t bits,uint32_t rm,uint32_t gm,uint32_t bm,uint32_t am,
                   std::vector<unsigned char> payload,std::vector<unsigned char> expected) {
        auto b=dds(1,1,0); auto* h=reinterpret_cast<uint32_t*>(b.data())+1;
        h[19]=flags; h[21]=bits; h[22]=rm; h[23]=gm; h[24]=bm; h[25]=am;
        b.resize(128); b.insert(b.end(),payload.begin(),payload.end());
        auto* r=decode(b); assert(r); auto* n=csf_rw::rasterNative(r);
        auto* p=n->lock(0,rw::Raster::LOCKREAD); assert(p && !std::memcmp(p,expected.data(),4));
        n->unlock(0); assert(destroyRaster(r));
    };
    pixel(0x40,16,0xF800,0x7E0,0x1F,0,{0,0xF8},{0,0,255,255});
    pixel(0x20000,8,255,0,0,0,{128},{128,128,128,255});
    pixel(2,8,0,0,0,255,{128},{255,255,255,128});
    std::vector<unsigned char> pal(1025,0); pal[4]=17; pal[5]=34; pal[6]=51; pal[7]=68; pal[1024]=1;
    pixel(0x20,8,0,0,0,0,pal,{51,34,17,68});
}
int main() {
    Fixture fixture;
    assert(init(nullptr,0,1024)); assert(open(nullptr)); assert(start());
    // Before init the legacy raw host stream remains valid across cutover.
    std::string host=(fixture.path/"nested"/"asset.bin").string();
    { auto* f=std::fopen(host.c_str(),"wb"); assert(f); assert(std::fwrite("abcdefg",1,7,f)==7); std::fclose(f); }
    auto* old=streamOpen(2,1,host.data()); assert(old);
    assert(fsInit(3) && !fsInit(-1));
    std::string root=fixture.path.string()+"/";
    auto* disk=createDisk(2,root.c_str(),"win"); assert(disk && reg(disk)); assert(find("win")==disk);
    assert(!reg(disk) && !createDisk(1,root.c_str(),"win"));
    setDefault(disk); assert(rtSelect("nested/asset.bin")==disk);
    assert(exists("nested\\asset.bin") && !exists("absent.bin"));
    std::string tooLong(256,'x'); assert(!exists(tooLong.c_str()) && !rtFopen(tooLong.c_str(),"rb"));
    char joined[16]{};
    assert(makePath(joined,sizeof(joined),"X:","a/b\\c",'\\')==joined && !std::strcmp(joined,"X:a\\b\\c"));
    assert(!makePath(joined,7,"X:","a/b\\c",'\\'));
    char buffer[16]{}; assert(streamRead(old,buffer,3)==3 && !std::memcmp(buffer,"abc",3)); assert(streamClose(old,nullptr));
    auto* f=rtFopen("nested\\asset.bin","rb"); assert(f && f->owner==disk && f->length[0]==7 && f->status==2);
    assert(complete(f)==int(reinterpret_cast<uintptr_t>(f)) && complete(nullptr)==0);
    auto* other=rtFopen("nested/asset.bin","rb"); assert(other && !rtFopen("nested/asset.bin","rb")); assert(!rtFopen("missing","rb"));
    assert(rtFread(buffer,2,4,f)==3 && !std::memcmp(buffer,"abcdefg",7) && rtFeof(f));
    assert(!rtFread(buffer,0,4,f) && !rtFread(buffer,2,UINT32_MAX,f));
    assert(!rtFseek(f,-2,2) && f->position[0]==5); assert(rtFread(buffer,1,4,f)==2 && !std::memcmp(buffer,"fg",2));
    assert(rtFclose(f)==0 && rtFclose(other)==0);
    for(int i=0;i<4;++i) assert(!rtFopen("missing","rb")); // failed opens release pool entries
    auto* s=streamOpen(2,1,const_cast<char*>("nested\\asset.bin")); assert(s);
    assert(streamSkip(s,2)==s && streamRead(s,buffer,8)==5 && !std::memcmp(buffer,"cdefg",5)); assert(streamClose(s,nullptr));
    f=rtFopen("new.bin","wb"); assert(f && rtFwrite("one",1,3,f)==3 && rtFclose(f)==0);
    f=rtFopen("new.bin","ab"); assert(f && rtFwrite("two",1,3,f)==3 && rtFclose(f)==0);
    f=rtFopen("new.bin","rb"); assert(f && rtFread(buffer,1,16,f)==6 && !std::memcmp(buffer,"onetwo",6)); assert(!rtFclose(f));
    f=fopenFlags("nested/asset.bin",17,callback,&callbacks); assert(f && f->status==3);
    syncAll(); assert(callbacks==1 && callbackOp==2 && callbackCount==7 && f->status==2);
    assert(rtFread(buffer,1,3,f)==0 && f->status==3 && f->position[0]==0); syncAll(); assert(callbacks==2 && callbackOp==5 && callbackCount==3 && f->position[0]==3);
    assert(!rtFseek(f,1,0)); syncAll(); assert(callbacks==3 && callbackOp==4 && callbackCount==1); assert(!rtFclose(f));
    // Game-owned vfs callbacks must win over loose files. A substitute archive
    // reader verifies dispatch and ownership without reimplementing game code.
    auto* v=static_cast<VirtualFS*>(rw::Engine::memfuncs.rwmalloc(sizeof(VirtualFS),rw::MEMDUR_GLOBAL)); assert(v); std::memset(v,0,sizeof(*v));
    v->base.getFile=virtualGet; v->base.open=virtualOpen; v->base.close=virtualClose; v->base.read=virtualRead; v->base.seek=virtualSeek; v->base.eof=virtualEOF; v->file.owner=&v->base;
    assert(initFS(&v->base,1,"vfs","vfs:")==1 && reg(&v->base)); setDefault(&v->base);
    s=streamOpen(2,1,const_cast<char*>("archive.dds")); assert(s && virtualOpens==1); assert(streamRead(s,buffer,12)==7 && !std::memcmp(buffer,"archive",7)); assert(streamClose(s,nullptr));
    f=rtFopen("vfs:archive.dds","rb"); assert(f && virtualOpens==2); assert(!rtFclose(f));
    virtualContents=dds(7,5,0x31545844,3);
    s=streamOpen(2,1,const_cast<char*>("archive.dds")); assert(s);
    auto* raster=readDDS(s); assert(raster && raster->width==7 && csf_rw::rasterNative(raster)->getNumLevels()==3);
    assert(streamClose(s,nullptr) && destroyRaster(raster));
    assert(rtSelect((root+"nested/asset.bin").c_str())==&v->base);
    auto* explicitDisk=createDisk(1,"x:","other"); assert(explicitDisk && reg(explicitDisk));
    assert(rtSelect("X:asset.bin")==explicitDisk && rtSelect("VFS:archive.dds")==&v->base);
    setDefault(disk);
    checkDDS();
    fsTerm(); assert(!csf_rw::fileManagerActive()); fsTerm();
    assert(fsInit(1)); disk=createDisk(1,root.c_str(),"win"); assert(disk && reg(disk));
    assert(!createDisk(1,"other:","other")); fsTerm();
    assert(stop() && close() && term());
    std::puts("rw_fs: disk/pool/manager/custom dispatch/streams/DDS passed");
}
