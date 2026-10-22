// Optional extracted-asset regression. Does not require a GPU or game process.
#include "globals.h"
#include "paths.h"
#include "boundary/rw/world.h"
#include "boundary/rw/plugins.h"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
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
CSF_FUNC(uint32_t,__cdecl,read,0x006293E0,(Stream*,void*,uint32_t));
CSF_FUNC(World*,__cdecl,readWorld,0x0059FEA0,(Stream*));
CSF_FUNC(int,__cdecl,deleteWorld,0x0058CBD0,(World*));
CSF_FUNC(World*,__cdecl,sectors,0x0058CFF0,(World*,Sector* (*)(Sector*,void*),void*));
CSF_FUNC(World*,__cdecl,materials,0x0058CF30,(World*,Material* (*)(Material*,void*),void*));
CSF_FUNC(World*,__cdecl,clumps,0x0058CEF0,(World*,Clump* (*)(Clump*,void*),void*));
CSF_FUNC(int,__cdecl,setID,0x00593D60,(void*,int));
CSF_FUNC(int,__cdecl,getID,0x00593D80,(void*));
static unsigned properties;
static Stream* propertyRead(Stream* stream,int length,void* object,int,int) {
    auto* s=static_cast<Sector*>(object);
    uint32_t fields[2];assert(read(stream,fields,8)==8 && fields[0]==1 && fields[1]<=1);
    assert(length==12+(fields[1]?s->numTriangles:0));
    std::vector<uint8_t> payload(fields[1]?s->numTriangles:0);
    if(!payload.empty()) assert(read(stream,payload.data(),uint32_t(payload.size()))==payload.size());
    ++properties;return stream;
}
// Texture lookup is deliberately a test double: this test covers BSP decoding,
// not mission texture dictionaries or GPU upload. Materials still stream via
// the real librw decoder and boundary adoption path.
static rw::Texture* texture(const char* name,const char* mask) {
    auto* t=rw::Texture::create(nullptr);assert(t);
    std::strncpy(t->name,name,31);std::strncpy(t->mask,mask,31);return t;
}
static uint32_t word(const std::vector<uint8_t>& b,size_t p) {
    assert(p+4<=b.size());uint32_t v;std::memcpy(&v,b.data()+p,4);return v;
}
struct Counts { World* world;uint64_t vertices=0,triangles=0;unsigned sectors=0; };
static Sector* visit(Sector* s,void* p) {
    auto& c=*static_cast<Counts*>(p);++c.sectors;c.vertices+=s->numVertices;c.triangles+=s->numTriangles;
    assert(s->mesh && s->type==-1 && getID(s)==0);assert(setID(s,int(c.sectors))==int(c.sectors) && getID(s)==int(c.sectors));
    assert(s->bounds.inf.x<=s->bounds.sup.x && s->collisionBounds.inf.z<=s->collisionBounds.sup.z);
    for(unsigned i=0;i<s->numTriangles;++i) {
        auto& t=s->triangles[i];assert(t.v[0]<s->numVertices && t.v[1]<s->numVertices && t.v[2]<s->numVertices);
        assert(t.matId+s->materialBase<c.world->materials.numMaterials);
    }
    auto* meshes=reinterpret_cast<Mesh*>(s->mesh+1);
    for(unsigned i=0;i<s->mesh->numMeshes;++i) assert(materialNative(meshes[i].material));
    return s;
}
class ShortStream final:public rw::Stream {
    const std::vector<uint8_t>& data;uint32_t position;
public:
    ShortStream(const std::vector<uint8_t>& b,size_t at):data(b),position(uint32_t(at)) {}
    uint32_t read8(void* out,uint32_t n) override {
        n=std::min({n,997u,uint32_t(data.size()-position)});
        if(n) std::memcpy(out,data.data()+position,n);position+=n;return n;
    }
    uint32_t write8(const void*,uint32_t) override { assert(false);return 0; }
    void seek(int32_t n,int32_t whence=1) override {
        int64_t at=(whence==0?0:whence==1?position:data.size())+int64_t(n);
        assert(at>=0 && at<=int64_t(data.size()));position=uint32_t(at);
    }
    uint32_t tell() override { return position; }
    bool eof() override { return position==data.size(); }
};
static bool asset(const std::string& path) {
    auto* f=std::fopen(path.c_str(),"rb");
    if(!f) { std::printf("SKIP: extracted world absent: %s\n",path.c_str());return false; }
    assert(!std::fseek(f,0,SEEK_END));long length=std::ftell(f);assert(length>0);std::rewind(f);
    std::vector<uint8_t> bytes(static_cast<size_t>(length));assert(std::fread(bytes.data(),1,bytes.size(),f)==bytes.size());std::fclose(f);
    size_t at=0;
    for(;at+88<=bytes.size();++at) if(word(bytes,at)==11 && word(bytes,at+12)==1 && word(bytes,at+16)==64) break;
    assert(at+88<=bytes.size());
    uint32_t nt=word(bytes,at+40),nv=word(bytes,at+44),nl=word(bytes,at+52);
    for(int mode=0;mode<2;++mode) {
        properties=0;
        ShortStream shortInput(bytes,at+12);
        auto* stream=mode?streamBorrow(&shortInput):streamOpen(2,1,const_cast<char*>(path.c_str()));assert(stream);
        if(!mode) streamNative(stream)->seek(int32_t(at+12),0); // caller consumed WORLD header
        auto* w=readWorld(stream);assert(w);
        assert(streamNative(stream)->tell()==bytes.size());
        Counts c{w};assert(sectors(w,visit,&c)==w);
        assert(c.vertices==nv && c.triangles==nt && c.sectors==nl && properties==nl);
        unsigned nm=0,nc=0;
        assert(materials(w,[](Material* m,void* p)->Material* { assert(materialNative(m));++*static_cast<unsigned*>(p);return m; },&nm)==w);
        assert(nm==unsigned(w->materials.numMaterials));
        assert(clumps(w,[](Clump* c,void* p)->Clump* { ++*static_cast<unsigned*>(p);return c; },&nc)==w && nc==0);
        assert(deleteWorld(w) && worldsEmpty() && objectsEmpty());
        if(mode) streamUnborrow(stream);else assert(streamClose(stream,nullptr));
        std::printf("%s %s: materials=%u sectors=%u vertices=%llu triangles=%llu properties=%u passed\n",
            path.c_str(),mode?"short-read":"filename",nm,nl,(unsigned long long)c.vertices,(unsigned long long)c.triangles,properties);
    }
    return true;
}
int main() {
    assert(init(nullptr,0,0) && attachWorld() && attachUser());
    assert(registerABIPlugin(Registry::Sector,8,0xffffff00)>=0);
    assert(registerABIPluginStream(Registry::Sector,0xffffff00,reinterpret_cast<uintptr_t>(&propertyRead))>=0);
    assert(openEngine(nullptr) && start());auto saved=rw::Texture::readCB;rw::Texture::readCB=texture;
    const char* fr01=std::getenv("CSF_FR01_DIR");std::string dir=fr01?fr01:paksPath("Ransom/Maps/FR01");
    bool a=asset(std::string(dir)+"/FR01.rws"),b=asset(std::string(dir)+"/FR01_col.rws");
    rw::Texture::readCB=saved;assert(stop() && closeEngine() && term());
    std::puts(a && b?"rw real FR01 worlds passed":"rw real FR01 worlds: absent assets skipped");
}
