// Standalone ABI boundary over unmodified MIT librw. Contracts: original
// executable / export/funcs, not RenderWare SDK declarations or implementation.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "animation.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <memory>
#include <unordered_map>
#include <vector>
using namespace csf_rw;
CSF_FUNC(Stream*,__cdecl,openAnimFile,0x006298E0,(int,int,void*));
CSF_FUNC(int,__cdecl,closeAnimFile,0x00629810,(Stream*,Memory*));
CSF_FUNC(int,__cdecl,encodeFloat,0x0061F5E0,(float));
namespace {
using IF=rw::HAnimInterpFrame;
using KF=rw::HAnimKeyFrame;
using KH=rw::KeyFrameHeader;
using Info=rw::AnimInterpolatorInfo;
using K=Registry;
int __cdecl frameID(Frame*);
void* alloc(size_t n) {
    if(rw::Engine::state!=rw::Engine::Started || n>0x7fffffff) return nullptr;
    void* p=rw::Engine::memfuncs.rwmalloc(n,rw::MEMDUR_EVENT);
    if(p) std::memset(p,0,n); return p;
}
void release(void* p) { if(p) rw::Engine::memfuncs.rwfree(p); }
template<class T> void tail(K k,uint32_t id,void* p,const T& v) {
    int off=plugins(k).getPluginOffset(id);
    if(off>=0) std::memcpy(static_cast<char*>(p)+off,&v,sizeof(v));
}
float decode(uint16_t s) {
    uint32_t b=uint32_t(s&0x8000)<<16;
    if(s&0x7fff) b|=uint32_t(s&0x7ff)<<12 | uint32_t((s&0x7800)+229376)<<12;
    float f; std::memcpy(&f,&b,4); return f;
}
void applyPose(void* out,void* in) {
    auto* m=static_cast<rw::Matrix*>(out); auto* f=static_cast<IF*>(in);
    m->rotate(f->q,rw::COMBINEREPLACE); m->pos=f->t;
}
void blendPose(void* out,void* in1,void* in2,float a) {
    auto* o=static_cast<IF*>(out); auto* p=static_cast<IF*>(in1); auto* q=static_cast<IF*>(in2);
    // Original blend flips the second operand in place for the short arc.
    if(rw::dot(p->q,q->q)<0) q->q=rw::scale(q->q,-1);
    rw::Quat rot=rw::slerp(p->q,q->q,a); rw::V3d pos=rw::lerp(p->t,q->t,a);
    o->q=rot; o->t=pos;
}
void addPose(void* out,void* in1,void* in2) {
    auto* o=static_cast<IF*>(out); auto* p=static_cast<IF*>(in1); auto* q=static_cast<IF*>(in2);
    auto rot=rw::mult(p->q,q->q); auto pos=rw::add(p->t,q->t); o->q=rot; o->t=pos;
}
void interpolateCompressed(void* out,void* in1,void* in2,float t,void* custom) {
    auto* p=static_cast<CompressedKey*>(in1); auto* q=static_cast<CompressedKey*>(in2);
    auto* c=static_cast<float*>(custom); IF a{},b{};
    a.q={decode(p->value[0]),decode(p->value[1]),decode(p->value[2]),decode(p->value[3])};
    b.q={decode(q->value[0]),decode(q->value[1]),decode(q->value[2]),decode(q->value[3])};
    a.t={decode(p->value[4]),decode(p->value[5]),decode(p->value[6])};
    b.t={decode(q->value[4]),decode(q->value[5]),decode(q->value[6])};
    float alpha=(t-p->time)/(q->time-p->time);
    // Do not mutate compressed input keys (unlike the pose blend API).
    blendPose(out,&a,&b,alpha); auto* f=static_cast<IF*>(out);
    f->t={f->t.x*c[3]+c[0],f->t.y*c[4]+c[1],f->t.z*c[5]+c[2]};
}
const AnimationScheme* builtin(uint32_t id) {
    static AnimationScheme standard{},compressed{};
    if(id==1) {
        auto* n=Info::find(1); if(!n) return nullptr;
        standard={1,uint32_t(sizeof(IF)),uint32_t(sizeof(KF)),
            uintptr_t(&applyPose),uintptr_t(&blendPose),uintptr_t(n->interpCB),uintptr_t(&addPose),0,0,0,0,0};
        return &standard;
    }
    if(id==2) {
        compressed={2,uint32_t(sizeof(IF)),uint32_t(sizeof(CompressedKey)),
            uintptr_t(&applyPose),uintptr_t(&blendPose),uintptr_t(&interpolateCompressed),uintptr_t(&addPose),0,0,0,0,24};
        return &compressed;
    }
    return nullptr;
}
Animation* __cdecl createAnimation(int id,int count,int flags,float duration) {
    auto* s=animationScheme(id);
    if(!s || count<0 || !std::isfinite(duration) || duration<0) return nullptr;
    size_t bytes=size_t(count)*s->keyframeSize;
    if(bytes>0x7fffffff-sizeof(Animation)-s->customSize) return nullptr;
    auto* a=static_cast<Animation*>(alloc(sizeof(Animation)+bytes+s->customSize)); if(!a) return nullptr;
    *a={s,count,flags,duration,a+1,nullptr};
    if(s->customSize) a->customData=static_cast<char*>(a->keyframes)+bytes;
    return a;
}
struct InterpState {
    Interpolator* a=nullptr;
    rw::AnimInterpolator* n=nullptr;
    bool ownsNative=false;
    Info info{};
    rw::Animation animation{};
    ~InterpState() { if(ownsNative && n) n->destroy(); release(a); }
};
std::unordered_map<Interpolator*,std::unique_ptr<InterpState>> interps;
Interpolator* wrapInterpolator(rw::AnimInterpolator* n,bool own,int maxSize) {
    if(!n) return nullptr;
    auto s=std::make_unique<InterpState>(); s->n=n; s->ownsNative=own;
    int capacity=std::max(maxSize,int(sizeof(IF)));
    s->a=static_cast<Interpolator*>(alloc(sizeof(Interpolator)+size_t(n->numNodes)*capacity));
    if(!s->a) return nullptr;
    auto* a=s->a; a->maxSize=capacity; a->frameSize=capacity; a->keySize=-1;
    a->numNodes=n->numNodes; a->callbackTime=-1; a->parent=a;
    interps.emplace(a,std::move(s)); return a;
}
Interpolator* __cdecl createInterpolator(int count,int maxSize) {
    if(rw::Engine::state!=rw::Engine::Started || count<0 || maxSize<0 || size_t(count)*std::max(maxSize,int(sizeof(IF)))>0x7fffffff-sizeof(Interpolator)) return nullptr;
    return wrapInterpolator(rw::AnimInterpolator::create(count,std::max(maxSize,int(sizeof(IF)))),true,maxSize);
}
int __cdecl destroyAnimationOrInterpolator(void* p) {
    auto it=interps.find(static_cast<Interpolator*>(p));
    if(it!=interps.end()) interps.erase(it); else release(p);
    return 1; // 61EE90 is a raw free, also used for Animation by game callers.
}
int __cdecl numNodes(Animation* a) {
    if(!a || !a->scheme || !a->numFrames) return 0;
    // Original counts initial keys until a key whose previous is the first.
    for(int i=0;i<a->numFrames;i++) {
        auto* k=reinterpret_cast<KH*>(static_cast<char*>(a->keyframes)+size_t(i)*a->scheme->keyframeSize);
        if(k->prev==a->keyframes) return i;
    }
    return 0; // malformed chains cannot make the boundary walk off allocation.
}
InterpState* state(Interpolator* a) { auto i=interps.find(a); return i==interps.end()?nullptr:i->second.get(); }
void nativeInfo(InterpState& s,Animation* a) {
    const auto* d=a->scheme;
    s.info={int(d->id),int(d->interpolatedSize),int(d->keyframeSize),int(d->customSize),
        reinterpret_cast<Info::ApplyCB>(d->apply),reinterpret_cast<Info::BlendCB>(d->blend),
        reinterpret_cast<Info::InterpCB>(d->interpolate),reinterpret_cast<Info::AddCB>(d->add),
        reinterpret_cast<Info::MulRecipCB>(d->reciprocal),nullptr,nullptr,nullptr};
    s.animation={&s.info,a->numFrames,a->flags,a->duration,a->keyframes,a->customData};
}
void exportInterp(InterpState& s) {
    auto* a=s.a; auto* n=s.n;
    a->currentTime=n->currentTime; a->nextFrame=n->nextFrame;
    a->frameSize=n->currentInterpKeyFrameSize; a->keySize=n->currentAnimKeyFrameSize;
    a->apply=n->applyCB; a->blend=n->blendCB; a->interpolate=n->interpCB; a->add=n->addCB;
    std::memcpy(a->frames(),n->getFrames(),size_t(a->numNodes)*a->frameSize);
}
void importInterp(InterpState& s) {
    auto* a=s.a; auto* n=s.n;
    if(a->currentAnim) { nativeInfo(s,a->currentAnim); n->currentAnim=&s.animation; }
    n->currentTime=a->currentTime; n->nextFrame=a->nextFrame;
    n->currentInterpKeyFrameSize=a->frameSize; n->currentAnimKeyFrameSize=a->keySize;
    n->applyCB=a->apply; n->blendCB=a->blend; n->interpCB=a->interpolate; n->addCB=a->add;
    std::memcpy(n->getFrames(),a->frames(),size_t(a->numNodes)*a->frameSize);
}
int __cdecl setAnimation(Interpolator* a,Animation* animation) {
    auto* s=state(a);
    if(!s || !animation || !animation->scheme || animation->numFrames<2*a->numNodes ||
       animation->scheme->interpolatedSize>unsigned(a->maxSize) || !animation->scheme->interpolate) return 0;
    nativeInfo(*s,animation);
    if(!s->n->setCurrentAnim(&s->animation)) return 0;
    a->currentAnim=animation; exportInterp(*s); return 1;
}
int __cdecl copyInterpolator(Interpolator* a,Interpolator* b) {
    // Not a state copy! 61F120 copies only the pose array, leaving time,
    // animation, callback and subinterpolator metadata untouched.
    if(!state(a) || !state(b) || a->numNodes>b->numNodes || a->frameSize!=b->frameSize) return 0;
    std::memmove(a->frames(),b->frames(),size_t(a->numNodes)*a->frameSize); return 1;
}
int __cdecl blendInterpolator(Interpolator* out,Interpolator* a,Interpolator* b,float alpha) {
    if(!state(out) || !state(a) || !state(b) || !out->blend ||
       out->numNodes>a->numNodes || out->numNodes>b->numNodes) return 0;
    for(int i=0;i<out->numNodes;i++) out->blend(
        static_cast<char*>(out->frames())+size_t(i)*out->frameSize,
        static_cast<char*>(a->frames())+size_t(i)*a->frameSize,
        static_cast<char*>(b->frames())+size_t(i)*b->frameSize,alpha);
    return 1;
}
Interpolator* __cdecl subInterpolator(Interpolator* parent,int start,int count,int size) {
    if(!state(parent) || start<0 || count<0 || start>parent->numNodes-count) return nullptr;
    auto* out=createInterpolator(count,size==-1?parent->maxSize:size);
    if(out) { out->isSub=1; out->offset=start; out->parent=parent; }
    return out;
}
bool validKeys(Animation* a) {
    if(!a || a->numFrames<1) return false;
    auto base=reinterpret_cast<uintptr_t>(a->keyframes); auto end=base+size_t(a->numFrames)*a->scheme->keyframeSize;
    for(int i=0;i<a->numFrames;i++) {
        auto* k=reinterpret_cast<KH*>(base+size_t(i)*a->scheme->keyframeSize);
        auto prev=reinterpret_cast<uintptr_t>(k->prev);
        if(!std::isfinite(k->time)) return false;
        if(k->time==0) continue; // initial predecessors are never followed
        if(prev<base || prev>=end || (prev-base)%a->scheme->keyframeSize) return false;
    }
    return true;
}
int advance(Interpolator* a,float delta) {
    auto* s=state(a); if(!s || !a->currentAnim || !std::isfinite(delta)) return 0;
    if(delta==0) return 1;
    auto* anim=a->currentAnim; float duration=anim->duration;
    if(!(duration>0) || !validKeys(anim)) return 0;
    float old=a->currentTime, t=old+delta;
    if(!std::isfinite(t)) return 0;
    bool loop=t>duration || t<0;
    // Original uses >, not >=: the exact endpoint remains duration.
    if(t>duration) { t=std::fmod(t,duration); if(t==0) t=duration; }
    if(t<0) { t=std::fmod(t,duration); if(t<0) t+=duration; }
    bool callback=delta>0 ? old<a->callbackTime && old+delta>=a->callbackTime : old>a->callbackTime && old+delta<=a->callbackTime;
    if(loop) callback|=delta>0 ? t>=a->callbackTime : t<a->callbackTime;
    if(delta>0) {
        if(loop || !a->nextFrame) { if(!setAnimation(a,anim)) return 0; }
        importInterp(*s);
        // librw addTime drops the remainder on wrap; we already folded it.
        s->n->addTime(t-s->n->currentTime); exportInterp(*s);
    } else if(loop) {
        if(!setAnimation(a,anim)) return 0;
        importInterp(*s); s->n->addTime(t); exportInterp(*s);
    } else {
        // librw has no negative-time API. Walk each previous-key chain with
        // an allocation bound, then use the registered interpolation callback.
        for(int i=0;i<a->numNodes;i++) {
            auto* f=reinterpret_cast<rw::InterpFrameHeader*>(static_cast<char*>(a->frames())+size_t(i)*a->frameSize);
            int steps=0;
            while(t<f->keyFrame1->time) {
                f->keyFrame2=f->keyFrame1; f->keyFrame1=f->keyFrame1->prev;
                if(++steps>anim->numFrames) return 0;
            }
            a->interpolate(f,f->keyFrame1,f->keyFrame2,t,anim->customData);
        }
        a->currentTime=t; a->nextFrame=nullptr;
    }
    if(callback && a->timeCB && !a->timeCB(a,a->timeData)) a->timeCB=nullptr;
    if(loop && a->loopCB && !a->loopCB(a,a->loopData)) a->loopCB=nullptr;
    return 1;
}
int __cdecl addTime(Interpolator* a,float t) { return t<=0?1:advance(a,t); }
int __cdecl subtractTime(Interpolator* a,float t) { return advance(a,-t); }
int __cdecl setTime(Interpolator* a,float t) { return a?advance(a,t-a->currentTime):0; }
bool readExact(rw::Stream* s,void* p,uint32_t n) { return s && s->read8(p,n)==n; }
Animation* __cdecl readAnimationStream(Stream* stream) {
    auto* s=streamNative(stream); uint32_t version,id,count,flags; float duration;
    if(!readExact(s,&version,4) || version!=256 || !readExact(s,&id,4) ||
       !readExact(s,&count,4) || !readExact(s,&flags,4) || !readExact(s,&duration,4)) return nullptr;
    if(count>0x100000 || ((id==1 || id==2) && count<2)) return nullptr;
    auto* a=createAnimation(id,int(count),int(flags),duration); if(!a) return nullptr;
    bool ok=false;
    if(id==1 || id==2) {
        // Buffer and validate serialized offsets before librw's unchecked
        // pointer relocation. Disk strides are independent of host pointers.
        uint32_t diskSize=id==1?36:22;
        if(uint64_t(count)*diskSize+24<=0x10000000) {
            std::vector<uint8_t> bytes(size_t(count)*diskSize+(id==2?24:0));
            std::vector<bool> initialNull(count);
            ok=readExact(s,bytes.data(),uint32_t(bytes.size()));
            for(uint32_t i=0;ok && i<count;i++) {
                uint32_t prev; std::memcpy(&prev,bytes.data()+size_t(i)*diskSize+diskSize-4,4);
                uint32_t stride=id==1?36:24;
                if(prev%stride || prev/stride>=count) {
                    float time;std::memcpy(&time,bytes.data()+size_t(i)*diskSize,4);
                    if(time!=0) { ok=false;break; }
                    // Real retail .anm files encode initial null predecessors
                    // as a wrapped negative offset from the writer's heap.
                    // These are never followed; don't form wild host pointers.
                    initialNull[i]=true;prev=0;
                    std::memcpy(bytes.data()+size_t(i)*diskSize+diskSize-4,&prev,4);
                }
            }
            if(ok && id==1) {
                rw::StreamMemory input; input.open(bytes.data(),uint32_t(bytes.size()));
                auto* info=Info::find(1); rw::Animation n{info,int(count),int(flags),duration,a->keyframes,nullptr};
                info->streamRead(&input,&n); // librw supplies standard key decoder
                auto* keys=static_cast<KF*>(a->keyframes);
                for(uint32_t i=0;i<count;i++) if(initialNull[i]) keys[i].prev=nullptr;
            } else if(ok) {
                auto* keys=static_cast<CompressedKey*>(a->keyframes);
                for(uint32_t i=0;i<count;i++) {
                    auto* p=bytes.data()+size_t(i)*22; uint32_t prev;
                    std::memcpy(&keys[i].time,p,4); std::memcpy(keys[i].value,p+4,14); std::memcpy(&prev,p+18,4);
                    keys[i].prev=initialNull[i]?nullptr:keys+prev/24;
                }
                std::memcpy(a->customData,bytes.data()+size_t(count)*22,24);
            }
        }
    } else if(a->scheme->read) {
        using Read=Animation* (__cdecl *)(Stream*,Animation*);
        ok=reinterpret_cast<Read>(a->scheme->read)(stream,a)!=nullptr;
    }
    if(!ok || !validKeys(a)) { release(a); return nullptr; } return a;
}
Animation* __cdecl readAnimation(const char* path) {
    auto* s=openAnimFile(2,1,const_cast<char*>(path)); if(!s) return nullptr;
    Animation* a=nullptr;
    if(rw::findChunk(streamNative(s),27,nullptr,nullptr)) a=readAnimationStream(s);
    closeAnimFile(s,nullptr); return a;
}
Animation* __cdecl compressAnimation(Animation* a) {
    if(!a || !a->scheme || a->scheme->id!=1 || a->numFrames<=0) return nullptr;
    auto* out=createAnimation(2,a->numFrames,a->flags,a->duration); if(!out) return nullptr;
    auto* in=static_cast<KF*>(a->keyframes); auto* keys=static_cast<CompressedKey*>(out->keyframes);
    rw::V3d lo=in[0].t,hi=lo;
    for(int i=1;i<a->numFrames;i++) {
        lo={std::min(lo.x,in[i].t.x),std::min(lo.y,in[i].t.y),std::min(lo.z,in[i].t.z)};
        hi={std::max(hi.x,in[i].t.x),std::max(hi.y,in[i].t.y),std::max(hi.z,in[i].t.z)};
    }
    auto scale=rw::scale(rw::sub(hi,lo),0.5f), center=rw::add(lo,scale);
    float custom[6]={center.x,center.y,center.z,scale.x,scale.y,scale.z}; std::memcpy(out->customData,custom,24);
    for(int i=0;i<a->numFrames;i++) {
        keys[i].time=in[i].time;
        if(in[i].time==0) keys[i].prev=nullptr;
        else {
            auto base=reinterpret_cast<uintptr_t>(in), prev=reinterpret_cast<uintptr_t>(in[i].prev);
            if(prev<base || (prev-base)%sizeof(KF) || (prev-base)/sizeof(KF)>=unsigned(a->numFrames)) { release(out); return nullptr; }
            keys[i].prev=keys+(prev-base)/sizeof(KF);
        }
        float values[7]={in[i].q.x,in[i].q.y,in[i].q.z,in[i].q.w,
            (in[i].t.x-center.x)/scale.x,(in[i].t.y-center.y)/scale.y,(in[i].t.z-center.z)/scale.z};
        for(int j=0;j<7;j++) keys[i].value[j]=uint16_t(encodeFloat(values[j]));
    }
    return out;
}
struct HierState {
    Hierarchy a{};
    rw::HAnimHierarchy* n=nullptr;
    bool owned=false;
    std::vector<HAnimNode> nodes;
};
std::unordered_map<Hierarchy*,std::unique_ptr<HierState>> hierarchies;
HierState* hstate(Hierarchy* a) { auto i=hierarchies.find(a); return i==hierarchies.end()?nullptr:i->second.get(); }
Hierarchy* wrapHierarchy(rw::HAnimHierarchy* n,bool own) {
    if(!n) return nullptr;
    for(auto& p:hierarchies) if(p.second->n==n) return p.first;
    auto s=std::make_unique<HierState>(); s->n=n; s->owned=own;
    s->nodes.resize(n->numNodes); auto* a=&s->a;
    a->flags=n->flags; a->numNodes=n->numNodes; a->matrices=n->matrices; a->unaligned=n->matricesUnaligned;
    a->nodes=s->nodes.data(); a->parentFrame=frameFacade(n->parentFrame); a->parent=a; a->rootParentOffset=-1;
    a->interpolator=wrapInterpolator(n->interpolator,false,n->interpolator->maxInterpKeyFrameSize);
    if(!a->interpolator) return nullptr;
    for(int i=0;i<a->numNodes;i++) a->nodes[i]={n->nodeInfo[i].id,n->nodeInfo[i].index,n->nodeInfo[i].flags,frameFacade(n->nodeInfo[i].frame)};
    hierarchies.emplace(a,std::move(s)); return a;
}
void importHierarchy(HierState& s) {
    auto* a=&s.a; auto* n=s.n; n->flags=a->flags; n->parentFrame=frameNative(a->parentFrame);
    for(int i=0;i<a->numNodes;i++) n->nodeInfo[i]={a->nodes[i].id,a->nodes[i].index,a->nodes[i].flags,frameNative(a->nodes[i].frame)};
    if(auto* interp=state(a->interpolator)) importInterp(*interp);
}
int __cdecl destroyHierarchy(Hierarchy* a) {
    auto* s=hstate(a); if(!s) return 0;
    auto* n=s->n;
    if(n->parentFrame && plugins(K::Frame).getPluginOffset(286)>=0) {
        auto* d=rw::HAnimData::get(n->parentFrame);
        if(d->hierarchy==n) { d->hierarchy=nullptr; HAnimExtension e{d->id,nullptr}; tail(K::Frame,286,frameFacade(n->parentFrame),e); }
    }
    interps.erase(a->interpolator);
    // Subhierarchies alias parent nodes/matrices, never use librw's owning dtor.
    bool sub=(a->flags&1)!=0;
    if(sub) { n->interpolator->destroy(); release(n); }
    else n->destroy();
    hierarchies.erase(a); return 0; // original success value is zero
}
Hierarchy* __cdecl createHierarchy(int count,int* nodeFlags,int* ids,int flags,int maxSize) {
    if(rw::Engine::state!=rw::Engine::Started || count<0 || count>65536 || maxSize<0) return nullptr;
    auto* n=rw::HAnimHierarchy::create(count,nodeFlags,ids,flags,std::max(maxSize,int(sizeof(IF))));
    if(!n) return nullptr;
    auto* a=wrapHierarchy(n,true); if(!a) n->destroy(); return a;
}
Hierarchy* __cdecl copyHierarchy(Hierarchy* a,int flags,int maxSize) {
    if(!hstate(a)) return nullptr;
    auto* b=createHierarchy(a->numNodes,nullptr,nullptr,flags,maxSize); if(!b) return nullptr;
    for(int i=0;i<a->numNodes;i++) { b->nodes[i]=a->nodes[i]; b->nodes[i].frame=nullptr; }
    return b;
}
Hierarchy* __cdecl subHierarchy(Hierarchy* a,int start,int flags,int maxSize) {
    auto* source=hstate(a); if(!source || start<0 || start>=a->numNodes) return nullptr;
    int count=1,depth=1;
    if(!(a->nodes[start].flags&1)) {
        while(start+count<a->numNodes && depth) {
            int f=a->nodes[start+count++].flags; if(f&2) depth++; if(f&1) depth--;
        }
        if(depth) return nullptr;
    }
    int parent=-1; std::vector<int> stack;
    for(int i=0;i<start;i++) {
        int f=a->nodes[i].flags; if(f&2) stack.push_back(parent);
        if(f&1) { if(stack.empty()) return nullptr; parent=stack.back(); stack.pop_back(); } else parent=i;
    }
    if(maxSize==-1) maxSize=a->interpolator->maxSize;
    // Create only an interpolator; matrix and node slices remain parent-owned.
    auto* n=static_cast<rw::HAnimHierarchy*>(alloc(sizeof(rw::HAnimHierarchy))); if(!n) return nullptr;
    n->flags=flags|1; n->numNodes=count; n->matrices=a->matrices?a->matrices+start:nullptr;
    n->nodeInfo=source->n->nodeInfo+start; n->parentFrame=frameNative(a->nodes[start].frame); n->parentHierarchy=source->n;
    n->interpolator=rw::AnimInterpolator::create(count,std::max(maxSize,int(sizeof(IF))));
    if(!n->interpolator) { release(n); return nullptr; }
    auto* b=wrapHierarchy(n,true); if(!b) { n->interpolator->destroy(); release(n); return nullptr; }
    b->nodes=a->nodes+start; b->parent=a; b->rootParentOffset=parent;
    b->interpolator->isSub=1; b->interpolator->offset=start; b->interpolator->parent=a->interpolator;
    return b;
}
rw::Matrix* __cdecl matrixArray(Hierarchy* a) { return a?a->matrices:nullptr; }
Hierarchy* __cdecl detachHierarchy(Hierarchy* a) {
    if(!hstate(a)) return nullptr;
    for(int i=0;i<a->numNodes;i++) a->nodes[i].frame=nullptr;
    importHierarchy(*hstate(a)); return a;
}
Hierarchy* __cdecl attachHierarchy(Hierarchy* a) {
    auto* s=hstate(a); if(!s) return nullptr;
    importHierarchy(*s);
    // Original visits the subtree, assigning every matching node, including
    // duplicate IDs. librw attach() instead searches for unattached frames.
    std::vector<rw::Frame*> todo;
    if(s->n->parentFrame) todo.push_back(s->n->parentFrame);
    while(!todo.empty()) {
        auto* f=todo.back(); todo.pop_back();
        int id=frameID(frameFacade(f));rw::HAnimData::get(f)->id=id;
        for(int i=0;i<a->numNodes;i++) if(a->nodes[i].id==id) {
            a->nodes[i].frame=frameFacade(f); s->n->nodeInfo[i].frame=f;
        }
        f->updateObjects();
        std::vector<rw::Frame*> children;
        for(auto* c=f->child;c;c=c->next) children.push_back(c);
        for(auto i=children.rbegin();i!=children.rend();++i) todo.push_back(*i);
    }
    frameExport();
    return a;
}
int __cdecl updateHierarchy(Hierarchy* a) {
    auto* s=hstate(a); if(!s || !a->interpolator->apply) return 0;
    importHierarchy(*s);
    // librw handles the ordinary root matrix stack. Supplement its TODOs for
    // subhierarchy parent matrices and the original local/LTM update flags.
    rw::Matrix root; root.setIdentity();
    if(!(a->flags&2) && (a->flags&1) && a->rootParentOffset>=0 && a->parent->matrices) root=a->parent->matrices[a->rootParentOffset];
    else if((a->flags&2) || !(a->flags&0x4000)) {
        auto* f=frameNative(a->parentFrame); if(f && f->getParent()) root=*f->getParent()->getLTM();
    }
    rw::Matrix localToWorld; localToWorld.setIdentity();
    auto* f=frameNative((a->flags&1)?a->parent->parentFrame:a->parentFrame);
    if(f && f->getParent()) localToWorld=*f->getParent()->getLTM();
    // Resolve ALL frame identities before writing native matrices. frameNative
    // imports direct game writes and would otherwise overwrite prior results.
    std::vector<rw::Frame*> frames;
    for(int i=0;i<a->numNodes;i++) frames.push_back(frameNative(a->nodes[i].frame));
    std::vector<rw::Matrix> scratch((a->flags&2)?a->numNodes:0);
    auto* matrices=(a->flags&2)?scratch.data():a->matrices;
    if(a->numNodes && !matrices) return 0;
    if((a->flags&0x2000) && s->n->parentFrame) {
        auto* rootFrame=s->n->parentFrame->root;
        if(!(rootFrame->object.privateFlags&rw::Frame::HIERARCHYSYNC))
            rw::engine->frameDirtyList.add(&rootFrame->inDirtyList);
        rootFrame->object.privateFlags|=rw::Frame::HIERARCHYSYNCOBJ;
    }
    rw::Matrix* parent=&root; std::vector<rw::Matrix*> stack;
    for(int i=0;i<a->numNodes;i++) {
        rw::Matrix local; local.setIdentity();
        a->interpolator->apply(&local,static_cast<char*>(a->interpolator->frames())+size_t(i)*a->interpolator->frameSize);
        rw::Matrix::mult(matrices+i,&local,parent);
        if(auto* n=frames[i]) {
            if(a->flags&0x1000) { n->matrix=local; if(!(a->flags&0x2000)) n->updateObjects(); }
            if(a->flags&0x2000) {
                if(!(a->flags&2) && (a->flags&0x4000)) rw::Matrix::mult(&n->ltm,matrices+i,&localToWorld); else n->ltm=matrices[i];
                n->object.privateFlags=(n->object.privateFlags&~12)|8;
            }
        }
        int flag=a->nodes[i].flags&3;
        if(flag==2) stack.push_back(parent);
        if(flag==1) { if(stack.empty()) { if(i!=a->numNodes-1) return 0; } else { parent=stack.back(); stack.pop_back(); } }
        else if(flag!=3) parent=matrices+i;
    }
    frameExport(); return 1;
}
int __cdecl frameID(Frame* a) {
    int off=plugins(K::Frame).getPluginOffset(286), id=-1;
    if(a && off>=0) std::memcpy(&id,reinterpret_cast<char*>(a)+off,4);
    return id;
}
Hierarchy* __cdecl frameHierarchy(Frame* a) {
    int off=plugins(K::Frame).getPluginOffset(286); HAnimExtension e{};
    if(a && off>=0) std::memcpy(&e,reinterpret_cast<char*>(a)+off,sizeof(e));
    return e.hierarchy;
}
std::unordered_map<Geometry*,std::unique_ptr<Skin>> skins;
Skin* publishSkin(Geometry* a,rw::Geometry* n) {
    auto* skin=rw::Skin::get(n); if(!skin) { tail(K::Geometry,278,a,static_cast<Skin*>(nullptr)); return nullptr; }
    auto& p=skins[a]; if(!p) p=std::make_unique<Skin>();
    *p={skin->numBones,skin->numUsedBones,skin->usedBones,skin->inverseMatrices,skin->numWeights,
        skin->indices,skin->weights,skin->boneLimit,skin->numMeshes,skin->rleSize,skin->remapIndices,
        skin->rleCount,skin->rle,skin->platformData,nullptr,skin->data};
    tail(K::Geometry,278,a,p.get()); return p.get();
}
Skin* __cdecl geometrySkin(Geometry* a) { auto* n=geometryNative(a); return n && plugins(K::Geometry).getPluginOffset(278)>=0?publishSkin(a,n):nullptr; }
Atomic* __cdecl setSkinHierarchy(Atomic* a,Hierarchy* h) {
    auto* n=atomicNative(a); auto* b=hierarchyNative(h); if(!n || (h && !b)) return nullptr;
    rw::Skin::setHierarchy(n,b); tail(K::Atomic,278,a,h); return a;
}
Hierarchy* __cdecl getSkinHierarchy(Atomic* a) {
    auto* n=atomicNative(a); return n?wrapHierarchy(rw::Skin::getHierarchy(n),false):nullptr;
}
int __cdecl skinType(Atomic* a) {
    auto* n=atomicNative(a); if(!n || !n->pipeline || n->pipeline->pluginID!=278) return 0;
    return int(n->pipeline->pluginData);
}
int __cdecl skinSplit(Skin* a) { return a && a->remapIndices!=nullptr; }
// MatFX owning storage stays native; public APIs translate frame identities.
int __cdecl atomicEffects(Atomic* a) { auto* n=atomicNative(a); return n?rw::MatFX::getEffects(n):0; }
int __cdecl materialEffects(Material* a) { auto* n=materialNative(a); return n?rw::MatFX::getEffects(n):0; }
Material* __cdecl envFrame(Material* a,Frame* f) {
    auto* n=materialNative(a); auto* b=frameNative(f); if(!n || (f && !b)) return nullptr;
    auto* fx=rw::MatFX::get(n); if(!fx || fx->getEffectIndex(2)<0) return nullptr;
    fx->setEnvFrame(b); return a;
}
Material* __cdecl envAlpha(Material* a,int alpha) {
    auto* n=materialNative(a); if(!n) return nullptr;
    auto* fx=rw::MatFX::get(n); if(!fx || fx->getEffectIndex(2)<0) return nullptr;
    fx->setEnvFBAlpha(alpha); return a;
}
Material* __cdecl uvMatrices(Material* a,rw::Matrix* base,rw::Matrix* dual) {
    auto* n=materialNative(a); if(!n) return nullptr;
    auto* fx=rw::MatFX::get(n); if(!fx || fx->getEffectIndex(5)<0) return nullptr;
    fx->setUVTransformMatrices(base,dual); return a;
}
}
namespace csf_rw {
bool animationFacadesEmpty() { return interps.empty() && hierarchies.empty() && skins.empty(); }
void* animationCopyFrame(void* dst,void* src,int offset,int) {
    HAnimExtension e{}; std::memcpy(&e,static_cast<char*>(src)+offset,sizeof(e));
    auto* f=frameNative(static_cast<Frame*>(dst)); if(!f) return nullptr;
    auto* d=rw::HAnimData::get(f); d->id=e.id;
    // A native clone may have already run its owning copy callback.
    if(d->hierarchy) {
        animationPublish(K::Frame,dst,f); return dst;
    }
    if(e.hierarchy && !(e.hierarchy->flags&1)) {
        auto* h=copyHierarchy(e.hierarchy,e.hierarchy->flags,e.hierarchy->interpolator->maxSize);
        if(!h) return nullptr;
        h->parentFrame=static_cast<Frame*>(dst);
        auto* s=hstate(h); s->owned=false; s->n->parentFrame=f; d->hierarchy=s->n;
        e.hierarchy=h;
    } else e.hierarchy=nullptr;
    std::memcpy(static_cast<char*>(dst)+offset,&e,sizeof(e)); return dst;
}
const AnimationScheme* builtinAnimationScheme(uint32_t id) { return builtin(id); }
rw::HAnimHierarchy* hierarchyNative(Hierarchy* a) { auto* s=hstate(a); if(!s) return nullptr; importHierarchy(*s); return s->n; }
void animationPublish(Registry k,void* a,void* n) {
    if(k==K::Frame && plugins(k).getPluginOffset(286)>=0) {
        auto* d=rw::HAnimData::get(static_cast<rw::Frame*>(n));
        HAnimExtension e{d->id,wrapHierarchy(d->hierarchy,false)}; tail(k,286,a,e);
    }
    if(k==K::Geometry && plugins(k).getPluginOffset(278)>=0) publishSkin(static_cast<Geometry*>(a),static_cast<rw::Geometry*>(n));
    if(k==K::Atomic) {
        auto* b=static_cast<rw::Atomic*>(n);
        if(plugins(k).getPluginOffset(278)>=0) tail(k,278,a,wrapHierarchy(rw::Skin::getHierarchy(b),false));
        if(plugins(k).getPluginOffset(288)>=0) tail(k,288,a,int(rw::MatFX::getEffects(b)));
    }
}
void animationForget(Registry k,void* a) {
    if(k==K::Geometry) skins.erase(static_cast<Geometry*>(a));
    if(k==K::Frame) {
        auto* f=static_cast<Frame*>(a);
        for(auto i=hierarchies.begin();i!=hierarchies.end();) {
            auto& s=*i->second;
            if(s.a.parentFrame==f && !s.owned) { interps.erase(s.a.interpolator); i=hierarchies.erase(i); }
            else { for(int j=0;j<s.a.numNodes;j++) if(s.a.nodes[j].frame==f) s.a.nodes[j].frame=nullptr; ++i; }
        }
    }
}
}
HOOK(0x0061EE10,createAnimation,"boundary-rw");
HOOK(0x0061EE90,destroyAnimationOrInterpolator,"boundary-rw");
HOOK(0x0061EEB0,readAnimationStream,"boundary-rw");
HOOK(0x0061F3C0,readAnimation,"boundary-rw");
HOOK(0x0061EFB0,numNodes,"boundary-rw");
HOOK(0x0061EFE0,createInterpolator,"boundary-rw");
HOOK(0x0061F050,setAnimation,"boundary-rw");
HOOK(0x0061F120,copyInterpolator,"boundary-rw");
HOOK(0x0061F320,blendInterpolator,"boundary-rw");
HOOK(0x0061F380,subInterpolator,"boundary-rw");
HOOK(0x0061F150,addTime,"boundary-rw");
HOOK(0x0061F410,subtractTime,"boundary-rw");
HOOK(0x0061F590,setTime,"boundary-rw");
HOOK(0x007C5D00,compressAnimation,"boundary-rw");
HOOK(0x0056CC30,createHierarchy,"boundary-rw");
HOOK(0x0056CD30,destroyHierarchy,"boundary-rw");
HOOK(0x0056CDB0,subHierarchy,"boundary-rw");
HOOK(0x0056CED0,copyHierarchy,"boundary-rw");
HOOK(0x0056CF50,detachHierarchy,"boundary-rw");
HOOK(0x0056CF80,matrixArray,"boundary-rw");
HOOK(0x0056CF90,frameHierarchy,"boundary-rw");
HOOK(0x0056CFA0,frameID,"boundary-rw");
HOOK(0x0056D3F0,updateHierarchy,"boundary-rw");
HOOK(0x0056DB50,attachHierarchy,"boundary-rw");
HOOK(0x00587020,setSkinHierarchy,"boundary-rw");
HOOK(0x00587040,getSkinHierarchy,"boundary-rw");
HOOK(0x00587050,geometrySkin,"boundary-rw");
HOOK(0x005870E0,skinSplit,"boundary-rw");
HOOK(0x005870F0,skinType,"boundary-rw");
HOOK(0x005712A0,atomicEffects,"boundary-rw");
HOOK(0x005712B0,materialEffects,"boundary-rw");
HOOK(0x005715D0,envFrame,"boundary-rw");
HOOK(0x00571610,envAlpha,"boundary-rw");
HOOK(0x00571920,uvMatrices,"boundary-rw");
#endif
