#include "globals.h"
#include "boundary/rw/image.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <initializer_list>
using I=csf_rw::Image; using R=csf_rw::Raster; using T=csf_rw::Texture; using D=csf_rw::TexDictionary;
using S=csf_rw::Stream;
CSF_GLOBAL(void*,gameEngine,0x008B68C0);
CSF_FUNC(S*,__cdecl,streamOpen,0x006298E0,(int,int,void*));
CSF_FUNC(int,__cdecl,streamClose,0x00629810,(S*,csf_rw::Memory*));
CSF_FUNC(int,__cdecl,findChunk,0x00629AE0,(S*,uint32_t,uint32_t*,uint32_t*));
CSF_FUNC(int,__cdecl,init,0x00627360,(const csf_rw::MemoryFunctions*,char,int));
CSF_FUNC(int,__cdecl,open,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,close,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
CSF_FUNC(char,__stdcall,maxAniso,0x00566520,());
CSF_FUNC(T*,__cdecl,aniso,0x00566530,(T*,char));
CSF_FUNC(int,__cdecl,width,0x005870D0,(R*));
CSF_FUNC(I*,__cdecl,createImage,0x00625060,(int,int,int));
CSF_FUNC(I*,__cdecl,allocateImage,0x006250C0,(I*));
CSF_FUNC(const char*,__cdecl,setPath,0x00625270,(const char*));
CSF_FUNC(const char*,__stdcall,getPath,0x00625320,());
CSF_FUNC(I*,__cdecl,readImage,0x006254C0,(const char*));
using RI=I* (__cdecl *)(const char*); using WI=I* (__cdecl *)(I*,const char*);
CSF_FUNC(int,__cdecl,registerImage,0x00625680,(const char*,RI,WI));
CSF_FUNC(I*,__cdecl,writeImage,0x006257E0,(I*,const char*));
CSF_FUNC(int,__cdecl,destroyImage,0x00625F10,(I*));
CSF_FUNC(int,__cdecl,rasterPlugin,0x00627430,(uint32_t));
CSF_FUNC(R*,__cdecl,unlock,0x00627490,(R*));
CSF_FUNC(int,__cdecl,destroyRaster,0x00627570,(R*));
CSF_FUNC(R*,__cdecl,push,0x006275B0,(R*));
CSF_FUNC(R*,__stdcall,pop,0x00627650,());
CSF_FUNC(int,__cdecl,levels,0x006276A0,(R*));
CSF_FUNC(R*,__cdecl,subRaster,0x00627710,(R*,R*,rw::Rect*));
CSF_FUNC(R*,__cdecl,createRaster,0x00627770,(int,int,int,int));
CSF_FUNC(unsigned char*,__cdecl,lock,0x00627810,(R*,int,int));
using FT=T* (__cdecl *)(const char*); using RT=T* (__cdecl *)(const char*,const char*);
CSF_FUNC(int,__cdecl,setFind,0x006315B0,(FT));
CSF_FUNC(int,__cdecl,setRead,0x006315D0,(RT));
CSF_FUNC(D*,__stdcall,createDictionary,0x00631650,());
using TC=T* (__cdecl *)(T*,void*); using DC=D* (__cdecl *)(D*,void*);
CSF_FUNC(D*,__cdecl,visitTextures,0x006316E0,(D*,TC,void*));
CSF_FUNC(int,__cdecl,visitDictionaries,0x00631730,(DC,void*));
CSF_FUNC(T*,__cdecl,createTexture,0x00631780,(R*));
CSF_FUNC(int,__cdecl,destroyTexture,0x006317E0,(T*));
CSF_FUNC(T*,__cdecl,setName,0x00631800,(T*,const char*));
CSF_FUNC(D*,__cdecl,addTexture,0x006318E0,(D*,T*));
CSF_FUNC(T*,__cdecl,removeTexture,0x00631920,(T*));
CSF_FUNC(T*,__cdecl,findTexture,0x00631950,(D*,const char*));
CSF_FUNC(D*,__cdecl,setCurrent,0x00631990,(D*));
CSF_FUNC(D*,__stdcall,current,0x006319B0,());
CSF_FUNC(T*,__cdecl,readTexture,0x006319C0,(const char*,const char*));
CSF_FUNC(int,__cdecl,destroyDictionary,0x00632340,(D*));
CSF_FUNC(I*,__cdecl,fromRaster,0x00633010,(I*,R*));
CSF_FUNC(R*,__cdecl,fromImage,0x00633040,(R*,I*));
CSF_FUNC(I*,__cdecl,findFormat,0x00633070,(I*,int,int*,int*,int*,int*));
CSF_FUNC(I*,__cdecl,bmpRead,0x007C5680,(const char*));
CSF_FUNC(I*,__cdecl,bmpWrite,0x007C5CA0,(I*,const char*));
CSF_FUNC(I*,__cdecl,pngWrite,0x007C8D30,(I*,const char*));
CSF_FUNC(I*,__cdecl,pngRead,0x007C9050,(const char*));
static T* callbackTexture;
static void* stateRaster;
static void setNativeState(int state,void* value) { assert(state==rw::TEXTURERASTER); stateRaster=value; }
static void* getNativeState(int state) { assert(state==rw::TEXTURERASTER); return stateRaster; }
static size_t shortWrite(const void*,size_t,size_t,void*) { return 0; }
static void* failAllocation(size_t,rw::uint32) { return nullptr; }
static int pluginConstructs,pluginDestructs;
static void* pluginLast;
static void* pluginCtor(void* object,int off,int size) {
    assert(size==4); pluginLast=object; ++pluginConstructs;
    *reinterpret_cast<int*>(static_cast<char*>(object)+off)=12345; return object;
}
static void* pluginDtor(void* object,int off,int size) {
    assert(size==4 && *reinterpret_cast<int*>(static_cast<char*>(object)+off)==12345);
    ++pluginDestructs; return object;
}
static T* __cdecl findCB(const char*) { return callbackTexture; }
static T* __cdecl readCB(const char*,const char*) { return callbackTexture; }
static T* __cdecl countTexture(T* t,void* p) { assert(t); ++*static_cast<int*>(p); return t; }
static T* __cdecl deleteTexture(T* t,void* p) { ++*static_cast<int*>(p); assert(destroyTexture(t)); return t; }
static D* __cdecl countDictionary(D* d,void* p) { assert(d); ++*static_cast<int*>(p); return d; }
static void pixels(I* i) {
    assert(allocateImage(i)==i); assert(i->stride==i->width*4);
    for (int y=0;y<i->height;++y) for (int x=0;x<i->width;++x) {
        auto* p=i->pixels+y*i->stride+x*4; p[0]=x*17; p[1]=y*23; p[2]=123; p[3]=255;
    }
}
int main() {
    assert(!createImage(1,1,32));
    for (int cycle=0;cycle<2;++cycle) {
        assert(init(nullptr,0,0));
        for (auto kind: {csf_rw::Registry::Raster,csf_rw::Registry::Texture,csf_rw::Registry::Dictionary})
            if (csf_rw::plugins(kind).getPluginOffset(0xfeed)<0)
                assert(csf_rw::plugins(kind).registerPlugin(4,0xfeed,pluginCtor,pluginDtor,nullptr)>=0);
        assert(open(nullptr) && start());
        assert(!createImage(-1,1,32)); assert(!createImage(INT32_MAX,2,32));
        auto* initial=current(); assert(initial);
        auto mallocCallback=rw::Engine::memfuncs.rwmalloc;
        rw::Engine::memfuncs.rwmalloc=failAllocation;
        assert(!createImage(1,1,32) && !createRaster(1,1,32,0x504));
        assert(!createTexture(nullptr) && !createDictionary());
        rw::Engine::memfuncs.rwmalloc=mallocCallback;
        assert(registerImage("bmp",bmpRead,bmpWrite)); assert(registerImage("png",pngRead,pngWrite));
        assert(!std::strcmp(setPath("a/;b/"),"a/;b/")); assert(!std::strcmp(getPath(),"a/;b/"));
        setPath(nullptr);
        for (int depth: {4,8,16,24,32}) {
            auto* i=createImage(8,4,depth); assert(i && allocateImage(i));
            assert(i->stride==8*(depth<8?1:depth/8));
            assert(depth>8 || i->palette); assert(destroyImage(i));
        }
        // Game-supplied pixel storage with flags=0 remains borrowed at destroy.
        auto* borrowed=createImage(2,1,32); unsigned char buffer[8]{};
        borrowed->pixels=buffer; borrowed->stride=8; assert(destroyImage(borrowed));
        auto* image=createImage(8,4,32); assert(image); pixels(image);
        assert(!stop());
        const char* bmp="/tmp/csf-rw3-image.bmp"; const char* png="/tmp/csf-rw3-image.png";
        assert(bmpWrite(image,bmp)==image); auto* b=bmpRead(bmp); assert(b && b->width==8 && b->height==4);
        assert(destroyImage(b));
        assert(pngWrite(image,png)==image); auto* p=pngRead(png); assert(p && p->width==8);
        auto* np=csf_rw::imageNative(p);
        // Read fixture may be RGB without alpha; compare the native RGB words.
        assert(np->pixels[2]==123); assert(destroyImage(p));
        assert(writeImage(image,bmp)==image); b=readImage(bmp); assert(b && destroyImage(b));
        b=readImage("/tmp/csf-rw3-image"); assert(b && destroyImage(b));
        assert(!readImage("/tmp/csf-rw3-no-such-file")); assert(!writeImage(image,"/no-such-dir/image.bmp"));
        assert(!pngRead("/tmp/csf-rw3-no-such-file.png"));
        auto writeCallback=rw::engine->filefuncs.rwfwrite;
        rw::engine->filefuncs.rwfwrite=shortWrite;
        assert(!writeImage(image,bmp));
        rw::engine->filefuncs.rwfwrite=writeCallback;
        // Codec bridges normalize strided input without mutating it; BMP row
        // padding and 32->24 conversion used to generate malformed files.
        for (int iw: {1,2,3,5}) {
            auto* odd=createImage(iw,3,32); pixels(odd);
            auto* oldPixels=odd->pixels;
            assert(bmpWrite(odd,bmp)==odd && odd->pixels==oldPixels && odd->depth==32);
            b=bmpRead(bmp); assert(b && b->width==iw && b->height==3);
            for (int y=0;y<3;++y) for (int x=0;x<iw;++x)
                assert(!std::memcmp(b->pixels+y*b->stride+x*3,odd->pixels+y*odd->stride+x*4,3));
            assert(destroyImage(b) && destroyImage(odd));
        }
        int w,h,d,f; assert(findFormat(image,4,&w,&h,&d,&f)==image);
        auto* r=createRaster(w,h,d,f); assert(r && width(r)==8);
        assert(pluginLast==r && rasterPlugin(0xfeed)==sizeof(R));
        assert(fromImage(r,image)==r);
        auto* copy=createImage(w,h,32); assert(copy); allocateImage(copy);
        assert(fromRaster(copy,r)==copy); assert(!std::memcmp(copy->pixels,image->pixels,8*4*4));
        assert(destroyImage(copy)); assert(destroyRaster(r));
        for (int paletteDepth: {4,8}) {
            auto* pal=createImage(8,4,paletteDepth); assert(pal && allocateImage(pal));
            std::memset(pal->pixels,3,pal->stride*pal->height);
            std::memset(pal->palette,255,(1<<paletteDepth)*4);
            pal->palette[12]=20; pal->palette[13]=40; pal->palette[14]=60;
            assert(findFormat(pal,4,&w,&h,&d,&f)==pal);
            auto* pr=createRaster(w,h,d,f); assert(pr && fromImage(pr,pal)==pr);
            auto* pc=createImage(w,h,paletteDepth); assert(pc && allocateImage(pc));
            assert(fromRaster(pc,pr)==pc);
            assert(!std::memcmp(pc->pixels,pal->pixels,pc->stride*pc->height));
            assert(!std::memcmp(pc->palette,pal->palette,(1<<paletteDepth)*4));
            assert(destroyImage(pc) && destroyImage(pal) && destroyRaster(pr));
        }
        // All colour representations handled by librw's offline D3D9 codec;
        // native camera/Z surfaces are deliberately left to the device slice.
        for (int fmt: {0x100,0x200,0x300,0x400,0x500,0x600,0xa00,0x2500,0x4500}) {
            int depth=(fmt==0x500 || fmt==0x600)?32:16;
            if (fmt==0x400 || fmt&0x6000) depth=8;
            r=createRaster(8,4,depth,fmt|4|0x8000); assert(r);
            assert(levels(r)==4); assert(!lock(r,-1,1)); assert(!lock(r,4,1));
            for (int level=0;level<levels(r);++level) {
                auto* data=lock(r,level,1); assert(data); assert(!lock(r,level,1));
                std::memset(data,0,r->stride*r->height); assert(!destroyRaster(r)); assert(unlock(r)==r);
                assert(r->width==8 && r->height==4);
                assert(lock(r,level,2) && unlock(r));
            }
            assert(destroyRaster(r));
        }
        assert(rasterPlugin(0xdeadbeef)==-1);
        assert(!createRaster(8,4,32,0x502)); assert(!createRaster(8,4,32,0x701));
        r=createRaster(8,4,32,0x504); assert(r);
        // Device fixture verifies the engine facade's inward/outward raster
        // translation (NULL's real state callbacks deliberately store nothing).
        auto oldSet=rw::engine->device.setRenderState; auto oldGet=rw::engine->device.getRenderState;
        rw::engine->device.setRenderState=setNativeState; rw::engine->device.getRenderState=getNativeState;
        auto* slots=static_cast<uintptr_t*>(gameEngine);
        auto setState=reinterpret_cast<int (__cdecl *)(int,void*)>(slots[8]);
        auto getState=reinterpret_cast<int (__cdecl *)(int,void*)>(slots[9]);
        assert(setState(rw::TEXTURERASTER,r) && stateRaster==csf_rw::rasterNative(r));
        R* restored=nullptr; assert(getState(rw::TEXTURERASTER,&restored) && restored==r);
        assert(setState(rw::TEXTURERASTER,nullptr));
        rw::engine->device.setRenderState=oldSet; rw::engine->device.getRenderState=oldGet;
        assert(push(r)==r); assert(!destroyRaster(r)); assert(pop()==r); assert(!pop());
        auto* sub=createRaster(0,0,0,0x80); assert(sub); rw::Rect rect{1,1,2,2};
        assert(subRaster(sub,r,&rect)==sub && sub->parent==r && sub->width==2 && sub->depth==32);
        assert(!destroyRaster(r)); assert(destroyRaster(sub));
        auto* t=createTexture(r); assert(t && t->raster==r && t->refCount==1);
        assert(!destroyRaster(r)); assert(setName(t,"hello")==t);
        assert(maxAniso()==1); assert(aniso(t,1)==t);
        for (unsigned filter=1;filter<=6;++filter) for (unsigned u=1;u<=4;++u) for (unsigned v=1;v<=4;++v) {
            t->filterAddressing=filter|(u<<8)|(v<<12);
            auto* nt=csf_rw::textureNative(t); assert(nt->getFilter()==filter && nt->getAddressU()==u && nt->getAddressV()==v);
        }
        auto* dict=createDictionary(); assert(dict && setCurrent(dict)==dict && current()==dict);
        assert(addTexture(dict,t)==dict); assert(findTexture(dict,"HELLO")==t);
        assert(readTexture("hello",nullptr)==t && t->refCount==2);
        assert(destroyTexture(t) && t->refCount==1);
        csf_rw::Memory memory{};
        auto* output=streamOpen(3,2,&memory); assert(output);
        unsigned settings=t->filterAddressing;
        assert(csf_rw::textureStreamWrite(t,output)); assert(streamClose(output,&memory));
        auto* input=streamOpen(3,1,&memory); assert(input);
        assert(findChunk(input,rw::ID_TEXTURE,nullptr,nullptr));
        assert(csf_rw::textureStreamRead(input)==t && t->refCount==2 && t->filterAddressing==settings);
        assert(streamClose(input,nullptr)); rw::Engine::memfuncs.rwfree(memory.start);
        assert(destroyTexture(t) && t->refCount==1);
        int count=0; assert(visitTextures(dict,countTexture,&count)==dict && count==1);
        count=0; assert(visitDictionaries(countDictionary,&count) && count==2);
        assert(removeTexture(t)==t && !t->dict && !findTexture(dict,"hello"));
        callbackTexture=t; assert(setFind(findCB)); assert(readTexture("x",nullptr)==t && t->refCount==2);
        assert(destroyTexture(t)); assert(setFind(nullptr)); assert(setRead(readCB));
        assert(readTexture("x",nullptr)==t && t->dict==dict && t->refCount==1);
        assert(setRead(nullptr)); callbackTexture=nullptr;
        assert(!stop());
        count=0; assert(visitTextures(dict,deleteTexture,&count)==dict && count==1);
        assert(destroyDictionary(dict)); assert(setCurrent(initial)==initial);
        auto* held=createTexture(nullptr); auto* owning=createDictionary();
        assert(held && owning); held->refCount=2; assert(addTexture(owning,held)==owning);
        assert(destroyDictionary(owning) && held->refCount==1 && !held->dict);
        assert(destroyTexture(held));
        // There is no allocated TextureSetRaster slot: game writes +0 directly.
        auto* swapTexture=createTexture(nullptr); auto* swapRaster=createRaster(2,2,32,0x504);
        assert(swapTexture && swapRaster);
        swapTexture->raster=swapRaster;
        assert(csf_rw::textureNative(swapTexture)->raster==csf_rw::rasterNative(swapRaster));
        swapTexture->raster=nullptr;
        assert(!csf_rw::textureNative(swapTexture)->raster);
        assert(destroyTexture(swapTexture) && destroyRaster(swapRaster));
        assert(destroyImage(image)); std::remove(bmp); std::remove(png);
        assert(csf_rw::imagesEmpty()); assert(stop() && close() && term());
        assert(pluginConstructs==pluginDestructs);
    }
    std::puts("rw image/texture/offline raster facade tests passed (42 slots)");
}
