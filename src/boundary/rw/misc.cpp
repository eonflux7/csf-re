// Small standalone contracts, derived from the executable and MIT librw.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "animation.h"
#include "world.h"
#include <cstring>
#include <cmath>
#include <vector>
using namespace csf_rw;
CSF_GLOBAL(int, miscPVSWorldOffset, 0x008BE7A4);
CSF_GLOBAL(int, miscPVSSectorOffset, 0x008BE7A8);
CSF_GLOBAL(World*, miscPVSWorld, 0x008BE7A0);
CSF_GLOBAL(int, miscPVSIndex, 0x008BE7C0);
CSF_GLOBAL(rw::V3d, miscPVSPosition, 0x008BE7C4);
CSF_GLOBAL(void*, miscPVSSector, 0x008BE7D0);
CSF_GLOBAL(int, miscMorphAtomic, 0x008B09E4);
CSF_GLOBAL(int, miscMorphGeometry, 0x008B09E8);
CSF_FUNC(World*,__cdecl,restoreSectorCallback,0x0058CC60,(World*,SectorRender));
CSF_FUNC(rw::Matrix*,__cdecl,normalizeStreamMatrix,0x006246A0,(rw::Matrix*,const rw::Matrix*));
namespace {
rw::RawMatrix activeWorld;
bool hasWorld=false;
void __cdecl setWorld(rw::Matrix* m) {
    hasWorld=m!=nullptr;
    if(m) rw::convMatrix(&activeWorld,m);
}
rw::RawMatrix* __cdecl composed(rw::RawMatrix* out) {
    if(!out || !rw::engine || !rw::engine->currentCamera) return nullptr;
    // These are populated by librw's d3d::beginUpdate, including its X-axis
    // handedness flip and D3D depth projection. Never use the ABI viewMatrix
    // or the boundary's SetTransform snapshots (native cameras bypass them).
    auto* c=rw::engine->currentCamera;
    rw::RawMatrix result;
    // Same evaluation order as d3d::uploadMatrices in d3drender.cpp.
    if(hasWorld) {
        rw::RawMatrix worldView;rw::RawMatrix::mult(&worldView,&activeWorld,&c->devView);
        rw::RawMatrix::mult(&result,&worldView,&c->devProj);
    } else rw::RawMatrix::mult(&result,&c->devView,&c->devProj);
    rw::RawMatrix::transpose(out,&result); return out;
}
int __cdecl setTexture(Texture* t,unsigned stage) {
    if(stage>=8 || !rw::engine) return 0;
    auto* n=textureNative(t); if(t && !n) return 0;
#ifdef RW_D3D9
    rw::d3d::setTexture(stage,n);
#else
    if(stage==0) rw::SetRenderStatePtr(rw::TEXTURERASTER,n?n->raster:nullptr);
#endif
    return 1;
}
uint32_t __cdecl matrixSize(const rw::Matrix*) { return 64; }
rw::Matrix* __cdecl matrixWrite(rw::Matrix* m,Stream* s) {
    auto* n=streamNative(s); if(!m || !n || !streamPrepareWrite(s,76)) return nullptr;
    rw::Matrix v=*m;
    if((v.flags&3)==3) normalizeStreamMatrix(&v,m);
    float values[13];
    std::memcpy(values,&v.right,12); std::memcpy(values+3,&v.up,12);
    std::memcpy(values+6,&v.at,12); std::memcpy(values+9,&v.pos,12);
    uint32_t flags=v.flags&3; std::memcpy(values+12,&flags,4);
    uint32_t h[]={13,64,0x1c020037,1,52,0x1c020037};
    return n->write8(h,24)==24 && n->write8(values,52)==52?m:nullptr;
}
int __cdecl animationSize(Animation* a) {
    if(!a || !a->scheme) return 0;
    if(a->scheme->getSize) return 20+reinterpret_cast<int (__cdecl *)(Animation*)>(a->scheme->getSize)(a);
    if(a->scheme->id==1) return 20+a->numFrames*36;
    if(a->scheme->id==2) return 44+a->numFrames*22;
    return 0;
}
int __cdecl systemRequest(void* device,int request,void* out,void* in,int n) {
    // The incoming device is the ORIGINAL facade, not rw::Device. Translate
    // device IDs; never call an original executable pointer from its table.
    if(request==22) { if(!out) return 0; uint16_t id=2;std::memcpy(out,&id,2);return 1; }
    // Original request numbers do not match librw::DeviceReq.
    int mapped=-1;
    switch(request) {
    case 13:mapped=rw::DEVICEGETNUMSUBSYSTEMS;break;
    case 14:mapped=rw::DEVICEGETSUBSSYSTEMINFO;break;
    case 15:mapped=rw::DEVICEGETCURRENTSUBSYSTEM;break;
    case 16:mapped=rw::DEVICESETSUBSYSTEM;break;
    }
    if(mapped>=0 && rw::engine && rw::engine->device.system(rw::DeviceReq(mapped),out,n)) return 1;
    switch(request) {
    case 13: if(!out) return 0; *static_cast<int*>(out)=1; return 1;
    case 14: if(n || !out) return 0; std::strcpy(static_cast<char*>(out),"Only rendering sub system"); return 1;
    case 15: if(!out) return 0; *static_cast<int*>(out)=0; return 1;
    case 16:return n==0;
    case 17:case 18:return 1;
    default:return 0;
    }
}
World* __cdecl pvsUnhook(World* w) {
    if(!w || miscPVSWorldOffset<0) return w;
    auto* p=reinterpret_cast<char*>(w)+miscPVSWorldOffset;
    int hooked; std::memcpy(&hooked,p+20,4);
    if(hooked) {
        // Original saved RpWorldSectorCallBackRender at +40.
        SectorRender callback=nullptr; std::memcpy(&callback,p+40,sizeof(callback));
        restoreSectorCallback(w,callback); std::memset(p+20,0,4); std::memset(p+40,0,sizeof(callback));
    }
    return w;
}
Sector* __cdecl pvsPosition(World* w,const rw::V3d* pos) {
    if(!w || !pos || miscPVSSectorOffset<0) return nullptr;
    miscPVSWorld=w; miscPVSPosition=*pos; miscPVSIndex=-1; miscPVSSector=nullptr;
    std::vector<Sector*> pending; if(w->root) pending.push_back(w->root);
    int index=0;
    while(!pending.empty()) {
        auto* s=pending.back(); pending.pop_back();
        if(s->type>=0) { auto* p=reinterpret_cast<Plane*>(s); if(p->right) pending.push_back(p->right); if(p->left) pending.push_back(p->left); continue; }
        char* p=reinterpret_cast<char*>(s)+miscPVSSectorOffset;
        rw::BBox bounds; std::memcpy(&bounds,p+28,sizeof(bounds));
        if(miscPVSIndex==-1 || (pos->x<=bounds.sup.x && pos->x>=bounds.inf.x && pos->y<=bounds.sup.y && pos->y>=bounds.inf.y && pos->z<=bounds.sup.z && pos->z>=bounds.inf.z)) {
            miscPVSIndex=index; miscPVSSector=p;
        }
        std::memset(p+16,0,4); ++index;
    }
    return miscPVSSector?reinterpret_cast<Sector*>(static_cast<char*>(miscPVSSector)-miscPVSSectorOffset):nullptr;
}
struct MorphSequence { int flags; int16_t start,end; float duration; };
struct MorphAtomic { MorphSequence* sequence; float time; Geometry* geometry; };
struct MorphGeometry { int count; float (__cdecl *advance)(Atomic*,float); MorphSequence* sequence; };
Atomic* __cdecl morphTime(Atomic* a,float dt) {
    if(!a || !a->geometry) return nullptr;
    if(miscMorphAtomic<0 || miscMorphGeometry<0) return a;
    char* storage=reinterpret_cast<char*>(a)+miscMorphAtomic;
    MorphAtomic state; MorphGeometry g;
    std::memcpy(&state,storage,sizeof(state));
    std::memcpy(&g,reinterpret_cast<char*>(a->geometry)+miscMorphGeometry,sizeof(g));
    if(state.geometry!=a->geometry || !state.sequence) { state.sequence=g.sequence; state.geometry=a->geometry; }
    if(!state.sequence) { std::memcpy(storage,&state,sizeof(state)); return a; }
    state.time+=dt;std::memcpy(storage,&state,sizeof(state));
    while(state.time>state.sequence->duration) {
        if(!g.advance) return nullptr;
        state.time=g.advance(a,state.time);
        // The callback may switch the active sequence through the plugin.
        MorphAtomic changed;std::memcpy(&changed,storage,sizeof(changed));
        state.sequence=changed.sequence;state.geometry=changed.geometry;
        if(!state.sequence) return nullptr;
        std::memcpy(storage,&state,sizeof(state));
    }
    auto& i=a->interpolator; i.flags|=3; i.start=state.sequence->start; i.end=state.sequence->end;
    i.position=state.time; i.time=state.sequence->duration; i.reciprocalTime=1/i.time;
    if(!(i.flags&4) && a->object.object.parent) objectFrameChanged(&a->object);
    return a;
}
int32_t __stdcall noCapture(const void*,void** out,void*) {
    if(out) *out=nullptr;
    return static_cast<int32_t>(0x88780078u); // DSERR_NODRIVER, offline voice disabled
}
}
HOOK(0x0059B990,setWorld,"boundary-rw");
HOOK(0x0059BA20,composed,"boundary-rw");
HOOK(0x0062F950,setTexture,"boundary-rw");
HOOK(0x0063BD90,matrixSize,"boundary-rw");
HOOK(0x0063BDA0,matrixWrite,"boundary-rw");
HOOK(0x0061EF90,animationSize,"boundary-rw");
HOOK(0x006269C0,systemRequest,"boundary-rw");
HOOK(0x005861F0,pvsUnhook,"boundary-rw");
HOOK(0x005860C0,pvsPosition,"boundary-rw");
HOOK(0x005774B0,morphTime,"boundary-rw");
HOOK(0x007F78EC,noCapture,"boundary-rw");
#endif
