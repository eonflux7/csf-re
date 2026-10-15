// Independent BSP envelope decoder, derived from 59FEA0/59F990/59F440.
// This librw checkout has no World::streamRead or native sectors. CPU sector
// arrays are backed by librw Geometry; owning headers always remain private.
struct WorldReadFailure {};
struct WorldReadContext {
    const uint8_t* base=nullptr;int failedPosition=-1;
};
WorldReadContext* worldReadContext=nullptr;
struct WorldReadScope {
    WorldReadContext context;
    WorldReadScope() { worldReadContext=&context; }
    ~WorldReadScope() { worldReadContext=nullptr; }
};
void checkWorld(bool b,int line=__builtin_LINE()) {
    if(!b) {
        if(startupTracing()) std::fprintf(stderr,"RW WorldStreamRead rejected input/callback at world_stream.inc:%d\n",line);
        throw WorldReadFailure{};
    }
}
// Upstream Geometry::create/buildMeshes assume their nested allocations
// succeed. Turn a failed engine allocation into an unwind before unchecked
// writes, and reclaim unfinished native storage without touching librw.
struct SectorAllocations;
SectorAllocations* sectorAllocations=nullptr;
struct SectorAllocations {
    rw::MemoryFunctions saved=rw::Engine::memfuncs;
    std::unordered_set<void*> blocks;
    SectorAllocations* previous=sectorAllocations;
    bool committed=false;
    static void* alloc(size_t n,rw::uint32 h) {
        auto* a=sectorAllocations;auto* p=a->saved.rwmalloc(n?n:1,h);
        checkWorld(p!=nullptr);try { a->blocks.insert(p); } catch(...) { a->saved.rwfree(p);throw; } return p;
    }
    static void free(void* p) {
        auto* a=sectorAllocations;a->blocks.erase(p);a->saved.rwfree(p);
    }
    static void* realloc(void* p,size_t n,rw::uint32 h) {
        auto* a=sectorAllocations;auto* q=a->saved.rwrealloc(p,n?n:1,h);checkWorld(q!=nullptr);
        a->blocks.erase(p);a->blocks.insert(q);return q;
    }
    SectorAllocations() {
        sectorAllocations=this;rw::Engine::memfuncs.rwmalloc=alloc;
        rw::Engine::memfuncs.rwfree=free;rw::Engine::memfuncs.rwrealloc=realloc;
        rw::Engine::memfuncs.rwmustmalloc=alloc;rw::Engine::memfuncs.rwmustrealloc=realloc;
    }
    ~SectorAllocations() {
        rw::Engine::memfuncs=saved;sectorAllocations=previous;
        if(!committed) for(auto* p:blocks) saved.rwfree(p);
    }
};
rw::Geometry* sectorGeometry(int nv,int nt,unsigned flags) {
    SectorAllocations allocations;
    int before=rw::Geometry::numAllocated;
    try {
        auto* g=rw::Geometry::create(nv,nt,flags);checkWorld(g!=nullptr);
        allocations.committed=true;return g;
    } catch(...) { rw::Geometry::numAllocated=before;throw; }
}
struct WorldInput final:rw::Stream {
    const uint8_t* bytes; uint32_t size,pos=0;
    WorldInput(const uint8_t* p,uint32_t n):bytes(p),size(n) {}
    rw::uint32 read8(void* p,rw::uint32 n) override {
        checkWorld(n<=size-pos);if(n) std::memcpy(p,bytes+pos,n);pos+=n;return n;
    }
    rw::uint32 write8(const void*,rw::uint32) override { throw WorldReadFailure{}; }
    void seek(rw::int32 n,rw::int32 whence=1) override {
        int64_t at=(whence==0?0:whence==1?pos:size)+int64_t(n);
        checkWorld(at>=0 && at<=size);pos=uint32_t(at);
    }
    rw::uint32 tell() override { return pos; }
    bool eof() override { return pos==size; }
    uint32_t word() { uint32_t n;read8(&n,4);return n; }
};
struct WorldChunk { uint32_t id,version; const uint8_t* bytes; uint32_t size; };
WorldChunk worldChunk(WorldInput& in) {
    uint32_t id=in.word(),size=in.word(),stamp=in.word();
    uint32_t version=stamp&0xffff0000?(((stamp>>14)&0x3ff00)+0x30000)|((stamp>>16)&0x3f):stamp<<8;
    checkWorld(size<=in.size-in.pos);
    WorldChunk c{id,version,in.bytes+in.pos,size};in.pos+=size;return c;
}
WorldChunk worldExpect(WorldInput& in,uint32_t id) {
    auto c=worldChunk(in);checkWorld(c.id==id && c.version>=0x35000 && c.version<=0x37002);return c;
}
void sectorUserData(const WorldChunk& chunk,void* facade) {
    int offset=plugins(Registry::Sector).getPluginOffset(rw::ID_USERDATA);
    if(offset<0) return;
    rw::Geometry* geometry=nullptr;
    for(auto& w:worlds) for(auto& leaf:w.second.leaves) if(leaf->facade==facade) geometry=leaf->geometry;
    checkWorld(geometry!=nullptr);
    FORLIST(l,rw::Geometry::s_plglist.plugins) {
        auto* plugin=LLLinkGetData(l,rw::Plugin,inParentList);
        if(plugin->id!=rw::ID_USERDATA || !plugin->read) continue;
        auto* extension=PLUGINOFFSET(rw::UserDataExtension,geometry,plugin->offset);
        // Reject repeated owned payloads; don't orphan an earlier allocation.
        checkWorld(extension->numArrays==0 && !extension->arrays);
        auto before=*extension;SectorAllocations allocations;
        WorldInput input(chunk.bytes,chunk.size);
        try {
            checkWorld(plugin->read(&input,int(chunk.size),geometry,plugin->offset,plugin->size)!=nullptr && input.eof());
            std::memcpy(static_cast<char*>(facade)+offset,extension,sizeof(*extension));
            allocations.committed=true;
        } catch(...) { *extension=before;throw; }
        return;
    }
}
void worldExtension(const WorldChunk& c,Registry k,void* facade) {
    checkWorld(c.id==rw::ID_EXTENSION);
    WorldInput in(c.bytes,c.size);
    while(!in.eof()) {
        auto p=worldChunk(in);
        if(k==Registry::Sector && p.id==rw::ID_USERDATA) sectorUserData(p,facade);
        auto ids=pluginStreamIDs(k);
        if(std::find(ids.begin(),ids.end(),p.id)==ids.end()) continue;
        WorldInput payload(p.bytes,p.size);
        auto* stream=streamBorrow(&payload);checkWorld(stream!=nullptr);
        try {
            checkWorld(readPluginPayload(k,p.id,stream,int(p.size),facade) && payload.eof());
        } catch(...) {
            if(worldReadContext) worldReadContext->failedPosition=int(p.bytes-worldReadContext->base)+int(payload.pos);
            streamUnborrow(stream);throw;
        }
        streamUnborrow(stream);
    }
}
Sector* readWorldTree(WorldInput& in,World* w,Record& rec,const rw::BBox& cell,int depth,
                      uint64_t& vertices,uint64_t& triangles,uint32_t& planes) {
    checkWorld(depth<64 && rec.leaves.size()<65536);
    auto chunk=worldChunk(in);checkWorld(chunk.version>=0x35000 && chunk.version<=0x37002);
    WorldInput body(chunk.bytes,chunk.size);
    auto structure=worldExpect(body,rw::ID_STRUCT);
    WorldInput data(structure.bytes,structure.size);
    if(chunk.id==10) {
        checkWorld(structure.size==24 || (structure.size==16 && !(w->flags&0x40000000)));
        uint32_t axis=data.word();float value;data.read8(&value,4);
        uint32_t leftLeaf=data.word(),rightLeaf=data.word();
        float left=value,right=value;
        if(structure.size==24) { data.read8(&left,4);data.read8(&right,4); }
        if(!(w->flags&0x40000000)) left=right=value;
        checkWorld((axis==0 || axis==4 || axis==8) && leftLeaf<=1 && rightLeaf<=1);
        auto node=std::make_unique<Plane>();auto* p=node.get();
        *p={int(axis),value,nullptr,nullptr,left,right};rec.planes.push_back(std::move(node));++planes;
        rw::BBox a=cell,b=cell;
        // Plane type is an offset in V3d, not an axis ordinal.
        std::memcpy(reinterpret_cast<char*>(&a.sup)+axis,&left,4);
        std::memcpy(reinterpret_cast<char*>(&b.inf)+axis,&right,4);
        p->left=readWorldTree(body,w,rec,a,depth+1,vertices,triangles,planes);
        p->right=readWorldTree(body,w,rec,b,depth+1,vertices,triangles,planes);
        checkWorld((p->left->type<0)==bool(leftLeaf) && (p->right->type<0)==bool(rightLeaf) && body.eof());
        return reinterpret_cast<Sector*>(p);
    }
    checkWorld(chunk.id==9 && structure.size>=44);
    uint32_t base=data.word(),nt=data.word(),nv=data.word();
    rw::BBox tight;data.read8(&tight.inf,12);data.read8(&tight.sup,12);
    data.word();data.word(); // collision/padding words
    checkWorld(base<=65535 && nt<=65535 && nv<=65535);
    unsigned uv=(w->flags>>16)&255;
    if(!uv) uv=w->flags&128?2:w->flags&4?1:0;
    checkWorld(uv<=8);
    uint64_t expected=44+uint64_t(nv)*(12+(w->flags&16?4:0)+(w->flags&8?4:0)+uv*8)+nt*8;
    checkWorld(expected==structure.size);
    auto leaf=std::make_unique<Leaf>();auto* state=leaf.get();rec.leaves.push_back(std::move(leaf));
    state->facade=static_cast<Sector*>(rw::Engine::memfuncs.rwmalloc(plugins(Registry::Sector).size,0));
    checkWorld(state->facade!=nullptr);auto* s=state->facade;
    std::memset(s,0,plugins(Registry::Sector).size);s->type=-1;
    s->bounds=cell;s->collisionBounds=tight;s->materialBase=uint16_t(base);
    s->numVertices=uint16_t(nv);s->numTriangles=uint16_t(nt);ring(s->atomics);ring(s->lights);
    unsigned flags=(w->flags&0xff)|rw::Geometry::POSITIONS|(uv<<16);
    state->geometry=sectorGeometry(int(nv),int(nt),flags);
    checkWorld(state->geometry!=nullptr);auto* g=state->geometry;
    state->materials.reserve(rec.materials.size());
    for(auto* m:rec.materials) {
        auto* native=materialNative(m);checkWorld(native!=nullptr);
        checkWorld(g->matList.appendMaterial(native)>=0);
        state->materials.push_back(native);streamObjectExport(Registry::Material,native);
    }
    s->vertices=g->morphTargets[0].vertices;
    state->triangles.resize(nt);s->triangles=nt?state->triangles.data():nullptr;
    if(nv) data.read8(s->vertices,nv*12);
    if(w->flags&16) {
        state->normals.resize(nv*4);data.read8(state->normals.data(),nv*4);s->normals=state->normals.data();
        for(unsigned i=0;i<nv;++i) g->morphTargets[0].normals[i]=rw::makeV3d(
            state->normals[i*4]/128.0f,state->normals[i*4+1]/128.0f,state->normals[i*4+2]/128.0f);
    }
    if(w->flags&8) { s->colors=g->colors;data.read8(s->colors,nv*4); }
    for(unsigned i=0;i<uv;++i) { s->texCoords[i]=g->texCoords[i];data.read8(s->texCoords[i],nv*8); }
    if(nt) data.read8(s->triangles,nt*8);
    for(unsigned i=0;i<nt;++i) {
        auto& t=s->triangles[i];
        if(structure.version<0x36002) { auto old=t;t={{old.v[1],old.v[2],old.matId},old.v[0]}; }
        checkWorld(t.v[0]<nv && t.v[1]<nv && t.v[2]<nv && uint32_t(t.matId)+base<rec.materials.size());
    }
    // The native geometry uses absolute material indices. Sector triangles
    // must retain their original base-relative IDs for direct game readers.
    for(unsigned i=0;i<nt;++i) { g->triangles[i]=s->triangles[i];g->triangles[i].matId+=uint16_t(base); }
    checkWorld(constructPlugins(Registry::Sector,s));state->constructed=true;
    worldExtension(worldExpect(body,rw::ID_EXTENSION),Registry::Sector,s);checkWorld(body.eof());
    vertices+=nv;triangles+=nt;return s;
}
void buildSectorMeshes(Record& rec) {
    for(auto& leaf:rec.leaves) {
        auto* state=leaf.get();auto* s=state->facade;auto* g=state->geometry;
        {
            SectorAllocations allocations;
            try {
                // 59EEF0 publishes an empty mesh header even for a zero-face
                // streamed leaf. Avoid buildMeshes' zero-material memset on a
                // null temporary, while retaining native header ownership.
                if(s->numTriangles) g->buildMeshes();
                else g->allocateMeshes(0,0,0);
                allocations.committed=true;
            }
            catch(...) { g->meshHeader=nullptr;throw; }
        }
        if(auto* h=g->meshHeader) {
            state->meshes.resize(sizeof(MeshHeader)+h->numMeshes*sizeof(Mesh));
            s->mesh=reinterpret_cast<MeshHeader*>(state->meshes.data());
            *s->mesh={h->flags,h->numMeshes,h->serialNum,h->totalIndices,0};
            auto* meshes=reinterpret_cast<Mesh*>(s->mesh+1);
            for(unsigned i=0;i<h->numMeshes;++i) {
                auto& mesh=h->getMeshes()[i];Material* material=nullptr;
                for(auto* m:rec.materials) if(materialNative(m)==mesh.material) { material=m;break; }
                checkWorld(material!=nullptr);meshes[i]={mesh.indices,mesh.numIndices,material};
            }
        }
    }
}
World* __cdecl readWorld(Stream* stream) {
    startupTrace("0x0059FEA0","RpWorldStreamRead (CPU BSP)");
    auto* source=streamNative(stream);
    if(!source || worldReadContext || rw::Engine::state!=rw::Engine::Started) return nullptr;
    WorldReadScope scope;
    std::vector<uint8_t> bytes;
    World* w=nullptr;
    try {
        // The outer WORLD header has already been consumed. Bound all plugin
        // views and leave the next chunk in the parent stream untouched.
        for(;;) {
            checkWorld(!source->eof());
            if(auto* memory=dynamic_cast<rw::StreamMemory*>(source)) checkWorld(memory->position<memory->length);
            uint32_t h[3];checkWorld(source->read8(h,12)==12);
            checkWorld(h[1]<=256u*1024*1024 && bytes.size()+12+h[1]<=512u*1024*1024);
            size_t at=bytes.size();bytes.resize(at+12+h[1]);std::memcpy(bytes.data()+at,h,12);
            checkWorld(!h[1] || source->read8(bytes.data()+at+12,h[1])==h[1]);
            if(h[0]==rw::ID_EXTENSION) break;
        }
        scope.context.base=bytes.data();
        WorldInput in(bytes.data(),uint32_t(bytes.size()));auto head=worldExpect(in,rw::ID_STRUCT);
        checkWorld(head.size==64);WorldInput header(head.bytes,head.size);
        uint32_t rootLeaf=header.word();rw::V3d inverse;header.read8(&inverse,12);
        uint32_t nt=header.word(),nv=header.word(),np=header.word(),nl=header.word(),collision=header.word(),flags=header.word();
        rw::BBox bounds;header.read8(&bounds,24);
        // Native-sector payloads need a platform decoder that librw lacks.
        checkWorld(rootLeaf<=1 && collision==0 && !(flags&0x1000000) && np<65536 && nl<=65536);
        w=static_cast<World*>(rw::Engine::memfuncs.rwmalloc(plugins(Registry::World).size,0));checkWorld(w!=nullptr);
        std::memset(w,0,plugins(Registry::World).size);w->object={7,0,0,0,nullptr};w->flags=flags;
        w->renderOrder=2;w->clumpFrame=(flags>>16)&255;
        if(!w->clumpFrame) w->clumpFrame=flags&128?2:flags&4?1:0;
        w->origin=rw::scale(inverse,-1.0f);w->bounds=bounds;w->sectorRender=emptyRender;
        ring(w->clumps);ring(w->localLights);ring(w->globalLights);w->currentClump=&w->clumps;
        auto* n=rw::World::create(&bounds);checkWorld(n!=nullptr);
        try { Record record{n};record.streamed=true;record.constructed=false;worlds.emplace(w,std::move(record)); }
        catch(...) { n->destroy();throw; }
        auto& rec=worlds.at(w);
        auto materials=worldExpect(in,rw::ID_MATLIST);
        uint32_t materialCursor=0;
        if(!streamWorldMaterials(materials.bytes,materials.size,rec.materials,&materialCursor)) {
            scope.context.failedPosition=int(materials.bytes-bytes.data())+int(materialCursor);checkWorld(false);
        }
        w->materials={rec.materials.data(),int(rec.materials.size()),int(rec.materials.size())};
        uint64_t gotVertices=0,gotTriangles=0;uint32_t gotPlanes=0;
        w->root=readWorldTree(in,w,rec,bounds,0,gotVertices,gotTriangles,gotPlanes);
        checkWorld((w->root->type<0)==bool(rootLeaf) && gotVertices==nv && gotTriangles==nt && gotPlanes==np && rec.leaves.size()==nl);
        checkWorld(constructPlugins(Registry::World,w));rec.constructed=true;
        worldExtension(worldExpect(in,rw::ID_EXTENSION),Registry::World,w);checkWorld(in.eof());
        buildSectorMeshes(rec);
        return w;
    } catch(...) {
        if(w) {
            auto p=worlds.find(w);
            if(p!=worlds.end()) {
                releaseLeaves(p->second);
                if(p->second.constructed) destructPlugins(Registry::World,w);
                disposeLeaves(p->second);
                for(auto* m:p->second.materials) materialDestroy(m);
                p->second.native->destroy();worlds.erase(p);
            }
            rw::Engine::memfuncs.rwfree(w);
        }
        if(scope.context.failedPosition>=0 && size_t(scope.context.failedPosition)<=bytes.size())
            source->seek(scope.context.failedPosition-int(bytes.size()));
        return nullptr;
    }
}
