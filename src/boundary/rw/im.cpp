#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "device.h"
#include "startup.h"
#include <cmath>
#include <vector>

namespace {
struct DeviceIm2DVertex { float x,y,z,w; uint32_t color; float u,v; };
std::vector<csf_rw::Im3DVertex> batch;
bool transformed;
// Unpatched librw D3D9 allocates fixed 10,000-entry immediate buffers. Original
// Transform permits 65,536, but upstream Im3D has no capacity check at all.
// Until chunked uploads exist, reject oversized batches instead of corrupting
// memory or claiming success for upstream Im2D's silent oversized no-op.
constexpr int immediateCapacity=10000;
bool ready() { return csf_rw::deviceReady() && rw::engine && rw::Engine::state==rw::Engine::Started; }
bool primitive(int t,unsigned n) {
    switch(t) {
    case rw::PRIMTYPELINELIST: return n>=2 && n%2==0;
    case rw::PRIMTYPEPOLYLINE: return n>=2;
    case rw::PRIMTYPETRILIST: return n>=3 && n%3==0;
    case rw::PRIMTYPETRISTRIP: case rw::PRIMTYPETRIFAN: return n>=3;
    case rw::PRIMTYPEPOINTLIST: return n>=1;
    default: return false;
    }
}
bool indicesValid(const uint16_t* p,int n,unsigned vertices) {
    if (!p || n<=0 || n>immediateCapacity) return false;
    for (int i=0;i<n;++i) if (p[i]>=vertices) return false;
    return true;
}
bool convert(const csf_rw::Im2DVertex* p,int n,std::vector<DeviceIm2DVertex>& out) {
    if (!p || n<=0 || n>immediateCapacity) return false;
    try { out.resize(n); } catch (...) { return false; }
    for(int i=0;i<n;++i) {
        if (!std::isfinite(p[i].recipZ) || p[i].recipZ==0) return false;
        // D3D9 librw uses camera Z at +c and divides in its vertex shader;
        // the original game writes reciprocal Z at +c. UVs stay unmultiplied.
        float w=1.0f/p[i].recipZ; if (!std::isfinite(w)) return false;
        out[i]={p[i].x,p[i].y,p[i].z,w,p[i].color,p[i].u,p[i].v};
    }
    return true;
}
// Engine +30/+34 callers (00726770, 00738870) pass RW primitive numbers,
// original 28-byte vertices and 16-bit indices. These are cdecl dispatch
// entries, not original-address slots in the allocated slice inventory.
int __cdecl im2DPrimitive(int t,const csf_rw::Im2DVertex* p,int n) {
    std::vector<DeviceIm2DVertex> native;
    ++csf_rw::im2DFrameCalls[0];
    if (!ready() || !primitive(t,n>0 ? n : 0) || !convert(p,n,native)) return ++csf_rw::im2DFrameCalls[1], 0;
    rw::im2d::RenderPrimitive(static_cast<rw::PrimitiveType>(t),native.data(),n); return 1;
}
int __cdecl im2DIndexed(int t,const csf_rw::Im2DVertex* p,int n,const uint16_t* idx,int count) {
    std::vector<DeviceIm2DVertex> native;
    ++csf_rw::im2DFrameCalls[0];
    if (!ready() || n<=0 || !primitive(t,count>0 ? count : 0) ||
        !indicesValid(idx,count,n) || !convert(p,n,native)) return ++csf_rw::im2DFrameCalls[1], 0;
    rw::im2d::RenderIndexedPrimitive(static_cast<rw::PrimitiveType>(t),native.data(),n,const_cast<uint16_t*>(idx),count); return 1;
}
int __cdecl im2DLine(const csf_rw::Im2DVertex* p,int n,int a,int b) {
    if (a<0 || b<0 || a>=n || b>=n || n>65536) return 0;
    uint16_t idx[]={static_cast<uint16_t>(a),static_cast<uint16_t>(b)};
    return im2DIndexed(rw::PRIMTYPELINELIST,p,n,idx,2);
}
int __cdecl im2DTriangle(const csf_rw::Im2DVertex* p,int n,int a,int b,int c) {
    if (a<0 || b<0 || c<0 || a>=n || b>=n || c>=n || n>65536) return 0;
    uint16_t idx[]={static_cast<uint16_t>(a),static_cast<uint16_t>(b),static_cast<uint16_t>(c)};
    return im2DIndexed(rw::PRIMTYPETRILIST,p,n,idx,3);
}
// 006c4330 brackets a particle batch; 006e3080 writes stride 36 and colour
// +18. Same D3D9 POD vertex layout, but copy to keep storage valid until End.
// Original Transform adds XYZ|RGBA even when callers pass flags=0. Reject the
// librw LIGHTING extension: game flag 32 is not an established extension ABI.
csf_rw::Im3DVertex* __cdecl im3DTransform(csf_rw::Im3DVertex* p,unsigned n,rw::Matrix* world,unsigned flags) {
    if (!ready() || !p || !n || n>immediateCapacity || transformed || (flags&~31u)) return nullptr;
    try { batch.assign(p,p+n); } catch (...) { return nullptr; }
    rw::im3d::Transform(batch.data(),static_cast<int>(n),world,flags|0x18);
    transformed=true; return p;
}
// 006c4330 consumes BOOL; End without a current Transform fails. librw End
// returns void, so the boundary owns the batch-lifetime/state contract.
int __stdcall im3DEnd() {
    if (!ready() || !transformed) return 0;
    rw::im3d::End(); transformed=false; batch.clear(); return 1;
}
// 006c4330/006e3250 pass uint16 indices. Bound-check before upstream uploads
// buffers (upstream assumes valid primitive/index counts).
int __cdecl im3DIndexed(int t,const uint16_t* idx,int n) {
    if (!ready() || !transformed || !primitive(t,n>0 ? n : 0) || !indicesValid(idx,n,batch.size())) return 0;
    rw::im3d::RenderIndexedPrimitive(static_cast<rw::PrimitiveType>(t),const_cast<uint16_t*>(idx),n); return 1;
}
// 006c4330/00705300 draw the current transformed batch, not new vertices.
int __cdecl im3DPrimitive(int t) {
    if (!ready() || !transformed || !primitive(t,batch.size())) return 0;
    rw::im3d::RenderPrimitive(static_cast<rw::PrimitiveType>(t)); return 1;
}
int nativeState(int s) {
    // Numeric RW3 state ids recovered from game dispatch calls. librw starts
    // at zero and changes ordering; passing them straight through is wrong.
    switch(s) {
    case 1:return rw::TEXTURERASTER; case 2:return rw::TEXTUREADDRESS;
    case 3:return rw::TEXTUREADDRESSU; case 4:return rw::TEXTUREADDRESSV;
    case 6:return rw::ZTESTENABLE; case 8:return rw::ZWRITEENABLE;
    case 9:return rw::TEXTUREFILTER; case 10:return rw::SRCBLEND;
    case 11:return rw::DESTBLEND; case 12:return rw::VERTEXALPHA;
    case 14:return rw::FOGENABLE; case 15:return rw::FOGCOLOR;
    case 20:return rw::CULLMODE; case 21:return rw::STENCILENABLE;
    case 22:return rw::STENCILFAIL; case 23:return rw::STENCILZFAIL;
    case 24:return rw::STENCILPASS; case 25:return rw::STENCILFUNCTION;
    case 26:return rw::STENCILFUNCTIONREF; case 27:return rw::STENCILFUNCTIONMASK;
    case 28:return rw::STENCILFUNCTIONWRITEMASK; case 29:return rw::ALPHATESTFUNC;
    case 30:return rw::ALPHATESTREF; default:return -1;
    }
}
}
namespace csf_rw {
void resetImmediateBoundary() { transformed=false; batch.clear(); }
int setGameRenderState(int s,void* value) {
    int n=nativeState(s); if (!ready() || n<0) return 0;
    uintptr_t word=reinterpret_cast<uintptr_t>(value);
    if (((s==2 || s==3 || s==4) && (word<1 || word>4)) ||
        (s==9 && (word<1 || word>6)) ||
        ((s==10 || s==11) && (word<1 || word>11)) ||
        (s==20 && (word<1 || word>3)) ||
        ((s>=22 && s<=25) && (word<1 || word>8))) return 0;
    if (s==1) {
        auto* raster=rasterNative(static_cast<Raster*>(value));
        if (value && !raster) return 0;
        value=raster;
    } else if (s==29) {
        uintptr_t v=reinterpret_cast<uintptr_t>(value);
        if (v==8) value=reinterpret_cast<void*>(rw::ALPHAALWAYS);
        else if (v==7) value=reinterpret_cast<void*>(rw::ALPHAGREATEREQUAL);
        else if (v==2) value=reinterpret_cast<void*>(rw::ALPHALESS);
        else return 0;
    }
    rw::SetRenderStatePtr(n,value); return 1;
}
int getGameRenderState(int s,void* out) {
    int n=nativeState(s); if (!ready() || n<0 || !out) return 0;
    void* v=rw::GetRenderStatePtr(n);
    if (s==1) {
        void* native=v; v=rasterFacade(static_cast<rw::Raster*>(v));
        if (native && !v) return 0;
    }
    else if (s==29) {
        uintptr_t x=reinterpret_cast<uintptr_t>(v);
        if (x>rw::ALPHALESS) return 0;
        v=reinterpret_cast<void*>(x==rw::ALPHAALWAYS ? 8 : x==rw::ALPHAGREATEREQUAL ? 7 : 2);
    }
    *static_cast<void**>(out)=v; return 1;
}
bool installImmediateDispatch(void* facade) {
    if (!facade || !rw::engine) return false;
    auto* slots=static_cast<uintptr_t*>(facade);
    slots[8]=reinterpret_cast<uintptr_t>(&setGameRenderState);
    slots[9]=reinterpret_cast<uintptr_t>(&getGameRenderState);
    slots[10]=reinterpret_cast<uintptr_t>(&im2DLine);
    slots[11]=reinterpret_cast<uintptr_t>(&im2DTriangle);
    slots[12]=reinterpret_cast<uintptr_t>(&im2DPrimitive);
    slots[13]=reinterpret_cast<uintptr_t>(&im2DIndexed);
    return true;
}
}
HOOK(0x0063CED0,im3DTransform,"boundary-rw");
HOOK(0x0063CFA0,im3DEnd,"boundary-rw");
HOOK(0x0063CFD0,im3DIndexed,"boundary-rw");
HOOK(0x0063D130,im3DPrimitive,"boundary-rw");
#endif
