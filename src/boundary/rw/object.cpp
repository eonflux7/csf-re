// Standalone RW-layout facades over unmodified MIT librw, not SDK code.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "object.h"
#include "device.h"
#include "startup.h"
#include <unordered_map>
#include <vector>
#include <cstring>
#include <cmath>
#include <limits>
#include <climits>

using namespace csf_rw;
CSF_FUNC(ObjectWithFrame*, __cdecl, attachFrame, 0x0063CE30, (ObjectWithFrame*, Frame*));
CSF_FUNC(Frame*, __stdcall, createFrame, 0x006235D0, ());
CSF_FUNC(Frame*, __cdecl, addChild, 0x00623B90, (Frame*, Frame*));
CSF_FUNC(int, __cdecl, destroyFrame, 0x00623B10, (Frame*));
CSF_FUNC(rw::Matrix*, __cdecl, getLTM, 0x00623720, (Frame*));
CSF_FUNC(int, __stdcall, syncFrames, 0x006327D0, ());

namespace {
using K=Registry;
std::unordered_map<Material*,rw::Material*> materials;
struct GeoState { rw::Geometry* n; std::vector<Material*> materials; std::vector<MorphTarget> morphs; std::vector<unsigned char> meshes; };
std::unordered_map<Geometry*,GeoState> geometries;
std::unordered_map<Atomic*,rw::Atomic*> atomics;
std::unordered_map<Light*,rw::Light*> lights;
std::unordered_map<Camera*,rw::Camera*> cameras;
std::unordered_map<Clump*,rw::Clump*> clumps;
void ring(Link& l) { l.next=l.prev=&l; }
void unlink(Link& l) { if(l.next && l.prev) { l.next->prev=l.prev; l.prev->next=l.next; } ring(l); }
void append(Link& root, Link& l) { l.next=&root; l.prev=root.prev; root.prev->next=&l; root.prev=&l; }
template<class A> A* allocate(K k) {
    if(rw::Engine::state!=rw::Engine::Started) return nullptr;
    auto* p=static_cast<A*>(rw::Engine::memfuncs.rwmalloc(plugins(k).size,rw::MEMDUR_EVENT));
    if(p) std::memset(p,0,plugins(k).size); return p;
}
void freeFacade(void* p) { rw::Engine::memfuncs.rwfree(p); }
template<class A,class N> N* lookup(const std::unordered_map<A*,N*>& map,A* a) {
    auto it=map.find(a); return it==map.end()?nullptr:it->second;
}
template<class A,class N> bool insert(std::unordered_map<A*,N*>& map,A* a,N* n) {
    try { return map.emplace(a,n).second; } catch(...) { return false; }
}
template<class A> std::vector<A*> snapshot(Link& root, size_t offset) {
    std::vector<A*> result;
    for(Link* l=root.next;l!=&root;l=l->next)
        result.push_back(reinterpret_cast<A*>(reinterpret_cast<char*>(l)-offset));
    return result;
}
Material* __cdecl materialTexture(Material*,Texture*);
int __cdecl geometryDestroy(Geometry*);
int __cdecl atomicDestroy(Atomic*);
int __cdecl lightDestroy(Light*);
int __cdecl cameraDestroy(Camera*);
int __cdecl clumpDestroy(Clump*);
bool geometryCanRelease(Geometry* g) {
    auto it=geometries.find(g); if(it==geometries.end()) return false;
    if(g->refCount>1) return true;
    // Preflight before any facade/plugin identity is destroyed. The image
    // slice rejects final texture release while its raster is locked/parented.
    std::unordered_map<Material*,int> occurrences;
    std::unordered_map<Texture*,int> releases;
    for(auto* m:it->second.materials) ++occurrences[m];
    for(auto& p:occurrences) {
        auto* n=lookup(materials,p.first); if(!n || p.first->refCount<=0) return false;
        if(p.first->refCount<=p.second && p.first->texture) ++releases[p.first->texture];
    }
    for(auto& p:releases) if(!textureCanRelease(p.first,p.second)) return false;
    return true;
}
Atomic* __cdecl resyncSphere(Atomic*);
Atomic* __cdecl defaultRender(Atomic*);
void __cdecl syncFacade(ObjectWithFrame* o) {
    objectFrameChanged(o);
    if(o->object.type==rw::Atomic::ID) {
        auto* a=reinterpret_cast<Atomic*>(o);
        if(a->interpolator.flags&2) resyncSphere(a);
    }
    objectExport();
}
void exportMaterial(Material* a,rw::Material* n) {
    a->color=n->color; a->surfaceProps=n->surfaceProps; a->refCount=static_cast<short>(n->refCount);
    a->texture=textureFacade(n->texture);
}
void exportGeometry(Geometry* a,GeoState& s) {
    auto* n=s.n;
    a->object.type=rw::Geometry::ID; a->flags=n->flags; a->lockedSinceInst=n->lockedSinceInst;
    a->refCount=static_cast<short>(n->refCount); a->numTriangles=n->numTriangles; a->numVertices=n->numVertices;
    a->numMorphTargets=n->numMorphTargets; a->numTexCoordSets=n->numTexCoordSets;
    a->triangles=n->triangles; a->colors=n->colors;
    for(int i=0;i<8;i++) a->texCoords[i]=n->texCoords[i];
    a->matList={s.materials.data(),static_cast<int>(s.materials.size()),static_cast<int>(s.materials.size())};
    a->morphTargets=s.morphs.data();
    for(int i=0;i<n->numMorphTargets;i++) {
        auto& m=s.morphs[i]; auto& b=n->morphTargets[i];
        m={a,b.boundingSphere,b.vertices,b.normals};
    }
}
void exportLight(Light* a,rw::Light* n) {
    a->radius=n->radius; a->color=n->color; a->minusCosAngle=n->minusCosAngle;
    a->object.object.privateFlags=n->object.object.privateFlags;
}
void exportCamera(Camera* a,rw::Camera* n) {
    a->projection=n->projection; a->viewWindow=n->viewWindow; a->viewOffset=n->viewOffset;
    a->reciprocalViewWindow={1.0f/n->viewWindow.x,1.0f/n->viewWindow.y};
    a->nearPlane=n->nearPlane; a->farPlane=n->farPlane; a->fogPlane=n->fogPlane;
    a->zScale=n->zScale; a->zShift=n->zShift;
}
void exportCameraSpatial(Camera* a,rw::Camera* n) {
    a->viewMatrix=n->viewMatrix; a->frustumBoundBox=n->frustumBoundBox;
    std::memcpy(a->frustumPlanes,n->frustumPlanes,sizeof(a->frustumPlanes));
    std::memcpy(a->frustumCorners,n->frustumCorners,sizeof(a->frustumCorners));
}
void exportAtomic(Atomic* a,rw::Atomic* n) {
    a->boundingSphere=n->boundingSphere; a->worldBoundingSphere=n->worldBoundingSphere;
    a->object.object.privateFlags=n->object.object.privateFlags;
}
// Callback pointers at +48 and parent +4 are game pointers. The native render
// callback is a typed adapter, never a cast to a game callback.
void nativeRender(rw::Atomic* n) {
    for(auto& p:atomics) if(p.second==n) { p.first->renderCB(p.first); return; }
}
Atomic* adoptAtomic(rw::Atomic* n) {
    auto* a=allocate<Atomic>(K::Atomic); if(!a) return nullptr;
    a->object.object={rw::Atomic::ID,0,n->object.object.flags,n->object.object.privateFlags,nullptr};
    a->object.sync=syncFacade; ring(a->inClump); ring(a->worldSectors);
    a->renderCB=defaultRender; a->interpolator={3,0,0,1.0f,1.0f,0.0f};
    for(auto& p:geometries) if(p.second.n==n->geometry) { a->geometry=p.first; break; }
    exportAtomic(a,n);
    if(!insert(atomics,a,n)) { freeFacade(a); return nullptr; }
    n->renderCB=nativeRender;
    if(!constructPlugins(K::Atomic,a)) { atomics.erase(a); freeFacade(a); return nullptr; }
    return a;
}
// 006c72d0 constructs a temporary atomic to capture its default callback +48.
// Other callers install geometry/frame and read +18/+4 directly.
Atomic* __stdcall atomicCreate() {
    if(rw::Engine::state!=rw::Engine::Started) return nullptr;
    auto* n=rw::Atomic::create(); if(!n) return nullptr;
    auto* a=adoptAtomic(n); if(!a) n->destroy(); return a;
}
// Geometry ownership is read as a signed 16-bit count at +0E. librw releases
// the old geometry even on self-assignment; the game API does not. Release via
// the facade so material/plugin/texture maps cannot outlive the native owner.
Atomic* __cdecl atomicGeometry(Atomic* a,Geometry* g,unsigned char flags) {
    auto* n=atomicNative(a); auto* b=geometryNative(g);
    if(!n || (g && !b)) return nullptr;
    if(a->geometry==g) return a;
    if(b && b->refCount>=SHRT_MAX) return nullptr;
    if(a->geometry && !geometryCanRelease(a->geometry)) return nullptr;
    auto* old=a->geometry; n->geometry=nullptr;
    n->setGeometry(b,flags);
    if(g) exportGeometry(g,geometries.at(g));
    if(old) geometryDestroy(old);
    a->geometry=g;
    exportAtomic(a,n); return a;
}
// Callers detach/destroy atomics independently of their frame. The atomic
// owns one geometry reference, NOT its frame; remove both facade/native links.
int __cdecl atomicDestroy(Atomic* a) {
    auto* n=atomicNative(a); if(!n || n->world || (a->geometry && !geometryCanRelease(a->geometry))) return 0;
    destructPlugins(K::Atomic,a);
    if(a->clump) { auto* c=lookup(clumps,a->clump); if(c) c->removeAtomic(n); unlink(a->inClump); }
    auto* g=a->geometry;
    // Geometry/material dtors may still inspect their owning atomic/frame.
    // Keep those identities live until the owned reference has been released.
    if(g) geometryDestroy(g);
    n->geometry=nullptr; a->geometry=nullptr;
    attachFrame(&a->object,nullptr);
    atomics.erase(a); n->destroy(); freeFacade(a); return 1;
}
// SetFrame returns the atomic, preserves +4 facade identity and participates in
// FrameForAllObjects. librw frame lists must only contain native objects.
Atomic* __cdecl atomicFrame(Atomic* a,Frame* f) {
    if(!lookup(atomics,a) || (f && !frameNative(f))) return nullptr;
    if(!attachFrame(&a->object,f)) return nullptr;
    a->object.object.privateFlags|=rw::Atomic::WORLDBOUNDDIRTY; return a;
}
// The callers invalidate bit 1 at interpolator +50 and expect the local sphere
// +1C to reflect their +54/+56 morph indices and +5C/+60 interpolation values.
// librw has no interpolator: compose its vector operations over POD targets.
Atomic* __cdecl resyncSphere(Atomic* a) {
    auto* n=atomicNative(a); if(!n) return nullptr;
    if(!a->geometry) return a;
    auto* g=geometryNative(a->geometry); if(!g || !g->numMorphTargets) return nullptr;
    int first=a->interpolator.start, last=a->interpolator.end;
    if(first<0 || last<0 || first>=g->numMorphTargets || last>=g->numMorphTargets) first=last=0;
    auto& s=g->morphTargets[first].boundingSphere; auto& t=g->morphTargets[last].boundingSphere;
    float f=first==last?0.0f:a->interpolator.position*a->interpolator.reciprocalTime;
    n->boundingSphere={rw::add(s.center,rw::scale(rw::sub(t.center,s.center),f)),s.radius+(t.radius-s.radius)*f};
    n->object.object.privateFlags|=rw::Atomic::WORLDBOUNDDIRTY;
    a->interpolator.flags&=~2u; exportAtomic(a,n); return a;
}
// ClumpRender callers use null as failure and install facade callbacks. librw's
// void render cannot propagate their failures; keep rendering remaining members.
Clump* __cdecl clumpRender(Clump* c) {
    if(!clumpNative(c)) return nullptr;
    bool ok=true;
    for(auto* a:snapshot<Atomic>(c->atomics,offsetof(Atomic,inClump))) {
        if(!atomics.count(a) || !(a->object.object.flags&rw::Atomic::RENDER)) continue;
        if(a->object.object.parent) getLTM(static_cast<Frame*>(a->object.object.parent));
        if(!a->renderCB || !a->renderCB(a)) ok=false;
    }
    return ok?c:nullptr;
}
Atomic* __cdecl defaultRender(Atomic* a) {
    auto* n=atomicNative(a); if(!n || !n->geometry) return nullptr;
    // No fake success on NULL: actual GPU instancing belongs to slice 7.
    if(rw::platform==rw::PLATFORM_NULL || a->pipeline) return nullptr;
    rw::Atomic::defaultRenderCB(n); exportAtomic(a,n); return a;
}
using AtomicCB=Atomic* (__cdecl *)(Atomic*,void*);
using LightCB=Light* (__cdecl *)(Light*,void*);
using MaterialCB=Material* (__cdecl *)(Material*,void*);
// ForAll callbacks consume game identities and stop on null; snapshots permit
// removing the current member, unlike an unsafely advanced native link walker.
Clump* __cdecl clumpAtomics(Clump* c,AtomicCB cb,void* data) {
    if(!clumpNative(c) || !cb) return nullptr;
    for(auto* a:snapshot<Atomic>(c->atomics,offsetof(Atomic,inClump)))
        if(atomics.count(a) && a->clump==c && !cb(a,data)) break;
    return c;
}
// Light membership is an original plugin extension, not librw's light prefix.
// Translate using the sidecar's membership instead of a guessed game offset.
Clump* __cdecl clumpLights(Clump* c,LightCB cb,void* data) {
    auto* n=clumpNative(c); if(!n || !cb) return nullptr;
    std::vector<Light*> list;
    FORLIST(l,n->lights) { auto* b=rw::Light::fromClump(l); for(auto& p:lights) if(p.second==b) list.push_back(p.first); }
    for(auto* a:list) if(lights.count(a) && lights.at(a)->clump==n && !cb(a,data)) break;
    return c;
}
// RemoveLight does not destroy it; callers subsequently destroy its frame and
// light. Native membership lives outside the old 0x40 game header.
Clump* __cdecl clumpRemoveLight(Clump* c,Light* a) {
    auto* n=clumpNative(c); auto* b=lightNative(a);
    if(!n || !b || b->clump!=n) return nullptr;
    n->removeLight(b); return c;
}
// Both count callers expect integers, not object handles. GetNumLights has ONE
// argument (export ABI records a spurious second; original body/RET confirms).
int __cdecl clumpAtomicCount(Clump* c) { auto* n=clumpNative(c); return n?n->countAtomics():0; }
int __cdecl clumpLightCount(Clump* c) { auto* n=clumpNative(c); return n?n->countLights():0; }
// Material visitors see +0 texture and +4 colour, plus the facade plugin tail.
Geometry* __cdecl geometryMaterials(Geometry* g,MaterialCB cb,void* data) {
    if(!geometryNative(g) || !cb) return nullptr;
    auto list=geometries.at(g).materials;
    for(auto* a:list) if(materials.count(a) && !cb(a,data)) break;
    return g;
}
// Caller obtains a separately held geometry reference; export the count before
// another inward conversion can overwrite librw's increment.
Geometry* __cdecl geometryAddRef(Geometry* g) {
    auto* n=geometryNative(g); if(!n || n->refCount>=SHRT_MAX) return nullptr;
    n->addRef(); exportGeometry(g,geometries.at(g)); return g;
}
// Final geometry release owns one reference to each material-list entry. Strip
// the native list before release and use facade destruction for its owners.
int __cdecl geometryDestroy(Geometry* g) {
    auto* n=geometryNative(g); if(!n) return 0;
    if(!geometryCanRelease(g)) return 0;
    if(n->refCount>1) { n->destroy(); exportGeometry(g,geometries.at(g)); return 1; }
    destructPlugins(K::Geometry,g);
    auto list=geometries.at(g).materials;
    n->matList.numMaterials=0;
    for(auto* m:list) materialDestroy(m);
    geometries.erase(g); n->destroy(); freeFacade(g);
    return 1;
}
// SetTexture callers expect a retained texture (+54) and release of the old
// texture. librw releases before retain (unsafe on self-assignment at ref=1).
// Retain with native setTexture on an empty owner, export, then facade release.
Material* __cdecl materialTexture(Material* a,Texture* t) {
    auto* n=lookup(materials,a); auto* b=textureNative(t);
    if(!n || a->refCount<=0 || (t && (!b || b->refCount<=0 || b->refCount==INT_MAX))) return nullptr;
    n->refCount=a->refCount; n->color=a->color; n->surfaceProps=a->surfaceProps;
    auto* old=textureFacade(n->texture);
    if(n->texture && !old) return nullptr;
    n->texture=nullptr; n->setTexture(b);
    if(t) textureExport(t);
    if(old && !textureRelease(old)) {
        // Keep ownership tracked if a locked raster prevented final release.
        n->texture=nullptr; if(t) textureRelease(t);
        n->texture=textureNative(old); a->texture=old; return nullptr;
    }
    a->texture=t; return a;
}
// Radius/color are read directly at +14/+18; alpha is part of the game colour
// struct but native setColor only accepts RGB. Preserve it explicitly.
Light* __cdecl lightRadius(Light* a,float r) {
    auto* n=lightNative(a); if(!n) return nullptr; n->radius=r;
    if(n->getFrame()) n->getFrame()->updateObjects();
    frameExport(); exportLight(a,n); return a;
}
Light* __cdecl lightColor(Light* a,const rw::RGBAf* color) {
    auto* n=lightNative(a); if(!n || !color) return nullptr;
    n->setColor(color->red,color->green,color->blue); n->color.alpha=color->alpha; exportLight(a,n); return a;
}
// 006fd180 forwards ST0, consumed as double. librw's float angle is promoted;
// angles are radians, unlike the matrix/quat degree APIs in slice 1.
double __cdecl lightAngle(Light* a) {
    auto* n=lightNative(a); if(!n) return 0.0;
    // Original getter saturates finite out-of-domain cosines. Keep the game
    // field unchanged, but prevent librw acosf from returning a spurious NaN.
    float saved=n->minusCosAngle;
    if(saved>1) n->minusCosAngle=1; else if(saved<-1) n->minusCosAngle=-1;
    double result=n->getAngle(); n->minusCosAngle=saved; return result;
}
// SetConeAngle callers pass radians and expect their light handle back.
Light* __cdecl lightSetAngle(Light* a,float v) {
    auto* n=lightNative(a); if(!n || v<0 || v>1.5707964f) return nullptr;
    n->setAngle(v); exportLight(a,n); return a;
}
// Light destroy is separate from frame destroy and must unlink clump membership
// without allowing native destruction to leave the game's object ring stale.
int __cdecl lightDestroy(Light* a) {
    auto* n=lightNative(a); if(!n || n->world) return 0;
    destructPlugins(K::Light,a); if(n->clump) n->clump->removeLight(n);
    attachFrame(&a->object,nullptr); lights.erase(a); n->destroy(); freeFacade(a); return 1;
}
// Create receives an 8-bit subtype (ambient/directional/point/spot); native
// minusCosAngle defaults to +1 but the original constructor's field is zero.
Light* __cdecl lightCreate(unsigned char type) {
    auto* a=allocate<Light>(K::Light); if(!a) return nullptr;
    auto* n=rw::Light::create(type); if(!n) { freeFacade(a); return nullptr; }
    n->minusCosAngle=0;
    a->object.object={rw::Light::ID,type,n->object.object.flags,n->object.object.privateFlags,nullptr};
    a->object.sync=syncFacade; ring(a->worldSectors); ring(a->inWorld); exportLight(a,n);
    if(!insert(lights,a,n)) { n->destroy(); freeFacade(a); return nullptr; }
    if(!constructPlugins(K::Light,a)) { lights.erase(a); n->destroy(); freeFacade(a); return nullptr; }
    return a;
}
// Clip setters return a camera handle, recompute device Z mapping and dirty its
// frame; facade layout places buffers before view/clip, unlike librw.
Camera* __cdecl cameraNear(Camera* a,float v) { auto* n=cameraNative(a); if(!n) return nullptr; n->setNearPlane(v); exportCamera(a,n); return a; }
Camera* __cdecl cameraFar(Camera* a,float v) { auto* n=cameraNative(a); if(!n) return nullptr; n->setFarPlane(v); exportCamera(a,n); return a; }
Camera* __cdecl defaultBegin(Camera* a) {
    auto* n=cameraNative(a); if(!n || !syncFrames()) return nullptr;
    if(deviceReady() || rw::platform!=rw::PLATFORM_NULL) {
        if(!n->getFrame() || !n->frameBuffer || n->frameBuffer->width<=0 || n->frameBuffer->height<=0 ||
           (n->frameBuffer->type!=rw::Raster::CAMERA && n->frameBuffer->type!=rw::Raster::CAMERATEXTURE) ||
           (n->zBuffer && n->zBuffer->type!=rw::Raster::ZBUFFER) ||
           rw::engine->currentCamera || !beginDeviceCamera(n)) return nullptr;
    } else n->beginUpdate();
    frameExport(); objectExport(); return a;
}
Camera* __cdecl defaultEnd(Camera* a) {
    auto* n=cameraNative(a); if(!n) return nullptr;
    if(deviceReady() || rw::platform!=rw::PLATFORM_NULL) {
        if(rw::engine->currentCamera!=n || !endDeviceCamera(n)) return nullptr;
    } else n->endUpdate();
    rw::engine->currentCamera=nullptr; return a;
}
// 00726a80 checks the callback's handle/null result. The facade +18 callback
// is game-facing; the native callback accepts a native Camera. NULL's device
// callbacks are intentionally void no-ops, NOT the asserting raster callbacks.
// The device seam delegates D3D9/recording updates; native callbacks are void,
// so GPU HRESULTs remain unobservable rather than fabricated here.
Camera* __cdecl cameraBegin(Camera* a) {
    startupTrace("0x00628E50","CameraBeginUpdate");
    if(!lookup(cameras,a) || !a->beginUpdate) return nullptr;
    return a->beginUpdate(a);
}
Camera* __cdecl cameraEnd(Camera* a) {
    startupTrace("0x00628E40","CameraEndUpdate");
    if(!lookup(cameras,a) || !a->endUpdate) return nullptr;
    return a->endUpdate(a);
}
// 00726a80/00708e40 pass RGBA bytes and clear mode, expect camera/null. NULL
// intentionally does no buffer clear. D3D9/recording delegates through the
// device seam; never dereference an ABI Raster as a native one.
Camera* __cdecl cameraClear(Camera* a,rw::RGBA* color,unsigned mode) {
    startupTrace("0x00628F60","CameraClear");
    auto* n=cameraNative(a); if(!n || !color) return nullptr;
    if(deviceReady() || rw::platform!=rw::PLATFORM_NULL) {
        if(!n->frameBuffer || n->frameBuffer->width<=0 || n->frameBuffer->height<=0 ||
           (n->frameBuffer->type!=rw::Raster::CAMERA && n->frameBuffer->type!=rw::Raster::CAMERATEXTURE) ||
           ((mode&2u) && (!n->zBuffer || n->zBuffer->type!=rw::Raster::ZBUFFER)) ||
           (mode&~7u) || !clearDeviceCamera(n,color,mode)) return nullptr;
    } else n->clear(color,mode);
    return a;
}
// Sphere test uses the synchronized planes (0 outside, 1 boundary, 2 inside).
// Do not rebuild them per test: game callers sync the camera frame beforehand.
int __cdecl cameraTest(Camera* a,const rw::Sphere* s) {
    auto* n=cameraNative(a); return n && s?n->frustumTestSphere(s):rw::Camera::SPHEREOUTSIDE;
}
// Projection accepts only 1/2; native setProjection accepts arbitrary integers.
Camera* __cdecl cameraProjection(Camera* a,int p) { auto* n=cameraNative(a); if(!n || p<1 || p>2) return nullptr; n->setProjection(p); n->setNearPlane(n->nearPlane); exportCamera(a,n); return a; }
// View window is read directly with its reciprocal at +70/+74. The native
// camera has no reciprocal fields; maintain them in the outward bridge.
Camera* __cdecl cameraWindow(Camera* a,const rw::V2d* v) {
    auto* n=cameraNative(a); if(!n || !v || v->x<=0 || v->y<=0) return nullptr;
    n->setViewWindow(v); exportCamera(a,n); return a;
}
// Camera owns neither its frame nor its buffers. Destroy must only detach.
int __cdecl cameraDestroy(Camera* a) {
    auto* n=cameraNative(a); if(!n || n->world || (deviceReady() && rw::engine->currentCamera==n)) return 0;
    destructPlugins(K::Camera,a); if(n->clump) n->clump->removeCamera(n);
    if(rw::engine->currentCamera==n) rw::engine->currentCamera=nullptr;
    attachFrame(&a->object,nullptr); cameras.erase(a); n->destroy(); freeFacade(a); return 1;
}
// Create callers write facade +4/+60/+64 themselves. Initialize readable
// matrix/frustum storage deterministically until the first frame sync.
Camera* __stdcall cameraCreate() {
    startupTrace("0x006290B0","CameraCreate");
    auto* a=allocate<Camera>(K::Camera); if(!a) return nullptr;
    auto* n=rw::Camera::create(); if(!n) { freeFacade(a); return nullptr; }
    a->object.object={rw::Camera::ID,0,0,0,nullptr}; a->object.sync=syncFacade;
    a->beginUpdate=defaultBegin; a->endUpdate=defaultEnd;
    n->setNearPlane(n->nearPlane); exportCamera(a,n); a->viewMatrix.setIdentity();
    if(!insert(cameras,a,n)) { n->destroy(); freeFacade(a); return nullptr; }
    if(!constructPlugins(K::Camera,a)) { cameras.erase(a); n->destroy(); freeFacade(a); return nullptr; }
    return a;
}
void destroyTree(Frame* f) { while(f->child) destroyTree(f->child); destroyFrame(f); }
// Clump destroy owns its atomics/lights/cameras AND frame hierarchy. Native
// recursive destruction bypasses facade identities; remove members first and
// route each owned release through the corresponding boundary.
int __cdecl clumpDestroy(Clump* c) {
    auto* n=clumpNative(c); if(!n || n->world) return 0;
    // Account all references this clump will release, not just the first atomic.
    std::unordered_map<Geometry*,int> references;
    std::unordered_map<Material*,int> materialRefs;
    std::unordered_map<Texture*,int> textureRefs;
    for(auto* a:snapshot<Atomic>(c->atomics,offsetof(Atomic,inClump))) {
        auto* b=atomicNative(a); if(!b || b->world) return 0;
        if(a->geometry) ++references[a->geometry];
    }
    for(auto& p:references) if(p.first->refCount<=p.second)
        for(auto* m:geometries.at(p.first).materials) ++materialRefs[m];
    for(auto& p:materialRefs) if(p.first->refCount<=p.second && p.first->texture) ++textureRefs[p.first->texture];
    for(auto& p:textureRefs) if(!textureCanRelease(p.first,p.second)) return 0;
    for(auto& p:lights) if(p.second->clump==n && p.second->world) return 0;
    for(auto& p:cameras) if(p.second->clump==n && p.second->world) return 0;
    destructPlugins(K::Clump,c);
    for(auto* a:snapshot<Atomic>(c->atomics,offsetof(Atomic,inClump))) atomicDestroy(a);
    std::vector<Light*> ls; for(auto& p:lights) if(p.second->clump==n) ls.push_back(p.first);
    for(auto* a:ls) lightDestroy(a);
    std::vector<Camera*> cs; for(auto& p:cameras) if(p.second->clump==n) cs.push_back(p.first);
    for(auto* a:cs) cameraDestroy(a);
    auto* f=static_cast<Frame*>(c->object.parent); n->setFrame(nullptr);
    if(f) destroyTree(f);
    c->object.parent=nullptr; n->setFrame(nullptr);
    clumps.erase(c); n->destroy(); freeFacade(c); return 1;
}
Frame* cloneTree(Frame* source,std::unordered_map<Frame*,Frame*>& map) {
    auto* f=createFrame(); if(!f) return nullptr;
    f->matrix=source->matrix;
    try { map.emplace(source,f); } catch(...) { destroyFrame(f); return nullptr; }
    plugins(K::Frame).copy(f,source);
    for(Frame* child=source->child;child;child=child->next) {
        auto* copy=cloneTree(child,map); if(!copy) { destroyTree(f); return nullptr; } addChild(f,copy);
    }
    return f;
}
// Clump clone callers expect a distinct hierarchy, shared retained geometries,
// copied plugin tails and facade callbacks. Both original and native clone only
// atomics, NOT lights/cameras. Native clone loses interpolated local spheres and
// dereferences a null root: compose its object clone primitives, reject null root.
Clump* __cdecl clumpClone(Clump* c) {
    auto* n=clumpNative(c); if(!n || n->world || !c->object.parent) return nullptr;
    std::unordered_map<Geometry*,int> copies;
    for(auto* a:snapshot<Atomic>(c->atomics,offsetof(Atomic,inClump))) if(a->geometry) ++copies[a->geometry];
    for(auto& p:copies) if(!geometryNative(p.first) || p.first->refCount>SHRT_MAX-p.second) return nullptr;
    auto* out=clumpCreate(); if(!out) return nullptr;
    std::unordered_map<Frame*,Frame*> frames;
    if(c->object.parent) {
        auto* f=cloneTree(static_cast<Frame*>(c->object.parent),frames);
        if(!f) { clumpDestroy(out); return nullptr; } clumpSetFrame(out,f);
    }
    for(auto* a:snapshot<Atomic>(c->atomics,offsetof(Atomic,inClump))) {
        auto* native=atomicNative(a); auto* b=native?native->clone():nullptr;
        if(!b) { clumpDestroy(out); return nullptr; }
        // Clone increments the shared native count; export BEFORE import.
        if(a->geometry) exportGeometry(a->geometry,geometries.at(a->geometry));
        auto* copy=adoptAtomic(b);
        if(!copy) { b->geometry=nullptr; b->destroy(); if(a->geometry) geometryDestroy(a->geometry); clumpDestroy(out); return nullptr; }
        copy->geometry=a->geometry; copy->renderCB=a->renderCB; copy->interpolator=a->interpolator; copy->pipeline=a->pipeline;
        copy->boundingSphere=a->boundingSphere; b->boundingSphere=a->boundingSphere;
        plugins(K::Atomic).copy(copy,a); clumpAddAtomic(out,copy);
        auto it=frames.find(static_cast<Frame*>(a->object.object.parent));
        if(it==frames.end()) { clumpDestroy(out); return nullptr; }
        atomicFrame(copy,it->second);
    }
    out->callback=c->callback;
    plugins(K::Clump).copy(out,c); return out;
}
}

