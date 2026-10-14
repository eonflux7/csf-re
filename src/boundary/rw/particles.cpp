// Independent game-facing particle ownership boundary; no SDK and no native
// particle toolkit in librw. Unimplemented advanced/stream callbacks retain
// their address-keyed fatal slots, never success/no-op substitutes.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "particles.h"
#include "startup.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <limits>
#include <vector>

namespace {
using namespace csf_rw;
std::vector<ParticlePropTable*> tables;
std::vector<ParticleClass*> emitterClasses, particleClasses;
std::vector<ParticleEmitter*> emitters;
void* allocate(size_t n) {
    if (n>UINT32_MAX || rw::Engine::state!=rw::Engine::Started) return nullptr;
    return rw::Engine::memfuncs.rwmalloc(n,0x30130);
}
void release(void* p) { if(p) rw::Engine::memfuncs.rwfree(p); }
template<class T> T* zeroAllocate(size_t n=sizeof(T)) {
    auto* p=static_cast<T*>(allocate(n)); if(p) std::memset(p,0,n); return p;
}
template<class T> bool remember(std::vector<T*>& v,T* p) {
    try { v.push_back(p); } catch(...) { return false; } return true;
}
template<class T> void forget(std::vector<T*>& v,T* p) {
    auto i=std::find(v.begin(),v.end(),p); if(i!=v.end()) v.erase(i);
}
template<class T> void ringInsert(T*& tail,T* p) {
    p->next=tail ? tail->next : p; if(tail) tail->next=p; tail=p;
}
template<class T> void ringRemove(T*& tail,T* p) {
    if(!tail) return;
    auto* prev=tail; do { if(prev->next==p) {
        if(prev==p) tail=nullptr;
        else { prev->next=p->next; if(tail==p) tail=prev; }
        p->next=p; return;
    } prev=prev->next; } while(prev!=tail);
}
ParticlePropTable* tableTail;
ParticleClass *emitterTail, *particleTail;
bool tableMatches(ParticlePropTable* t,int n,const int32_t* ids,const int32_t* sizes) {
    if(t->numProperties!=n) return false;
    // Original 57C6F0 compares by ID (not array order), against padded sizes.
    for(int i=0;i<n;i++) {
        bool found=false;
        for(int j=0;j<n;j++) if(ids[i]==t->ids[j]) {
            if(sizes[i]!=t->sizes[j]) return false;
            found=true; break;
        }
        if(!found) return false;
    }
    return true;
}
ParticlePropTable* __cdecl propCreate(int n,const int32_t* ids,const int32_t* sizes) {
    if(n<=0 || n>4096 || !ids || !sizes) return nullptr;
    uint32_t total=0, sum=0;
    for(int i=0;i<n;i++) {
        if(sizes[i]<0 || sizes[i]>INT32_MAX-3) return nullptr;
        uint32_t padded=(uint32_t(sizes[i])+3)&~3u;
        if(total>uint32_t(INT32_MAX)-padded) return nullptr;
        total+=padded; sum+=uint32_t(ids[i]);
    }
    for(auto* t:tables) if(tableMatches(t,n,ids,sizes)) { ++t->refCount; return t; }
    // Four n+1 arrays; offsets[n] is the end sentinel. The original leaves
    // several padding/sentinel words undefined; we deliberately zero them.
    size_t bytes=sizeof(ParticlePropTable)+4*size_t(n+1)*4;
    auto* t=zeroAllocate<ParticlePropTable>(bytes); if(!t) return nullptr;
    t->allocation=t;
    t->ids=reinterpret_cast<int32_t*>(t+1);
    t->offsets=t->ids+n+1; t->sizes=t->offsets+n+1; t->unroundedSizes=t->sizes+n+1;
    total=0;
    for(int i=0;i<n;i++) {
        t->ids[i]=ids[i]; t->offsets[i]=int32_t(total);
        t->unroundedSizes[i]=sizes[i]; t->sizes[i]=(sizes[i]+3)&~3;
        total+=uint32_t(t->sizes[i]);
    }
    t->offsets[n]=int32_t(total); t->numProperties=n; t->refCount=1;
    t->id=((uint32_t(n)*16+16)&0xffffu) | uint32_t(int32_t(sum)>>10) | (sum<<10);
    if(!remember(tables,t)) { release(t); return nullptr; }
    ringInsert(tableTail,t); return t;
}
int __cdecl propDestroy(ParticlePropTable* t) {
    if(!t) return 0;
    if(--t->refCount<=0) { ringRemove(tableTail,t); forget(tables,t); release(t->allocation); }
    return 1;
}
int __cdecl propOffset(ParticlePropTable* t,int32_t id) {
    if(!t) return -1;
    for(int i=0;i<t->numProperties;i++) if(t->ids[i]==id) return t->offsets[i];
    return -1;
}
ParticleClass* __cdecl classSetProperties(ParticleClass* c,ParticlePropTable* t) {
    if(!c || !t) return nullptr;
    // Acquire first: self-assignment of a last-reference table is safe.
    ++t->refCount; propDestroy(c->properties); c->properties=t;
    c->objectSize=t->offsets[t->numProperties]; return c;
}
ParticleClass* __cdecl classGetProperties(ParticleClass* c,ParticlePropTable** t) {
    if(!c || !t) return nullptr; *t=c->properties; return c; // borrowed
}
ParticleClass* setCallbacks(ParticleClass* c,int n,const uintptr_t* a,int width) {
    if(!c || n<=0 || n>64 || !a) return nullptr;
    auto* p=static_cast<uintptr_t*>(allocate(size_t(n)*width*sizeof(uintptr_t)));
    if(!p) return nullptr;
    std::memcpy(p,a,size_t(n)*width*sizeof(uintptr_t));
    release(c->callbacks); c->callbacks=p; c->numCallbackSets=n; return c;
}
ParticleClass* __cdecl emitterSetCallbacks(ParticleClass* c,int n,const uintptr_t* a) { return setCallbacks(c,n,a,11); }
ParticleClass* __cdecl particleSetCallbacks(ParticleClass* c,int n,const uintptr_t* a) { return setCallbacks(c,n,a,7); }
int classDestroy(ParticleClass* c,bool particle) {
    if(!c) return 0;
    if(--c->refCount<=0) {
        auto& v=particle ? particleClasses : emitterClasses;
        auto& tail=particle ? particleTail : emitterTail;
        ringRemove(tail,c); forget(v,c);
        release(c->callbacks); propDestroy(c->properties); release(c);
    }
    return 1;
}
int __cdecl emitterClassDestroy(ParticleClass* c) { return classDestroy(c,false); }
int __cdecl particleClassDestroy(ParticleClass* c) { return classDestroy(c,true); }

struct Schema {
    int32_t ids[17]{}, sizes[17]{}; int n=0;
    void add(int32_t id,int32_t size) { ids[n]=id; sizes[n++]=size; }
};
bool schema(Schema& s,uint32_t flags,const int32_t* adv,bool particle,bool advanced) {
    if(particle) {
        s.add(0,16);
        for(auto p: {std::array<int,3>{64,6,12}, {2,1,12}, {4,2,32}, {8,3,32}, {128,7,64}, {32,5,32}, {16,4,8}})
            if(flags&uint32_t(p[0])) s.add(p[1],p[2]);
    } else {
        s.add(0,40);
        for(auto p: {std::array<int,3>{1,1,172}, {2,2,64}, {4,3,64}, {32,6,52}, {64,8,116}, {16,5,32}, {8,4,16}})
            if(flags&uint32_t(p[0])) s.add(p[1],p[2]);
    }
    if(!advanced) return true;
    if(flags&0x1e00) { if(!adv) return false; for(int i: {2,3,4}) if(adv[i]<0 || adv[i]>100000) return false; }
    if(flags&0x100) s.add(0x1000001,particle ? 8 : 20);
    if(flags&0x200) s.add(0x1000002,particle ? 4+36*adv[2] : 20+40*adv[2]);
    if(flags&0x400) s.add(0x1000003,particle ? 4+36*adv[4] : 28+40*adv[4]);
    if(flags&0x800) s.add(0x1000004,particle ? 4+20*adv[4] : 28+40*adv[4]);
    if(flags&0x1000) s.add(0x1000005,particle ? 4+20*adv[3] : 20+24*adv[3]);
    if(!particle) switch(flags&0xff0000) {
    case 0x10000:
        if(!adv || adv[0]<0 || adv[0]>100000) return false;
        s.add(0x1000006,12*(adv[0]+2)+(adv[1]==1 ? 12*adv[0] : 0)); break;
    case 0x20000: s.add(0x1000007,20); break;
    case 0x30000: s.add(0x1000008,12); break;
    }
    return true;
}

ParticleEmitter* __cdecl emitterCreateCallback(Atomic*,ParticleEmitter*,void*);
ParticleEmitter* __cdecl emitterDestroyCallback(Atomic*,ParticleEmitter*,void*);
ParticleEmitter* __cdecl emitterEndUpdate(Atomic*,ParticleEmitter*,void*);
ParticleEmitter* __cdecl emitterBeginUpdate(Atomic*,ParticleEmitter*,const float*);
ParticleEmitter* __cdecl emitterSchedule(Atomic*,ParticleEmitter*,void*);
ParticleEmitter* __cdecl standardEmit(Atomic*,ParticleEmitter*,void*);
ParticleEmitter* __cdecl sphereEmit(Atomic*,ParticleEmitter*,void*);
ParticleBatch* __cdecl standardParticleUpdate(ParticleEmitter*,ParticleBatch*,const float*);
std::array<uintptr_t,11> standardEmitterCallbacks() {
    return {reinterpret_cast<uintptr_t>(standardEmit),reinterpret_cast<uintptr_t>(emitterBeginUpdate),reinterpret_cast<uintptr_t>(emitterSchedule),reinterpret_cast<uintptr_t>(emitterEndUpdate),0,
        reinterpret_cast<uintptr_t>(emitterCreateCallback),reinterpret_cast<uintptr_t>(emitterDestroyCallback),CSF_ADDR_FN(0x0057F0F0),
        CSF_ADDR_FN(0x0057F450),CSF_ADDR_FN(0x0057F7A0),CSF_ADDR_FN(0x0057EDE0)};
}
bool defaultCallbacks(ParticleClass* c,bool particle,bool advanced) {
    // PrtAdv compacts each event column separately: standard callback first,
    // then shape, then optional interpolator callbacks. It is not row-major
    // concatenation of the source callback sets.
    uintptr_t sets[8][11]{}; int n=1, width=particle ? 7 : 11;
    auto base=standardEmitterCallbacks();
    if(particle) sets[0][0]=reinterpret_cast<uintptr_t>(standardParticleUpdate);
    else std::copy(base.begin(),base.end(),sets[0]);
    uint32_t f=c->id;
    if(advanced && !particle) {
        auto& a=sets[n];
        switch(f&0xff0000) {
        case 0x10000: a[0]=CSF_ADDR_FN(0x0057AE40);a[5]=CSF_ADDR_FN(0x0057B5B0);a[7]=CSF_ADDR_FN(0x0057B3A0);a[8]=CSF_ADDR_FN(0x0057B4A0);a[9]=CSF_ADDR_FN(0x0057B330);a[10]=CSF_ADDR_FN(0x0057B280);++n;break;
        case 0x20000: a[0]=CSF_ADDR_FN(0x0057B640);a[7]=CSF_ADDR_FN(0x0057BAB0);a[8]=CSF_ADDR_FN(0x0057BB40);a[9]=CSF_ADDR_FN(0x0057BA70);a[10]=CSF_ADDR_FN(0x0057BA00);++n;break;
        case 0x30000: a[0]=reinterpret_cast<uintptr_t>(sphereEmit);a[7]=CSF_ADDR_FN(0x0057BFB0);a[8]=CSF_ADDR_FN(0x0057C020);a[9]=CSF_ADDR_FN(0x0057BF70);a[10]=CSF_ADDR_FN(0x0057BF10);++n;break;
        }
    }
    if(advanced && (f&0x100)) {
        auto& a=sets[n++];
        if(particle) a[0]=CSF_ADDR_FN(0x0057A600);
        else { a[0]=CSF_ADDR_FN(0x0057A9A0);a[7]=CSF_ADDR_FN(0x0057AD80);a[8]=CSF_ADDR_FN(0x0057ADE0);a[9]=CSF_ADDR_FN(0x0057AD40);a[10]=CSF_ADDR_FN(0x0057ACD0); }
    }
    if(advanced && (f&0x1600)) {
        auto& a=sets[n++];
        if(particle) a[0]=CSF_ADDR_FN(0x00578DE0);
        else { a[0]=CSF_ADDR_FN(0x00579000);a[5]=CSF_ADDR_FN(0x0057A450);a[7]=CSF_ADDR_FN(0x0057A230);a[8]=CSF_ADDR_FN(0x0057A380);a[9]=CSF_ADDR_FN(0x0057A1B0);a[10]=CSF_ADDR_FN(0x0057A090); }
    }
    if(advanced && (f&0x800)) {
        auto& a=sets[n++];
        if(particle) a[0]=CSF_ADDR_FN(0x00578670);
        else { a[0]=CSF_ADDR_FN(0x00578760);a[5]=CSF_ADDR_FN(0x00578D40);a[7]=CSF_ADDR_FN(0x00578C60);a[8]=CSF_ADDR_FN(0x00578CE0);a[9]=CSF_ADDR_FN(0x00578C10);a[10]=CSF_ADDR_FN(0x00578BA0); }
    }
    uintptr_t result[8*11]{};
    for(int col=0;col<width;col++) {
        int row=0;
        for(int i=0;i<n;i++) if(sets[i][col]) result[row++*width+col]=sets[i][col];
    }
    return setCallbacks(c,n,result,width)!=nullptr;
}
ParticleClass* classCreate(uint32_t flags,const int32_t* adv,bool particle,bool advanced) {
    Schema s; if(!schema(s,flags,adv,particle,advanced)) return nullptr;
    auto& v=particle ? particleClasses : emitterClasses;
    if(!advanced) for(auto* c:v) if(c->id==flags) { ++c->refCount; return c; }
    auto* t=propCreate(s.n,s.ids,s.sizes); if(!t) return nullptr;
    if(advanced) for(auto* c:v) if(c->properties==t) { ++c->refCount; propDestroy(t); return c; }
    auto* c=zeroAllocate<ParticleClass>(); if(!c) { propDestroy(t); return nullptr; }
    c->id=flags; c->refCount=1;
    classSetProperties(c,t); propDestroy(t);
    if(!defaultCallbacks(c,particle,advanced) || !remember(v,c)) {
        release(c->callbacks); propDestroy(c->properties); release(c); return nullptr;
    }
    ringInsert(particle ? particleTail : emitterTail,c); return c;
}
ParticleClass* __cdecl emitterStdCreate(uint32_t f) {
    csf_rw::startupTrace("0x0057FA90","emitter standard class"); return classCreate(f,nullptr,false,false);
}
ParticleClass* __cdecl particleStdCreate(uint32_t f) {
    csf_rw::startupTrace("0x0057F970","particle standard class"); return classCreate(f,nullptr,true,false);
}
ParticleClass* __cdecl emitterAdvCreate(uint32_t f,const int32_t* a) {
    csf_rw::startupTrace("0x005784E0","emitter advanced class"); return classCreate(f,a,false,true);
}
// Despite its database name EClassStdCreate, 5785B0 creates a particle class.
// It consumes two cdecl arguments, not the three stale stack words in 6E0E00.
ParticleClass* __cdecl particleAdvCreate(uint32_t f,const int32_t* a) {
    csf_rw::startupTrace("0x005785B0","particle advanced class"); return classCreate(f,a,true,true);
}

size_t propertySize(int id,int original) {
    if(id==0 && original==40) return sizeof(ParticleEmitter);
    if(id==1 && original==172) return sizeof(EmitterStandard);
    if(id==6 && original==52) return sizeof(EmitterTank);
    return size_t(original);
}
size_t emitterSize(ParticleClass* c) {
    size_t size=0; auto* t=c->properties;
    for(int i=0;i<t->numProperties;i++) size+=propertySize(t->ids[i],t->sizes[i]);
    return std::max(size,sizeof(ParticleEmitter));
}
ParticleEmitter* __cdecl emitterCreateCallback(Atomic*,ParticleEmitter* e,void*) {
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));
    if(s) {
        e->flags|=0x7f; s->seed=1; s->maxParticles=0;
        s->size={1,1}; s->uv1={1,1}; s->color={255,255,255,255};
        s->rotation=0; s->uv0={0,0};
        std::memset(s->reserved,0,sizeof(s->reserved)); s->texture=nullptr;
    }
    return e;
}
ParticleEmitter* __cdecl emitterDestroyCallback(Atomic*,ParticleEmitter* e,void*) {
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));
    if(s && (e->flags&0x200) && s->texture) {
        CSF_FUNC(int,__cdecl,destroyTexture,0x006317E0,(Texture*)); destroyTexture(s->texture);
    }
    return e;
}
ParticleEmitter* __cdecl emitterEndUpdate(Atomic*,ParticleEmitter* e,void*) {
    if(e->numParticles>0) {
        auto* t=static_cast<EmitterTank*>(particleProperty(e,6));
        if(!t) return nullptr;
        // Original tests this word unsigned: negative values are also nonzero.
        if(t->numParticles!=0 && (!t->atomic || !t->atomic->renderCB || !t->atomic->renderCB(t->atomic))) return nullptr;
    }
    return e;
}
ParticleEmitter* __cdecl emitterBeginUpdate(Atomic* a,ParticleEmitter* e,const float* dt) {
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));
    if(!s || !a || !dt) return nullptr;
    s->ltm=a->object.object.parent ? &static_cast<Frame*>(a->object.object.parent)->ltm : nullptr;
    if(a->object.object.parent) {
        CSF_FUNC(rw::Matrix*,__cdecl,getLtm,0x00623720,(Frame*)); s->ltm=getLtm(static_cast<Frame*>(a->object.object.parent));
    }
    s->currentTime+=*dt;
    auto* p=static_cast<EmitterTank*>(particleProperty(e,6));
    if(p && (uint32_t(p->flags)&0x20000) && p->atomic) {
        int off=plugins(Registry::Atomic).getPluginOffset(303);
        auto* tank=off<0 ? nullptr : *reinterpret_cast<Tank**>(reinterpret_cast<char*>(p->atomic)+off);
        if(!tank) return nullptr;
        float angle; std::memcpy(&angle,tank->tail+7,4); // original PTank +C0
        angle+=*dt*s->rotation;
        constexpr float pi=3.14159274101257324219f, twoPi=6.28318548202514648438f;
        if(angle>pi) angle-=twoPi; else if(angle< -pi) angle+=twoPi;
        std::memcpy(tank->tail+7,&angle,4); tank->dirty|=0x20000;
    }
    return e;
}
ParticleEmitter* __cdecl emitterSchedule(Atomic*,ParticleEmitter* e,void*) {
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1)); if(!s) return nullptr;
    if(s->currentTime>=s->nextEmit) {
        s->previousEmit=s->currentTime;
        s->seed=s->seed*0x0bb38435u+0x3619636bu;
        // Constants read from the image: 2^-32, 1. The original uses an
        // unsigned integer-to-x87 conversion; retaining double here avoids
        // rounding the uint32 seed before scaling.
        double r=double(s->seed)*0x1p-32;
        s->nextEmit=float(double(s->currentTime)+double(s->emitGap)+(r*2-1)*double(s->emitGapBias));
    }
    return e;
}

