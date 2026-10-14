#ifdef CSF_RW_D3D9_LINK_ONLY
// Cross-compiled link probe only. NEVER run as a native recording-device test:
// it deliberately references real unmodified librw D3D9 lifecycle/device APIs.
#include <windows.h>
#include <d3d9.h>
#include "globals.h"
#include "boundary/rw/device.h"
int main() {
    if (!rw::Engine::init()) return 1;
    rw::EngineOpenParams params{GetDesktopWindow()};
    if (!rw::Engine::open(&params) || !rw::Engine::start()) return 2;
    if (!csf_rw::bindD3D9Device(rw::d3d::d3ddevice)) return 3;
    uintptr_t dispatch[75]{};
    if (!csf_rw::installImmediateDispatch(dispatch)) return 4;
    csf_rw::bindD3D9Device(nullptr);
    rw::Engine::stop();rw::Engine::close();rw::Engine::term();
    return 0;
}
#else
#include "globals.h"
#include "boundary/rw/device.h"
#include <array>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <vector>
using namespace csf_rw;
CSF_FUNC(int,__cdecl,init,0x00627360,(const MemoryFunctions*,char,int));
CSF_FUNC(int,__cdecl,open,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,closeEngine,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
CSF_FUNC(void*,__stdcall,currentDevice,0x0062B670,());
CSF_FUNC(void*,__stdcall,caps,0x0062C110,());
CSF_FUNC(int,__cdecl,declaration,0x0062B720,(void*));
CSF_FUNC(int,__cdecl,vertexShader,0x0062B760,(void*));
CSF_FUNC(int,__cdecl,pixelShader,0x0062B7A0,(void*));
CSF_FUNC(int,__cdecl,indices,0x0062B970,(void*));
CSF_FUNC(int,__cdecl,streams,0x0062B840,(const VertexStream*,int));
CSF_FUNC(int,__cdecl,draw,0x0062B9A0,(unsigned,int,unsigned,unsigned,unsigned,unsigned));
CSF_FUNC(int,__cdecl,setTransform,0x0062BA10,(unsigned,const float*));
CSF_FUNC(unsigned,__cdecl,getTransform,0x0062BB70,(unsigned,float*));
CSF_FUNC(int,__cdecl,createVS,0x0062C060,(const uint32_t*,void**));
CSF_FUNC(int,__cdecl,createPS,0x0062C0C0,(const uint32_t*,void**));
CSF_FUNC(int,__cdecl,constantVS,0x0062C0A0,(unsigned,const float*,unsigned));
CSF_FUNC(int,__cdecl,constantPS,0x0062C0F0,(unsigned,const float*,unsigned));
CSF_FUNC(int,__cdecl,state,0x0062D650,(unsigned,unsigned));
CSF_FUNC(uint32_t,__cdecl,pixel,0x00625870,(const rw::RGBA*,int));
CSF_FUNC(Raster*,__cdecl,rasterCreate,0x00627770,(int,int,int,int));
CSF_FUNC(int,__cdecl,rasterDestroy,0x00627570,(Raster*));
CSF_FUNC(Im3DVertex*,__cdecl,transform,0x0063CED0,(Im3DVertex*,unsigned,rw::Matrix*,unsigned));
CSF_FUNC(int,__stdcall,end,0x0063CFA0,());
CSF_FUNC(int,__cdecl,renderIndexed,0x0063CFD0,(int,const uint16_t*,int));
CSF_FUNC(int,__cdecl,renderPrimitive,0x0063D130,(int));
CSF_FUNC(Pipeline*,__stdcall,pipeCreate,0x00633710,());
CSF_FUNC(void,__cdecl,pipeDestroy,0x00633780,(Pipeline*));
CSF_FUNC(int,__cdecl,pipeMax,0x006337F0,(int));
CSF_FUNC(Pipeline*,__cdecl,pipeLock,0x00638210,(Pipeline*));
CSF_FUNC(Pipeline*,__cdecl,pipeAdd,0x006383C0,(Pipeline*,unsigned*,void*,...));
CSF_FUNC(Pipeline*,__cdecl,pipeUnlock,0x00637E30,(Pipeline*));
CSF_FUNC(void*,__cdecl,pipeFind,0x00638320,(Pipeline*,const char*,void*,unsigned*));
CSF_FUNC(void*,__cdecl,pipeCallback,0x00592690,(void*,PipelineCallback));
CSF_FUNC(const char**,__stdcall,definition,0x005935D0,());

struct Fake final : DeviceBackend {
    bool failCaps{},fail{};
    int declarations{},vs{},ps{},ib{},draws{},stateCalls{};
    unsigned lastState{},lastValue{},type{},minimum{},vertices{},first{},primitives{};
    int base{};
    struct StreamCall { unsigned n; void* buffer; unsigned offset,stride; };
    std::vector<StreamCall> calls;
    std::array<float,16> matrix{};
    unsigned matrixType{},constantStart{},constantCount{};
    bool constantPixel{},shaderPixel{};
    void* lastHandle{};
    void* raw() override { return this; }
    bool caps(void* p) override { std::memset(p,0x5a,304); return !failCaps; }
    int declaration(void* p) override { ++declarations;lastHandle=p;return fail ? -1 : 0; }
    int vertexShader(void* p) override { ++vs;lastHandle=p;return fail ? -1 : 0; }
    int pixelShader(void* p) override { ++ps;lastHandle=p;return fail ? -1 : 0; }
    int indices(void* p) override { ++ib;lastHandle=p;return fail ? -1 : 0; }
    int stream(unsigned n,void* p,unsigned offset,unsigned stride) override {
        calls.push_back({n,p,offset,stride}); return fail ? -1 : 0;
    }
    int draw(unsigned t,int b,unsigned m,unsigned v,unsigned f,unsigned n) override {
        ++draws;type=t;base=b;minimum=m;vertices=v;first=f;primitives=n;return fail ? -1 : 0;
    }
    int transform(unsigned t,const float* p) override {
        matrixType=t;std::memcpy(matrix.data(),p,64);return fail ? -1 : 0;
    }
    int createShader(bool pixel,const uint32_t*,void** out) override {
        shaderPixel=pixel;if(fail) return -1;*out=this;return 0;
    }
    int constants(bool pixel,unsigned start,const float*,unsigned n) override {
        constantPixel=pixel;constantStart=start;constantCount=n;return fail ? -1 : 0;
    }
    int renderState(unsigned s,unsigned v) override { ++stateCalls;lastState=s;lastValue=v;return fail ? -1 : 0; }
};
static std::vector<Im3DVertex> captured3D;
static unsigned capturedFlags;
static rw::Matrix* capturedWorld;
static int immediateDraws,ends,lastNativeState;
static void* lastNativeValue;
static void capture3D(void* p,int n,rw::Matrix* world,uint32_t flags) {
    captured3D.assign(static_cast<Im3DVertex*>(p),static_cast<Im3DVertex*>(p)+n);
    capturedWorld=world;capturedFlags=flags;
}
static void draw3D(rw::PrimitiveType) { ++immediateDraws; }
static void indexed3D(rw::PrimitiveType,void*,int) { ++immediateDraws; }
static void end3D() { ++ends; }
struct Native2D { float x,y,z,w;uint32_t color;float u,v; };
static std::vector<Native2D> captured2D;
static void indexed2D(rw::PrimitiveType,void* p,int n,void*,int) {
    captured2D.assign(static_cast<Native2D*>(p),static_cast<Native2D*>(p)+n);++immediateDraws;
}
static void primitive2D(rw::PrimitiveType,void* p,int n) { indexed2D(rw::PRIMTYPENONE,p,n,nullptr,0); }
static void nativeSet(int s,void* p) { lastNativeState=s;lastNativeValue=p; }
static void* nativeGet(int s) { assert(s==lastNativeState);return lastNativeValue; }
static int pipelineCalls;
static void* expectedEntry;
static void* expectedAtomic;
static void __cdecl pipelineDraw(void* e,void* a,uint8_t type,uint32_t flags) {
    assert(e==expectedEntry && a==expectedAtomic && type==1 && flags==0x55);++pipelineCalls;
}

int main() {
    assert(!currentDevice() && !caps());
    assert(declaration(nullptr)<0 && !transform(nullptr,0,nullptr,0));
    assert(init(nullptr,0,0) && open(nullptr) && start());
    Fake fake;
    fake.failCaps=true;bindDeviceBackend(&fake);assert(!deviceReady());
    fake.failCaps=false;bindDeviceBackend(&fake);
    assert(deviceReady() && currentDevice()==&fake);
    auto* capBytes=static_cast<unsigned char*>(caps());assert(capBytes && capBytes[303]==0x5a);
    capBytes[0xec]=0x80;assert(caps()==capBytes && capBytes[0xec]==0x80);
    assert(declaration(&fake)==0 && vertexShader(&fake)==0 && pixelShader(nullptr)==0 && indices(&fake)==0);
    assert(fake.declarations==1 && fake.vs==1 && fake.ps==1 && fake.ib==1);
    assert(declaration(&fake)==0 && fake.declarations==2); // no equality cache
    VertexStream s[2]={{&fake,12,36,0,0,0},{nullptr,40,80,0,0,0}};
    assert(streams(s,1)==0 && fake.calls.size()==2);
    assert(fake.calls[0].offset==12 && fake.calls[0].stride==36);
    assert(fake.calls[1].buffer==nullptr && fake.calls[1].offset==0 && fake.calls[1].stride==0);
    assert(streams(s,0)==0 && fake.calls[2].offset==0);
    assert(draw(4,-3,2,9,11,3)==0 && fake.base==-3 && fake.minimum==2 && fake.vertices==9 && fake.first==11 && fake.primitives==3);
    assert(draw(99,0,0,0,0,0)<0);
    float matrix[16];for(int i=0;i<16;++i) matrix[i]=float(i);
    assert(setTransform(2,matrix));float result[16];assert(getTransform(2,result)==2 && !std::memcmp(matrix,result,64));
    fake.fail=true;matrix[0]=123;assert(!setTransform(2,matrix));getTransform(2,result);assert(result[0]==0);
    fake.fail=false;assert(setTransform(256,nullptr));getTransform(256,result);assert(result[0]==1 && result[15]==1);
    uint32_t code=0;void* shader=nullptr;
    assert(createVS(&code,&shader) && shader==&fake && !fake.shaderPixel);
    assert(createPS(&code,&shader) && fake.shaderPixel);
    fake.fail=true;assert(!createVS(&code,&shader) && shader==nullptr);fake.fail=false;
    assert(constantVS(17,matrix,3)==0 && !fake.constantPixel && fake.constantStart==17 && fake.constantCount==3);
    assert(constantPS(4,matrix,2)==0 && fake.constantPixel && fake.constantCount==2);
    assert(state(195,0)==0);fake.lastValue=99; // simulate raw COM write
    assert(state(195,0)==0 && fake.stateCalls==2 && fake.lastValue==0);
    assert(state(999,0)<0);
    rw::RGBA color{255,128,64,255};
    assert(pixel(&color,0x500)==0xffff8040u && pixel(&color,0x600)==0xff8040u);
    assert(pixel(&color,0x100)==0xfe08 && pixel(&color,0x200)==0xfc08);
    assert(pixel(&color,0x300)==0xff84 && pixel(&color,0xa00)==0x7e08);

    auto saved=rw::engine->device;
    rw::engine->device.im3DTransform=capture3D;
    rw::engine->device.im3DRenderPrimitive=draw3D;
    rw::engine->device.im3DRenderIndexedPrimitive=indexed3D;
    rw::engine->device.im3DEnd=end3D;
    rw::engine->device.im2DRenderPrimitive=primitive2D;
    rw::engine->device.im2DRenderIndexedPrimitive=indexed2D;
    rw::engine->device.setRenderState=nativeSet;rw::engine->device.getRenderState=nativeGet;
    Im3DVertex v[3]{};v[0].position={1,2,3};v[0].normal={4,5,6};v[0].color=0x81234567;v[0].u=0.25f;
    rw::Matrix world;world.setIdentity();
    assert(!end() && !renderPrimitive(rw::PRIMTYPETRILIST));
    assert(transform(v,3,&world,0)==v && capturedFlags==0x18 && capturedWorld==&world);
    assert(captured3D[0].color==v[0].color && captured3D[0].normal.z==6 && captured3D[0].u==0.25f);
    assert(!transform(v,3,nullptr,0));
    uint16_t idx[]={0,1,2};assert(renderIndexed(rw::PRIMTYPETRILIST,idx,3));
    idx[2]=3;assert(!renderIndexed(rw::PRIMTYPETRILIST,idx,3));
    assert(renderPrimitive(rw::PRIMTYPETRILIST));assert(end() && ends==1 && !end());
    assert(transform(v,3,nullptr,1)==v && capturedFlags==0x19);
    invalidateDeviceBoundary();assert(!end());getTransform(2,result);assert(result[0]==1);
    assert(!transform(v,10001,nullptr,0) && !transform(v,65537,nullptr,0) && !transform(v,3,nullptr,32));
    uintptr_t engineSlots[75]{};assert(installImmediateDispatch(engineSlots));
    auto indexed=reinterpret_cast<int(__cdecl*)(int,const Im2DVertex*,int,const uint16_t*,int)>(engineSlots[13]);
    Im2DVertex v2[3]{};for(auto& x:v2) {x.recipZ=0.25f;x.color=0xaabbccdd;x.u=0.5f;}
    idx[2]=2;assert(indexed(rw::PRIMTYPETRILIST,v2,3,idx,3));
    assert(captured2D[0].w==4 && captured2D[0].color==0xaabbccdd && captured2D[0].u==0.5f && v2[0].recipZ==0.25f);
    auto prim=reinterpret_cast<int(__cdecl*)(int,const Im2DVertex*,int)>(engineSlots[12]);
    auto line=reinterpret_cast<int(__cdecl*)(const Im2DVertex*,int,int,int)>(engineSlots[10]);
    auto triangle=reinterpret_cast<int(__cdecl*)(const Im2DVertex*,int,int,int,int)>(engineSlots[11]);
    assert(prim(rw::PRIMTYPETRILIST,v2,3) && line(v2,3,0,1) && triangle(v2,3,0,1,2));
    v2[0].recipZ=0;assert(!prim(rw::PRIMTYPETRILIST,v2,3));
    assert(setGameRenderState(6,reinterpret_cast<void*>(1)) && lastNativeState==rw::ZTESTENABLE);
    void* value=nullptr;assert(getGameRenderState(6,&value) && value==reinterpret_cast<void*>(1));
    assert(setGameRenderState(29,reinterpret_cast<void*>(7)) && lastNativeValue==reinterpret_cast<void*>(rw::ALPHAGREATEREQUAL));
    assert(getGameRenderState(29,&value) && value==reinterpret_cast<void*>(7));
    assert(!setGameRenderState(7,nullptr) && !setGameRenderState(29,reinterpret_cast<void*>(4)));
    assert(!setGameRenderState(9,reinterpret_cast<void*>(999)));
    Raster* raster=rasterCreate(4,4,32,rw::Raster::TEXTURE|rw::Raster::C8888);assert(raster);
    assert(setGameRenderState(1,raster) && lastNativeState==rw::TEXTURERASTER && lastNativeValue==rasterNative(raster));
    assert(getGameRenderState(1,&value) && value==raster);
    assert(setGameRenderState(1,nullptr) && rasterDestroy(raster));

    assert(pipeMax(4)==4);Pipeline* p=pipeCreate();assert(p && !pipelinesEmpty());
    assert(!pipeUnlock(p) && pipeLock(p)==p);unsigned nodeIndex=99;
    assert(!pipeAdd(p,nullptr,&fake,nullptr));
    assert(pipeAdd(p,&nodeIndex,const_cast<char**>(definition()),nullptr)==p && nodeIndex==0);
    assert(!pipeAdd(p,nullptr,const_cast<char**>(definition()),nullptr));assert(pipeUnlock(p)==p);
    void* node=pipeFind(p,*definition(),nullptr,&nodeIndex);assert(node && nodeIndex==0);
    assert(!pipeFind(p,"unknown",nullptr,&nodeIndex) && nodeIndex==UINT32_MAX);
    assert(!pipeFind(p,*definition(),node,nullptr));assert(pipeCallback(node,pipelineDraw)==node);
    expectedEntry=matrix;expectedAtomic=v;
    assert(dispatchPipeline(p,matrix,v,1,0x55) && pipelineCalls==1);
    assert(pipeLock(p)==p && !dispatchPipeline(p,matrix,v,1,0x55));assert(pipeUnlock(p)==p);
    pipeDestroy(p);assert(pipelinesEmpty());
    assert(pipeMax(0)==0 && !pipeCreate());assert(pipeMax(1)==1);
    rw::engine->device=saved;
    bindDeviceBackend(nullptr);assert(!currentDevice() && !caps() && declaration(nullptr)<0);
    assert(stop() && closeEngine() && term());
    std::puts("rw_device: PASS (recording device only, no GPU)");
}
#endif
