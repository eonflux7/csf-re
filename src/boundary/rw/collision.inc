// Boundary decoder for the collision extension found in retail CSF assets.
// Contract: plugin 0x11D, version word, COLLTREE(0x2C)/STRUCT, 36-byte
// header, 16-byte branch nodes and optional u16 triangle remap. The game's
// direct readers require the pointer/header layout in collision.h.
using CollisionTree=csf_rw::CollisionTree;
void* collisionCtor(void* object,int offset,int size) { return zero(object,offset,size); }
void* collisionDtor(void* object,int offset,int) {
    CollisionTree* tree;
    std::memcpy(&tree,static_cast<char*>(object)+offset,sizeof(tree));
    if(tree) rw::Engine::memfuncs.rwfree(tree);
    tree=nullptr;std::memcpy(static_cast<char*>(object)+offset,&tree,sizeof(tree));return object;
}
uint32_t collisionWord(const uint8_t* p) { uint32_t v;std::memcpy(&v,p,4);return v; }
uint16_t collisionShort(const uint8_t* p) { uint16_t v;std::memcpy(&v,p,2);return v; }
bool collisionGraph(const uint8_t* nodes,unsigned count,unsigned entries) {
    if(!count) return entries<255; // the implicit root stores its count in one byte
    std::vector<uint8_t> visited(count,0),height(count,0);
    auto visit=[&](auto& self,unsigned index,unsigned depth)->bool {
        if(index>=count || depth>=32 || visited[index]==1) return false;
        if(visited[index]==2) return depth+height[index]<=32;
        visited[index]=1;height[index]=1;
        for(unsigned side:{0u,8u}) {
            auto* child=nodes+index*16+side;
            unsigned first=collisionShort(child+2),n=child[1];
            float split;std::memcpy(&split,child+4,4);
            if((child[0]&12)>8 || !std::isfinite(split)) return false;
            if(n==255) {
                if(!self(self,first,depth+1)) return false;
                height[index]=std::max(unsigned(height[index]),1u+height[first]);
            }
            else if(first+n>entries) return false;
        }
        visited[index]=2;return true;
    };
    return visit(visit,0,0);
}
csf_rw::Stream* __cdecl collisionRead(csf_rw::Stream* stream,int length,void* object,int offset,int size) {
    auto* input=csf_rw::streamNative(stream);
    if(!input || !object || size!=sizeof(void*) || length<64 || length>64+65535*18) return nullptr;
    CollisionTree* old;
    std::memcpy(&old,static_cast<char*>(object)+offset,sizeof(old));
    if(old) return nullptr; // do not overwrite an owned tree on duplicate payloads
    std::vector<uint8_t> bytes(size_t(length),uint8_t(0));
    if(input->read8(bytes.data(),length)!=unsigned(length)) return nullptr;
    auto* p=bytes.data();
    // This implements the modern stream shape present in the mission assets;
    // legacy pre-3.6 tree streams remain unsupported, rather than becoming empty.
    if(collisionWord(p)<0x36001 || collisionWord(p)>0x37002 ||
       collisionWord(p+4)!=44 || collisionWord(p+8)!=unsigned(length-16) ||
       collisionWord(p+16)!=1 || collisionWord(p+20)!=unsigned(length-28)) return nullptr;
    uint32_t flags=collisionWord(p+28),entries=collisionWord(p+56),nodes=collisionWord(p+60);
    uint64_t expected=64+uint64_t(nodes)*16+((flags&1)?uint64_t(entries)*2:0);
    if((flags&~1u) || entries>65535 || nodes>65535 || expected!=unsigned(length)) return nullptr;
    // Stream bounds are inf/sup; direct game readers expect runtime sup/inf.
    rw::BBox box;std::memcpy(&box.inf,p+32,12);std::memcpy(&box.sup,p+44,12);
    for(unsigned axis=0;axis<3;++axis) {
        float hi,lo;std::memcpy(&hi,reinterpret_cast<char*>(&box.sup)+axis*4,4);
        std::memcpy(&lo,reinterpret_cast<char*>(&box.inf)+axis*4,4);
        if(!std::isfinite(hi) || !std::isfinite(lo) || hi<lo) return nullptr;
    }
    unsigned triangles=static_cast<uint8_t*>(object)[0]==8?
        static_cast<csf_rw::Geometry*>(object)->numTriangles:static_cast<csf_rw::Sector*>(object)->numTriangles;
    if(entries>triangles || !collisionGraph(p+64,nodes,entries)) return nullptr;
    if(flags&1) for(unsigned i=0;i<entries;++i)
        if(collisionShort(p+64+nodes*16+i*2)>=triangles) return nullptr;
    size_t dataSize=size_t(length)-64;
    auto* tree=static_cast<CollisionTree*>(rw::Engine::memfuncs.rwmalloc(sizeof(CollisionTree)+dataSize,0));
    if(!tree) return nullptr;
    auto* data=reinterpret_cast<uint8_t*>(tree+1);
    *tree={flags,box,uint16_t(entries),uint16_t(nodes),nodes?data:nullptr,
           flags&1?reinterpret_cast<uint16_t*>(data+nodes*16):nullptr};
    if(dataSize) std::memcpy(data,p+64,dataSize);
    std::memcpy(static_cast<char*>(object)+offset,&tree,sizeof(tree));
    return stream;
}
