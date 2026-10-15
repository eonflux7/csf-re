#include "globals.h"
#include "boundary/rw/world.h"
#include "boundary/rw/plugins.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>
#include <unordered_set>
#include <string>
using namespace csf_rw;
CSF_GLOBAL(int,staticFrameFlag,0x008A6450);
CSF_GLOBAL(World*,pvsCurrent,0x008BE7A0);
CSF_FUNC(int,__cdecl,init,0x00627360,(const MemoryFunctions*,char,int));
CSF_FUNC(int,__stdcall,attachWorld,0x0058D0B0,());
CSF_FUNC(int,__stdcall,attachLt,0x0056F3C0,());
CSF_FUNC(int,__stdcall,attachPvs,0x005865E0,());
CSF_FUNC(int,__stdcall,attachUser,0x0058C040,());
CSF_FUNC(int,__cdecl,open,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,close,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
CSF_FUNC(Stream*,__cdecl,streamOpen,0x006298E0,(int,int,void*));
CSF_FUNC(int,__cdecl,streamClose,0x00629810,(Stream*,Memory*));
CSF_FUNC(uint32_t,__cdecl,read,0x006293E0,(Stream*,void*,uint32_t));
CSF_FUNC(World*,__cdecl,readWorld,0x0059FEA0,(Stream*));
CSF_FUNC(int,__cdecl,deleteWorld,0x0058CBD0,(World*));
using SC=Sector* (__cdecl *)(Sector*,void*);
CSF_FUNC(World*,__cdecl,sectors,0x0058CFF0,(World*,SC,void*));
CSF_FUNC(World*,__cdecl,sectorWorld,0x0058D1A0,(Sector*));
CSF_FUNC(uint32_t,__cdecl,pvs,0x005863D0,(World*));
CSF_FUNC(uint32_t,__cdecl,getStyle,0x0056EFF0,());
CSF_FUNC(int,__cdecl,setStyle,0x0056F2B0,(uint32_t,World*));
CSF_FUNC(int,__cdecl,queryMaps,0x0056F380,(World*));
CSF_FUNC(int,__cdecl,setStatic,0x006329C0,(int));
CSF_FUNC(rw::Matrix*,__cdecl,matrixRead,0x0063BF20,(Stream*,rw::Matrix*));
CSF_FUNC(int,__cdecl,matrixDelete,0x00624D10,(rw::Matrix*));
CSF_FUNC(rw::BBox*,__cdecl,bboxAdd,0x0063C9B0,(rw::BBox*,rw::V3d*));
CSF_FUNC(rw::BBox*,__cdecl,bboxCalc,0x0063C8B0,(rw::BBox*,rw::V3d*,int));
CSF_FUNC(int,__cdecl,lineSphere,0x007DA4E0,(const rw::V3d*,const rw::Sphere*,float*));
CSF_FUNC(int,__cdecl,lineTriangle,0x00620640,(const rw::V3d*,const rw::V3d*,const rw::V3d*,const rw::V3d*,const rw::V3d*,float*));
CSF_FUNC(int,__cdecl,sphereTriangle,0x00620840,(const rw::Sphere*,const rw::V3d*,const rw::V3d*,const rw::V3d*,rw::V3d*,float*));
using SI=Sector* (__cdecl *)(Intersection*,Sector*,void*);
CSF_FUNC(World*,__cdecl,intersections,0x0056B9B0,(World*,Intersection*,SI,void*));
using AC=Atomic* (__cdecl *)(Atomic*,void*);
CSF_FUNC(Sector*,__cdecl,atomics,0x0058DE50,(Sector*,AC,void*));
using WC=CollisionTriangle* (__cdecl *)(Intersection*,Sector*,CollisionTriangle*,float,void*);
using ATC=CollisionTriangle* (__cdecl *)(Intersection*,CollisionTriangle*,float,void*);
using WA=Atomic* (__cdecl *)(Intersection*,Sector*,Atomic*,float,void*);
CSF_FUNC(World*,__cdecl,collision,0x0056BAF0,(World*,Intersection*,WC,void*));
CSF_FUNC(Atomic*,__cdecl,atomicCollision,0x0056BDF0,(Atomic*,Intersection*,ATC,void*));
CSF_FUNC(World*,__cdecl,atomicIntersections,0x0056BBD0,(World*,Intersection*,WA,void*));
CSF_FUNC(Atomic*,__cdecl,defaultRender,0x0058E680,(Atomic*));
CSF_FUNC(Atomic*,__stdcall,newAtomic,0x0058F5C0,());
CSF_FUNC(int,__cdecl,deleteAtomic,0x0058F060,(Atomic*));
CSF_FUNC(Frame*,__stdcall,newFrame,0x006235D0,());
CSF_FUNC(int,__cdecl,deleteFrame,0x00623B10,(Frame*));
CSF_FUNC(Frame*,__cdecl,updateFrame,0x006236A0,(Frame*));
CSF_FUNC(Atomic*,__cdecl,setFrame,0x0058F300,(Atomic*,Frame*));
CSF_FUNC(Atomic*,__cdecl,setGeometry,0x0058EFE0,(Atomic*,Geometry*,unsigned char));
CSF_FUNC(int,__cdecl,deleteGeometry,0x00590FC0,(Geometry*));
CSF_FUNC(World*,__cdecl,addClump,0x0058E280,(World*,Clump*));
CSF_FUNC(World*,__cdecl,removeClump,0x0058E310,(World*,Clump*));
CSF_FUNC(int,__cdecl,deleteClump,0x0058F740,(Clump*));
CSF_FUNC(Camera*,__stdcall,newCamera,0x006290B0,());
CSF_FUNC(int,__cdecl,deleteCamera,0x00629070,(Camera*));
CSF_FUNC(Camera*,__cdecl,cameraSectors,0x0058DEF0,(Camera*,SC,void*));
static constexpr uint32_t id=0xffffff00;
static int wc,sc,mc,wd,sd,md,failCtor=-1,failRead=-1;
static std::vector<int> order;
static void* ctor(void* p,int off,int size) {
    if(failCtor==0) return nullptr;if(failCtor>0) --failCtor;
    assert(size==4);*reinterpret_cast<int*>(static_cast<char*>(p)+off)=777;return p;
}
static void* worldCtor(void* p,int off,int n) { auto* r=ctor(p,off,n);if(r) ++wc;return r; }
static void* sectorCtor(void* p,int off,int n) { auto* r=ctor(p,off,n);if(r) ++sc;return r; }
static void* materialCtor(void* p,int off,int n) { auto* r=ctor(p,off,n);if(r) ++mc;return r; }
static void* worldDtor(void* p,int,int) {
    // Original streamed sectors live in the world's allocation until AFTER
    // the world destructor. A world plugin can still inspect their headers
    // and CPU arrays after sector plugin destruction.
    auto* w=static_cast<World*>(p);unsigned count=0;
    assert(sectors(w,[](Sector* s,void* data)->Sector* {
        ++*static_cast<unsigned*>(data);assert(sectorWorld(s));
        if(s->numVertices) { assert(s->vertices);assert(s->vertices[0].x==-1 || s->vertices[0].x==1); }
        return s;
    },&count)==w && count>0);
    ++wd;return p;
}
static void* sectorDtor(void* p,int,int) { assert(sectorWorld(static_cast<Sector*>(p)));++sd;return p; }
static void* materialDtor(void* p,int,int) { ++md;return p; }
static Stream* gameRead(Stream* s,int length,void* p,int off,int size) {
    assert(length==4 && size==4);int value;assert(read(s,&value,4)==4);
    if(value==9) {
        auto* w=static_cast<World*>(p);assert(w->root);
        auto* leaf=w->root->type<0?w->root:reinterpret_cast<Plane*>(w->root)->left;
        assert(!leaf->mesh);
    }
    else { auto* leaf=static_cast<Sector*>(p);assert(leaf->type==-1 && leaf->numVertices==3 && !leaf->mesh && sectorWorld(leaf)); }
    order.push_back(value);*reinterpret_cast<int*>(static_cast<char*>(p)+off)=value;
    return value==failRead?nullptr:s;
}
using Bytes=std::vector<uint8_t>;
template<class T> void put(Bytes& b,const T& v) { auto* p=reinterpret_cast<const uint8_t*>(&v);b.insert(b.end(),p,p+sizeof(v)); }
void append(Bytes& b,const Bytes& v) { b.insert(b.end(),v.begin(),v.end()); }
Bytes chunk(uint32_t kind,Bytes b) {
    Bytes result;put(result,kind);put(result,uint32_t(b.size()));put(result,uint32_t(rw::libraryIDPack(rw::version,0)));append(result,b);return result;
}
Bytes extension(int value) { Bytes p;put(p,value);Bytes b=chunk(id,p);append(b,chunk(0xffffff01,Bytes{1,2,3}));return chunk(3,b); }
rw::BBox bounds{rw::makeV3d(4,4,4),rw::makeV3d(-4,-4,-4)};
Bytes leaf(int base,int triMaterial,float x,int value) {
    Bytes b;put(b,uint32_t(base));put(b,uint32_t(1));put(b,uint32_t(3));
    put(b,rw::makeV3d(x,-1,0));put(b,rw::makeV3d(x,1,1));put(b,uint32_t(0));put(b,uint32_t(0));
    rw::V3d v[3]={{x,-1,0},{x,1,0},{x,0,1}};put(b,v);
    int8_t normals[12]={127,0,0,0,127,0,0,0,127,0,0,0};put(b,normals);
    rw::RGBA colors[3]={{1,2,3,4},{5,6,7,8},{9,10,11,12}};put(b,colors);
    rw::TexCoords uv[3]={{0,0},{1,0},{0,1}};put(b,uv);put(b,uv);
    rw::Triangle t={{0,1,2},uint16_t(triMaterial)};
    if(rw::version<0x36002) t={{uint16_t(triMaterial),0,1},2};
    put(b,t);
    Bytes user;put(user,int32_t(1));put(user,int32_t(5));for(char ch:std::string("name\0",5)) put(user,ch);
    put(user,uint32_t(rw::USERDATAINT));put(user,int32_t(1));put(user,int32_t(42));
    auto ext=extension(value);Bytes extBody(ext.begin()+12,ext.end());append(extBody,chunk(rw::ID_USERDATA,user));
    b=chunk(1,b);append(b,chunk(3,extBody));return chunk(9,b);
}
Bytes fixture(bool single=false,bool overlap=true,bool compactUV=false) {
    rw::MaterialList list{};list.init();auto* a=rw::Material::create();auto* b=rw::Material::create();
    a->color={10,20,30,255};b->color={40,50,60,255};list.appendMaterial(a);list.appendMaterial(b);list.appendMaterial(a);a->destroy();b->destroy();
    auto* buffer=static_cast<uint8_t*>(rw::Engine::memfuncs.rwmalloc(4096,0));assert(buffer);
    rw::StreamMemory output;output.open(buffer,4096,4096);list.streamWrite(&output);
    Bytes materials(output.data+12,output.data+output.tell());auto* storage=output.data;output.close();rw::Engine::memfuncs.rwfree(storage);list.deinit();
    Bytes h;put(h,uint32_t(single));put(h,rw::makeV3d(-1,-2,-3));
    put(h,uint32_t(single?1:2));put(h,uint32_t(single?3:6));put(h,uint32_t(single?0:1));put(h,uint32_t(single?1:2));put(h,uint32_t(0));put(h,uint32_t((overlap?0x40000000:0)|(compactUV?0x9c:0x20098)));put(h,bounds);
    Bytes tree;
    if(single) tree=leaf(1,0,-1,1);
    else {
        Bytes plane;put(plane,uint32_t(0));put(plane,float(0));put(plane,uint32_t(1));put(plane,uint32_t(1));
        if(overlap) { put(plane,float(.5));put(plane,float(-.5)); }
        plane=chunk(1,plane);append(plane,leaf(1,0,-1,1));append(plane,leaf(0,2,1,2));tree=chunk(10,plane);
    }
    Bytes result=chunk(1,h);append(result,chunk(8,materials));append(result,tree);append(result,extension(9));put(result,uint32_t(0xcafebabe));return result;
}
Bytes emptyFixture() {
    Bytes header;put(header,uint32_t(1));put(header,rw::V3d{});
    for(unsigned count:{0,0,0,1,0,2}) put(header,uint32_t(count));
    put(header,bounds);
    Bytes count;put(count,uint32_t(0));
    Bytes sector;for(unsigned i=0;i<3;++i) put(sector,uint32_t(0));
    put(sector,bounds.inf);put(sector,bounds.sup);put(sector,uint32_t(0));put(sector,uint32_t(0));
    sector=chunk(1,sector);append(sector,chunk(3,{}));
    Bytes result=chunk(1,header);append(result,chunk(8,chunk(1,count)));
    append(result,chunk(9,sector));append(result,chunk(3,{}));put(result,uint32_t(0xcafebabe));return result;
}
static Sector* visit(Sector* s,void* p) { static_cast<std::vector<Sector*>*>(p)->push_back(s);return s; }
static Sector* intersect(Intersection*,Sector* s,void* p) { return visit(s,p); }
static Atomic* visitAtomic(Atomic* a,void* p) { static_cast<std::vector<Atomic*>*>(p)->push_back(a);return a; }
struct Hits { Intersection* query;std::vector<float> points,distances;bool stop=false; };
static CollisionTriangle* atomicHit(Intersection* q,CollisionTriangle* hit,float distance,void* data) {
    auto& h=*static_cast<Hits*>(data);assert(q==h.query && hit->index==0 && hit->normal.x==1 && hit->point.x==hit->vertices[0]->x);
    h.points.push_back(hit->point.x);h.distances.push_back(distance);return h.stop?nullptr:hit;
}
static CollisionTriangle* worldHit(Intersection* q,Sector* sector,CollisionTriangle* hit,float distance,void* data) {
    assert(sectorWorld(sector) && hit->vertices[0]==sector->vertices);return atomicHit(q,hit,distance,data);
}
static Atomic* atomicWorldHit(Intersection*,Sector*,Atomic* a,float,void* data) { return visitAtomic(a,data); }
static World* load(Bytes& bytes) {
    Memory m{bytes.data(),uint32_t(bytes.size())};auto* s=streamOpen(3,1,&m);assert(s);auto* w=readWorld(s);
    if(w) { uint32_t next;assert(read(s,&next,4)==4 && next==0xcafebabe); }assert(streamClose(s,nullptr));return w;
}
static rw::MemoryFunctions saved;
static std::unordered_set<void*> allocated;
static int failAlloc=-1;
static void* trackedMalloc(size_t n,rw::uint32 hint) {
    if(failAlloc==0) return nullptr;if(failAlloc>0) --failAlloc;
    auto* p=saved.rwmalloc(n,hint);if(p) allocated.insert(p);return p;
}
static void trackedFree(void* p) { allocated.erase(p);saved.rwfree(p); }
static void* trackedRealloc(void* p,size_t n,rw::uint32 hint) {
    if(failAlloc==0) return nullptr;if(failAlloc>0) --failAlloc;
    auto* q=saved.rwrealloc(p,n,hint);if(q || !n) { allocated.erase(p);if(q) allocated.insert(q); }return q;
}
int main() {
    assert(init(nullptr,0,0) && attachWorld() && attachLt() && attachPvs() && attachUser());
    assert(registerABIPlugin(Registry::World,4,id,worldCtor,worldDtor)>=0);
    assert(registerABIPlugin(Registry::Sector,4,id,sectorCtor,sectorDtor)>=0);
    assert(registerABIPlugin(Registry::Material,4,id,materialCtor,materialDtor)>=0);
    for(auto k:{Registry::World,Registry::Sector}) assert(registerABIPluginStream(k,id,reinterpret_cast<uintptr_t>(&gameRead),0,0)>=0);
    assert(open(nullptr) && start());rw::version=0x36003;
    auto bytes=fixture();saved=rw::Engine::memfuncs;rw::Engine::memfuncs.rwmalloc=trackedMalloc;rw::Engine::memfuncs.rwfree=trackedFree;rw::Engine::memfuncs.rwrealloc=trackedRealloc;
    auto* w=load(bytes);assert(w && w->origin.x==1 && w->clumpFrame==2 && (order==std::vector<int>{1,2,9}));
    auto* plane=reinterpret_cast<Plane*>(w->root);assert(plane->leftValue==.5 && plane->rightValue==-.5);
    assert(w->materials.numMaterials==3 && w->materials.materials[0]==w->materials.materials[2]);
    std::vector<Sector*> leaves;assert(sectors(w,visit,&leaves)==w && leaves.size()==2);
    assert(leaves[0]->bounds.sup.x==.5 && leaves[1]->bounds.inf.x==-.5);
    for(auto* s:leaves) {
        assert(s->numTriangles==1 && s->numVertices==3 && s->texCoords[1] && s->colors[1].red==5 && sectorWorld(s)==w);
        auto* meshes=reinterpret_cast<Mesh*>(s->mesh+1);assert(s->mesh->numMeshes==1 && meshes[0].numIndices==3);
        assert(meshes[0].material==w->materials.materials[s->materialBase+s->triangles[0].matId]);
    }
    assert(leaves[0]->triangles[0].matId==0 && leaves[1]->triangles[0].matId==2);
    for(auto* s:leaves) {
        rw::UserDataExtension user{};std::memcpy(&user,reinterpret_cast<char*>(s)+plugins(Registry::Sector).getPluginOffset(rw::ID_USERDATA),sizeof(user));
        assert(user.numArrays==1 && !std::strcmp(user.arrays[0].name,"name") && user.arrays[0].getInt(0)==42);
    }
    assert(pvs(w)==0 && pvsCurrent==w);
    auto pvsOffset=plugins(Registry::World).getPluginOffset(298);uint32_t enabled=1;
    std::memcpy(reinterpret_cast<char*>(w)+pvsOffset,&enabled,4);assert(pvs(w)==1);
    assert(setStyle(4,w)==1 && getStyle()==4 && queryMaps(w)==0);
    Texture t{};t.filterAddressing=0x12345607;
    auto offset=plugins(Registry::Sector).getPluginOffset(276);
    for(auto* s:leaves) { auto* p=reinterpret_cast<char*>(s)+offset+20;auto* pointer=&t;std::memcpy(p,&pointer,sizeof(pointer)); }
    assert(queryMaps(w)==1 && setStyle(0,w)==1 && t.filterAddressing==0x12345606);
    assert(setStyle(4,w)==1 && t.filterAddressing==0x12345601);
    for(auto* s:leaves) std::memset(reinterpret_cast<char*>(s)+offset+20,0,sizeof(void*));
    Intersection q{};q.box={rw::makeV3d(-2,3,3),rw::makeV3d(-3,-3,-3)};q.type=4;std::vector<Sector*> seen;
    assert(intersections(w,&q,intersect,&seen)==w && seen.size()==1 && seen[0]==leaves[0]);
    q.box={{110,110,110},{100,100,100}};seen.clear();
    assert(intersections(w,&q,intersect,&seen)==w && seen==std::vector<Sector*>({leaves[1]}));
    q.type=2;q.point={0,100,100};seen.clear();assert(intersections(w,&q,intersect,&seen)==w && seen==leaves);
    q.type=1;q.line={{0,100,100},{0,110,100}};seen.clear();
    assert(intersections(w,&q,intersect,&seen)==w && seen==std::vector<Sector*>({leaves[1],leaves[0]}));
    q.type=1;q.line={{2,0,.5f},{-2,0,.5f}};Hits hits{&q};
    assert(collision(w,&q,worldHit,&hits)==w && hits.points==std::vector<float>({1,-1}));assert(hits.distances==std::vector<float>({.25f,.75f}));
    hits.points.clear();hits.stop=true;assert(collision(w,&q,worldHit,&hits)==w && hits.points.size()==1);
    q.type=3;q.sphere={{-1,0,.5f},.2f};hits.points.clear();hits.distances.clear();hits.stop=false;
    assert(collision(w,&q,worldHit,&hits)==w && hits.points==std::vector<float>({-1}) && hits.distances[0]==0);
    q.type=4;q.box={{-0.9f,.2f,.6f},{-1.1f,-.2f,.4f}};hits.points.clear();assert(collision(w,&q,worldHit,&hits)==w && hits.points.size()==1);
    auto* camera=newCamera();assert(camera);auto cameraOffset=plugins(Registry::Camera).getPluginOffset(1289);
    auto* cache=reinterpret_cast<char*>(camera)+cameraOffset;auto** array=leaves.data();uint32_t count=2;
    std::memcpy(cache,&array,sizeof(array));std::memcpy(cache+8,&count,4);seen.clear();assert(cameraSectors(camera,visit,&seen)==camera && seen==leaves);
    std::memset(cache,0,12);assert(deleteCamera(camera));
    auto* clump=clumpCreate();auto* frame=newFrame();auto* atomic=newAtomic();auto* geometry=geometryCreate(3,1,rw::Geometry::POSITIONS);
    assert(clump && frame && atomic && geometry);clumpSetFrame(clump,frame);assert(setFrame(atomic,frame));
    geometry->morphTargets[0].vertices[0]={0,-1,0};geometry->morphTargets[0].vertices[1]={0,1,0};geometry->morphTargets[0].vertices[2]={0,0,1};geometry->triangles[0]={{0,1,2},0};geometry->morphTargets[0].boundingSphere={{0,0,.5f},1};
    assert(setGeometry(atomic,geometry,0) && deleteGeometry(geometry) && clumpAddAtomic(clump,atomic) && addClump(w,clump));
    assert(atomic->worldSectors.next!=&atomic->worldSectors);
    q.type=1;q.line={{2,0,.5f},{-2,0,.5f}};hits.points.clear();hits.distances.clear();
    assert(atomicCollision(atomic,&q,atomicHit,&hits)==atomic && hits.points==std::vector<float>({0}) && hits.distances[0]==.5f);
    std::vector<Atomic*> dynamic;assert(atomicIntersections(w,&q,atomicWorldHit,&dynamic)==w && dynamic==std::vector<Atomic*>({atomic}));
    frame->matrix.pos.x=20;assert(updateFrame(frame));dynamic.clear();assert(atomicIntersections(w,&q,atomicWorldHit,&dynamic)==w && dynamic.empty());
    assert(atomic->worldSectors.next==&atomic->worldSectors);assert(!defaultRender(atomic));
    assert(removeClump(w,clump) && deleteClump(clump));
    // Real tie ring, including a null cursor and early stop/mutation contract.
    struct Tie { Link link;Atomic* atomic; };Atomic a{},b{};Tie ta{},tb{};
    auto* s=leaves[0];ta={{&tb.link,&s->atomics},&a};tb={{&s->atomics,&ta.link},&b};s->atomics={&ta.link,&tb.link};
    std::vector<Atomic*> atoms;assert(atomics(s,visitAtomic,&atoms)==s && atoms==std::vector<Atomic*>({&a,&b}));s->atomics={&s->atomics,&s->atomics};
    ta={{&tb.link,&s->atomics},&a};tb={{&s->atomics,&ta.link},&b};s->atomics={&ta.link,&tb.link};
    struct Removal { Tie *first,*second;unsigned calls=0; } removal{&ta,&tb};
    assert(atomics(s,[](Atomic* atomic,void* data)->Atomic* {
        auto& r=*static_cast<Removal*>(data);++r.calls;
        for(auto* tie:{r.first,r.second}) { tie->link.prev->next=tie->link.next;tie->link.next->prev=tie->link.prev; }
        return atomic;
    },&removal)==s && removal.calls==1 && s->atomics.next==&s->atomics && s->atomics.prev==&s->atomics);
    assert(deleteWorld(w) && allocated.empty() && objectsEmpty() && worldsEmpty());
    for(int stage:{1,2,9}) { failRead=stage;assert(!load(bytes));assert(allocated.empty() && worldsEmpty() && objectsEmpty()); }failRead=-1;
    {
        Memory memory{bytes.data(),uint32_t(bytes.size())};auto* stream=streamOpen(3,1,&memory);size_t expected=0;
        for(size_t at=0;at+16<=bytes.size();++at) {
            uint32_t kind,length,value;std::memcpy(&kind,bytes.data()+at,4);std::memcpy(&length,bytes.data()+at+4,4);std::memcpy(&value,bytes.data()+at+12,4);
            if(kind==id && length==4 && value==1) { expected=at+16;break; }
        }
        failRead=1;assert(!readWorld(stream) && streamNative(stream)->tell()==expected);failRead=-1;
        assert(streamClose(stream,nullptr) && allocated.empty());
    }
    for(int stage=0;stage<5;++stage) { failCtor=stage;assert(!load(bytes));assert(allocated.empty() && worldsEmpty() && objectsEmpty()); }failCtor=-1;
    int failures=0;
    for(int i=0;i<200;++i) {
        failAlloc=i;auto* x=load(bytes);failAlloc=-1;
        if(x) { assert(deleteWorld(x));break; }++failures;
        assert(allocated.empty() && worldsEmpty() && objectsEmpty());
    }
    assert(failures>10 && failures<200);
    for(size_t n:{size_t(0),size_t(12),size_t(80),bytes.size()-8}) { Bytes truncated(bytes.begin(),bytes.begin()+n);assert(!load(truncated));assert(allocated.empty()); }
    for(auto [at,value]:{std::pair<size_t,uint32_t>{28,3},{48,0x1000000}}) {
        auto invalid=bytes;std::memcpy(invalid.data()+at,&value,4);assert(!load(invalid));assert(allocated.empty() && worldsEmpty() && objectsEmpty());
    }
    auto invalidUser=bytes;
    for(size_t at=0;at+16<invalidUser.size();++at) {
        uint32_t kind;std::memcpy(&kind,invalidUser.data()+at,4);
        if(kind==rw::ID_USERDATA) { uint32_t count=2;std::memcpy(invalidUser.data()+at+12,&count,4);break; }
    }
    assert(!load(invalidUser) && allocated.empty() && worldsEmpty() && objectsEmpty());
    // Both root kinds, old/new triangle word order, and the legacy 16-byte
    // non-overlapping plane envelope. The parent sentinel must survive each.
    for(uint32_t version:{0x35000u,0x36001u,0x36003u,0x37002u}) {
        rw::version=version;
        for(bool single:{false,true}) {
            auto variant=fixture(single,false);auto* loaded=load(variant);assert(loaded);
            std::vector<Sector*> list;assert(sectors(loaded,visit,&list)==loaded && list.size()==(single?1:2));
            assert((loaded->root->type<0)==single);
            for(auto* sector:list) assert(sector->triangles[0].v[0]==0 && sector->triangles[0].v[1]==1 && sector->triangles[0].v[2]==2);
            if(!single) { auto* p=reinterpret_cast<Plane*>(loaded->root);assert(p->leftValue==0 && p->rightValue==0); }
            assert(deleteWorld(loaded) && allocated.empty() && worldsEmpty() && objectsEmpty());
        }
    }
    rw::version=0x36003;
    {
        // If the explicit UV count is absent, TEXTURED2 wins over TEXTURED,
        // including when an exporter sets both low-byte flags.
        auto compact=fixture(false,true,true);auto* loaded=load(compact);
        assert(loaded && loaded->clumpFrame==2);
        auto* p=reinterpret_cast<Plane*>(loaded->root);assert(p->left->texCoords[1] && p->right->texCoords[1]);
        assert(deleteWorld(loaded) && allocated.empty() && worldsEmpty() && objectsEmpty());
    }
    {
        auto empty=emptyFixture();auto* loaded=load(empty);assert(loaded && loaded->root->type==-1);
        assert(!loaded->root->numVertices && !loaded->root->numTriangles && loaded->root->mesh && !loaded->root->mesh->numMeshes);
        assert(loaded->sectorRender(loaded->root)==loaded->root);
        assert(deleteWorld(loaded) && allocated.empty() && worldsEmpty() && objectsEmpty());
    }
    assert(wc==wd && sc==sd && mc==md);
    assert(setStatic(1)==1 && staticFrameFlag==1);assert(setStatic(0)==0 && staticFrameFlag==0);
    rw::V3d points[3]={{1,2,3},{-1,4,0},{2,-2,1}};rw::BBox box{};assert(bboxCalc(&box,points,3)==&box && box.inf.y==-2 && box.sup.y==4);rw::V3d point{7,0,-1};assert(bboxAdd(&box,&point)==&box && box.sup.x==7 && box.inf.z==-1);
    rw::V3d line[2]={{0,0,0},{0,0,1}};rw::Sphere sphere{{0,0,4},1};float distance=-1;assert(lineSphere(line,&sphere,&distance) && distance==4);sphere.center={2,0,4};assert(!lineSphere(line,&sphere,&distance));
    rw::V3d origin{.2f,.2f,1},delta{0,0,-2},v0{0,0,0},v1{1,0,0},v2{0,1,0};assert(lineTriangle(&origin,&delta,&v0,&v1,&v2,&distance) && distance==.5f);assert(!lineTriangle(&origin,&delta,&v0,&v2,&v1,&distance));
    rw::V3d normal{};sphere={{.2f,.2f,.1f},.2f};assert(sphereTriangle(&sphere,&v0,&v1,&v2,&normal,&distance) && normal.z==1 && distance==.1f);
    sphere.center={1.05f,0,0};assert(sphereTriangle(&sphere,&v0,&v1,&v2,&normal,&distance));sphere.center={2,2,0};assert(!sphereTriangle(&sphere,&v0,&v1,&v2,&normal,&distance));
    Bytes matrix;float values[12]={1,0,0,0,1,0,0,0,1,4,5,6};put(matrix,values);put(matrix,uint32_t(3));matrix=chunk(1,matrix);
    Memory memory{matrix.data(),uint32_t(matrix.size())};auto* stream=streamOpen(3,1,&memory);auto* m=matrixRead(stream,nullptr);assert(m && m->pos.x==4 && m->flags==3);assert(matrixDelete(m) && streamClose(stream,nullptr));
    rw::Engine::memfuncs=saved;assert(stop() && close() && term());
    std::printf("rw BSP streaming/support passed (%d allocation failure points)\n",failures);
}
