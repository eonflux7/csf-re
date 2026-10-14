// Standalone boundary, not a reimplementation of the original RW driver.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "device.h"
#include <array>
#include <cstdarg>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#ifdef _WIN32
#include <windows.h>
#include <d3d9.h>
#endif

namespace {
csf_rw::DeviceBackend* backend;
alignas(4) std::array<unsigned char, 304> deviceCaps{};
std::unordered_map<unsigned, std::array<float,16>> transforms;
constexpr int failure = static_cast<int>(0x8876086cu); // D3DERR_INVALIDCALL
constexpr std::array<float,16> identity{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

#ifdef _WIN32
class D3D9Backend final : public csf_rw::DeviceBackend {
public:
    IDirect3DDevice9* device{};
    void* raw() override { return device; }
    bool caps(void* p) override { return SUCCEEDED(device->GetDeviceCaps(static_cast<D3DCAPS9*>(p))); }
    int declaration(void* p) override { return device->SetVertexDeclaration(static_cast<IDirect3DVertexDeclaration9*>(p)); }
    int vertexShader(void* p) override { return device->SetVertexShader(static_cast<IDirect3DVertexShader9*>(p)); }
    int pixelShader(void* p) override { return device->SetPixelShader(static_cast<IDirect3DPixelShader9*>(p)); }
    int indices(void* p) override { return device->SetIndices(static_cast<IDirect3DIndexBuffer9*>(p)); }
    int stream(unsigned n,void* p,unsigned off,unsigned stride) override { return device->SetStreamSource(n,static_cast<IDirect3DVertexBuffer9*>(p),off,stride); }
    int draw(unsigned t,int base,unsigned min,unsigned n,unsigned start,unsigned count) override { return device->DrawIndexedPrimitive(static_cast<D3DPRIMITIVETYPE>(t),base,min,n,start,count); }
    int transform(unsigned t,const float* p) override { return device->SetTransform(static_cast<D3DTRANSFORMSTATETYPE>(t),reinterpret_cast<const D3DMATRIX*>(p)); }
    int createShader(bool pixel,const uint32_t* p,void** out) override {
        if (pixel) return device->CreatePixelShader(reinterpret_cast<const DWORD*>(p),reinterpret_cast<IDirect3DPixelShader9**>(out));
        return device->CreateVertexShader(reinterpret_cast<const DWORD*>(p),reinterpret_cast<IDirect3DVertexShader9**>(out));
    }
    int constants(bool pixel,unsigned start,const float* p,unsigned n) override {
        return pixel ? device->SetPixelShaderConstantF(start,p,n) : device->SetVertexShaderConstantF(start,p,n);
    }
    int renderState(unsigned s,unsigned v) override { return device->SetRenderState(static_cast<D3DRENDERSTATETYPE>(s),v); }
} d3d9;
static_assert(sizeof(D3DCAPS9) == 304);
#endif

// 006dc290 caches the real COM pointer (+5c) for screenshots/gamma/caps.
void* __stdcall currentDevice() { return backend ? backend->raw() : nullptr; }
// 006dc290 mutates caps +ec. Keep a stable writable snapshot, not a temporary
// or pointer into a native driver whose caps the game would silently corrupt.
void* __stdcall caps() { return backend ? deviceCaps.data() : nullptr; }
// 0070a070 passes real shader/declaration COM handles from game-owned water
// resources. Return HRESULT; all calls must reach the device: raw game COM
// calls make equality-suppression unsafe. Do not cast an owning RW facade.
int __cdecl declaration(void* p) { return backend ? backend->declaration(p) : failure; }
// 0070a070 installs the water object's raw vertex-shader handle, then null.
int __cdecl vertexShader(void* p) { return backend ? backend->vertexShader(p) : failure; }
// 0070a070 similarly binds/clears the raw pixel shader; no RW object conversion.
int __cdecl pixelShader(void* p) { return backend ? backend->pixelShader(p) : failure; }
// 0070a070 takes the raw index buffer from old instance-header +20.
int __cdecl indices(void* p) { return backend ? backend->indices(p) : failure; }
// 0070a070 supplies two 16-byte streams at original mesh header +28. The
// second argument selects real offsets; false binds both with zero offset.
int __cdecl streams(const csf_rw::VertexStream* p,int offsets) {
    if (!backend || !p) return failure;
    int result=0;
    for (unsigned n=0;n<2;++n) {
        int hr=backend->stream(n,p[n].buffer,p[n].buffer && offsets ? p[n].offset : 0,p[n].buffer ? p[n].stride : 0);
        if (hr<0) result=hr;
    }
    return result;
}
// 0070a070 forwards D3D primitive numbers and signed base vertex from the
// original instance records. This is not rw::PrimitiveType (different enum).
int __cdecl draw(unsigned t,int base,unsigned min,unsigned n,unsigned start,unsigned count) {
    if (!backend || t<1 || t>6) return failure;
    return backend->draw(t,base,min,n,start,count);
}
// 0070d700 saves/restores view and projection D3DMATRIX values. Null means
// identity, NOT rw::Matrix: all 16 floats are the D3D value ABI.
int __cdecl setTransform(unsigned t,const float* p) {
    if (!backend || (t!=2 && t!=3 && (t<16 || t>23) && (t<256 || t>511))) return 0;
    const float* src=p ? p : identity.data();
    int hr=backend->transform(t,src);
    if (hr<0) return 0;
    std::array<float,16> value; std::memcpy(value.data(),src,64);
    try { transforms[t]=value; } catch (...) { return 0; }
    return 1;
}
// 0070d700 consumes output bytes, not a return boolean. Original absent
// transforms read as identity; retain that policy and invalidate on reset.
unsigned __cdecl getTransform(unsigned t,float* out) {
    if (out) {
        auto it=transforms.find(t);
        std::memcpy(out,it==transforms.end() ? identity.data() : it->second.data(),64);
    }
    return t;
}
// 00709990 supplies compiled DWORD tokens and address of its COM handle.
// Original shader creators return BOOL, while constant setters return HRESULT.
int __cdecl createVertexShader(const uint32_t* p,void** out) {
    if (!out) return 0;
    *out=nullptr; return backend && p && backend->createShader(false,p,out)>=0;
}
// 00709990 expects BOOL and its pixel-shader COM handle written through out.
int __cdecl createPixelShader(const uint32_t* p,void** out) {
    if (!out) return 0;
    *out=nullptr; return backend && p && backend->createShader(true,p,out)>=0;
}
// 00709bc0 uploads rows in float4 register units, not float/byte counts.
int __cdecl vertexConstants(unsigned start,const float* p,unsigned n) {
    return backend && (p || !n) ? backend->constants(false,start,p,n) : failure;
}
// 00709bc0 uploads pixel constants in the same float4-register units.
int __cdecl pixelConstants(unsigned start,const float* p,unsigned n) {
    return backend && (p || !n) ? backend->constants(true,start,p,n) : failure;
}
// 0070a070 toggles D3D state 27 around draws. Always forward, including
// identical values after direct COM writes. No stale boundary cache survives
// GetCurrentD3DDevice. Native librw cache coordination remains an integration
// requirement; this does NOT claim to invalidate its private RW state cache.
int __cdecl renderState(unsigned s,unsigned value) {
    return backend && s<210 ? backend->renderState(s,value) : failure;
}
// 00536160 passes a colour and the raster's compact format byte <<8. Pixel
// packing is a POD ABI bridge; unsupported/paletted formats fail to zero.
uint32_t __cdecl rgbaPixel(const rw::RGBA* c,int format) {
    if (!c) return 0;
    unsigned r=c->red,g=c->green,b=c->blue,a=c->alpha;
    switch (format&0xf00) {
    case 0x100: return ((a>=128)<<15)|((r>>3)<<10)|((g>>3)<<5)|(b>>3);
    case 0x200: return ((r>>3)<<11)|((g>>2)<<5)|(b>>3);
    case 0x300: return ((a>>4)<<12)|((r>>4)<<8)|((g>>4)<<4)|(b>>4);
    case 0x500: return (a<<24)|(r<<16)|(g<<8)|b;
    case 0x600: return (r<<16)|(g<<8)|b;
    case 0xa00: return ((r>>3)<<10)|((g>>3)<<5)|(b>>3);
    default: return 0;
    }
}
}

namespace csf_rw {
void invalidateDeviceBoundary() { transforms.clear(); resetImmediateBoundary(); }
void bindDeviceBackend(DeviceBackend* p) {
    backend=nullptr; deviceCaps.fill(0); invalidateDeviceBoundary();
    if (p && p->raw() && p->caps(deviceCaps.data())) backend=p;
}
bool bindD3D9Device(void* p) {
#ifdef _WIN32
    d3d9.device=static_cast<IDirect3DDevice9*>(p);
    bindDeviceBackend(p ? &d3d9 : nullptr); return backend!=nullptr;
#else
    (void)p; bindDeviceBackend(nullptr); return false;
#endif
}
bool deviceReady() { return backend!=nullptr; }
}
HOOK(0x0062B670,currentDevice,"boundary-rw");
HOOK(0x0062B720,declaration,"boundary-rw");
HOOK(0x0062B760,vertexShader,"boundary-rw");
HOOK(0x0062B7A0,pixelShader,"boundary-rw");
HOOK(0x0062B840,streams,"boundary-rw");
HOOK(0x0062B970,indices,"boundary-rw");
HOOK(0x0062B9A0,draw,"boundary-rw");
HOOK(0x0062BA10,setTransform,"boundary-rw");
HOOK(0x0062BB70,getTransform,"boundary-rw");
HOOK(0x0062C060,createVertexShader,"boundary-rw");
HOOK(0x0062C0A0,vertexConstants,"boundary-rw");
HOOK(0x0062C0C0,createPixelShader,"boundary-rw");
HOOK(0x0062C0F0,pixelConstants,"boundary-rw");
HOOK(0x0062C110,caps,"boundary-rw");
HOOK(0x0062D650,renderState,"boundary-rw");
HOOK(0x00625870,rgbaPixel,"boundary-rw");

// Rx policy is deliberately a ONE-NODE callback carrier, not an Rx graph.
// 0070a230 builds precisely this shape, then 00709900 stores it at atomic +70.
// Slice 4/6 must import the atomic's +70 and call dispatchPipeline only after
// producing an ORIGINAL-LAYOUT instance header. Never pass librw's header to
// 0070a070: it reads old header +20/+28/+48/+4c and 36-byte mesh records.
namespace csf_rw { struct Pipeline { uintptr_t slots[13]; }; }
namespace {
struct Node { uintptr_t slots[10]; };
struct NodeData { uintptr_t slots[4]; };
struct OneNode { csf_rw::Pipeline facade; Node node; NodeData data; };
static_assert(sizeof(void*)!=4 || (sizeof(csf_rw::Pipeline)==0x34 && sizeof(Node)==0x28 && sizeof(NodeData)==0x10));
// Private descriptor, only its name pointer is published (game dereferences
// getter return to FindNodeByName). No original node-definition internals.
const char* definitionName="csfD3D9AtomicAllInOne";
std::unordered_set<csf_rw::Pipeline*> pipelines;
unsigned maxNodes=1;
bool valid(csf_rw::Pipeline* p) { return pipelines.find(p)!=pipelines.end(); }
OneNode* record(csf_rw::Pipeline* p) { return reinterpret_cast<OneNode*>(p); }
// 0070a230 creates an opaque carrier; callbacks/data are NOT native pointers.
csf_rw::Pipeline* __stdcall pipelineCreate() {
    if (!rw::engine || !maxNodes) return nullptr;
    auto* r=static_cast<OneNode*>(rw::Engine::memfuncs.rwmalloc(sizeof(OneNode),rw::MEMDUR_GLOBAL));
    if (!r) return nullptr;
    std::memset(r,0,sizeof(*r));
    try { pipelines.insert(&r->facade); }
    catch (...) { rw::Engine::memfuncs.rwfree(r); return nullptr; }
    return &r->facade;
}
// 0070a230/0070d560 destroy the carrier after detaching it from atomics.
void __cdecl pipelineDestroy(csf_rw::Pipeline* p) {
    if (!valid(p)) return;
    pipelines.erase(p); rw::Engine::memfuncs.rwfree(record(p));
}
// 0070d5c0 sets a node budget. Retain the setting, but it does not enable
// unsupported graph fragments (AddFragment remains explicitly single-node).
int __cdecl pipelineMax(int n) { if (n<0) return 0; maxNodes=n; return n; }
// 0070a230 expects pointer/null, not a BOOL and not native ObjPipeline.
csf_rw::Pipeline* __cdecl pipelineLock(csf_rw::Pipeline* p) {
    if (!valid(p)) return nullptr;
    p->slots[0]=1; return p;
}
// 0070a230 requires one fragment with a NULL terminator. Do not traverse,
// execute or copy a game-supplied node definition or accept arbitrary graphs.
csf_rw::Pipeline* __cdecl addFragment(csf_rw::Pipeline* p,unsigned* first,void* def,...) {
    if (!valid(p) || !p->slots[0] || p->slots[1] || !maxNodes || def!=&definitionName) return nullptr;
    va_list args; va_start(args,def); void* next=va_arg(args,void*); va_end(args);
    if (next) return nullptr;
    auto* r=record(p);
    r->node.slots[0]=reinterpret_cast<uintptr_t>(&definitionName);
    r->node.slots[5]=reinterpret_cast<uintptr_t>(&r->data);
    p->slots[1]=1; p->slots[2]=reinterpret_cast<uintptr_t>(&r->node);
    if (first) *first=0;
    return p;
}
// 0070a230 passes spurious trailing arguments left from preceding calls;
// original Unlock reads only pipeline. Empty/unsupported shapes fail.
csf_rw::Pipeline* __cdecl pipelineUnlock(csf_rw::Pipeline* p) {
    if (!valid(p) || !p->slots[0] || p->slots[1]!=1) return nullptr;
    p->slots[0]=0; return p;
}
// 0070a230 passes the getter's name; return a stable facade node with private
// data at +14 (SetRenderCallBack consumes it), never a librw ObjPipeline.
void* __cdecl findNode(csf_rw::Pipeline* p,const char* name,void* after,unsigned* index) {
    if (index) *index=UINT32_MAX;
    if (!valid(p) || !name || p->slots[1]!=1 || after || std::strcmp(name,definitionName)!=0) return nullptr;
    if (index) *index=0;
    return &record(p)->node;
}
// 0070a230 installs 0070a070. Four trailing caller arguments are stale cdecl
// pushes, NOT part of this slot's two-argument contract.
void* __cdecl setRenderCallback(void* node,csf_rw::PipelineCallback cb) {
    for (auto* p:pipelines) if (&record(p)->node==node) {
        record(p)->data.slots[3]=reinterpret_cast<uintptr_t>(cb); return node;
    }
    return nullptr;
}
// Misnamed RwOsGetFileInterface in the inventory: 0070a230 uses its result as
// an all-in-one NODE DEFINITION and dereferences its name pointer. This extra
// slice-2 address is required by the supported one-node policy.
const char** __stdcall allInOneDefinition() { return &definitionName; }
}
namespace csf_rw {
bool pipelinesEmpty() { return pipelines.empty(); }
bool dispatchPipeline(Pipeline* p,void* resourceEntry,void* atomic,uint8_t type,uint32_t flags) {
    if (!deviceReady() || !valid(p) || p->slots[0] || p->slots[1]!=1 || !resourceEntry || !atomic) return false;
    auto cb=reinterpret_cast<PipelineCallback>(record(p)->data.slots[3]);
    if (!cb) return false;
    cb(resourceEntry,atomic,type,flags); return true;
}
}
HOOK(0x00633710,pipelineCreate,"boundary-rw");
HOOK(0x00633780,pipelineDestroy,"boundary-rw");
HOOK(0x006337F0,pipelineMax,"boundary-rw");
HOOK(0x00638210,pipelineLock,"boundary-rw");
HOOK(0x006383C0,addFragment,"boundary-rw");
HOOK(0x00637E30,pipelineUnlock,"boundary-rw");
HOOK(0x00638320,findNode,"boundary-rw");
HOOK(0x00592690,setRenderCallback,"boundary-rw");
HOOK(0x005935D0,allInOneDefinition,"boundary-rw");
#endif
