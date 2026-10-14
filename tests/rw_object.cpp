#include "globals.h"
#include "boundary/rw/object.h"
#include <cassert>
#include <cmath>
#include <cstdio>
using namespace csf_rw;
using Ctor=rw::Constructor; using Dtor=rw::Destructor; using Copy=rw::CopyConstructor;
CSF_FUNC(int,__cdecl,regAtomic,0x0058F210,(int,unsigned,Ctor,Dtor,Copy));
CSF_FUNC(int,__cdecl,regGeometry,0x00590BA0,(int,unsigned,Ctor,Dtor,Copy));
CSF_FUNC(int,__cdecl,regMaterial,0x00591590,(int,unsigned,Ctor,Dtor,Copy));
CSF_FUNC(int,__cdecl,regLight,0x0059CEC0,(int,unsigned,Ctor,Dtor,Copy));
CSF_FUNC(int,__cdecl,init,0x00627360,(const MemoryFunctions*,char,int));
CSF_FUNC(int,__cdecl,open,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,close,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
CSF_FUNC(Frame*,__stdcall,newFrame,0x006235D0,());
CSF_FUNC(int,__cdecl,deleteFrame,0x00623B10,(Frame*));
CSF_FUNC(Frame*,__cdecl,addFrame,0x00623B90,(Frame*,Frame*));
CSF_FUNC(int,__stdcall,syncFrames,0x006327D0,());
CSF_FUNC(Frame*,__cdecl,updateFrame,0x006236A0,(Frame*));
CSF_FUNC(ObjectWithFrame*,__cdecl,setFrame,0x0063CE30,(ObjectWithFrame*,Frame*));
CSF_FUNC(Atomic*,__stdcall,newAtomic,0x0058F5C0,());
CSF_FUNC(Atomic*,__cdecl,sphere,0x0058EB40,(Atomic*));
CSF_FUNC(Atomic*,__cdecl,setGeometry,0x0058EFE0,(Atomic*,Geometry*,unsigned char));
CSF_FUNC(Atomic*,__cdecl,setAtomicFrame,0x0058F300,(Atomic*,Frame*));
CSF_FUNC(int,__cdecl,deleteAtomic,0x0058F060,(Atomic*));
CSF_FUNC(Clump*,__cdecl,render,0x0058EEB0,(Clump*));
using AC=Atomic* (__cdecl *)(Atomic*,void*); using LC=Light* (__cdecl *)(Light*,void*); using MC=Material* (__cdecl *)(Material*,void*);
CSF_FUNC(Clump*,__cdecl,visitAtomics,0x0058EF00,(Clump*,AC,void*));
CSF_FUNC(Clump*,__cdecl,visitLights,0x0058EF90,(Clump*,LC,void*));
CSF_FUNC(Clump*,__cdecl,removeLight,0x0058F1D0,(Clump*,Light*));
CSF_FUNC(int,__cdecl,atomicCount,0x0058F560,(Clump*));
CSF_FUNC(int,__cdecl,lightCount,0x0058F590,(Clump*));
CSF_FUNC(int,__cdecl,deleteClump,0x0058F740,(Clump*));
CSF_FUNC(Clump*,__cdecl,cloneClump,0x0058FC60,(Clump*));
CSF_FUNC(Geometry*,__cdecl,visitMaterials,0x00590670,(Geometry*,MC,void*));
CSF_FUNC(Geometry*,__cdecl,holdGeometry,0x00590B90,(Geometry*));
CSF_FUNC(int,__cdecl,deleteGeometry,0x00590FC0,(Geometry*));
CSF_FUNC(Material*,__cdecl,setTexture,0x00591560,(Material*,Texture*));
CSF_FUNC(Texture*,__cdecl,newTexture,0x00631780,(Raster*));
CSF_FUNC(int,__cdecl,deleteTexture,0x006317E0,(Texture*));
CSF_FUNC(Raster*,__cdecl,newRaster,0x00627770,(int,int,int,int));
CSF_FUNC(int,__cdecl,deleteRaster,0x00627570,(Raster*));
CSF_FUNC(Light*,__cdecl,newLight,0x0059D040,(unsigned char));
CSF_FUNC(int,__cdecl,deleteLight,0x0059D000,(Light*));
CSF_FUNC(Light*,__cdecl,radius,0x0059CBD0,(Light*,float));
CSF_FUNC(Light*,__cdecl,colour,0x0059CBF0,(Light*,const rw::RGBAf*));
CSF_FUNC(double,__cdecl,angle,0x0059CC50,(Light*));
CSF_FUNC(Light*,__cdecl,cone,0x0059CE80,(Light*,float));
CSF_FUNC(Camera*,__stdcall,newCamera,0x006290B0,());
CSF_FUNC(int,__cdecl,deleteCamera,0x00629070,(Camera*));
CSF_FUNC(Camera*,__cdecl,nearPlane,0x00628E90,(Camera*,float));
CSF_FUNC(Camera*,__cdecl,farPlane,0x00628EC0,(Camera*,float));
CSF_FUNC(int,__cdecl,testSphere,0x00628EF0,(Camera*,const rw::Sphere*));
CSF_FUNC(Camera*,__cdecl,projection,0x00628F90,(Camera*,int));
CSF_FUNC(Camera*,__cdecl,window,0x00629000,(Camera*,const rw::V2d*));
CSF_FUNC(Camera*,__cdecl,begin,0x00628E50,(Camera*));
CSF_FUNC(Camera*,__cdecl,clear,0x00628F60,(Camera*,rw::RGBA*,unsigned));
static bool approximately(float a,float b) { return std::fabs(a-b)<1e-4f; }
static int renders;
static int constructors, destructors, copies;
static decltype(rw::Engine::memfuncs.rwmalloc) savedAllocator;
static int failAfter;
static void* failAllocator(size_t n,rw::uint32 hint) { return failAfter--==0?nullptr:savedAllocator(n,hint); }
static rw::Camera* clearSeen;
static void recordClear(rw::Camera* n,rw::RGBA* color,rw::uint32 mode) {
    assert(color->red==10 && color->alpha==40 && mode==3); clearSeen=n;
}
static void* pluginCtor(void* p,int offset,int size) {
    assert(size==4); *reinterpret_cast<int*>(static_cast<char*>(p)+offset)=777; ++constructors; return p;
}
static void* pluginDtor(void* p,int offset,int size) {
    assert(size==4 && *reinterpret_cast<int*>(static_cast<char*>(p)+offset)==777); ++destructors; return p;
}
static void* pluginCopy(void* p,void* source,int offset,int size) {
    assert(size==4 && *reinterpret_cast<int*>(static_cast<char*>(source)+offset)==777);
    *reinterpret_cast<int*>(static_cast<char*>(p)+offset)=777; ++copies; return p;
}
static Atomic* __cdecl renderFail(Atomic*) { ++renders; return nullptr; }
static Atomic* __cdecl renderOK(Atomic* a) { ++renders; return a; }
static Atomic* __cdecl atomicStop(Atomic*,void* p) { ++*static_cast<int*>(p); return nullptr; }
static Light* __cdecl lightStop(Light*,void* p) { ++*static_cast<int*>(p); return nullptr; }
static Material* __cdecl materialStop(Material*,void* p) { ++*static_cast<int*>(p); return nullptr; }
static Camera* __cdecl beginFailure(Camera*) { return nullptr; }
static Atomic* __cdecl destroyVisit(Atomic* a,void* p) { ++*static_cast<int*>(p); assert(deleteAtomic(a)); return a; }
int main() {
    assert(init(nullptr,0,0));
    assert(regAtomic(4,42,pluginCtor,pluginDtor,pluginCopy)==sizeof(Atomic));
    assert(regGeometry(4,42,pluginCtor,pluginDtor,pluginCopy)==sizeof(Geometry));
    assert(regMaterial(4,42,pluginCtor,pluginDtor,pluginCopy)==sizeof(Material));
    assert(regLight(4,42,pluginCtor,pluginDtor,pluginCopy)==sizeof(Light));
    assert(open(nullptr)); assert(start());
    savedAllocator=rw::Engine::memfuncs.rwmalloc;
    for(int failure=0;failure<2;++failure) {
        rw::Engine::memfuncs.rwmalloc=failAllocator;
        failAfter=failure; assert(!newAtomic());
        failAfter=failure; assert(!newLight(1));
        failAfter=failure; assert(!materialCreate());
        failAfter=failure; assert(!newCamera());
        rw::Engine::memfuncs.rwmalloc=savedAllocator;
    }
    auto* material=materialCreate(); auto* other=materialCreate(); assert(material && other);
    auto* texture=newTexture(nullptr); auto* second=newTexture(nullptr); assert(texture && second);
    assert(setTexture(material,texture)==material && texture->refCount==2);
    auto* nativeMaterial=materialNative(material); ++material->refCount;
    assert(setTexture(material,texture)==material && nativeMaterial->refCount==2);
    assert(materialDestroy(material) && material->refCount==1);
    assert(setTexture(material,texture)==material && texture->refCount==2);
    assert(deleteTexture(texture) && texture->refCount==1);
    assert(setTexture(material,texture)==material && texture->refCount==1);
    assert(setTexture(material,second)==material && second->refCount==2);
    assert(deleteTexture(second) && second->refCount==1);
    auto* g=geometryCreate(3,1,rw::Geometry::POSITIONS|rw::Geometry::NORMALS|rw::Geometry::PRELIT); assert(g);
    assert(g->numVertices==3 && g->numTriangles==1 && g->numMorphTargets==1 && g->refCount==1);
    assert(g->morphTargets[0].parent==g && g->colors && g->morphTargets[0].vertices);
    assert(geometryAppendMaterial(g,material) && geometryAppendMaterial(g,other));
    assert(g->matList.numMaterials==2 && g->matList.materials[0]==material && material->refCount==2);
    material->color={1,2,3,4}; material->surfaceProps={0.2f,0.3f,0.4f};
    assert(materialNative(material)->color.red==1);
    assert(approximately(materialNative(material)->surfaceProps.ambient,0.2f));
    assert(materialDestroy(material) && material->refCount==1); assert(materialDestroy(other));
    ++material->refCount; // game short +18 write, imported before native release
    assert(materialNative(material)->refCount==2 && materialDestroy(material) && material->refCount==1);
    auto* replacement=materialCreate(); auto* nativeReplacement=materialNative(replacement); assert(replacement);
    g->matList.materials[1]=replacement;
    assert(geometryNative(g)->matList.materials[1]==nativeReplacement && replacement->refCount==2);
    assert(materialDestroy(replacement) && replacement->refCount==1);
    int visited=0; assert(visitMaterials(g,materialStop,&visited)==g && visited==1);
    assert(holdGeometry(g)==g && g->refCount==2); assert(deleteGeometry(g) && g->refCount==1);
    g->morphTargets[0].boundingSphere={rw::makeV3d(1,2,3),4};
    auto* directAtomic=newAtomic(); directAtomic->geometry=g;
    assert(atomicNative(directAtomic)->geometry==geometryNative(g) && g->refCount==2);
    directAtomic->geometry=nullptr; assert(!atomicNative(directAtomic)->geometry && g->refCount==1);
    assert(deleteAtomic(directAtomic));
    auto* a=newAtomic(); auto* b=newAtomic(); assert(a && b);
    assert(setGeometry(a,g,0)==a && g->refCount==2 && a->geometry==g);
    assert(setGeometry(a,g,0)==a && g->refCount==2);
    assert(setGeometry(b,g,1)==b && g->refCount==3 && b->boundingSphere.radius==0);
    assert(sphere(b)==b && b->boundingSphere.radius==4 && !(b->interpolator.flags&2));
    assert(geometryAddMorphTargets(g,1) && g->numMorphTargets==2);
    g->morphTargets[1].boundingSphere={rw::makeV3d(5,6,7),8};
    b->interpolator.end=1; b->interpolator.position=0.5f;
    assert(sphere(b)==b && approximately(b->boundingSphere.center.x,3) && approximately(b->boundingSphere.radius,6));
    b->interpolator.start=b->interpolator.end=7; // original invalid indices fall back to target 0
    assert(sphere(b)==b && b->boundingSphere.radius==4);
    b->interpolator.start=b->interpolator.end=0;
    auto* c=clumpCreate(); auto* root=newFrame(); auto* child=newFrame(); assert(c && root && child);
    addFrame(root,child); child->matrix.pos=rw::makeV3d(10,20,30); child->matrix.flags=0;
    clumpSetFrame(c,root); assert(setAtomicFrame(a,child)==a); assert(setAtomicFrame(b,root)==b);
    assert(a->object.object.parent==child && atomicNative(a)->getFrame()==frameNative(child));
    assert(clumpAddAtomic(c,a) && clumpAddAtomic(c,b)); assert(atomicCount(c)==2);
    visited=0; assert(visitAtomics(c,atomicStop,&visited)==c && visited==1);
    a->renderCB=renderFail; b->renderCB=renderOK; renders=0; assert(!render(c) && renders==2);
    a->object.object.flags&=~rw::Atomic::RENDER; renders=0; assert(render(c)==c && renders==1);
    a->object.object.flags|=rw::Atomic::RENDER;
    auto* light=newLight(0x81); auto* light2=newLight(0x80); assert(light && light2);
    assert(light->minusCosAngle==0 && angle(light)>1.5);
    rw::RGBAf rgba={0.2f,0.4f,0.6f,0.8f}; assert(colour(light,&rgba)==light && light->color.alpha==rgba.alpha);
    assert(radius(light,25)==light && light->radius==25); assert(cone(light,0.6f)==light && approximately(angle(light),0.6f));
    assert(!cone(light,-0.1f) && !cone(light,2.0f));
    light->minusCosAngle=-2; assert(angle(light)==0 && light->minusCosAngle==-2);
    assert(cone(light,0.6f));
    assert(setFrame(&light->object,child)); assert(clumpAddLight(c,light) && clumpAddLight(c,light2));
    assert(lightCount(c)==2); visited=0; assert(visitLights(c,lightStop,&visited)==c && visited==1);
    assert(removeLight(c,light2)==c && lightCount(c)==1); assert(deleteLight(light2));
    auto* camera=newCamera(); assert(camera); auto* cf=newFrame(); assert(cf);
    assert(setFrame(&camera->object,cf));
    assert(nearPlane(camera,1)==camera && farPlane(camera,10)==camera);
    rw::V2d view={1,0.5f}; assert(window(camera,&view)==camera && camera->reciprocalViewWindow.y==2);
    assert(!projection(camera,0) && projection(camera,1)==camera);
    assert(syncFrames());
    assert(rw::engine->frameDirtyList.link.next==&rw::engine->frameDirtyList.link);
    auto* worldSphere=atomicNative(a)->getWorldBoundingSphere(); objectExport();
    assert(approximately(worldSphere->center.x,11) && approximately(worldSphere->center.y,22));
    assert(approximately(a->worldBoundingSphere.center.z,33) && approximately(a->worldBoundingSphere.radius,4));
    rw::Sphere inside={rw::makeV3d(0,0,5),0.1f}, outside={rw::makeV3d(0,0,-5),0.1f}, boundary={rw::makeV3d(0,0,1),0.1f};
    assert(testSphere(camera,&inside)==2 && testSphere(camera,&outside)==0 && testSphere(camera,&boundary)==1);
    assert(approximately(camera->frustumCorners[0].z,1) && approximately(camera->frustumCorners[4].z,10));
    assert(projection(camera,2)==camera); assert(syncFrames()); assert(testSphere(camera,&inside)==2);
    assert(cameraNative(camera)->frameBuffer==nullptr);
    assert(begin(camera)==camera && rw::engine->currentCamera==cameraNative(camera));
    assert(camera->endUpdate(camera)==camera);
    auto saved=camera->beginUpdate; camera->beginUpdate=beginFailure; assert(!begin(camera)); camera->beginUpdate=saved;
    rw::RGBA clearColor={10,20,30,40}; assert(clear(camera,&clearColor,3)==camera);
    auto* buffer=newRaster(8,8,32,rw::Raster::NORMAL|rw::Raster::C8888); assert(buffer);
    camera->frameBuffer=buffer; camera->zBuffer=buffer;
    auto clearCallback=rw::engine->device.clearCamera; rw::engine->device.clearCamera=recordClear;
    assert(clear(camera,&clearColor,3)==camera && clearSeen==cameraNative(camera));
    assert(clearSeen->frameBuffer==rasterNative(buffer) && clearSeen->zBuffer==rasterNative(buffer));
    rw::engine->device.clearCamera=clearCallback;
    auto platform=rw::platform; rw::platform=rw::PLATFORM_D3D9;
    assert(!begin(camera) && !clear(camera,&clearColor,3) && !camera->endUpdate(camera));
    rw::platform=platform;
    assert(!stop());
    a->boundingSphere={rw::makeV3d(4,5,6),12}; a->interpolator.flags&=~2u;
    c->callback=&visited;
    auto* clone=cloneClump(c); assert(clone && clone!=c && clone->object.parent!=root);
    assert(atomicCount(clone)==2 && lightCount(clone)==0 && g->refCount==5);
    auto* ca=reinterpret_cast<Atomic*>(reinterpret_cast<char*>(clone->atomics.next)-offsetof(Atomic,inClump));
    assert(ca->geometry==g && ca->renderCB==a->renderCB && ca->object.object.parent!=child);
    assert(ca->boundingSphere.radius==12 && atomicNative(ca)->boundingSphere.radius==12 && clone->callback==c->callback);
    assert(deleteClump(clone) && g->refCount==3);
    assert(copies==2); // original clump clone copies atomics, not lights/cameras
    assert(deleteFrame(cf)); assert(camera->object.object.parent==nullptr && cameraNative(camera)->getFrame()==nullptr);
    assert(deleteCamera(camera));
    assert(deleteRaster(buffer)); // camera did not own either borrowed buffer
    auto* direct=newCamera(); auto* directFrame=newFrame(); assert(direct && directFrame);
    direct->object.object.parent=directFrame; // mutable +4 import must repair both frame lists
    assert(cameraNative(direct)->getFrame()==frameNative(directFrame));
    assert(directFrame->objects.next==&direct->object.inFrame);
    direct->object.object.parent=nullptr; assert(cameraNative(direct)->getFrame()==nullptr);
    assert(directFrame->objects.next==&directFrame->objects);
    assert(deleteCamera(direct) && deleteFrame(directFrame));
    visited=0; assert(visitAtomics(c,destroyVisit,&visited)==c && visited==2 && atomicCount(c)==0 && g->refCount==1);
    assert(deleteClump(c)); assert(deleteGeometry(g));
    assert(objectsEmpty()); assert(constructors==destructors); assert(stop()); assert(close()); assert(term());
    assert(init(nullptr,0,0) && open(nullptr) && start());
    auto* empty=clumpCreate(); assert(!cloneClump(empty)); // no valid hierarchy to clone
    clumpSetFrame(empty,newFrame()); auto* copy=cloneClump(empty); assert(copy);
    assert(deleteClump(copy)); assert(deleteClump(empty));
    auto* lone=newAtomic(); assert(deleteAtomic(lone));
    assert(stop() && close() && term());
    std::puts("rw object: all 32 implemented slots passed");
}
