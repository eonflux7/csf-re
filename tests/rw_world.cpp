#include "globals.h"
#include "boundary/rw/world.h"
#include <cassert>
#include <cstring>
#include <cmath>
#include <vector>
#include <cstdio>
using namespace csf_rw;
CSF_FUNC(int,__cdecl,init,0x00627360,(const MemoryFunctions*,char,int));
CSF_FUNC(int,__stdcall,attachWorld,0x0058D0B0,());
CSF_FUNC(int,__cdecl,open,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,close,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
CSF_FUNC(World*,__cdecl,newWorld,0x0058CCB0,(const rw::BBox*));
CSF_FUNC(int,__cdecl,deleteWorld,0x0058CBD0,(World*));
CSF_FUNC(World*,__cdecl,addClump,0x0058E280,(World*,Clump*));
CSF_FUNC(World*,__cdecl,removeClump,0x0058E310,(World*,Clump*));
CSF_FUNC(World*,__cdecl,removeAtomic,0x0058DE00,(World*,Atomic*));
CSF_FUNC(World*,__cdecl,getClumpWorld,0x0058DF40,(Clump*));
CSF_FUNC(World*,__cdecl,getAtomicWorld,0x0058DE40,(Atomic*));
CSF_FUNC(Clump*,__cdecl,getLightClump,0x0058F0E0,(Light*));
CSF_FUNC(World*,__cdecl,getLightWorld,0x0058DFD0,(Light*));
using CC=Clump* (__cdecl *)(Clump*,void*);
using LC=Light* (__cdecl *)(Light*,void*);
CSF_FUNC(World*,__cdecl,lights,0x0058CF80,(World*,LC,void*));
CSF_FUNC(World*,__cdecl,clumps,0x0058CEF0,(World*,CC,void*));
using SC=Sector* (__cdecl *)(Sector*,void*);
CSF_FUNC(World*,__cdecl,sectors,0x0058CFF0,(World*,SC,void*));
CSF_FUNC(World*,__cdecl,getSectorWorld,0x0058D1A0,(Sector*));
CSF_FUNC(World*,__cdecl,setRender,0x0058CC60,(World*,SectorRender));
CSF_FUNC(SectorRender,__cdecl,getRender,0x0058CC80,(World*));
CSF_FUNC(Frame*,__stdcall,newFrame,0x006235D0,());
CSF_FUNC(Frame*,__cdecl,updateFrame,0x006236A0,(Frame*));
CSF_FUNC(Atomic*,__stdcall,newAtomic,0x0058F5C0,());
CSF_FUNC(Atomic*,__cdecl,setAtomicFrame,0x0058F300,(Atomic*,Frame*));
CSF_FUNC(Atomic*,__cdecl,setGeometry,0x0058EFE0,(Atomic*,Geometry*,unsigned char));
CSF_FUNC(rw::Sphere*,__cdecl,sphere,0x0058EC60,(Atomic*));
CSF_FUNC(Light*,__cdecl,newLight,0x0059D040,(unsigned char));
CSF_FUNC(int,__cdecl,deleteClump,0x0058F740,(Clump*));
CSF_FUNC(int,__cdecl,deleteGeometry,0x00590FC0,(Geometry*));
static Sector* __cdecl customRender(Sector* s) { return s; }
static Sector* __cdecl sectorVisit(Sector* s,void* p) { assert(s->type==-1); ++*static_cast<int*>(p);return nullptr; }
static Clump* __cdecl recordClump(Clump* c,void* p) { static_cast<std::vector<Clump*>*>(p)->push_back(c);return c; }
static Clump* __cdecl stopClump(Clump*,void* p) { ++*static_cast<int*>(p);return nullptr; }
static Light* __cdecl recordLight(Light* l,void* p) { static_cast<std::vector<Light*>*>(p)->push_back(l);return l; }
static Clump* __cdecl detachClump(Clump* c,void* p) {
    assert(removeClump(static_cast<World*>(p),c));assert(deleteClump(c));return c;
}
int main() {
    assert(init(nullptr,0,0));assert(attachWorld());assert(open(nullptr));assert(start());
    rw::BBox box={rw::makeV3d(100,100,100),rw::makeV3d(-100,-100,-100)};
    auto* w=newWorld(&box);auto* other=newWorld(&box);assert(w && other);
    auto* c=clumpCreate();auto* second=clumpCreate();auto* f=newFrame();auto* a=newAtomic();auto* l=newLight(1);
    assert(c && second && f && a && l);clumpSetFrame(c,f);assert(setAtomicFrame(a,f));
    auto* g=geometryCreate(0,0,rw::Geometry::POSITIONS);assert(g);
    g->morphTargets[0].boundingSphere={rw::makeV3d(1,2,3),2};assert(setGeometry(a,g,0));assert(deleteGeometry(g));
    assert(clumpAddAtomic(c,a));assert(clumpAddLight(c,l));assert(getLightClump(l)==c);
    auto* l2=newLight(1);auto* point=newLight(128);assert(l2 && point);
    assert(clumpAddLight(c,l2) && clumpAddLight(c,point));
    assert(addClump(w,c)==w);assert(w->numClumps==1 && getClumpWorld(c)==w);
    assert(getAtomicWorld(a)==w && getLightWorld(l)==w && atomicNative(a)->world==clumpNative(c)->world);
    std::vector<Light*> lightSeen;assert(lights(w,recordLight,&lightSeen)==w);
    assert((lightSeen==std::vector<Light*>{l2,l,point}));
    std::vector<Light*> nativeLightSeen;
    FORLIST(link,clumpNative(c)->world->globalLights) nativeLightSeen.push_back(lightFacade(rw::Light::fromWorld(link)));
    FORLIST(link,clumpNative(c)->world->localLights) nativeLightSeen.push_back(lightFacade(rw::Light::fromWorld(link)));
    assert(nativeLightSeen==lightSeen);
    assert(!addClump(other,c));assert(!removeClump(other,c));assert(!deleteWorld(w));
    assert(addClump(w,second)==w);assert(w->numClumps==2);
    std::vector<Clump*> seen;assert(clumps(w,recordClump,&seen)==w);assert((seen==std::vector<Clump*>{second,c}));
    int count=0;assert(clumps(w,stopClump,&count)==w && count==1);
    w->currentClump=&second->inWorld;assert(removeClump(w,second)==w);assert(w->currentClump==&c->inWorld);
    assert(deleteClump(second));assert(w->numClumps==1);
    f->matrix.right=rw::makeV3d(2,0,0);f->matrix.up=rw::makeV3d(0,3,0);f->matrix.at=rw::makeV3d(0,0,4);
    f->matrix.pos=rw::makeV3d(10,20,30);f->matrix.flags=0;assert(updateFrame(f));
    auto* s=sphere(a);assert(s==&a->worldBoundingSphere && std::fabs(s->radius-8)<1e-5f);
    assert(s->center.x==12 && s->center.y==26 && s->center.z==42);
    assert(sphere(a)->radius==8); // cached native sphere must not scale twice
    assert(removeAtomic(w,a)==w && !getAtomicWorld(a) && !atomicNative(a)->world);
    assert(!removeAtomic(other,a));assert(removeClump(w,c)==w); // already-detached atomic
    assert(!getClumpWorld(c) && !getLightWorld(l));assert(deleteClump(c));
    count=0;assert(sectors(w,sectorVisit,&count)==w && count==1);assert(getSectorWorld(w->root)==w);
    Sector unknown{};assert(!getSectorWorld(&unknown));
    auto original=getRender(w);assert(original && setRender(w,customRender)==w && getRender(w)==customRender);
    assert(setRender(w,nullptr)==w && getRender(w)==original);
    w->root->numVertices=1;assert(original(w->root)==w->root);w->root->numVertices=0;
    c=clumpCreate();second=clumpCreate();assert(addClump(w,c) && addClump(w,second));
    assert(clumps(w,detachClump,w)==w && w->numClumps==0 && w->clumps.next==&w->clumps);
    assert(deleteWorld(w) && deleteWorld(other));assert(objectsEmpty() && framesEmpty() && worldsEmpty());
    assert(stop() && close() && term());std::puts("rw world membership and sphere tests passed");
}
