// Transactional stream boundary over the unmodified MIT librw reader.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "object.h"
#include "plugins.h"
#include "animation.h"
#include "startup.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace csf_rw {
// Transaction-private seam: custom game texture readers can leave temporary
// Image facades behind when an engine allocation aborts their C-style code.
std::vector<Image*> streamImageCheckpoint();
void streamImageRollback(const std::vector<Image*>&);
}
namespace {
using namespace csf_rw;
using K=Registry;
struct ReadFailure {};
void require(bool b,int line=__builtin_LINE()) {
    if(!b) {
        if(startupTracing()) std::fprintf(stderr,"RW ClumpStreamRead rejected input/callback at clump.cpp:%d\n",line);
        throw ReadFailure{};
    }
}
// Unlike StreamMemory, this never copies a partial word, wraps position, or
// turns a failed seek into a seemingly successful findChunk. It is also used
// to bound each plugin callback to its own payload.
struct Input final:rw::Stream {
    const uint8_t* data; uint32_t length,position=0;
    Input(const uint8_t* p,uint32_t n):data(p),length(n) {}
    rw::uint32 read8(void* p,rw::uint32 n) override {
        require(n<=length-position && (!n || p));
        if(n) std::memcpy(p,data+position,n); position+=n; return n;
    }
    rw::uint32 write8(const void*,rw::uint32) override { throw ReadFailure{}; }
    void seek(rw::int32 off,rw::int32 whence=1) override {
        int64_t p=(whence==0?0:whence==1?position:length)+int64_t(off);
        require(p>=0 && p<=length); position=uint32_t(p);
    }
    rw::uint32 tell() override { return position; }
    // Match stdio EOF: a successful exact-length read has not failed. librw
    // checks eof() AFTER reading a chunk header, so position==length rejects
    // a valid trailing empty extension (and enters its unsafe failgeo path).
    // All overreads throw above; successful root consumption is checked below.
    bool eof() override { return false; }
};
uint32_t word(const uint8_t* p) { uint32_t n; std::memcpy(&n,p,4); return n; }
struct Chunk { uint32_t id,version; const uint8_t* p; uint32_t size; };
std::vector<Chunk> chunks(const uint8_t* p,uint32_t n) {
    std::vector<Chunk> out;
    while(n) {
        require(n>=12); uint32_t size=word(p+4); require(size<=n-12);
        out.push_back({word(p),word(p+8),p+12,size}); p+=12+size; n-=12+size;
    }
    return out;
}
uint32_t version(uint32_t stamp) {
    return stamp&0xffff0000 ? ((stamp>>14)&0x3ff00)+0x30000 | ((stamp>>16)&0x3f) : stamp<<8;
}
// librw's reader trusts file-supplied counts/indices (and a 12-byte stack
// struct). Validate the graph's structural envelope before invoking it. Plugin
// payloads remain the registered decoder's responsibility, within a bounded IO.
struct Shape {
    int frames=0,geometries=0;
    void check(const Chunk& c,int depth=0) {
        require(depth<32);
        if(c.id==rw::ID_EXTENSION) { chunks(c.p,c.size); return; }
        bool complex=c.id==rw::ID_FRAMELIST || c.id==rw::ID_GEOMETRYLIST ||
            c.id==rw::ID_GEOMETRY || c.id==rw::ID_MATLIST || c.id==rw::ID_MATERIAL ||
            c.id==rw::ID_TEXTURE || c.id==rw::ID_ATOMIC || c.id==rw::ID_LIGHT || c.id==rw::ID_CAMERA;
        if(!complex) return;
        auto list=chunks(c.p,c.size); require(!list.empty() && list[0].id==rw::ID_STRUCT);
        auto& s=list[0]; uint32_t v=version(s.version);
        if(c.id==rw::ID_FRAMELIST) {
            require(s.size>=4); frames=int(word(s.p));
            require(frames>0 && frames<=65536 && s.size==4+uint64_t(frames)*56);
            for(int i=0;i<frames;++i) {
                int parent=int(word(s.p+4+i*56+48));
                require(i==0?parent==-1:parent>=0 && parent<i);
            }
            require(list.size()==size_t(frames)+1);
            for(size_t i=1;i<list.size();++i) require(list[i].id==rw::ID_EXTENSION);
        } else if(c.id==rw::ID_GEOMETRYLIST) {
            require(s.size==4); geometries=int(word(s.p));
            require(geometries>=0 && geometries<=65536 && list.size()==size_t(geometries)+1);
        } else if(c.id==rw::ID_ATOMIC) {
            require(v>=0x35000 && v<=0x37002 && s.size==16);
            require(word(s.p)<unsigned(frames) && word(s.p+4)<unsigned(geometries));
        } else if(c.id==rw::ID_GEOMETRY) {
            require(s.size>=16 && v>=0x35000 && v<=0x37002);
            uint32_t flags=word(s.p),tris=word(s.p+4),verts=word(s.p+8),morphs=word(s.p+12);
            require(tris<=1000000 && verts<=65535 && morphs>=1 && morphs<=64 && ((flags>>16)&255)<=8);
            // Avoid native morph-target writes through null normal/vertex data.
            uint64_t at=16;
            if(!(flags&rw::Geometry::NATIVE)) {
                unsigned uv=(flags>>16)&255;
                if(!uv) uv=flags&rw::Geometry::TEXTURED?1:flags&rw::Geometry::TEXTURED2?2:0;
                at+=uint64_t(verts)*(flags&rw::Geometry::PRELIT?4:0)+uint64_t(verts)*uv*8+uint64_t(tris)*8;
            }
            for(uint32_t i=0;i<morphs;++i) {
                require(at+24<=s.size);
                bool vertices=word(s.p+at+16)!=0,normals=word(s.p+at+20)!=0;
                require(!normals || (flags&rw::Geometry::NORMALS));
                require(!(flags&rw::Geometry::NATIVE) || (!vertices && !normals));
                at+=24+uint64_t(verts)*12*(unsigned(vertices)+unsigned(normals));
            }
            require(at==s.size);
        } else if(c.id==rw::ID_MATLIST) {
            require(s.size>=4); int count=int(word(s.p));
            require(count>=0 && count<=65536 && s.size==4+uint64_t(count)*4);
            for(int i=0;i<count;++i) { int index=int(word(s.p+4+4*i)); require(index<0 || index<i); }
        } else if(c.id==rw::ID_MATERIAL) require(s.size==28);
        else if(c.id==rw::ID_LIGHT) require(s.size==24);
        else if(c.id==rw::ID_CAMERA) require(s.size==32);
        else if(c.id==rw::ID_TEXTURE) {
            require(s.size==4 && list.size()>=4);
            for(int i=1;i<=2;++i) require(list[i].id==rw::ID_STRING && list[i].size<=32 &&
                list[i].size>0 && std::memchr(list[i].p,0,list[i].size));
        }
        if(c.id!=rw::ID_GEOMETRYLIST && c.id!=rw::ID_FRAMELIST && c.id!=rw::ID_MATLIST)
            require(list.back().id==rw::ID_EXTENSION);
        for(size_t i=1;i<list.size();++i) check(list[i],depth+1);
    }
    void root(const std::vector<uint8_t>& bytes) {
        auto list=chunks(bytes.data(),uint32_t(bytes.size()));
        require(list.size()>=4 && list[0].id==rw::ID_STRUCT && list[0].size==12);
        uint32_t v=version(list[0].version); require(v>=0x35000 && v<=0x37002);
        require(list[1].id==rw::ID_FRAMELIST && list[2].id==rw::ID_GEOMETRYLIST && list.back().id==rw::ID_EXTENSION);
        for(auto& c:list) check(c);
        size_t at=3;
        for(unsigned type:{rw::ID_ATOMIC,rw::ID_LIGHT,rw::ID_CAMERA}) {
            uint32_t count=word(list[0].p+(type==rw::ID_ATOMIC?0:type==rw::ID_LIGHT?4:8));
            require(count<=65536);
            for(uint32_t i=0;i<count;++i) {
                if(type!=rw::ID_ATOMIC) {
                    require(at<list.size() && list[at].id==rw::ID_STRUCT && list[at].size==4 &&
                            word(list[at].p)<unsigned(frames)); ++at;
                }
                require(at<list.size() && list[at++].id==type);
            }
        }
        require(at+1==list.size());
    }
};

rw::PluginList& nativePlugins(K k) {
    switch(k) {
    case K::Frame:return rw::Frame::s_plglist;
    case K::Atomic:return rw::Atomic::s_plglist;
    case K::Geometry:return rw::Geometry::s_plglist;
    case K::Material:return rw::Material::s_plglist;
    case K::Light:return rw::Light::s_plglist;
    case K::Camera:return rw::Camera::s_plglist;
    case K::Clump:return rw::Clump::s_plglist;
    case K::Texture:return rw::Texture::s_plglist;
    case K::Dictionary:return rw::TexDictionary::s_plglist;
    default:return rw::Raster::s_plglist;
    }
}
struct Transaction;
// The existing boundary/engine registries are single-threaded. Reentrant clump
// reads are explicitly rejected; callbacks may still read their payload streams.
Transaction* active=nullptr;
void* observerCreate(void*,int,int);
void* observerDestroy(void*,int,int);
rw::Stream* payload(rw::Stream*,int,void*,int,int);
int sizePayload(void*,int,int);
rw::Stream* writePayload(rw::Stream*,int,void*,int,int);
void rights(void*,int,int,uint32_t);
void completed(void*,int,int);
void* noop(void* p,int,int) { return p; }
void* noopCopy(void* p,void*,int,int) { return p; }
struct Adapter {
    K kind;
    rw::PluginList& list;
    rw::LinkList saved;
    std::vector<std::unique_ptr<rw::Plugin>> nodes;
    struct Reader { uint32_t id; rw::StreamRead native; int offset,size; bool game;
        rw::StreamGetSize getSize; rw::RightsCallback rights; rw::StreamWrite write; };
    std::vector<Reader> readers;
    bool installed=false;
    explicit Adapter(K k):kind(k),list(nativePlugins(k)) { saved.init(); }
    void add(rw::Plugin p) { nodes.emplace_back(new rw::Plugin(p)); }
    void prepare(bool sizing=false) {
        rw::Plugin p{}; p.offset=int(kind); p.constructor=noop; p.destructor=sizing?noop:observerDestroy;
        p.copy=noopCopy;
        add(p);
        auto game=pluginStreamIDs(kind,!sizing);
        FORLIST(l,list.plugins) {
            auto* original=LLLinkGetData(l,rw::Plugin,inParentList);
            bool isGame=std::find(game.begin(),game.end(),original->id)!=game.end();
            if(original->read || (sizing && original->getSize) || isGame) {
                readers.push_back({original->id,original->read,original->offset,original->size,isGame,original->getSize,original->rightsCallback,original->write});
                auto bridge=p; bridge.offset=int(readers.size()-1); bridge.size=int(kind);
                bridge.id=original->id; bridge.destructor=noop; bridge.read=payload;
                if(original->rightsCallback) bridge.rightsCallback=rights;
                if(sizing) { bridge.getSize=sizePayload; bridge.write=writePayload; } add(bridge);
                game.erase(std::remove(game.begin(),game.end(),original->id),game.end());
            }
        }
        for(auto id:game) {
            readers.push_back({id,nullptr,0,0,true,nullptr,nullptr,nullptr}); auto bridge=p;
            bridge.offset=int(readers.size()-1); bridge.size=int(kind); bridge.id=id;
            bridge.destructor=noop; bridge.read=payload;
            if(sizing) { bridge.getSize=sizePayload; bridge.write=writePayload; } add(bridge);
        }
        FORLIST(l,list.plugins) {
            auto copy=*LLLinkGetData(l,rw::Plugin,inParentList); copy.read=nullptr;
            if(sizing) { copy.getSize=nullptr; copy.write=nullptr; } add(copy);
        }
        p.destructor=noop; p.constructor=sizing?noop:observerCreate;
        p.alwaysCallback=sizing?nullptr:completed; add(p);
    }
    static void move(rw::LinkList& to,rw::LinkList& from) {
        to.init(); if(from.isEmpty()) return;
        to.link.next=from.link.next; to.link.prev=from.link.prev;
        to.link.next->prev=to.link.prev->next=&to.link; from.init();
    }
    void install() {
        move(saved,list.plugins);
        for(auto& n:nodes) list.plugins.append(&n->inParentList);
        installed=true;
    }
    ~Adapter() { if(installed) { list.plugins.init(); move(list.plugins,saved); } }
};
struct Transaction {
    rw::MemoryFunctions memory=rw::Engine::memfuncs;
    std::unordered_set<void*> allocations;
    std::unordered_map<void*,K> live;
    struct UserBefore { K kind; int offset,size; rw::UserDataExtension value; rw::Destructor destructor; };
    std::unordered_map<void*,UserBefore> userBefore;
    std::vector<Image*> imagesBefore=streamImageCheckpoint();
    std::vector<std::pair<void*,K>> garbage;
    std::vector<std::unique_ptr<Adapter>> adapters;
    bool committed=false,cleaning=false;
    int counters[8]={rw::Clump::numAllocated,rw::Frame::numAllocated,rw::Atomic::numAllocated,
        rw::Geometry::numAllocated,rw::Material::numAllocated,rw::Light::numAllocated,
        rw::Camera::numAllocated,rw::Texture::numAllocated};
    Adapter& adapter(K k) { for(auto& a:adapters) if(a->kind==k) return *a; throw ReadFailure{}; }
    static void* alloc(size_t n,rw::uint32 h) {
        auto* p=active->memory.rwmalloc(n,h);
        if(!p) {
            if(!n) return nullptr; // librw's default allocator deliberately does this
            if(startupTracing()) std::fprintf(stderr,"RW clump allocation failed: %zu bytes, hint %x\n",n,unsigned(h));
            throw std::bad_alloc();
        }
        try { active->allocations.insert(p); } catch(...) { active->memory.rwfree(p); throw; } return p;
    }
    static void release(void* p) { active->allocations.erase(p); active->memory.rwfree(p); }
    static void* resize(void* p,size_t n,rw::uint32 h) {
        auto* q=active->memory.rwrealloc(p,n,h); if(!q && n) throw std::bad_alloc();
        active->allocations.erase(p); if(q) active->allocations.insert(q); return q;
    }
    Transaction() {
        for(K k:{K::Frame,K::Atomic,K::Geometry,K::Material,K::Light,K::Camera,K::Clump,K::Texture,K::Raster,K::Dictionary}) {
            adapters.emplace_back(new Adapter(k)); adapters.back()->prepare();
        }
        // No fallible operations after installing the temporary dispatch lists.
        for(auto& a:adapters) a->install();
        active=this;
        rw::Engine::memfuncs={alloc,resize,release,alloc,resize};
    }
    void discardGarbage() {
        for(auto& p:garbage) if(p.second==K::Frame) std::free(p.first); else release(p.first);
        garbage.clear();
    }
    void destroy(K k,void* p) {
        switch(k) {
        case K::Clump:static_cast<rw::Clump*>(p)->destroy();break;
        case K::Atomic:static_cast<rw::Atomic*>(p)->destroy();break;
        case K::Geometry:static_cast<rw::Geometry*>(p)->destroy();break;
        case K::Material:static_cast<rw::Material*>(p)->destroy();break;
        case K::Light:static_cast<rw::Light*>(p)->destroy();break;
        case K::Camera:static_cast<rw::Camera*>(p)->destroy();break;
        case K::Frame:static_cast<rw::Frame*>(p)->destroyHierarchy();break;
        case K::Texture:static_cast<rw::Texture*>(p)->destroy();break;
        case K::Dictionary:static_cast<rw::TexDictionary*>(p)->destroy();break;
        default:static_cast<rw::Raster*>(p)->destroy();break;
        }
    }
    ~Transaction() {
        cleaning=true;
        if(!committed) {
            // An exception can leave the current object attached to a frame
            // but not yet added to its clump. Destroy these before the clump
            // owns/destroys the frames (Frame::destroyHierarchy doesn't detach
            // such objects in upstream librw).
            for(K k:{K::Atomic,K::Light,K::Camera}) for(;;) {
                void* orphan=nullptr;
                for(auto& p:live) if(p.second==k) {
                    bool attached=k==K::Atomic?static_cast<rw::Atomic*>(p.first)->clump!=nullptr:
                        k==K::Light?static_cast<rw::Light*>(p.first)->clump!=nullptr:
                        static_cast<rw::Camera*>(p.first)->clump!=nullptr;
                    if(!attached) { orphan=p.first; break; }
                }
                if(!orphan) break; destroy(k,orphan);
            }
            // Release roots first, then any objects the failed reader had not
            // linked yet. Refcounted leftovers can include geometry-list holds.
            for(K k:{K::Clump,K::Atomic,K::Light,K::Camera,K::Geometry,K::Material,K::Frame,K::Dictionary,K::Texture,K::Raster}) {
                for(;;) {
                    void* p=nullptr;
                    for(auto& entry:live) if(entry.second==k) {
                        p=entry.first;
                        if(k==K::Frame) while(static_cast<rw::Frame*>(p)->getParent()) p=static_cast<rw::Frame*>(p)->getParent();
                        break;
                    }
                    if(!p) break; destroy(k,p);
                }
            }
        }
        for(auto& p:userBefore) {
            auto& before=p.second;
            auto* current=PLUGINOFFSET(rw::UserDataExtension,p.first,before.offset);
            if(committed) {
                auto fresh=*current; *current=before.value;
                before.destructor(p.first,before.offset,before.size); *current=fresh;
            } else *current=before.value;
            if(auto* a=streamObjectExport(before.kind,p.first)) {
                int off=plugins(before.kind).getPluginOffset(rw::ID_USERDATA);
                if(off>=0) std::memcpy(static_cast<char*>(a)+off,current,sizeof(*current));
            }
        }
        discardGarbage();
        if(!committed) streamImageRollback(imagesBefore);
        streamImageExport(); // includes preexisting dictionary textures' counts
        rw::Engine::memfuncs=memory;
        // If an exception interrupted a native function, its temporary frame /
        // geometry index arrays have no C++ RAII. Only still-live allocations
        // from this transaction are reclaimed, never preexisting textures.
        if(!committed) {
            for(void* p:allocations) memory.rwfree(p);
            rw::Clump::numAllocated=counters[0]; rw::Frame::numAllocated=counters[1];
            rw::Atomic::numAllocated=counters[2]; rw::Geometry::numAllocated=counters[3];
            rw::Material::numAllocated=counters[4]; rw::Light::numAllocated=counters[5];
            rw::Camera::numAllocated=counters[6]; rw::Texture::numAllocated=counters[7];
        }
        active=nullptr;
    }
};
struct Sizes {
    std::vector<std::unique_ptr<Adapter>> adapters;
    uint32_t savedVersion=rw::version;
    uint32_t savedBuild=rw::build;
    Sizes() {
        for(K k:{K::Frame,K::Atomic,K::Geometry,K::Material,K::Light,K::Camera,K::Clump,K::Texture,K::Dictionary}) {
            adapters.emplace_back(new Adapter(k)); adapters.back()->prepare(true);
        }
        for(auto& a:adapters) a->install();
        rw::version=0x37002; // original writer's version, not an original address
        rw::build=55;
    }
    ~Sizes() { rw::version=savedVersion; rw::build=savedBuild; }
};
Sizes* sizes=nullptr;
int sizePayload(void* n,int index,int kind) {
    Adapter* adapter=nullptr;
    for(auto& a:sizes->adapters) if(a->kind==K(kind)) { adapter=a.get(); break; }
    require(adapter); auto& r=adapter->readers.at(index);
    int game=r.game?pluginPayloadSize(K(kind),r.id,streamObjectExport(K(kind),n)):-1;
    if(game>=0) return game;
    return r.getSize?r.getSize(n,r.offset,r.size):0;
}
rw::Stream* writePayload(rw::Stream* s,int length,void* n,int index,int kind) {
    Adapter* adapter=nullptr;
    for(auto& a:sizes->adapters) if(a->kind==K(kind)) { adapter=a.get(); break; }
    require(adapter); auto& r=adapter->readers.at(index);
    uint32_t start=s->tell();
    if(r.game && pluginPayloadSize(K(kind),r.id,streamObjectExport(K(kind),n))>=0) {
        auto* borrowed=streamBorrow(s,true); require(borrowed);
        bool ok=false;
        try { ok=writePluginPayload(K(kind),r.id,borrowed,length,streamObjectExport(K(kind),n)); }
        catch(...) { streamUnborrow(borrowed); throw; }
        streamUnborrow(borrowed); require(ok);
    } else require(r.write && r.write(s,length,n,r.offset,r.size));
    require(s->tell()-start==uint32_t(length)); return s;
}
struct Output final:rw::Stream {
    std::vector<uint8_t> bytes;
    rw::uint32 write8(const void* p,rw::uint32 n) override {
        if(!n) return 0;
        require(n<=512u*1024*1024-bytes.size());
        auto* b=static_cast<const uint8_t*>(p); bytes.insert(bytes.end(),b,b+n); return n;
    }
    rw::uint32 read8(void*,rw::uint32) override { throw ReadFailure{}; }
    void seek(rw::int32,rw::int32=1) override { throw ReadFailure{}; }
    rw::uint32 tell() override { return uint32_t(bytes.size()); }
    bool eof() override { return false; }
};
Clump* __cdecl writeClump(Clump* a,Stream* s) {
    if(active || sizes || !a || !streamNative(s)) return nullptr;
    try {
        objectImport(); auto* n=clumpNative(a); require(n && n->getFrame());
        Sizes context; sizes=&context;
        Output body;
        try {
            // Compose native codecs around a boundary-owned unique geometry
            // table. Calling native Clump::streamWrite would write a shared
            // geometry (and invoke its game callbacks) once per atomic.
            std::vector<rw::Frame*> frames(n->getFrame()->count());
            rw::makeFrameList(n->getFrame(),frames.data());
            rw::FrameList_ fl{int(frames.size()),frames.data()};
            auto frameIndex=[&](rw::Frame* f) {
                auto it=std::find(frames.begin(),frames.end(),f);require(it!=frames.end());return int(it-frames.begin());
            };
            rw::writeChunkHeader(&body,rw::ID_STRUCT,12);
            body.writeI32(n->countAtomics());body.writeI32(n->countLights());body.writeI32(n->countCameras());
            fl.streamWrite(&body);
            std::vector<rw::Geometry*> geometries;
            FORLIST(l,n->atomics) {
                auto* g=rw::Atomic::fromClump(l)->geometry;require(g);
                if(std::find(geometries.begin(),geometries.end(),g)==geometries.end()) geometries.push_back(g);
            }
            Output geos;rw::writeChunkHeader(&geos,rw::ID_STRUCT,4);geos.writeI32(geometries.size());
            for(auto* g:geometries) require(g->streamWrite(&geos));
            rw::writeChunkHeader(&body,rw::ID_GEOMETRYLIST,geos.bytes.size());body.write8(geos.bytes.data(),geos.bytes.size());
            FORLIST(l,n->atomics) {
                auto* at=rw::Atomic::fromClump(l);
                int32_t data[]={frameIndex(at->getFrame()),int32_t(std::find(geometries.begin(),geometries.end(),at->geometry)-geometries.begin()),at->object.object.flags,0};
                rw::writeChunkHeader(&body,rw::ID_ATOMIC,40+rw::Atomic::s_plglist.streamGetSize(at));
                rw::writeChunkHeader(&body,rw::ID_STRUCT,16);body.write8(data,16);rw::Atomic::s_plglist.streamWrite(&body,at);
            }
            FORLIST(l,n->lights) {
                auto* light=rw::Light::fromClump(l);rw::writeChunkHeader(&body,rw::ID_STRUCT,4);body.writeI32(frameIndex(light->getFrame()));require(light->streamWrite(&body));
            }
            FORLIST(l,n->cameras) {
                auto* camera=rw::Camera::fromClump(l);rw::writeChunkHeader(&body,rw::ID_STRUCT,4);body.writeI32(frameIndex(camera->getFrame()));require(camera->streamWrite(&body));
            }
            rw::Clump::s_plglist.streamWrite(&body,n);
        } catch(...) { sizes=nullptr;throw; }
        uint32_t h[]={rw::ID_CLUMP,uint32_t(body.bytes.size()),0x1c020037};
        require(streamPrepareWrite(s,12+body.bytes.size())); auto* target=streamNative(s);
        require(target->write8(h,12)==12 && target->write8(body.bytes.data(),body.bytes.size())==body.bytes.size());sizes=nullptr;return a;
    } catch(...) { sizes=nullptr;return nullptr; }
}
uint32_t __cdecl clumpSize(Clump* a) {
    if(active || sizes || !a || rw::Engine::state!=rw::Engine::Started) return 0;
    try {
        objectImport(); auto* n=clumpNative(a); require(n && n->getFrame());
        Sizes context; sizes=&context;
        try {
            FORLIST(l,n->atomics) require(rw::Atomic::fromClump(l)->geometry);
            uint32_t result=n->streamGetSize();
            // Original 58F860 makes a unique geometry list. librw counts the
            // geometry once per atomic; remove duplicates for original GetSize.
            std::unordered_set<rw::Geometry*> seen;
            FORLIST(l,n->atomics) {
                auto* g=rw::Atomic::fromClump(l)->geometry; require(g);
                if(!seen.insert(g).second) result-=12+g->streamGetSize();
            }
            sizes=nullptr; return result;
        } catch(...) { sizes=nullptr; throw; }
    } catch(...) { return 0; }
}
void publish(K k,void* n) {
    void* a=streamObjectExport(k,n); require(a || k==K::Raster);
    if(a) animationPublish(k,a,n);
    int off=plugins(k).getPluginOffset(rw::ID_USERDATA);
    int nativeOffset=-1;
    FORLIST(l,active->adapter(k).saved) {
        auto* p=LLLinkGetData(l,rw::Plugin,inParentList);
        if(p->id==rw::ID_USERDATA) { nativeOffset=p->offset; break; }
    }
    if(a && off>=0 && nativeOffset>=0) {
        // This extension contains POD array headers, not native RW objects.
        auto* e=PLUGINOFFSET(rw::UserDataExtension,n,nativeOffset);
        std::memcpy(static_cast<char*>(a)+off,e,sizeof(*e));
    }
}
void* observerCreate(void* n,int kind,int) {
    auto k=K(kind); active->live.emplace(n,k);
    if(k==K::Light) static_cast<rw::Light*>(n)->minusCosAngle=0;
    if(k!=K::Raster) require(streamObjectCreate(k,n)!=nullptr);
    return n;
}
void* observerDestroy(void* n,int kind,int) {
    auto k=K(kind);
    if(auto* a=streamObjectForget(k,n)) active->garbage.emplace_back(a,k);
    active->live.erase(n); return n;
}
void completed(void* n,int kind,int) { if(!active->cleaning) publish(K(kind),n); }
void rights(void* n,int index,int kind,uint32_t data) {
    auto& r=active->adapter(K(kind)).readers.at(index);
    if(r.rights) r.rights(n,r.offset,r.size,data);
}
rw::Stream* payload(rw::Stream* stream,int length,void* n,int index,int kind) {
    require(length>=0); auto& r=active->adapter(K(kind)).readers.at(index);
    std::vector<uint8_t> bytes(length); if(length) stream->read8(bytes.data(),length);
    if(r.native) {
        Input input(bytes.data(),length);
        auto* user=r.id==rw::ID_USERDATA?PLUGINOFFSET(rw::UserDataExtension,n,r.offset):nullptr;
        rw::UserDataExtension saved{}; if(user) saved=*user;
        if(user && !active->live.count(n) && !active->userBefore.count(n)) {
            FORLIST(l,active->adapter(K(kind)).saved) {
                auto* p=LLLinkGetData(l,rw::Plugin,inParentList);
                if(p->id==rw::ID_USERDATA) {
                    active->userBefore.emplace(n,Transaction::UserBefore{K(kind),r.offset,r.size,saved,p->destructor});break;
                }
            }
        }
        try { require(r.native(&input,length,n,r.offset,r.size)!=nullptr); }
        catch(...) {
            // Upstream's UserData reader publishes numArrays before allocating
            // or initializing the array. Its destructor cannot traverse a
            // half-read string array; the allocation journal reclaims those
            // partial allocations, while the previous owning header is valid.
            if(user) *user=saved;
            stream->seek(int(input.tell())-length);
            throw;
        }
        auto prior=active->userBefore.find(n);
        bool retained=prior!=active->userBefore.end() && prior->second.value.arrays==saved.arrays;
        if(user && saved.arrays && !retained) {
            // Texture::read can return an already mapped/shared texture. Do
            // not leak its previous UserData allocation when another streamed
            // reference replaces the extension. Preserve it until decode has
            // succeeded, so a short payload still leaves the old owner valid.
            auto fresh=*user; *user=saved;
            FORLIST(l,active->adapter(K(kind)).saved) {
                auto* p=LLLinkGetData(l,rw::Plugin,inParentList);
                if(p->id==rw::ID_USERDATA) { p->destructor(n,p->offset,p->size); break; }
            }
            *user=fresh;
        }
    }
    publish(K(kind),n);
    if(r.game) {
        Input input(bytes.data(),length); auto* s=streamBorrow(&input);
        try {
            require(readPluginPayload(K(kind),r.id,s,length,streamObjectExport(K(kind),n)));
            require(input.position==input.length);
        } catch(...) {
            streamUnborrow(s); stream->seek(int(input.tell())-length); throw;
        }
        streamUnborrow(s);
    }
    return stream;
}
Clump* __cdecl readClump(Stream* s) {
    if(active || sizes || rw::Engine::state!=rw::Engine::Started) return nullptr;
    auto* source=streamNative(s); if(!source) return nullptr;
    try {
        // Caller has already consumed CLUMP's header, exactly as in the game.
        // Buffer through the root extension, preserving the parent's next chunk.
        std::vector<uint8_t> bytes;
        for(;;) {
            uint32_t h[3]; require(source->read8(h,12)==12);
            require(h[1]<=256u*1024*1024 && bytes.size()+12+h[1]<=512u*1024*1024);
            size_t pos=bytes.size(); bytes.resize(pos+12+h[1]);
            std::memcpy(bytes.data()+pos,h,12);
            if(h[1]) require(source->read8(bytes.data()+pos+12,h[1])==h[1]);
            if(h[0]==rw::ID_EXTENSION) break;
        }
        Shape{}.root(bytes);
        Transaction transaction;
        Input input(bytes.data(),uint32_t(bytes.size()));
        try {
            auto* n=rw::Clump::streamRead(&input); require(n && input.position==input.length);
            n->getFrame()->setHierarchyRoot(n->getFrame()); frameExport();
            // Refresh counts after the native geometry-list references are dropped.
            for(auto& p:transaction.live) if(p.second!=K::Raster) publish(p.second,p.first);
            auto* a=static_cast<Clump*>(streamObjectExport(K::Clump,n)); require(a);
            transaction.committed=true; return a;
        } catch(...) {
            source->seek(-int(bytes.size()-input.tell())); throw;
        }
    } catch(const std::exception& e) {
        if(startupTracing()) std::fprintf(stderr,"RW ClumpStreamRead aborted: %s\n",e.what());
        return nullptr;
    } catch(...) { return nullptr; }
}
void checkNativeTexture(const Chunk& c) {
    require(version(c.version)>=0x35000 && version(c.version)<=0x37002);
    auto list=chunks(c.p,c.size);
    require(list.size()==2 && list[0].id==rw::ID_STRUCT && list[1].id==rw::ID_EXTENSION);
    auto& st=list[0]; require(st.size>=88 && word(st.p)==rw::PLATFORM_D3D9);
    require(version(st.version)>=0x35000 && version(st.version)<=0x37002);
    auto* p=st.p;
    unsigned width=p[80]|unsigned(p[81])<<8,height=p[82]|unsigned(p[83])<<8;
    unsigned depth=p[84],levels=p[85],type=p[86],flags=p[87];
    uint32_t format=word(p+72),d3d=word(p+76);
    require(width && height && width<=16384 && height<=16384 && type==rw::Raster::TEXTURE && !(flags&2));
    require(levels>0 && levels<=unsigned(rw::Raster::calculateNumLevels(width,height)));
    require(levels==1 || (format&rw::Raster::MIPMAP));
    unsigned block=0;
    if(flags&8) {
        require(d3d>=0x31545844 && d3d<=0x35545844 && (d3d&0xffffff)==0x545844);
        block=d3d==0x31545844?8:16;
    } else {
        unsigned f=format&0xf00;
        bool palette=(format&(rw::Raster::PAL4|rw::Raster::PAL8))!=0;
        require(palette?depth==8:
            ((f==0x100 || f==0x200 || f==0x300 || f==0xa00)?depth==16:
             f==0x500?depth==32:f==0x600?depth==24:f==0x400 && depth==8));
    }
    uint64_t at=88;
    if(format&rw::Raster::PAL4) at+=128;
    else if(format&rw::Raster::PAL8) at+=1024;
    for(unsigned i=0;i<levels;++i) {
        require(at+4<=st.size); unsigned length=word(p+at); at+=4;
        unsigned w=std::max(1u,width>>i),h=std::max(1u,height>>i);
        uint64_t expected=block?uint64_t((w+3)/4)*((h+3)/4)*block:uint64_t(w)*h*(depth/8);
        require(length==expected && at+length<=st.size); at+=length;
    }
    require(at==st.size); chunks(list[1].p,list[1].size);
}
bool readAll(rw::Stream* s,void* out,uint32_t length) {
    uint32_t done=0;
    while(done<length) {
        uint32_t n=s->read8(static_cast<uint8_t*>(out)+done,length-done);
        if(!n || n>length-done) return false;
        done+=n;
    }
    return true;
}
TexDictionary* __cdecl readDictionary(Stream* s) {
    if(active || sizes || rw::Engine::state!=rw::Engine::Started) return nullptr;
    auto* source=streamNative(s); if(!source) return nullptr;
    try {
        std::vector<uint8_t> bytes;
        for(;;) {
            uint32_t h[3]; require(readAll(source,h,12));
            require(h[1]<=256u*1024*1024 && bytes.size()+12+h[1]<=512u*1024*1024);
            size_t at=bytes.size(); bytes.resize(at+12+h[1]); std::memcpy(bytes.data()+at,h,12);
            if(h[1]) require(readAll(source,bytes.data()+at+12,h[1]));
            if(h[0]==rw::ID_EXTENSION) break;
        }
        auto list=chunks(bytes.data(),bytes.size());
        require(list.size()>=2 && list[0].id==rw::ID_STRUCT && list[0].size==4);
        require(version(list[0].version)>=0x35000 && version(list[0].version)<=0x37002);
        uint32_t header=word(list[0].p),count=header&65535;
        require((header>>16)==0 || (header>>16)==2);
        require(list.size()==size_t(count)+2 && list.back().id==rw::ID_EXTENSION);
        chunks(list.back().p,list.back().size);
        for(unsigned i=0;i<count;++i) { require(list[i+1].id==rw::ID_TEXTURENATIVE); checkNativeTexture(list[i+1]); }
        Transaction transaction; Input input(bytes.data(),bytes.size());
        // Native TexDictionary::streamRead treats the 16-bit count as signed;
        // the executable's count is unsigned. Compose the native texture and
        // plugin codecs so valid dictionaries above 32767 are not truncated.
        input.seek(16,0);auto* n=rw::TexDictionary::create();require(n);
        for(uint32_t i=0;i<count;++i) {
            require(rw::findChunk(&input,rw::ID_TEXTURENATIVE,nullptr,nullptr));
            auto* t=rw::Texture::streamReadNative(&input);require(t);
            require(rw::Texture::s_plglist.streamRead(&input,t));n->add(t);
        }
        require(rw::TexDictionary::s_plglist.streamRead(&input,n) && input.tell()==bytes.size());
        streamImageExport(); auto* a=dictionaryFacade(n); require(a);
        transaction.committed=true; return a;
    } catch(...) { return nullptr; }
}
TexDictionary* __cdecl writeDictionary(TexDictionary* a,Stream* s) {
    if(active || sizes || !a || !streamNative(s)) return nullptr;
    try {
        auto* n=dictionaryNative(a); require(n && n->count()<65536);
        Sizes context; sizes=&context; Output out;
        try { n->streamWrite(&out); } catch(...) { sizes=nullptr; throw; }
        require(streamPrepareWrite(s,out.bytes.size()));
        require(streamNative(s)->write8(out.bytes.data(),out.bytes.size())==out.bytes.size());sizes=nullptr;return a;
    } catch(...) { sizes=nullptr;return nullptr; }
}
}
namespace csf_rw {
int __cdecl readNativeTextureABI(Stream* s,Texture** out,int length) {
    if(out) *out=nullptr;
    if(!out || active || sizes || length<12 || !streamNative(s)) return 0;
    try {
        auto* source=streamNative(s);uint32_t h[3];require(readAll(source,h,12) && h[0]==rw::ID_STRUCT);
        require(h[1]<=uint32_t(length-12) && h[1]<=256u*1024*1024);
        std::vector<uint8_t> bytes(24+h[1]);std::memcpy(bytes.data(),h,12);
        require(readAll(source,bytes.data()+12,h[1]));
        uint32_t ext[]={rw::ID_EXTENSION,0,h[2]};std::memcpy(bytes.data()+12+h[1],ext,12);
        checkNativeTexture({rw::ID_TEXTURENATIVE,h[2],bytes.data(),uint32_t(bytes.size())});
        Transaction transaction;Input input(bytes.data(),bytes.size());
        auto* n=rw::Texture::streamReadNative(&input);require(n);
        streamImageExport(); // adopt completed rasters while rollback can run
        *out=textureFacade(n);require(*out);transaction.committed=true;return 1;
    } catch(...) { return 0; }
}
bool readImageRegistry(Registry k,Stream* s,void* object) {
    if(active || sizes || !object || !streamNative(s)) return false;
    try {
        auto* source=streamNative(s);uint32_t length=0,v=0;
        require(rw::findChunk(source,rw::ID_EXTENSION,&length,&v) && v>=0x35000 && v<=0x37002 && length<=256u*1024*1024);
        std::vector<uint8_t> bytes(12+length);uint32_t h[]={rw::ID_EXTENSION,length,0x1c020037};std::memcpy(bytes.data(),h,12);
        require(readAll(source,bytes.data()+12,length));chunks(bytes.data()+12,length);
        void* n=k==K::Dictionary?static_cast<void*>(dictionaryNative(static_cast<TexDictionary*>(object))):static_cast<void*>(textureNative(static_cast<Texture*>(object)));
        require(n);Transaction transaction;Input input(bytes.data(),bytes.size());
        require(nativePlugins(k).streamRead(&input,n) && input.tell()==bytes.size());
        transaction.committed=true;return true;
    } catch(...) { return false; }
}
bool streamWorldMaterials(const uint8_t* bytes,uint32_t length,std::vector<Material*>& out,uint32_t* cursor) {
    if(active || sizes || !out.empty()) return false;
    try {
        Shape shape;
        shape.check({rw::ID_MATLIST,0,bytes,length});
        Transaction transaction;
        // librw checks eof() AFTER a chunk header. A final zero-length material
        // extension is valid; supply a private sentinel, never parent bytes.
        std::vector<uint8_t> bounded(bytes,bytes+length);bounded.resize(size_t(length)+12);
        Input input(bounded.data(),uint32_t(bounded.size()));
        try {
        rw::MaterialList list{};
        require(rw::MaterialList::streamRead(&input,&list) && input.tell()==length);
        std::vector<Material*> result;
        result.reserve(list.numMaterials);
        for(int i=0;i<list.numMaterials;++i) {
            publish(K::Material,list.materials[i]);
            auto* a=static_cast<Material*>(streamObjectExport(K::Material,list.materials[i]));
            require(a); result.push_back(a);
        }
        // Transfer references, not the native pointer array, to the world.
        rw::Engine::memfuncs.rwfree(list.materials);
        out.swap(result); transaction.committed=true; return true;
        } catch(...) { if(cursor) *cursor=input.tell();throw; }
    } catch(...) { return false; }
}
}
HOOK(0x0058FFC0,readClump,"boundary-rw");
HOOK(0x0058FD10,clumpSize,"boundary-rw");
HOOK(0x0058FDF0,writeClump,"boundary-rw");
HOOK(0x00630A20,readDictionary,"boundary-rw");
HOOK(0x006308E0,writeDictionary,"boundary-rw");
#endif
