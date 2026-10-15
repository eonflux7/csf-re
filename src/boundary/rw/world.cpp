// BSP/empty-world facade, membership and CPU queries over native librw objects.
// Sector geometry is native-owned, but librw's World has no BSP or sector
// renderer. World/camera visibility-cache production and Rx remain separate.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "world.h"
#include "startup.h"
#include "plugins.h"
#include <unordered_map>
#include <cstring>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <cmath>
#include <unordered_set>
#include <cstdio>
CSF_GLOBAL(int, cameraWorldOffset, 0x008B1348);
CSF_GLOBAL(int, lightWorldOffset, 0x008B1354);
CSF_GLOBAL(int, atomicWorldOffset, 0x008B134C);
CSF_GLOBAL(int, clumpWorldOffsetB, 0x008B1350);
CSF_GLOBAL(int, lightClumpOffsetW, 0x008B13B4);
CSF_GLOBAL(int, pvsWorldOffset, 0x008BE7A4);
CSF_GLOBAL(csf_rw::World*, pvsCurrentWorld, 0x008BE7A0);
CSF_GLOBAL(int, ltMapSectorOffset, 0x008BE56C);
CSF_GLOBAL(int, ltMapAtomicOffset, 0x008BE564);
CSF_GLOBAL(uint32_t, ltMapStyle, 0x008BE588);
CSF_GLOBAL(uint32_t, ltMapRasterFormat, 0x008BE5B0);
namespace {
using namespace csf_rw;
struct Leaf {
    Sector* facade=nullptr;
    rw::Geometry* geometry=nullptr;
    std::vector<int8_t> normals;
    std::vector<uint8_t> meshes;
    std::vector<rw::Triangle> triangles;
    std::vector<rw::Material*> materials;
    bool constructed=false;
    ~Leaf() {
        if(constructed) destructPlugins(Registry::Sector,facade);
        if(geometry) geometry->destroy();
        // World material-list references still hold these objects alive.
        for(auto* m:materials) streamObjectExport(Registry::Material,m);
        if(facade) rw::Engine::memfuncs.rwfree(facade);
    }
};
struct Record {
    rw::World* native;
    std::vector<std::unique_ptr<Leaf>> leaves;
    std::vector<std::unique_ptr<Plane>> planes;
    std::vector<Material*> materials;
    bool streamed=false,constructed=true;
    Sector* destroyingSector=nullptr;
};
void releaseLeaves(Record& record) {
    // Streamed sectors are part of the original world's allocation. Their
    // headers/CPU arrays outlive sector plugin destruction and remain readable
    // by the world destructor; reclaim native/facade storage only afterwards.
    for(auto it=record.leaves.rbegin();it!=record.leaves.rend();++it) {
        auto& leaf=**it;
        if(leaf.constructed) {
            destructPlugins(Registry::Sector,leaf.facade);leaf.constructed=false;
        }
    }
}
void disposeLeaves(Record& record) {
    while(!record.leaves.empty()) {
        auto leaf=std::move(record.leaves.back());record.leaves.pop_back();
        record.destroyingSector=leaf->facade;leaf.reset();record.destroyingSector=nullptr;
    }
}
std::unordered_map<World*,Record> worlds;
struct WorldTie { Link sectorLink;Atomic* atomic;Link atomicLink;Sector* sector; };
std::unordered_map<Atomic*,std::vector<std::unique_ptr<WorldTie>>> atomicTies;
bool tieAtomic(World*,Atomic*);
World* __cdecl removeClump(World*,Clump*);
void ring(Link& l) { l.next=l.prev=&l; }
void remove(Link& l) { l.prev->next=l.next;l.next->prev=l.prev;ring(l); }
void prepend(Link& head,Link& l) { l.prev=&head;l.next=head.next;head.next->prev=&l;head.next=&l; }
void untieAtomic(Atomic* a) {
    auto p=atomicTies.find(a);if(p==atomicTies.end()) return;
    for(auto& t:p->second) { remove(t->sectorLink);remove(t->atomicLink); }
    atomicTies.erase(p);
}
void reorderLight(rw::World* w,rw::Light* l) {
    // librw appends world lights; the original prepends. Enumeration order can
    // decide which local/directional lights fit a backend's finite light set.
    l->inWorld.remove();
    (l->getType()<rw::Light::POINT?w->globalLights:w->localLights).add(&l->inWorld);
}
World* find(rw::World* n) {
    for(auto& p:worlds) if(p.second.native==n) return p.first;
    return nullptr;
}
void publish(void* object,int offset,int delta,World* w) {
    if(offset>=0) std::memcpy(static_cast<char*>(object)+offset+delta,&w,sizeof(w));
}
Sector* __cdecl emptyRender(Sector* s) {
    // Rendering an actually empty leaf requires no draw. No claim of rendering
    // a loaded BSP: reject nonempty leaves rather than swallowing their draws.
    return s && s->type==-1 && !s->numTriangles ? s : nullptr;
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
    ring(s->atomics);ring(s->lights);
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
    p->second.native->addLight(n);reorderLight(p->second.native,n);
    prepend(l->object.object.subType>=128?w->localLights:w->globalLights,l->inWorld);
    publish(l,lightWorldOffset,0,w);return w;
}
World* __cdecl removeLight(World* w,Light* l) {
    startupTrace("0x0058DFA0","RpWorldRemoveLight");
    auto p=worlds.find(w);auto* n=lightNative(l);
    if(p==worlds.end() || !n || n->world!=p->second.native) return nullptr;
    p->second.native->removeLight(n);remove(l->inWorld);publish(l,lightWorldOffset,0,nullptr);return w;
}
World* __cdecl lightWorld(Light* l) { auto* n=lightNative(l);return n?find(n->world):nullptr; }
// World getters below read the word at the registered world-plugin offset.
// 58F0E0 reads the LIGHT clump plugin (8B13B4), not the world plugin.
// Its database name RpClumpGetWorld is misleading; callers pass lights.
Clump* __cdecl clumpGetWorld(Light* l) {
    Clump* c;std::memcpy(&c,reinterpret_cast<char*>(l)+lightClumpOffsetW,sizeof(c));return c;
}
World* __cdecl atomicGetWorld(Atomic* a) {
    World* w;std::memcpy(&w,reinterpret_cast<char*>(a)+atomicWorldOffset,sizeof(w));return w;
}
World* __cdecl clumpGetWorldB(Clump* c) {
    World* w;std::memcpy(&w,reinterpret_cast<char*>(c)+clumpWorldOffsetB,sizeof(w));return w;
}
World* __cdecl removeAtomic(World* w,Atomic* a) {
    auto p=worlds.find(w);auto* n=atomicNative(a);
    if(p==worlds.end() || !n || n->world!=p->second.native) return nullptr;
    p->second.native->removeAtomic(n);
    untieAtomic(a);
    publish(a,atomicWorldOffset,0,nullptr); return w;
}
// Original 58E280 inserts at the head; publish only after preflighting every
// member so librw's assertion-only membership API cannot partially add a clump.
World* __cdecl addClump(World* w,Clump* c) {
    auto p=worlds.find(w);auto* n=clumpNative(c);
    if(p==worlds.end() || !n || n->world) return nullptr;
    for(Link* l=c->atomics.next;l!=&c->atomics;l=l->next) {
        auto* a=reinterpret_cast<Atomic*>(reinterpret_cast<char*>(l)-offsetof(Atomic,inClump));
        auto* b=atomicNative(a); if(!b || b->clump!=n || b->world) return nullptr;
    }
    FORLIST(l,n->lights) {
        auto* b=rw::Light::fromClump(l);auto* a=lightFacade(b);
        if(!a || !lightNative(a) || b->world) return nullptr;
    }
    FORLIST(l,n->cameras) {
        auto* b=rw::Camera::fromClump(l);auto* a=cameraFacade(b);
        if(!a || !cameraNative(a) || b->world) return nullptr;
    }
    p->second.native->addClump(n);
    prepend(w->clumps,c->inWorld);++w->numClumps;
    publish(c,clumpWorldOffsetB,0,w);
    for(Link* l=c->atomics.next;l!=&c->atomics;l=l->next) {
        auto* a=reinterpret_cast<Atomic*>(reinterpret_cast<char*>(l)-offsetof(Atomic,inClump));
        publish(a,atomicWorldOffset,0,w);
    }
    FORLIST(l,n->lights) {
        auto* a=lightFacade(rw::Light::fromClump(l));
        reorderLight(p->second.native,rw::Light::fromClump(l));
        prepend(a->object.object.subType>=128?w->localLights:w->globalLights,a->inWorld);
        publish(a,lightWorldOffset,0,w);
    }
    FORLIST(l,n->cameras) publish(cameraFacade(rw::Camera::fromClump(l)),cameraWorldOffset,12,w);
    frameExport();objectExport();
    try {
        for(Link* l=c->atomics.next;l!=&c->atomics;l=l->next) {
            auto* a=reinterpret_cast<Atomic*>(reinterpret_cast<char*>(l)-offsetof(Atomic,inClump));
            if(!tieAtomic(w,a)) { removeClump(w,c);return nullptr; }
        }
    } catch(...) { removeClump(w,c);return nullptr; }
    return w;
}
World* __cdecl removeClump(World* w,Clump* c) {
    auto p=worlds.find(w);auto* n=clumpNative(c);
    if(p==worlds.end() || !n || n->world!=p->second.native) return nullptr;
    // An atomic may already have been independently removed from the world.
    // librw removeClump asserts for that case; compose its removal primitives
    // rather than reattaching detached objects just to satisfy the assertion.
    for(Link* l=c->atomics.next;l!=&c->atomics;l=l->next) {
        auto* a=reinterpret_cast<Atomic*>(reinterpret_cast<char*>(l)-offsetof(Atomic,inClump));
        auto* b=atomicNative(a);if(b && b->world==p->second.native) removeAtomic(w,a);
    }
    FORLIST(l,n->lights) {
        auto* a=lightFacade(rw::Light::fromClump(l));
        if(a && lightNative(a)->world==p->second.native) removeLight(w,a);
    }
    FORLIST(l,n->cameras) {
        auto* a=cameraFacade(rw::Camera::fromClump(l));
        if(a && cameraNative(a)->world==p->second.native) removeCamera(w,a);
    }
    if(w->currentClump==&c->inWorld) w->currentClump=c->inWorld.next;
    remove(c->inWorld);--w->numClumps;
    n->inWorld.remove();n->world=nullptr;
    publish(c,clumpWorldOffsetB,0,nullptr);return w;
}
using SectorCB=Sector* (__cdecl *)(Sector*,void*);
// The BSP belongs to the boundary; this librw checkout has no owning BSP type.
World* __cdecl worldSectors(World* w,SectorCB cb,void* data) {
    if(!worlds.count(w) || !cb || !w->root) return nullptr;
    // Push right, visit left first; callbacks returning null stop the walk.
    std::vector<Sector*> stack{w->root};
    while(!stack.empty()) {
        auto* s=stack.back();stack.pop_back();
        if(s->type<0) { if(!cb(s,data)) break; }
        else { auto* p=reinterpret_cast<Plane*>(s);stack.push_back(p->right);stack.push_back(p->left); }
    }
    return w;
}
World* __cdecl sectorWorld(Sector* s) {
    for(auto& p:worlds) {
        if(p.second.destroyingSector==s) return p.first;
        if(p.first->root==s) return p.first;
        for(auto& leaf:p.second.leaves) if(leaf->facade==s) return p.first;
    }
    return nullptr;
}
World* __cdecl setSectorRender(World* w,SectorRender cb) {
    if(!worlds.count(w)) return nullptr;
    w->sectorRender=cb?cb:emptyRender;return w;
}
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
#include "world_stream.inc"
#include "world_queries.inc"
World* __cdecl readWorldFacade(Stream* stream) {
    auto* w=readWorld(stream);if(!w) return nullptr;
    // librw builds one mesh per material-list slot, including unused slots.
    // World sectors expose only actual groups; don't publish empty meshes for
    // every other sector's materials. The borrowed native indices stay valid.
    for(auto& leaf:worlds.at(w).leaves) if(auto* header=leaf->facade->mesh) {
        auto* meshes=reinterpret_cast<Mesh*>(header+1);unsigned used=0;
        for(unsigned i=0;i<header->numMeshes;++i) if(meshes[i].numIndices) meshes[used++]=meshes[i];
        header->numMeshes=uint16_t(used);
    }
    return w;
}
}
namespace csf_rw {
bool worldsEmpty() { return worlds.empty() && atomicTies.empty(); }
bool worldRefreshAtomic(Atomic* a) {
    auto* n=atomicNative(a);auto* w=n?find(n->world):nullptr;
    if(!w) { untieAtomic(a);return true; }
    try { return tieAtomic(w,a); } catch(...) { return false; }
}
bool worldDestroy(World* w) {
    auto p=worlds.find(w);if(p==worlds.end() || !w->root ||
        (!p->second.streamed && (w->root->type!=-1 || w->root->numTriangles || w->root->numVertices)) || w->numClumps ||
        w->localLights.next!=&w->localLights || w->globalLights.next!=&w->globalLights) return false;
    // Native World has no camera ring: don't leave borrowed camera world links.
    // The object slice supplies this predicate for its owning camera map.
    if(worldHasCamera(p->second.native)) return false;
    if(p->second.streamed) {
        // Sector callbacks run with the world/materials still discoverable.
        releaseLeaves(p->second);
        if(p->second.constructed) destructPlugins(Registry::World,w);
        disposeLeaves(p->second);
        for(auto* m:p->second.materials) materialDestroy(m);
    } else {
        destructPlugins(Registry::Sector,w->root);destructPlugins(Registry::World,w);
        rw::Engine::memfuncs.rwfree(w->root);
    }
    p->second.native->destroy();worlds.erase(p);
    rw::Engine::memfuncs.rwfree(w);return true;
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
HOOK(0x0058E280,addClump,"boundary-rw");
HOOK(0x0058E310,removeClump,"boundary-rw");
HOOK(0x0058DE00,removeAtomic,"boundary-rw");
HOOK(0x0058CFF0,worldSectors,"boundary-rw");
HOOK(0x0058D1A0,sectorWorld,"boundary-rw");
HOOK(0x0058CC60,setSectorRender,"boundary-rw");
HOOK(0x0059FEA0,readWorldFacade,"boundary-rw");
HOOK(0x005863D0,pvsProgress,"boundary-rw");
HOOK(0x0056EFF0,getLtStyle,"boundary-rw");
HOOK(0x0056F2B0,setLtStyle,"boundary-rw");
HOOK(0x0056F380,queryLightMaps,"boundary-rw");
HOOK(0x0058DE50,sectorAtomics,"boundary-rw");
HOOK(0x0056B9B0,sectorIntersections,"boundary-rw");
HOOK(0x007DA4E0,lineSphere,"boundary-rw");
HOOK(0x00620640,lineTriangle,"boundary-rw");
HOOK(0x00620840,sphereTriangle,"boundary-rw");
HOOK(0x0056BAF0,worldCollision,"boundary-rw");
HOOK(0x0056BDF0,atomicCollision,"boundary-rw");
HOOK(0x0058DEF0,cameraSectors,"boundary-rw");
HOOK(0x0056BBD0,worldAtomicIntersections,"boundary-rw");
#endif