bool invokeEmitter(ParticleEmitter* e,int slot,Atomic* a,void* data) {
    auto* c=e->emitterClass;
    for(int i=0;i<c->numCallbackSets;i++) {
        auto cb=reinterpret_cast<EmitterCallback>(c->callbacks[i*11+slot]);
        if(cb && cb(a,e,data)!=e) return false;
    }
    return true;
}
bool invokeBatch(ParticleEmitter* e,ParticleBatch* b,int slot) {
    auto* c=b->particleClass;
    for(int i=0;i<c->numCallbackSets;i++) {
        auto cb=reinterpret_cast<BatchCallback>(c->callbacks[i*7+slot]);
        if(cb && cb(e,b,nullptr)!=b) return false;
    }
    return true;
}
ParticleEmitter* __cdecl allBatches(ParticleEmitter* e,BatchCallback cb,void* data) {
    if(!e || !cb) return nullptr;
    auto* tail=e->activeBatches;
    if(tail) {
        auto* first=tail->next; auto* b=first;
        do { auto* next=b->next; if(cb(e,b,data)!=b) return nullptr; b=next; } while(b!=first);
    }
    return e;
}
ParticleBatch* __cdecl newBatch(ParticleEmitter* e) {
    if(!e || !e->particleClass || e->batchSize<=0) return nullptr;
    auto* c=e->particleClass;
    if(c->objectSize<=0 || size_t(e->batchSize)>(UINT32_MAX-sizeof(ParticleBatch))/size_t(c->objectSize)) return nullptr;
    auto* b=e->inactiveBatches ? e->inactiveBatches->next : nullptr;
    if(b) ringRemove(e->inactiveBatches,b);
    else {
        b=zeroAllocate<ParticleBatch>(sizeof(ParticleBatch)+size_t(e->batchSize)*c->objectSize);
        if(!b) return nullptr;
        b->particleClass=c; b->maxParticles=e->batchSize; b->dataOffset=sizeof(ParticleBatch);
    }
    b->next=b; b->numParticles=0; b->newParticle=0;
    // 57FFA0 and the reuse path both discard the initializer's return value.
    // Preserve that unusual contract; it is not a blanket-success callback.
    invokeBatch(nullptr,b,2);
    ringInsert(e->activeBatches,b); return b;
}
bool clearBatches(ParticleEmitter* e) {
    // Failure leaves the unprocessed ring intact rather than freeing an
    // object whose destructor refused it. Cache next before every callback.
    while(e->activeBatches) {
        auto* b=e->activeBatches->next;
        if(!invokeBatch(nullptr,b,3)) return false;
        ringRemove(e->activeBatches,b); release(b);
    }
    while(e->inactiveBatches) { auto* b=e->inactiveBatches->next; ringRemove(e->inactiveBatches,b); release(b); }
    e->numParticles=0; return true;
}
ParticleEmitter* __cdecl setParticleClass(ParticleEmitter* e,ParticleClass* c,int size) {
    if(!e || !c || size<=0) return nullptr;
    ++c->refCount;
    if(!clearBatches(e)) { --c->refCount; return nullptr; }
    particleClassDestroy(e->particleClass); e->particleClass=c; e->batchSize=size; return e;
}

