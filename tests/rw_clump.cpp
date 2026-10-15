#include "globals.h"
#include "boundary/rw/world.h"
#include "boundary/rw/plugins.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>
#include <unordered_set>
using namespace csf_rw;
CSF_FUNC(int,__cdecl,init,0x00627360,(const MemoryFunctions*,char,int));
CSF_FUNC(int,__stdcall,attachWorld,0x0058D0B0,());
CSF_FUNC(int,__stdcall,attachUser,0x0058C040,());
CSF_FUNC(int,__cdecl,open,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,close,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
CSF_FUNC(Stream*,__cdecl,streamOpen,0x006298E0,(int,int,void*));
CSF_FUNC(int,__cdecl,streamClose,0x00629810,(Stream*,Memory*));
CSF_FUNC(uint32_t,__cdecl,read,0x006293E0,(Stream*,void*,uint32_t));
CSF_FUNC(Clump*,__cdecl,readClump,0x0058FFC0,(Stream*));
CSF_FUNC(uint32_t,__cdecl,clumpSize,0x0058FD10,(Clump*));
CSF_FUNC(int,__cdecl,deleteClump,0x0058F740,(Clump*));
CSF_FUNC(Texture*,__cdecl,newTexture,0x00631780,(Raster*));
CSF_FUNC(int,__cdecl,deleteTexture,0x006317E0,(Texture*));
using TextureRead=Texture* (__cdecl *)(const char*,const char*);
CSF_FUNC(int,__cdecl,setRead,0x006315D0,(TextureRead));
CSF_FUNC(World*,__cdecl,newWorld,0x0058CCB0,(const rw::BBox*));
CSF_FUNC(World*,__cdecl,addClump,0x0058E280,(World*,Clump*));
CSF_FUNC(World*,__cdecl,removeClump,0x0058E310,(World*,Clump*));
CSF_FUNC(int,__cdecl,deleteWorld,0x0058CBD0,(World*));
CSF_FUNC(Frame*,__stdcall,newFrame,0x006235D0,());
CSF_FUNC(int,__cdecl,deleteFrame,0x00623B10,(Frame*));
CSF_FUNC(Atomic*,__stdcall,newAtomic,0x0058F5C0,());
CSF_FUNC(int,__cdecl,deleteAtomic,0x0058F060,(Atomic*));
CSF_FUNC(int,__cdecl,deleteGeometry,0x00590FC0,(Geometry*));
CSF_FUNC(Light*,__cdecl,newLight,0x0059D040,(unsigned char));
CSF_FUNC(int,__cdecl,deleteLight,0x0059D000,(Light*));
CSF_FUNC(Camera*,__stdcall,newCamera,0x006290B0,());
CSF_FUNC(int,__cdecl,deleteCamera,0x00629070,(Camera*));
CSF_FUNC(Raster*,__cdecl,newRaster,0x00627770,(int,int,int,int));
CSF_FUNC(int,__cdecl,deleteRaster,0x00627570,(Raster*));
CSF_FUNC(Image*,__cdecl,newImage,0x00625060,(int,int,int));
CSF_FUNC(int,__cdecl,deleteImage,0x00625F10,(Image*));
static int constructed,destroyed,reads,failKind=-1,failConstructor=-1;
static std::vector<Registry> order;
// The game's object-property vendor ID is -256; the payload here is synthetic.
static constexpr uint32_t plugin=0xffffff00,unknown=0xffffff01;
static int offsets[13];
static decltype(rw::Engine::memfuncs.rwmalloc) savedMalloc;
static rw::MemoryFunctions allocator;
static std::unordered_set<void*> allocated;
static void* trackMalloc(size_t n,rw::uint32 h) {
    auto* p=allocator.rwmalloc(n,h);if(p) allocated.insert(p);return p;
}
static void trackFree(void* p) { allocated.erase(p);allocator.rwfree(p); }
static void* trackRealloc(void* p,size_t n,rw::uint32 h) {
    auto* q=allocator.rwrealloc(p,n,h);
    if(q || !n) { allocated.erase(p);if(q) allocated.insert(q); }return q;
}
static int failAllocation;
static void* failMalloc(size_t n,rw::uint32 h) { return n && failAllocation--==0?nullptr:savedMalloc(n,h); }
static void* ctor(void* p,int off,int size) {
    if(failConstructor==0) return nullptr;
    if(failConstructor>0) --failConstructor;
    assert(size==4); ++constructed; *reinterpret_cast<int*>(static_cast<char*>(p)+off)=777; return p;
}
static void* dtor(void* p,int off,int size) {
    assert(size==4); int value=*reinterpret_cast<int*>(static_cast<char*>(p)+off);
    assert(value==777 || (value>=1000 && value<1013)); ++destroyed; return p;
}
static Stream* __cdecl gameRead(Stream* s,int len,void* p,int off,int size) {
    assert(s && streamNative(s) && len==4 && size==4);
    int value=0; assert(read(s,&value,4)==4 && value>=1000 && value<1013);
    auto kind=Registry(value-1000); assert(off==offsets[value-1000]);
    assert(*reinterpret_cast<int*>(static_cast<char*>(p)+off)==777 ||
           (kind==Registry::Texture && *reinterpret_cast<int*>(static_cast<char*>(p)+off)==value));
    *reinterpret_cast<int*>(static_cast<char*>(p)+off)=value;
    if(kind==Registry::Frame) {
        auto* f=static_cast<Frame*>(p); assert(frameNative(f));
        assert(f->root && (f->child || f->object.parent));
    } else if(kind==Registry::Atomic) {
        auto* a=static_cast<Atomic*>(p); assert(atomicNative(a));
        assert(a->geometry && a->geometry->matList.numMaterials==1);
        assert(a->object.object.parent && !a->clump); // read before AddAtomic
        assert(a->geometry->matList.materials[0]->texture);
    } else if(kind==Registry::Geometry) assert(geometryNative(static_cast<Geometry*>(p)));
    else if(kind==Registry::Material) assert(materialNative(static_cast<Material*>(p)));
    else if(kind==Registry::Clump) {
        auto* c=static_cast<Clump*>(p); assert(clumpNative(c)->countAtomics()==2);
        assert(c->object.parent && c->atomics.next!=&c->atomics);
        assert(!readClump(s)); // nested reads cannot replace active registries
        // Ordinary facade factories remain usable inside a game callback:
        // observers must not create duplicate identities or double-run dtors.
        auto* frame=newFrame();assert(frame && deleteFrame(frame));
        auto* atomic=newAtomic();assert(atomic);
        auto* clone=atomicNative(atomic)->clone();assert(clone);
        auto* alias=static_cast<Atomic*>(streamObjectExport(Registry::Atomic,clone));
        assert(alias && deleteAtomic(alias) && deleteAtomic(atomic));
        auto* geometry=geometryCreate(0,0,rw::Geometry::POSITIONS);assert(geometry && deleteGeometry(geometry));
        auto* material=materialCreate();assert(material && materialDestroy(material));
        auto* light=newLight(128);assert(light && deleteLight(light));
        auto* camera=newCamera();assert(camera && deleteCamera(camera));
        auto* clump=clumpCreate();assert(clump && deleteClump(clump));
        auto* raster=newRaster(1,1,32,rw::Raster::TEXTURE|rw::Raster::DONTALLOCATE);assert(raster && deleteRaster(raster));
    }
    order.push_back(kind); ++reads;
    return int(kind)==failKind?nullptr:s;
}
static rw::Stream* nativeWrite(rw::Stream* s,int,void*,int kind,int) { s->writeI32(1000+kind);return s; }
static int nativeSize(void*,int,int) { return 4; }
static int __cdecl gameSize(void* p,int off,int size) {
    assert(size==4 && *reinterpret_cast<int*>(static_cast<char*>(p)+off)>=1000);return 4;
}
static Texture* __cdecl loadTexture(const char* name,const char*) {
    assert(!std::strcmp(name,"fixture") || !std::strcmp(name,"aux"));
    // Deliberately C-style, like a game texture loader: an allocation exception
    // between these operations must not strand a transient image facade.
    auto* temporary=newImage(1,1,32);assert(temporary);
    auto* t=newTexture(nullptr);assert(t);assert(deleteImage(temporary));
    std::strcpy(t->name,name); return t;
}
// A native plugin owning a secondary texture exercises the same lifetime path
// as MatFX: native material destruction, not the facade's primary-texture path.
static void* auxCtor(void* n,int off,int) { *PLUGINOFFSET(rw::Texture*,n,off)=nullptr;return n; }
static void* auxDtor(void* n,int off,int) {
    auto* t=*PLUGINOFFSET(rw::Texture*,n,off);if(t) t->destroy();return n;
}
static rw::Stream* auxRead(rw::Stream* s,int,void* n,int off,int) {
    s->readI32();*PLUGINOFFSET(rw::Texture*,n,off)=rw::Texture::read("aux",nullptr);return s;
}
static rw::Stream* auxWrite(rw::Stream* s,int,void*,int,int) { s->writeI32(0);return s; }
static int fakeFile;
static void* imageOpen(const char* path,const char*) {
    auto n=std::strlen(path);return n>=4 && !std::strcmp(path+n-4,".csf")?&fakeFile:nullptr;
}
static int imageClose(void* p) { assert(p==&fakeFile);return 0; }
static rw::Image* imageRead(const char*) {
    auto* i=rw::Image::create(1,1,32);assert(i);i->allocate();
    std::memset(i->pixels,255,4);return i;
}
static rw::PluginList& list(Registry k) {
    switch(k) {
    case Registry::Frame:return rw::Frame::s_plglist;
    case Registry::Atomic:return rw::Atomic::s_plglist;
    case Registry::Geometry:return rw::Geometry::s_plglist;
    case Registry::Material:return rw::Material::s_plglist;
    case Registry::Light:return rw::Light::s_plglist;
    case Registry::Camera:return rw::Camera::s_plglist;
    case Registry::Clump:return rw::Clump::s_plglist;
    default:return rw::Texture::s_plglist;
    }
}
template<class N> static void userFixture(N* object) {
    auto* e=rw::UserDataExtension::get(object);
    // Avoid upstream add()'s memcpy(dst,nullptr,0) nonnull-attribute UB.
    e->arrays=static_cast<rw::UserDataArray*>(rw::Engine::memfuncs.rwmalloc(sizeof(rw::UserDataArray),0));
    assert(e->add("fixture-name",rw::USERDATASTRING,1)==0);
    e->get(0)->setString(0,"root-value");
}
// Writer callback's offset is an encoding of object type, not native storage.
static void writerPlugin(Registry k,uint32_t id) {
    auto& l=list(k); l.registerPlugin(0,id,nullptr,nullptr,nullptr);
    l.registerStream(id,nullptr,nativeWrite,nativeSize);
    FORLIST(link,l.plugins) { auto* p=LLLinkGetData(link,rw::Plugin,inParentList); if(p->id==id) p->offset=int(k); }
}
static std::vector<uint8_t> fixture() {
    auto* c=rw::Clump::create(); auto* root=rw::Frame::create();auto* child=rw::Frame::create();
    root->matrix.pos=rw::makeV3d(1,2,3);child->matrix.pos=rw::makeV3d(4,5,6); root->addChild(child);
    c->setFrame(root);
    userFixture(root);
    auto* g=rw::Geometry::create(3,1,rw::Geometry::POSITIONS|rw::Geometry::NORMALS);
    auto* m=rw::Material::create();auto* t=rw::Texture::create(nullptr);std::strcpy(t->name,"fixture");userFixture(t);
    m->setTexture(t);t->destroy();g->matList.appendMaterial(m);m->destroy();
    g->triangles[0]={{0,1,2},0};
    for(int i=0;i<3;++i) { g->morphTargets[0].vertices[i]=rw::makeV3d(float(i),0,0);g->morphTargets[0].normals[i]=rw::makeV3d(0,1,0); }
    g->morphTargets[0].boundingSphere={rw::makeV3d(1,0,0),2};g->unlock();
    for(int i=0;i<2;++i) { auto* a=rw::Atomic::create();a->setFrame(i?child:root);a->setGeometry(g,0);c->addAtomic(a); }
    g->destroy();
    auto* light=rw::Light::create(128);light->setFrame(child);light->radius=12;c->addLight(light);
    auto* camera=rw::Camera::create();camera->setFrame(root);c->addCamera(camera);
    std::vector<uint8_t> bytes(c->streamGetSize()+12);rw::StreamMemory out;out.open(bytes.data(),uint32_t(bytes.size()));
    assert(c->streamWrite(&out));assert(out.tell()==bytes.size());c->setFrame(nullptr);c->destroy();
    // Upstream destroyHierarchy does not decrement Frame::numAllocated. Use
    // ordinary frame destruction for this writer-only native fixture.
    child->destroy();root->destroy();
    // librw writes the same geometry twice and always geometryIndex=0 (it has
    // no deduplicator). Thus both read atomics must still share one identity;
    // the unused second geometry must be torn down before the clump extension.
    return bytes;
}
static Clump* load(std::vector<uint8_t>& bytes) {
    Memory memory{bytes.data()+12,uint32_t(bytes.size()-12)};auto* s=streamOpen(3,1,&memory);assert(s);
    auto* c=readClump(s);assert(streamClose(s,nullptr));return c;
}
int main() {
    assert(init(nullptr,0,0));assert(attachWorld());assert(attachUser());
    for(auto k:{Registry::Frame,Registry::Atomic,Registry::Geometry,Registry::Material,
                Registry::Light,Registry::Camera,Registry::Clump,Registry::Texture}) {
        offsets[int(k)]=registerABIPlugin(k,4,plugin,ctor,dtor,nullptr);assert(offsets[int(k)]>=0);
        assert(registerABIPluginStream(k,plugin,reinterpret_cast<uintptr_t>(gameRead),0,reinterpret_cast<uintptr_t>(gameSize))==offsets[int(k)]);
        writerPlugin(k,plugin);writerPlugin(k,unknown);
    }
    rw::Material::registerPlugin(sizeof(rw::Texture*),0x12345680,auxCtor,auxDtor,nullptr);
    rw::Material::registerPluginStream(0x12345680,auxRead,auxWrite,nativeSize);
    assert(open(nullptr));assert(start());assert(setRead(loadTexture));rw::version=0x36003;
    assert(rw::Image::registerFileFormat("csf",imageRead,nullptr));
    allocator=rw::Engine::memfuncs;
    rw::Engine::memfuncs={trackMalloc,trackRealloc,trackFree,trackMalloc,trackRealloc};
    auto bytes=fixture(); assert(objectsEmpty() && framesEmpty());
    auto* c=load(bytes);assert(c);
    auto* root=static_cast<Frame*>(c->object.parent);assert(root->child && root->child->root==root);
    assert(root->matrix.pos.x==1 && root->child->matrix.pos.x==4);
    rw::UserDataExtension mirrored{};
    std::memcpy(&mirrored,reinterpret_cast<char*>(root)+plugins(Registry::Frame).getPluginOffset(rw::ID_USERDATA),sizeof(mirrored));
    assert(mirrored.numArrays==1 && !std::strcmp(mirrored.get(0)->getString(0),"root-value"));
    auto* a=reinterpret_cast<Atomic*>(reinterpret_cast<char*>(c->atomics.next)-offsetof(Atomic,inClump));
    auto* b=reinterpret_cast<Atomic*>(reinterpret_cast<char*>(a->inClump.next)-offsetof(Atomic,inClump));
    assert(a!=b && a->geometry==b->geometry && a->geometry->refCount==2);
    assert(a->geometry->matList.materials[0]->texture->refCount==1);
    assert(a->geometry->mesh && a->geometry->mesh->numMeshes==1);
    assert(reinterpret_cast<Mesh*>(a->geometry->mesh+1)->material==a->geometry->matList.materials[0]);
    assert(clumpNative(c)->countLights()==1 && clumpNative(c)->countCameras()==1);
    assert(c->lights.next!=&c->lights && c->cameras.next!=&c->cameras);
    // GetSize includes game size callbacks with facade offsets, and counts the
    // shared geometry only once (unlike upstream's writer).
    auto nativeBytes=clumpNative(c)->streamGetSize();
    auto expected=nativeBytes-12-geometryNative(a->geometry)->streamGetSize();
    assert(clumpSize(c)==expected && clumpSize(c)==expected);
    assert(order.front()==Registry::Frame && order.back()==Registry::Clump);
    rw::BBox box={rw::makeV3d(100,100,100),rw::makeV3d(-100,-100,-100)};
    auto* world=newWorld(&box);assert(world && addClump(world,c));assert(removeClump(world,c));assert(deleteWorld(world));
    assert(deleteClump(c));assert(objectsEmpty() && framesEmpty() && imagesEmpty());
    assert(constructed==destroyed);
    auto* held=newTexture(nullptr);assert(held);std::strcpy(held->name,"fixture");
    rw::TexDictionary::getCurrent()->add(textureNative(held));streamImageExport();
    c=load(bytes);assert(c && held->refCount==2 && deleteClump(c));assert(held->refCount==1);
    failKind=int(Registry::Clump);assert(!load(bytes));failKind=-1;
    assert(held->refCount==1 && textureNative(held)->refCount==1 && deleteTexture(held));
    assert(objectsEmpty() && framesEmpty() && imagesEmpty() && constructed==destroyed);
    // Every extension stage can abort, not just the root. The same fixture must
    // load again afterwards: maps, callbacks, plugin lists and ownership reset.
    for(auto k:{Registry::Frame,Registry::Texture,Registry::Material,Registry::Geometry,
                Registry::Atomic,Registry::Light,Registry::Camera,Registry::Clump}) {
        failKind=int(k); assert(!load(bytes));assert(objectsEmpty() && framesEmpty() && imagesEmpty());
        assert(constructed==destroyed);failKind=-1;c=load(bytes);assert(c && deleteClump(c));
    }
    for(int i=0;i<10;++i) {
        failConstructor=i;assert(!load(bytes));failConstructor=-1;
        assert(objectsEmpty() && framesEmpty() && imagesEmpty() && constructed==destroyed);
    }
    auto shortFile=bytes;shortFile.resize(shortFile.size()-1);assert(!load(shortFile));
    auto malformed=bytes;malformed[12+12]=255; // huge atomic count
    assert(!load(malformed));assert(objectsEmpty() && framesEmpty() && imagesEmpty());
    auto followed=bytes;uint32_t marker=0xf00d1234;
    followed.insert(followed.end(),reinterpret_cast<uint8_t*>(&marker),reinterpret_cast<uint8_t*>(&marker)+4);
    Memory memory{followed.data()+12,uint32_t(followed.size()-12)};auto* input=streamOpen(3,1,&memory);assert(input);
    c=readClump(input);uint32_t next=0;
    assert(c && read(input,&next,4)==4 && next==marker && deleteClump(c) && streamClose(input,nullptr));
    input=streamOpen(3,1,&memory);assert(input);failKind=int(Registry::Frame);
    assert(!readClump(input) && streamNative(input)->tell()<bytes.size()-12);failKind=-1;
    assert(streamClose(input,nullptr));
    savedMalloc=rw::Engine::memfuncs.rwmalloc;
    int oomTests=0;
    for(int i=0;i<128;++i) {
        failAllocation=i;rw::Engine::memfuncs.rwmalloc=failMalloc;
        c=load(bytes);rw::Engine::memfuncs.rwmalloc=savedMalloc;
        if(c) { assert(deleteClump(c));break; }
        ++oomTests;assert(objectsEmpty() && framesEmpty() && imagesEmpty());assert(constructed==destroyed);
    }
    assert(oomTests>20 && oomTests<128);
    // Exercise the production default texture path, including temporary Image
    // facades and offline raster pixels, not only a game texture-read callback.
    auto fileFunctions=rw::engine->filefuncs;
    rw::engine->filefuncs.rwfopen=imageOpen;rw::engine->filefuncs.rwfclose=imageClose;
    assert(setRead(nullptr));int imageOomTests=0;
    for(int i=0;i<160;++i) {
        failAllocation=i;rw::Engine::memfuncs.rwmalloc=failMalloc;
        c=load(bytes);rw::Engine::memfuncs.rwmalloc=savedMalloc;
        if(c) { assert(deleteClump(c));break; }
        ++imageOomTests;assert(objectsEmpty() && framesEmpty() && imagesEmpty());assert(constructed==destroyed);
    }
    assert(imageOomTests>oomTests && imageOomTests<160);
    rw::engine->filefuncs=fileFunctions;
    assert(rw::Frame::numAllocated==0 && rw::Geometry::numAllocated==0 && rw::Texture::numAllocated==0);
    assert(allocated.empty()); // includes raw partial-reader allocations, not just maps
    assert(constructed==destroyed);assert(stop() && close() && term());
    std::printf("rw_clump: graph, shared ownership, plugin order/offsets, unknown chunks, 8 rollback stages, %d/%d OOM points (game/default texture loader) OK (%d reads)\n",oomTests,imageOomTests,reads);
}
