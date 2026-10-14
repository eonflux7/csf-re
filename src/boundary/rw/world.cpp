// Empty-world startup and membership bridge over native librw World.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "world.h"
#include "startup.h"
#include <unordered_map>
#include <cstring>
#include <algorithm>
CSF_GLOBAL(int, cameraWorldOffset, 0x008B1348);
CSF_GLOBAL(int, lightWorldOffset, 0x008B1354);
CSF_GLOBAL(int, atomicWorldOffset, 0x008B134C);
CSF_GLOBAL(int, clumpWorldOffsetB, 0x008B1350);
CSF_GLOBAL(int, clumpWorldOffset, 0x008B13B4);
namespace {
using namespace csf_rw;
struct Record { rw::World* native; };
std::unordered_map<World*,Record> worlds;
void ring(Link& l) { l.next=l.prev=&l; }
void remove(Link& l) { l.prev->next=l.next;l.next->prev=l.prev;ring(l); }
void append(Link& head,Link& l) { l.next=&head;l.prev=head.prev;head.prev->next=&l;head.prev=&l; }
World* find(rw::World* n) {
    for(auto& p:worlds) if(p.second.native==n) return p.first;
    return nullptr;
}
void publish(void* object,int offset,int delta,World* w) {
#if UINTPTR_MAX == 0xffffffff
    if(offset>=0) std::memcpy(static_cast<char*>(object)+offset+delta,&w,sizeof(w));
#else
    (void)object;(void)offset;(void)delta;(void)w;
#endif
}
Sector* __cdecl emptyRender(Sector* s) {
    // Rendering an actually empty leaf requires no draw. No claim of rendering
    // a loaded BSP: reject nonempty leaves rather than swallowing their draws.
    return s && s->type==-1 && !s->numTriangles && !s->numVertices ? s : nullptr;
}
World* __cdecl createWorld(const rw::BBox* box) {
    startupTrace("0x0058CCB0","RpWorldCreate (empty leaf)");
    if(!rw::engine || rw::Engine::state!=rw::Engine::Started || !box) return nullptr;
    auto* w=static_cast<World*>(rw::Engine::memfuncs.rwmalloc(plugins(Registry::World).size,0));
    if(!w) return nullptr;
    std::memset(w,0,plugins(Registry::World).size);
    // 590400's database name is wrong: this is the 136-byte sector registry.
    size_t rootSize=std::max(size_t(plugins(Registry::Sector).size),sizeof(Sector));
    auto* s=static_cast<Sector*>(rw::Engine::memfuncs.rwmalloc(rootSize,0));
    if(!s) { rw::Engine::memfuncs.rwfree(w); return nullptr; }
    std::memset(s,0,rootSize); s->type=-1;s->bounds=s->collisionBounds=*box;
#if UINTPTR_MAX == 0xffffffff
    s->atomics.next=s->atomics.prev=reinterpret_cast<uintptr_t>(&s->atomics);
    s->lights.next=s->lights.prev=reinterpret_cast<uintptr_t>(&s->lights);
#endif
    w->object={7,0,0,0,nullptr};w->renderOrder=2;w->root=s;w->bounds=*box;
    ring(w->clumps);ring(w->localLights);ring(w->globalLights);w->currentClump=&w->clumps;
    w->sectorRender=emptyRender;
    auto* n=rw::World::create(const_cast<rw::BBox*>(box));
    if(!n) { rw::Engine::memfuncs.rwfree(s);rw::Engine::memfuncs.rwfree(w);return nullptr; }
    bool constructed=constructPlugins(Registry::World,w);
    if(!constructed || !constructPlugins(Registry::Sector,s)) {
        if(constructed) destructPlugins(Registry::World,w);
        n->destroy();rw::Engine::memfuncs.rwfree(s);rw::Engine::memfuncs.rwfree(w);return nullptr;
    }
    try { worlds.emplace(w,Record{n}); }
    catch(...) {
        destructPlugins(Registry::Sector,s);destructPlugins(Registry::World,w);
        n->destroy();rw::Engine::memfuncs.rwfree(s);rw::Engine::memfuncs.rwfree(w);return nullptr;
    }
    return w;
}
SectorRender __cdecl sectorRender(World* w) {
    startupTrace("0x0058CC80","RpWorldGetSectorRenderCallBack");
    return worlds.count(w)?w->sectorRender:nullptr;
}
World* __cdecl sectorCallback(World* w,Sector* (__cdecl *cb)(Sector*,void*)) {
    if(!worlds.count(w)) return nullptr;w->render=cb;return w;
}
World* __cdecl addCamera(World* w,Camera* c) {
    startupTrace("0x0058DD90","RpWorldAddCamera");
    auto p=worlds.find(w);auto* n=cameraNative(c);
    if(p==worlds.end() || !n || n->world) return nullptr;
    p->second.native->addCamera(n);publish(c,cameraWorldOffset,12,w);return w;
}
World* __cdecl removeCamera(World* w,Camera* c) {
    startupTrace("0x0058DDC0","RpWorldRemoveCamera");
    auto p=worlds.find(w);auto* n=cameraNative(c);
    if(p==worlds.end() || !n || n->world!=p->second.native) return nullptr;
    p->second.native->removeCamera(n);publish(c,cameraWorldOffset,12,nullptr);return w;
}
World* __cdecl cameraWorld(Camera* c) {
    auto* n=cameraNative(c);return n?find(n->world):nullptr;
}
World* __cdecl addLight(World* w,Light* l) {
    startupTrace("0x0058DF50","RpWorldAddLight");
    auto p=worlds.find(w);auto* n=lightNative(l);
    if(p==worlds.end() || !n || n->world) return nullptr;
    p->second.native->addLight(n);
    append(l->object.object.subType>=128?w->localLights:w->globalLights,l->inWorld);
    publish(l,lightWorldOffset,0,w);return w;
}
World* __cdecl removeLight(World* w,Light* l) {
    startupTrace("0x0058DFA0","RpWorldRemoveLight");
    auto p=worlds.find(w);auto* n=lightNative(l);
    if(p==worlds.end() || !n || n->world!=p->second.native) return nullptr;
    p->second.native->removeLight(n);remove(l->inWorld);publish(l,lightWorldOffset,0,nullptr);return w;
}
World* __cdecl lightWorld(Light* l) { auto* n=lightNative(l);return n?find(n->world):nullptr; }
// 0058F0E0 RpClumpGetWorld, 0058DE40 RpAtomicGetWorld, 0058DF40 (006FDFE0's clump lookup): the word at the
// world plugin's offset (registered globals), as the original reads it.
World* __cdecl clumpGetWorld(void* c) { return *reinterpret_cast<World**>(static_cast<char*>(c)+clumpWorldOffset); }
World* __cdecl atomicGetWorld(void* a) { return *reinterpret_cast<World**>(static_cast<char*>(a)+atomicWorldOffset); }
World* __cdecl clumpGetWorldB(void* c) { return *reinterpret_cast<World**>(static_cast<char*>(c)+clumpWorldOffsetB); }
// 0058CEF0: the world's clump ring (+0x2C, link at Clump+0x20); next read before the callback, null stops.
World* __cdecl worldClumps(World* w,Clump* (__cdecl *cb)(Clump*,void*),void* data) {
    for(Link* l=w->clumps.next;l!=&w->clumps;) {
        Link* next=l->next;
        if(!cb(reinterpret_cast<Clump*>(reinterpret_cast<char*>(l)-offsetof(Clump,inWorld)),data)) break;
        l=next;
    }
    return w;
}
// 0058CF30: the world's material list (+0x10 array, +0x14 count read once); null stops.
World* __cdecl worldMaterials(World* w,Material* (__cdecl *cb)(Material*,void*),void* data) {
    const int count=w->materials.numMaterials;
    for(int i=0;i<count;++i) if(!cb(w->materials.materials[i],data)) break;
    return w;
}
// 0058CF80: global (directional) lights, then local lights; next is read before the callback (it may remove the
// current light) and a null return ends both walks. Returns the world.
World* __cdecl worldLights(World* w,Light* (__cdecl *cb)(Light*,void*),void* data) {
    for(Link* ring:{&w->globalLights,&w->localLights})
        for(Link* l=ring->next;l!=ring;) {
            Link* next=l->next;
            if(!cb(reinterpret_cast<Light*>(reinterpret_cast<char*>(l)-offsetof(Light,inWorld)),data)) return w;
            l=next;
        }
    return w;
}
int __cdecl destroyWorld(World* w) {
    startupTrace("0x0058CBD0","RpWorldDestroy (empty world)");
    return worldDestroy(w);
}
}
namespace csf_rw {
bool worldsEmpty() { return worlds.empty(); }
bool worldDestroy(World* w) {
    auto p=worlds.find(w);if(p==worlds.end() || !w->root || w->root->type!=-1 ||
        w->root->numTriangles || w->root->numVertices || w->numClumps ||
        w->localLights.next!=&w->localLights || w->globalLights.next!=&w->globalLights) return false;
    // Native World has no camera ring: don't leave borrowed camera world links.
    // The object slice supplies this predicate for its owning camera map.
    if(worldHasCamera(p->second.native)) return false;
    destructPlugins(Registry::Sector,w->root);destructPlugins(Registry::World,w);
    p->second.native->destroy();worlds.erase(p);
    rw::Engine::memfuncs.rwfree(w->root);rw::Engine::memfuncs.rwfree(w);return true;
}
}
HOOK(0x0058CCB0,createWorld,"boundary-rw");
HOOK(0x0058CBD0,destroyWorld,"boundary-rw");
HOOK(0x0058CC80,sectorRender,"boundary-rw");
HOOK(0x0058CC90,sectorCallback,"boundary-rw");
HOOK(0x0058DD90,addCamera,"boundary-rw");
HOOK(0x0058DDC0,removeCamera,"boundary-rw");
HOOK(0x0058DDF0,cameraWorld,"boundary-rw");
HOOK(0x0058DF50,addLight,"boundary-rw");
HOOK(0x0058DFA0,removeLight,"boundary-rw");
HOOK(0x0058DFD0,lightWorld,"boundary-rw");
HOOK(0x0058CF80,worldLights,"boundary-rw");
HOOK(0x0058CEF0,worldClumps,"boundary-rw");
HOOK(0x0058CF30,worldMaterials,"boundary-rw");
HOOK(0x0058F0E0,clumpGetWorld,"boundary-rw");
HOOK(0x0058DE40,atomicGetWorld,"boundary-rw");
HOOK(0x0058DF40,clumpGetWorldB,"boundary-rw");
#endif
