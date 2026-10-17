#include "globals.h"
#include "boundary/rw/world.h"
#include "boundary/rw/plugins.h"
#include "boundary/rw/collision.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>
#include <unordered_set>
using namespace csf_rw;
CSF_FUNC(int,__cdecl,init,0x00627360,(const MemoryFunctions*,char,int));
CSF_FUNC(int,__stdcall,worldAttach,0x0058D0B0,());
CSF_FUNC(int,__stdcall,collisionAttach,0x0056C1B0,());
CSF_FUNC(int,__cdecl,openEngine,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,closeEngine,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
CSF_GLOBAL(int,sectorOffset,0x008B0930);
CSF_GLOBAL(int,geometryOffset,0x008B092C);
using Bytes=std::vector<uint8_t>;
static void word(Bytes& b,unsigned at,uint32_t v) { std::memcpy(b.data()+at,&v,4); }
static Bytes fixture() {
    Bytes b(84,0);word(b,0,0x37002);word(b,4,44);word(b,8,68);word(b,12,0x1c020037);
    word(b,16,1);word(b,20,56);word(b,24,0x1c020037);word(b,28,1);
    rw::BBox box{{1,1,1},{-1,-1,-1}};std::memcpy(b.data()+32,&box.inf,12);std::memcpy(b.data()+44,&box.sup,12);
    word(b,56,2);word(b,60,1);word(b,64,0x00000100);word(b,72,0x00010100);
    uint16_t remap[2]={1,0};std::memcpy(b.data()+80,remap,4);return b;
}
static rw::MemoryFunctions savedMemory;
static std::unordered_set<void*> live;
static void* __cdecl trackMalloc(size_t n,rw::uint32 hint) {
    void* p=savedMemory.rwmalloc(n,hint);if(p) live.insert(p);return p;
}
static void __cdecl trackFree(void* p) { live.erase(p);savedMemory.rwfree(p); }
static void* __cdecl failMalloc(size_t,rw::uint32) { return nullptr; }
static CollisionTree* tree(void* object,int offset) {
    CollisionTree* p;std::memcpy(&p,static_cast<char*>(object)+offset,sizeof(p));return p;
}
static bool read(Registry k,Bytes& data,void* object) {
    rw::StreamMemory input;input.open(data.data(),data.size(),data.size());
    auto* s=streamBorrow(&input);assert(s);
    bool ok=readPluginPayload(k,285,s,int(data.size()),object);streamUnborrow(s);return ok;
}
int main() {
    for(int repeat=0;repeat<2;++repeat) {
        assert(init(nullptr,0,0) && worldAttach() && collisionAttach());
        // Pre-fix attach registers neither of these: it cannot furnish the
        // offset/tree that 6EBA90 and 7C2110 read on the mission-load path.
        assert(plugins(Registry::Sector).getPluginOffset(285)>=int(sizeof(Sector)));
        assert(sectorOffset==plugins(Registry::Sector).getPluginOffset(285));
        assert(geometryOffset==plugins(Registry::Geometry).getPluginOffset(285));
        assert(!collisionAttach());
        assert(openEngine(nullptr) && start());
        savedMemory=rw::Engine::memfuncs;
        rw::Engine::memfuncs.rwmalloc=trackMalloc;rw::Engine::memfuncs.rwfree=trackFree;
        for(auto k:{Registry::Sector,Registry::Geometry}) {
            int offset=k==Registry::Sector?sectorOffset:geometryOffset;
            Bytes object(plugins(k).size,0);
            if(k==Registry::Sector) {
                auto* s=reinterpret_cast<Sector*>(object.data());s->type=-1;s->numTriangles=2;
            } else {
                auto* g=reinterpret_cast<Geometry*>(object.data());g->object.type=8;g->numTriangles=2;
            }
            assert(constructPlugins(k,object.data()) && !tree(object.data(),offset));
            auto valid=fixture();
            auto alloc=rw::Engine::memfuncs.rwmalloc;rw::Engine::memfuncs.rwmalloc=failMalloc;
            assert(!read(k,valid,object.data()) && !tree(object.data(),offset));
            rw::Engine::memfuncs.rwmalloc=alloc;
            for(auto edit:std::vector<std::pair<unsigned,uint32_t>>{
                {0,0x35000},{4,45},{8,67},{20,55},{28,2},{56,3},{60,65536},
                {64,0x0000ff00}, // node cycle
                {64,0x00020100}, // leaf beyond entries
                {80,0x00000002}, // remap past actual triangles
                {32,0x7fc00000}}) {
                auto bad=valid;word(bad,edit.first,edit.second);
                assert(!read(k,bad,object.data()) && !tree(object.data(),offset));
            }
            auto truncated=valid;truncated.pop_back();assert(!read(k,truncated,object.data()));
            assert(read(k,valid,object.data()));auto* t=tree(object.data(),offset);assert(t);
            assert(t->flags==1 && t->bounds.sup.x==1 && t->bounds.inf.z==-1);
            assert(t->numEntries==2 && t->numNodes==1 && t->remap[0]==1 && t->remap[1]==0);
            assert(t->nodes[1]==1 && t->nodes[9]==1 && t->nodes[10]==1);
            assert(!read(k,valid,object.data()) && tree(object.data(),offset)==t);
            destructPlugins(k,object.data());assert(!tree(object.data(),offset));
            // One implicit leaf, with identity triangle ordering and no remap.
            auto single=valid;single.resize(64);word(single,8,48);word(single,20,36);
            word(single,28,0);word(single,56,2);word(single,60,0);
            assert(read(k,single,object.data()));t=tree(object.data(),offset);
            assert(t && !t->nodes && !t->remap && t->numEntries==2);
            destructPlugins(k,object.data());assert(!tree(object.data(),offset));
        }
        assert(live.empty());rw::Engine::memfuncs=savedMemory;
        assert(stop() && closeEngine() && term());
    }
    std::puts("rw collision: registration, tree/remap, malformed inputs, ownership and restart passed");
}