Tank* getTank(Atomic* a) {
    int off=plugins(Registry::Atomic).getPluginOffset(303);
    return !a || off<0 ? nullptr : *reinterpret_cast<Tank**>(reinterpret_cast<char*>(a)+off);
}
ParticleEmitter* __cdecl atomicEmitter(Atomic* a) {
    int off=plugins(Registry::Atomic).getPluginOffset(304);
    return !a || off<0 ? nullptr : *reinterpret_cast<ParticleEmitter**>(reinterpret_cast<char*>(a)+off);
}
int __cdecl atomicIsEmitter(Atomic* a) { return atomicEmitter(a)!=nullptr; }
ParticleEmitter* __cdecl createEmitter(ParticleClass* c,void* data) { return particleEmitterCreate(c,data); }
int __cdecl destroyEmitter(ParticleEmitter* e) { return particleEmitterDestroy(e); }
int __cdecl tankLock(Atomic* a,TankLock* out,uint32_t flag,uint32_t mode) {
    if(!out) return 0;
    auto* t=getTank(a);
    if(!t || !(t->format.flags&flag)) { *out={nullptr,0}; return 0; }
    TankLock* in=nullptr;
    switch(flag) {
    case 1: in=&t->format.position;break;
    case 2: in=&t->format.color;break;
    case 4: in=&t->format.size;break;
    case 8: in=&t->format.matrix;break;
    case 16: in=&t->format.normal;break;
    case 32: in=&t->format.rotation;break;
    case 64: in=&t->format.texCoords;break;
    case 128: in=&t->format.colors;break;
    case 256: in=&t->format.texCoords4;break;
    }
    // Invalid combined flags with at least one supported bit return success
    // but leave the caller descriptor alone in the original. Preserve that
    // quirk; never index an array with a combined mask.
    if(in) *out=*in;
    if(t->format.stride) out->stride=t->format.stride;
    if(out->data && (mode&0x40000000)) t->locked|=flag|mode;
    return 1;
}
Atomic* __cdecl tankUnlock(Atomic* a) {
    auto* t=getTank(a); if(!t) return nullptr;
    t->dirty|=t->locked; t->locked=0; return a;
}
#include "particle_simulation.inc"
} // namespace

