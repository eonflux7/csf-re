// Real extracted assets are optional. No game, SDK, Windows or GPU required.
#include "globals.h"
#include "boundary/rw/world.h"
#include "boundary/rw/plugins.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace csf_rw;
CSF_FUNC(int,__cdecl,init,0x00627360,(const MemoryFunctions*,char,int));
CSF_FUNC(int,__stdcall,attachWorld,0x0058D0B0,());
CSF_FUNC(int,__stdcall,attachUser,0x0058C040,());
CSF_FUNC(int,__cdecl,openEngine,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,closeEngine,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
CSF_FUNC(Stream*,__cdecl,streamOpen,0x006298E0,(int,int,void*));
CSF_FUNC(int,__cdecl,streamClose,0x00629810,(Stream*,Memory*));
CSF_FUNC(uint32_t,__cdecl,readBytes,0x006293E0,(Stream*,void*,uint32_t));
CSF_FUNC(Stream*,__cdecl,skip,0x00629700,(Stream*,uint32_t));
CSF_FUNC(int,__cdecl,header,0x00629CE0,(Stream*,ChunkHeader*));
CSF_FUNC(World*,__cdecl,readWorld,0x0059FEA0,(Stream*));
CSF_FUNC(int,__cdecl,deleteWorld,0x0058CBD0,(World*));
using SC=Sector* (__cdecl *)(Sector*,void*);
CSF_FUNC(World*,__cdecl,sectors,0x0058CFF0,(World*,SC,void*));
CSF_FUNC(World*,__cdecl,sectorWorld,0x0058D1A0,(Sector*));
CSF_FUNC(int,__cdecl,setID,0x00593D60,(void*,int));
CSF_FUNC(int,__cdecl,getID,0x00593D80,(void*));
using MC=Material* (__cdecl *)(Material*,void*);
using CC=Clump* (__cdecl *)(Clump*,void*);
CSF_FUNC(World*,__cdecl,materials,0x0058CF30,(World*,MC,void*));
CSF_FUNC(World*,__cdecl,clumps,0x0058CEF0,(World*,CC,void*));
CSF_FUNC(int,__cdecl,deleteClump,0x0058F740,(Clump*));
CSF_FUNC(World*,__cdecl,addClump,0x0058E280,(World*,Clump*));
CSF_FUNC(World*,__cdecl,removeClump,0x0058E310,(World*,Clump*));
static unsigned propertyReads=0,propertyDtors=0;
static void* propertyCtor(void* p,int off,int n) { std::memset(static_cast<char*>(p)+off,0,n);return p; }
static void* propertyDtor(void* p,int,int) { ++propertyDtors;return p; }
static Stream* propertyRead(Stream* stream,int length,void* p,int off,int n) {
    auto* s=static_cast<Sector*>(p);assert(n==8 && sectorWorld(s));
    uint32_t words[2];assert(readBytes(stream,words,8)==8 && words[0]==1 && words[1]<=1);
    assert(length==12+(words[1]?s->numTriangles:0));
    std::memcpy(static_cast<char*>(p)+off,words,8);
    if(words[1]) assert(skip(stream,s->numTriangles));
    ++propertyReads;return stream;
}
struct Counts { unsigned leaves=0;uint64_t vertices=0,triangles=0,indices=0; };
static Sector* inspectSector(Sector* s,void* data) {
    auto& c=*static_cast<Counts*>(data);++c.leaves;
    c.vertices+=s->numVertices;c.triangles+=s->numTriangles;
    assert(s->type==-1 && sectorWorld(s) && s->mesh);
    assert(getID(s)==0);setID(s,int(c.leaves));assert(getID(s)==int(c.leaves));
    for(unsigned i=0;i<s->numTriangles;++i) for(unsigned j=0;j<3;++j) assert(s->triangles[i].v[j]<s->numVertices);
    auto* meshes=reinterpret_cast<Mesh*>(s->mesh+1);
    for(unsigned i=0;i<s->mesh->numMeshes;++i) {
        assert(meshes[i].material && meshes[i].indices && meshes[i].numIndices);
        c.indices+=meshes[i].numIndices;
    }
    return s;
}
static bool load(const std::string& path,unsigned expectedLeaves,unsigned expectedPlanes,
                 uint64_t expectedVertices,uint64_t expectedTriangles,unsigned expectedMaterials) {
    auto* file=std::fopen(path.c_str(),"rb");
    if(!file) { std::printf("SKIP: real world absent: %s\n",path.c_str());return false; }
    std::fclose(file);
    auto* stream=streamOpen(2,1,const_cast<char*>(path.c_str()));assert(stream);
    ChunkHeader h{};
    // Exactly 6BEC60's filename-stream view, not a synthetic header search.
    while(header(stream,&h) && h.type!=11) {
        if(h.type==94144) {
            assert(skip(stream,24));ChunkHeader matrix;assert(header(stream,&matrix));
            assert(skip(stream,matrix.length));uint32_t size;assert(readBytes(stream,&size,4)==4);
            assert(skip(stream,size));
        } else assert(skip(stream,h.length));
    }
    assert(h.type==11 && h.version==0x37002);
    auto before=propertyReads;auto* w=readWorld(stream);assert(w);
    assert(propertyReads-before==expectedLeaves);
    Counts c;assert(sectors(w,inspectSector,&c)==w);
    assert(c.leaves==expectedLeaves && c.vertices==expectedVertices && c.triangles==expectedTriangles);
    assert(w->materials.numMaterials==int(expectedMaterials));
    unsigned mc=0;assert(materials(w,[](Material* m,void* p)->Material* { assert(m);++*static_cast<unsigned*>(p);return m; },&mc)==w);
    assert(mc==expectedMaterials);
    unsigned cc=0;auto* clump=clumpCreate();assert(clump && addClump(w,clump)==w);
    assert(clumps(w,[](Clump* c,void* p)->Clump* { ++*static_cast<unsigned*>(p);return c; },&cc)==w && cc==1);
    assert(removeClump(w,clump)==w && deleteClump(clump));
    // The world is last in these two RWS files. The inflated length must not
    // move the parent cursor beyond its actual end.
    unsigned char next;assert(readBytes(stream,&next,1)==0);
    assert(streamClose(stream,nullptr));assert(deleteWorld(w));
    assert(propertyDtors==propertyReads && worldsEmpty());
    std::printf("FR01 boundary: %s materials=%u planes=%u sectors=%u vertices=%llu triangles=%llu indices=%llu OK\n",
                path.c_str(),mc,expectedPlanes,c.leaves,(unsigned long long)c.vertices,
                (unsigned long long)c.triangles,(unsigned long long)c.indices);
    return true;
}
int main() {
    const char* base=std::getenv("CSF_FR01_DIR");if(!base) base="/mnt/e/dev/csf-paks/Ransom/Maps/FR01";
    assert(init(nullptr,0,0) && attachWorld() && attachUser());
    assert(registerABIPlugin(Registry::Sector,8,0xffffff00,propertyCtor,propertyDtor)>=0);
    assert(registerABIPluginStream(Registry::Sector,0xffffff00,reinterpret_cast<uintptr_t>(propertyRead))>=0);
    assert(openEngine(nullptr) && start());
    bool a=load(std::string(base)+"/FR01.rws",40,39,178532,109855,560);
    bool b=load(std::string(base)+"/FR01_col.rws",326,325,84884,99653,14);
    assert(stop() && closeEngine() && term());
    std::puts(a && b?"real FR01 boundary tests complete":"real FR01 boundary tests: absent assets skipped");
}