namespace csf_rw {
bool objectsEmpty() { return materials.empty() && geometries.empty() && atomics.empty() && lights.empty() && cameras.empty() && clumps.empty(); }
bool worldHasCamera(rw::World* w) { for(auto& p:cameras) if(p.second->world==w) return true; return false; }
rw::Material* materialNative(Material* a) {
    auto* n=lookup(materials,a); if(!n || a->refCount<=0) return nullptr;
    if(n->texture && !textureFacade(n->texture)) return nullptr;
    if(a->texture!=textureFacade(n->texture) && !materialTexture(a,a->texture)) return nullptr;
    if(a->texture && !textureNative(a->texture)) return nullptr;
    n->color=a->color; n->surfaceProps=a->surfaceProps; n->refCount=a->refCount; return n;
}
rw::Geometry* geometryNative(Geometry* a) {
    auto it=geometries.find(a); if(it==geometries.end() || a->refCount<=0) return nullptr;
    auto& s=it->second; auto* n=s.n;
    // Structural storage is boundary-owned; in-place POD edits are shared.
    if(a->numVertices!=n->numVertices || a->numTriangles!=n->numTriangles || a->numMorphTargets!=n->numMorphTargets ||
       a->numTexCoordSets!=n->numTexCoordSets ||
       n->matList.numMaterials!=static_cast<int>(s.materials.size()) ||
       a->morphTargets!=s.morphs.data() || a->triangles!=n->triangles || a->colors!=n->colors ||
       a->matList.materials!=s.materials.data() || a->matList.numMaterials!=static_cast<int>(s.materials.size())) return nullptr;
    n->refCount=a->refCount; n->flags=a->flags; n->lockedSinceInst=a->lockedSinceInst;
    for(int i=0;i<8;i++) if(a->texCoords[i]!=n->texCoords[i]) return nullptr;
    for(int i=0;i<n->numMorphTargets;i++) {
        if(s.morphs[i].parent!=a || s.morphs[i].vertices!=n->morphTargets[i].vertices || s.morphs[i].normals!=n->morphTargets[i].normals) return nullptr;
        n->morphTargets[i].boundingSphere=s.morphs[i].boundingSphere;
    }
    for(size_t i=0;i<s.materials.size();++i) {
        auto* b=materialNative(s.materials[i]); if(!b) return nullptr;
        auto* old=n->matList.materials[i]; if(old==b) continue;
        Material* oldFacade=nullptr; for(auto& p:materials) if(p.second==old) { oldFacade=p.first; break; }
        if(!oldFacade || b->refCount>=SHRT_MAX) return nullptr;
        b->addRef(); exportMaterial(s.materials[i],b);
        if(!materialDestroy(oldFacade)) { materialDestroy(s.materials[i]); return nullptr; }
        n->matList.materials[i]=b;
    }
    return n;
}
rw::Atomic* atomicNative(Atomic* a) {
    auto* n=lookup(atomics,a); if(!n) return nullptr;
    auto* f=frameNative(static_cast<Frame*>(a->object.object.parent));
    if(a->object.object.parent && !f) return nullptr;
    if(n->getFrame()!=f) attachFrame(&a->object,static_cast<Frame*>(a->object.object.parent));
    n->object.object.flags=a->object.object.flags; n->object.object.privateFlags=a->object.object.privateFlags;
    n->boundingSphere=a->boundingSphere; n->worldBoundingSphere=a->worldBoundingSphere;
    auto* g=geometryNative(a->geometry); if(a->geometry && !g) return nullptr;
    if(n->geometry!=g) {
        Geometry* old=nullptr;
        for(auto& p:geometries) if(p.second.n==n->geometry) { old=p.first; break; }
        if(n->geometry && !old) return nullptr;
        if(old && !geometryCanRelease(old)) return nullptr;
        if(g && g->refCount>=SHRT_MAX) return nullptr;
        n->geometry=nullptr; n->setGeometry(g,rw::Atomic::SAMEBOUNDINGSPHERE);
        if(a->geometry) exportGeometry(a->geometry,geometries.at(a->geometry));
        if(old) geometryDestroy(old);
    }
    return n;
}
rw::Light* lightNative(Light* a) {
    auto* n=lookup(lights,a); if(!n) return nullptr;
    auto* f=frameNative(static_cast<Frame*>(a->object.object.parent)); if(a->object.object.parent && !f) return nullptr;
    if(n->getFrame()!=f) attachFrame(&a->object,static_cast<Frame*>(a->object.object.parent));
    n->object.object.flags=a->object.object.flags; n->object.object.privateFlags=a->object.object.privateFlags;
    n->radius=a->radius; n->color=a->color; n->minusCosAngle=a->minusCosAngle; return n;
}
rw::Camera* cameraNative(Camera* a) {
    auto* n=lookup(cameras,a); if(!n) return nullptr;
    auto* f=frameNative(static_cast<Frame*>(a->object.object.parent)); if(a->object.object.parent && !f) return nullptr;
    if(n->getFrame()!=f) attachFrame(&a->object,static_cast<Frame*>(a->object.object.parent));
    auto* fb=rasterNative(a->frameBuffer); auto* zb=rasterNative(a->zBuffer);
    if((a->frameBuffer && !fb) || (a->zBuffer && !zb)) return nullptr;
    n->frameBuffer=fb; n->zBuffer=zb; n->viewWindow=a->viewWindow; n->viewOffset=a->viewOffset;
    n->nearPlane=a->nearPlane; n->farPlane=a->farPlane; n->fogPlane=a->fogPlane; n->projection=a->projection;
    n->viewMatrix=a->viewMatrix; n->frustumBoundBox=a->frustumBoundBox;
    std::memcpy(n->frustumPlanes,a->frustumPlanes,sizeof(a->frustumPlanes));
    std::memcpy(n->frustumCorners,a->frustumCorners,sizeof(a->frustumCorners));
    return n;
}
rw::Clump* clumpNative(Clump* c) {
    auto* n=lookup(clumps,c); if(!n) return nullptr;
    auto* f=frameNative(static_cast<Frame*>(c->object.parent)); if(c->object.parent && !f) return nullptr;
    n->setFrame(f); return n;
}
void objectFrameChanged(ObjectWithFrame* o) {
    // Works for the generic synthetic slice-1 objects too: only tracked owners
    // acquire native frame links. Called BEFORE destroying an attached frame.
    auto* f=frameNative(static_cast<Frame*>(o->object.parent));
    if(auto* n=lookup(atomics,reinterpret_cast<Atomic*>(o)); n && n->getFrame()!=f) n->setFrame(f);
    if(auto* n=lookup(lights,reinterpret_cast<Light*>(o)); n && n->getFrame()!=f) n->setFrame(f);
    if(auto* n=lookup(cameras,reinterpret_cast<Camera*>(o)); n && n->getFrame()!=f) n->setFrame(f);
}
void objectExport() {
    for(auto& p:atomics) exportAtomic(p.first,p.second);
    for(auto& p:cameras) if(p.second->getFrame()) exportCameraSpatial(p.first,p.second);
}
void objectImport() {
    for(auto& p:atomics) atomicNative(p.first);
    for(auto& p:lights) lightNative(p.first);
    for(auto& p:cameras) cameraNative(p.first);
}
Material* materialCreate() {
    auto* a=allocate<Material>(K::Material); if(!a) return nullptr;
    auto* n=rw::Material::create(); if(!n) { freeFacade(a); return nullptr; }
    exportMaterial(a,n);
    if(!insert(materials,a,n)) { n->destroy(); freeFacade(a); return nullptr; }
    if(!constructPlugins(K::Material,a)) { materials.erase(a); n->destroy(); freeFacade(a); return nullptr; } return a;
}
int materialDestroy(Material* a) {
    auto* n=materialNative(a); if(!n) return 0;
    if(n->refCount>1) { n->destroy(); exportMaterial(a,n); return 1; }
    auto* t=a->texture;
    // A texture may reject final release while locked; do not lose its owner.
    if(t && !textureCanRelease(t)) return 0;
    destructPlugins(K::Material,a);
    if(t) textureRelease(t);
    a->texture=nullptr; n->texture=nullptr;
    materials.erase(a); n->destroy(); freeFacade(a); return 1;
}
Geometry* geometryCreate(int verts,int tris,unsigned flags) {
    // librw allocates combined data using signed int sizes and supports 8 UV
    // sets. The native fixture helper bounds those inputs before calling it.
    if(verts<0 || tris<0 || verts>65535 || tris>1000000 || ((flags>>16)&255)>8) return nullptr;
    auto* a=allocate<Geometry>(K::Geometry); if(!a) return nullptr;
    auto* n=rw::Geometry::create(verts,tris,flags); if(!n) { freeFacade(a); return nullptr; }
    try { GeoState s{}; s.n=n; s.morphs.resize(n->numMorphTargets); geometries.emplace(a,std::move(s)); }
    catch(...) { n->destroy(); freeFacade(a); return nullptr; }
    exportGeometry(a,geometries.at(a));
    if(!constructPlugins(K::Geometry,a)) { geometries.erase(a); n->destroy(); freeFacade(a); return nullptr; } return a;
}
bool geometryAppendMaterial(Geometry* g,Material* m) {
    auto* n=geometryNative(g); auto* b=materialNative(m); if(!n || !b || b->refCount>=SHRT_MAX) return false;
    auto& s=geometries.at(g);
    try { s.materials.push_back(m); } catch(...) { return false; }
    if(n->matList.appendMaterial(b)<0) { s.materials.pop_back(); exportGeometry(g,s); return false; }
    exportMaterial(m,b); exportGeometry(g,s); return true;
}
bool geometryAddMorphTargets(Geometry* g,int count) {
    auto* n=geometryNative(g); if(!n || count<0 || count>64-n->numMorphTargets) return false;
    auto& s=geometries.at(g);
    try { s.morphs.resize(n->numMorphTargets+count); } catch(...) { return false; }
    n->addMorphTargets(count); exportGeometry(g,s); return true;
}
Clump* clumpCreate() {
    auto* a=allocate<Clump>(K::Clump); if(!a) return nullptr;
    auto* n=rw::Clump::create(); if(!n) { freeFacade(a); return nullptr; }
    a->object.type=rw::Clump::ID; ring(a->atomics); ring(a->lights); ring(a->cameras); ring(a->inWorld);
    if(!insert(clumps,a,n)) { n->destroy(); freeFacade(a); return nullptr; }
    if(!constructPlugins(K::Clump,a)) { clumps.erase(a); n->destroy(); freeFacade(a); return nullptr; } return a;
}
bool clumpAddAtomic(Clump* c,Atomic* a) {
    auto* n=clumpNative(c); auto* b=atomicNative(a); if(!n || !b || b->clump) return false;
    n->addAtomic(b); a->clump=c; append(c->atomics,a->inClump); return true;
}
bool clumpAddLight(Clump* c,Light* a) {
    auto* n=clumpNative(c); auto* b=lightNative(a); if(!n || !b || b->clump) return false;
    n->addLight(b); return true;
}
void clumpSetFrame(Clump* c,Frame* f) { if(lookup(clumps,c) && (!f || frameNative(f))) { c->object.parent=f; clumpNative(c); } }
}