namespace csf_rw {
void* particleProperty(ParticleEmitter* e,int32_t id) {
    if(!e || !e->emitterClass || !e->emitterClass->properties) return nullptr;
    auto* t=e->emitterClass->properties; size_t offset=0;
    for(int i=0;i<t->numProperties;i++) {
        if(t->ids[i]==id) return reinterpret_cast<char*>(e)+offset;
        offset+=propertySize(t->ids[i],t->sizes[i]);
    }
    return nullptr;
}
ParticleEmitter* particleEmitterCreate(ParticleClass* c,void* data) {
    if(!c || !c->properties || c->objectSize<40 || c->properties->ids[0]!=0 || c->properties->sizes[0]!=40) return nullptr;
    auto* e=zeroAllocate<ParticleEmitter>(emitterSize(c)); if(!e) return nullptr;
    e->emitterClass=c; e->next=e; ++c->refCount;
    auto* t=static_cast<EmitterTank*>(particleProperty(e,6));
    if(t) { t->flags=-1;t->platformFlags=-1;t->srcBlend=2;t->dstBlend=2;t->vertexAlpha=1; }
    if(!invokeEmitter(e,5,nullptr,data) || !remember(emitters,e)) {
        // Run corresponding destroy callbacks for successfully initialized
        // properties, but never publish a failed emitter.
        invokeEmitter(e,6,nullptr,nullptr); emitterClassDestroy(c); release(e); return nullptr;
    }
    return e;
}
int particleEmitterDestroy(ParticleEmitter* e) {
    if(!e || !clearBatches(e) || !invokeEmitter(e,6,nullptr,nullptr)) return 0;
    auto* t=static_cast<EmitterTank*>(particleProperty(e,6));
    if(t) {
        if(t->atomic) {
            CSF_FUNC(Atomic*,__cdecl,setFrame,0x0058F300,(Atomic*,Frame*));
            CSF_FUNC(int,__cdecl,destroyFrame,0x00623B10,(Frame*));
            CSF_FUNC(int,__cdecl,destroyAtomic,0x0058F060,(Atomic*));
            auto* frame=static_cast<Frame*>(t->atomic->object.object.parent);
            setFrame(t->atomic,nullptr); if(frame) destroyFrame(frame); destroyAtomic(t->atomic);
        }
        release(t->input);
    }
    emitterClassDestroy(e->emitterClass); particleClassDestroy(e->particleClass);
    forget(emitters,e); release(e); return 1;
}
bool particlesEmpty() { return emitters.empty() && emitterClasses.empty() && particleClasses.empty() && tables.empty(); }
void particleBoundaryTerm() { tableTail=nullptr;emitterTail=nullptr;particleTail=nullptr; }
// Until emitter clone/stream formats are established, populated plugin copy
// fails rather than sharing an owned emitter between two atomics.
void* particleAtomicCtor(void* p,int off,int n) { std::memset(static_cast<char*>(p)+off,0,n); return p; }
void* particleAtomicDtor(void* p,int off,int) {
    auto** slot=reinterpret_cast<ParticleEmitter**>(static_cast<char*>(p)+off);
    while(*slot) {
        auto* e=(*slot)->next;
        ringRemove(*slot,e);
        if(!particleEmitterDestroy(e)) { ringInsert(*slot,e); return nullptr; }
    }
    return p;
}
void* particleAtomicCopy(void* d,void* s,int off,int n) {
    auto* e=*reinterpret_cast<ParticleEmitter**>(static_cast<char*>(s)+off);
    if(e) return nullptr;
    return particleAtomicCtor(d,off,n);
}
} // namespace csf_rw