// Caller 006e5530 (_rpAtomicResyncInterpolatedSphere): see facade/return/ownership contract above; packet in work/rw4-check/006e5530.packet.
HOOK(0x0058EB40,resyncSphere,"boundary-rw");
// Caller 00704000 (RpClumpRender): see facade/return/ownership contract above; packet in work/rw4-check/00704000.packet.
HOOK(0x0058EEB0,clumpRender,"boundary-rw");
// Caller 006f71a0 (RpClumpForAllAtomics): see facade/return/ownership contract above; packet in work/rw4-check/006f71a0.packet.
HOOK(0x0058EF00,clumpAtomics,"boundary-rw");
// Caller 006bd130 (RpClumpForAllLights): see facade/return/ownership contract above; packet in work/rw4-check/006bd130.packet.
HOOK(0x0058EF90,clumpLights,"boundary-rw");
// Caller 006e68e0 (RpAtomicSetGeometry): see facade/return/ownership contract above; packet in work/rw4-check/006e68e0.packet.
HOOK(0x0058EFE0,atomicGeometry,"boundary-rw");
// Caller 00703710 (RpAtomicDestroy): see facade/return/ownership contract above; packet in work/rw4-check/00703710.packet.
HOOK(0x0058F060,atomicDestroy,"boundary-rw");
// Caller 006bd130 (RpClumpRemoveLight): see facade/return/ownership contract above; packet in work/rw4-check/006bd130.packet.
HOOK(0x0058F1D0,clumpRemoveLight,"boundary-rw");
// Caller 006eed20 (RpAtomicSetFrame): see facade/return/ownership contract above; packet in work/rw4-check/006eed20.packet.
HOOK(0x0058F300,atomicFrame,"boundary-rw");
// Caller 006b3b40 (RpClumpGetNumAtomics): see facade/return/ownership contract above; packet in work/rw4-check/006b3b40.packet.
HOOK(0x0058F560,clumpAtomicCount,"boundary-rw");
// Caller 006be640 (RpClumpGetNumLights): see facade/return/ownership contract above; packet in work/rw4-check/006be640.packet.
HOOK(0x0058F590,clumpLightCount,"boundary-rw");
// Caller 006c72d0 (RpAtomicCreate): see facade/return/ownership contract above; packet in work/rw4-check/006c72d0.packet.
HOOK(0x0058F5C0,atomicCreate,"boundary-rw");
// Caller 006d41c0 (RpClumpDestroy): see facade/return/ownership contract above; packet in work/rw4-check/006d41c0.packet.
HOOK(0x0058F740,clumpDestroy,"boundary-rw");
// Caller 006d4740 (RpClumpClone): see facade/return/ownership contract above; packet in work/rw4-check/006d4740.packet.
HOOK(0x0058FC60,clumpClone,"boundary-rw");
// Caller 006c02f0 (RpGeometryForAllMaterials): see facade/return/ownership contract above; packet in work/rw4-check/006c02f0.packet.
HOOK(0x00590670,geometryMaterials,"boundary-rw");
// Caller 006e5940 (RpGeometryAddRef): see facade/return/ownership contract above; packet in work/rw4-check/006e5940.packet.
HOOK(0x00590B90,geometryAddRef,"boundary-rw");
// Caller 006e6370 (RpGeometryDestroy): see facade/return/ownership contract above; packet in work/rw4-check/006e6370.packet.
HOOK(0x00590FC0,geometryDestroy,"boundary-rw");
// Caller 006f8090 (MaterialSetTexture): see facade/return/ownership contract above; packet in work/rw4-check/006f8090.packet.
HOOK(0x00591560,materialTexture,"boundary-rw");
// Caller 006fd5d0 (RpLightSetRadius): see facade/return/ownership contract above; packet in work/rw4-check/006fd5d0.packet.
HOOK(0x0059CBD0,lightRadius,"boundary-rw");
// Caller 006fd0e0 (RpLightSetColor): see facade/return/ownership contract above; packet in work/rw4-check/006fd0e0.packet.
HOOK(0x0059CBF0,lightColor,"boundary-rw");
// Caller 006fd180 (RpLightGetConeAngle): see facade/return/ownership contract above; packet in work/rw4-check/006fd180.packet.
HOOK(0x0059CC50,lightAngle,"boundary-rw");
// Caller 006fd190 (RpLightSetConeAngle): see facade/return/ownership contract above; packet in work/rw4-check/006fd190.packet.
HOOK(0x0059CE80,lightSetAngle,"boundary-rw");
// Caller 006bd130 (RpLightDestroy): see facade/return/ownership contract above; packet in work/rw4-check/006bd130.packet.
HOOK(0x0059D000,lightDestroy,"boundary-rw");
// Caller 006fd400 (RpLightCreate): see facade/return/ownership contract above; packet in work/rw4-check/006fd400.packet.
HOOK(0x0059D040,lightCreate,"boundary-rw");
HOOK(0x00628E50,cameraBegin,"boundary-rw");
HOOK(0x00628E40,cameraEnd,"boundary-rw");
// Caller 006f5cd0 (RwCameraSetNearClipPlane): see facade/return/ownership contract above; packet in work/rw4-check/006f5cd0.packet.
HOOK(0x00628E90,cameraNear,"boundary-rw");
// Caller 006f5cd0 (RwCameraSetFarClipPlane): see facade/return/ownership contract above; packet in work/rw4-check/006f5cd0.packet.
HOOK(0x00628EC0,cameraFar,"boundary-rw");
// Caller 006d3760 (RwCameraFrustumTestSphere): see facade/return/ownership contract above; packet in work/rw4-check/006d3760.packet.
HOOK(0x00628EF0,cameraTest,"boundary-rw");
HOOK(0x00628F60,cameraClear,"boundary-rw");
// Caller 006f5cd0 (RwCameraSetProjection): see facade/return/ownership contract above; packet in work/rw4-check/006f5cd0.packet.
HOOK(0x00628F90,cameraProjection,"boundary-rw");
// Caller 006f5cd0 (RwCameraSetViewWindow): see facade/return/ownership contract above; packet in work/rw4-check/006f5cd0.packet.
HOOK(0x00629000,cameraWindow,"boundary-rw");
// Caller 00727bd0 (RwCameraDestroy): see facade/return/ownership contract above; packet in work/rw4-check/00727bd0.packet.
HOOK(0x00629070,cameraDestroy,"boundary-rw");
// Caller 006db160 (RwCameraCreate): see facade/return/ownership contract above; packet in work/rw4-check/006db160.packet.
HOOK(0x006290B0,cameraCreate,"boundary-rw");
#endif