HOOK(0x005784E0,emitterAdvCreate,"boundary-rw");
HOOK(0x005785B0,particleAdvCreate,"boundary-rw");
HOOK(0x0057F970,particleStdCreate,"boundary-rw");
HOOK(0x0057FA90,emitterStdCreate,"boundary-rw");
HOOK(0x0057FCF0,particleSetCallbacks,"boundary-rw");
HOOK(0x0057CE10,emitterSetCallbacks,"boundary-rw");
HOOK(0x0057CE90,classSetProperties,"boundary-rw");
HOOK(0x0057CED0,classGetProperties,"boundary-rw");
HOOK(0x0057C770,propDestroy,"boundary-rw");
HOOK(0x0057C9A0,propCreate,"boundary-rw");
HOOK(0x0057C8B0,propOffset,"boundary-rw");
HOOK(0x0057CD60,emitterClassDestroy,"boundary-rw");
HOOK(0x0057FC60,particleClassDestroy,"boundary-rw");
HOOK(0x0057ED00,emitterCreateCallback,"boundary-rw");
HOOK(0x0057F0B0,emitterEndUpdate,"boundary-rw");
HOOK(0x0057EDA0,emitterDestroyCallback,"boundary-rw");
HOOK(0x0057EF90,emitterBeginUpdate,"boundary-rw");
HOOK(0x0057F050,emitterSchedule,"boundary-rw");
HOOK(0x00580160,allBatches,"boundary-rw");
HOOK(0x00580230,newBatch,"boundary-rw");
HOOK(0x005802A0,setParticleClass,"boundary-rw");
HOOK(0x00580100,createEmitter,"boundary-rw");
HOOK(0x00580A10,destroyEmitter,"boundary-rw");
HOOK(0x0057C220,atomicIsEmitter,"boundary-rw");
HOOK(0x0057C290,atomicEmitter,"boundary-rw");
HOOK(0x005821D0,tankLock,"boundary-rw");
HOOK(0x005823E0,tankUnlock,"boundary-rw");
HOOK(0x0057CFE0,standardParticleUpdate,"boundary-rw");
HOOK(0x0057DBA0,standardEmit,"boundary-rw");
HOOK(0x0057BBD0,sphereEmit,"boundary-rw");
#endif
