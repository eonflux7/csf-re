CSF_FUNC(Atomic*,__stdcall,tankAtomicCreate,0x0058F5C0,());
CSF_FUNC(int,__cdecl,tankAtomicDestroy,0x0058F060,(Atomic*));
CSF_FUNC(Atomic*,__cdecl,tankSetGeometry,0x0058EFE0,(Atomic*,Geometry*,uint8_t));
CSF_FUNC(int,__cdecl,tankGeometryDestroy,0x00590FC0,(Geometry*));

struct ClusterSpec { uint32_t flag; int bytes; TankLock TankFormat::*member; };
constexpr ClusterSpec tankClusters[]={
    {1,12,&TankFormat::position},{8,64,&TankFormat::matrix},{16,12,&TankFormat::normal},
    {4,8,&TankFormat::size},{2,4,&TankFormat::color},{64,16,&TankFormat::texCoords},
    {32,4,&TankFormat::rotation},{128,16,&TankFormat::colors},{256,32,&TankFormat::texCoords4}
};
uint32_t tankNormalize(uint32_t f) {
    const uint32_t original=f;
    if((original&8) && (original&0x8000)) f=0x8000;
    for(auto pair:{std::array<uint32_t,2>{32,0x20000},{64,0x40000},{128,0x80000},{256,0x100000}})
        if((original&pair[0]) && (original&pair[1])) f&=pair[1];
    if(original&8) { if(original&1) f&=~1u;if(original&4) f&=~4u;if(original&32) f&=~32u; }
    else if(!(original&1)) f|=1;
    if(!(f&0x30000000)) f|=0x10000000;
    return f;
}
Atomic* __cdecl tankRender(Atomic* a) {
    auto* t=getTank(a);if(!t || !rw::engine || !rw::engine->currentCamera) return nullptr;
    if(t->numParticles<0 || t->numParticles>t->maxParticles) return nullptr;
    if(!t->numParticles) return a;
#ifdef RW_NULL
    return nullptr; // Native NULL tests do not certify a GPU delegation.
#else
    auto* camera=rw::engine->currentCamera->getFrame();if(!camera) return nullptr;
    auto* material=a->geometry && a->geometry->matList.numMaterials ? a->geometry->matList.materials[0] : nullptr;
    auto* texture=material ? textureNative(material->texture) : nullptr;
    constexpr rw::RenderState states[]={rw::TEXTURERASTER,rw::SRCBLEND,rw::DESTBLEND,rw::VERTEXALPHA,rw::CULLMODE};
    void* saved[5];for(int i=0;i<5;i++) saved[i]=rw::GetRenderStatePtr(states[i]);
    rw::SetRenderStatePtr(rw::TEXTURERASTER,texture ? texture->raster : nullptr);
    rw::SetRenderState(rw::SRCBLEND,t->tail[0]);rw::SetRenderState(rw::DESTBLEND,t->tail[1]);
    rw::SetRenderState(rw::VERTEXALPHA,t->tail[2]);rw::SetRenderState(rw::CULLMODE,rw::CULLNONE);
    bool ok=true;std::vector<Im3DVertex> vertices;
    auto* frame=frameNative(static_cast<Frame*>(a->object.object.parent));
    // Six vertices/quad: remain below unmodified librw's 10k immediate limit.
    for(int first=0;first<t->numParticles;first+=1000) {
        if(!particleTankQuads(a,*camera->getLTM(),first,std::min(1000,t->numParticles-first),vertices)) { ok=false;break; }
        rw::im3d::Transform(vertices.data(),int(vertices.size()),frame ? frame->getLTM() : nullptr,rw::im3d::EVERYTHING);
        rw::im3d::RenderPrimitive(rw::PRIMTYPETRILIST);rw::im3d::End();
    }
    for(int i=0;i<5;i++) rw::SetRenderStatePtr(states[i],saved[i]);
    if(ok) t->dirty=0;
    return ok ? a : nullptr;
#endif
}
Atomic* __cdecl tankCreate(int capacity,uint32_t flags,uint32_t platform) {
    if(capacity<=0) return nullptr;
    flags=tankNormalize(flags);
    size_t element=0;int clusters=0;
    for(auto c:tankClusters) if(flags&c.flag) { element+=c.bytes;clusters++; }
    if(!element || size_t(capacity)>(UINT32_MAX-16)/element) return nullptr;
    int off=plugins(Registry::Atomic).getPluginOffset(303);if(off<0) return nullptr;
    auto* a=tankAtomicCreate();if(!a) return nullptr;
    auto* t=zeroAllocate<Tank>();if(!t) { tankAtomicDestroy(a);return nullptr; }
    t->allocation=allocate(element*capacity+16);
    if(!t->allocation || !remember(ownedTanks,t)) { release(t->allocation);release(t);tankAtomicDestroy(a);return nullptr; }
    *reinterpret_cast<Tank**>(reinterpret_cast<char*>(a)+off)=t;
    t->maxParticles=capacity;t->dirty=UINT32_MAX;t->platformFlags=platform;
    t->format.clusterCount=clusters;t->format.flags=flags;
    auto* data=reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(t->allocation)+15)&~uintptr_t(15));
    std::memset(data,0,element*capacity);
    bool interleaved=(flags&0x20000000)!=0;
    t->format.stride=interleaved ? int(element) : 0;
    size_t offset=0;
    for(auto c:tankClusters) if(flags&c.flag) {
        t->format.*(c.member)={data+offset,interleaved ? int(element) : c.bytes};
        offset+=size_t(c.bytes)*(interleaved ? 1 : capacity);
    }
    t->tail[0]=5;t->tail[1]=6;t->tail[2]=1;
    payloadSet<float>(t->tail,5*4,1);payloadSet<float>(t->tail,6*4,1);
    for(int i=8;i<=12;i++) t->tail[i]=0xff000000;
    // Default UV rectangle (+D8..+F4) and constant matrix (+F8..+134).
    rw::V2d uv[4]={{0,0},{0,1},{1,1},{1,0}};std::memcpy(t->tail+13,uv,sizeof(uv));
    rw::Matrix identity;identity.setIdentity();std::memcpy(t->tail+21,&identity,sizeof(identity));
    auto* g=geometryCreate(0,0,0);auto* m=materialCreate();
    bool ok=g && m && geometryAppendMaterial(g,m) && tankSetGeometry(a,g,0);
    if(m) materialDestroy(m);if(g) tankGeometryDestroy(g);
    if(!ok) { tankAtomicDestroy(a);return nullptr; }
    a->renderCB=tankRender;return a;
}
int __cdecl tankDestroy(Atomic* a) { return tankAtomicDestroy(a); }
int32_t* __cdecl tankDataFormat(Atomic* a) {
    auto* t=getTank(a);return t ? &t->format.clusterCount : nullptr;
}
ParticleEmitter* __cdecl emitterSetTank(ParticleEmitter* e,Atomic* a) {
    auto* p=static_cast<EmitterTank*>(particleProperty(e,6));auto* t=getTank(a);
    if(!p || p->atomic || !t) return nullptr;
    int count=t->format.clusterCount;
    if(count<=0 || count>9) return nullptr;
    // The original transfers ownership *before* allocating cursors. A failed
    // allocation still leaves the atomic bound, for emitter teardown to own.
    release(p->input);p->input=nullptr;p->output=nullptr;p->strides=nullptr;
    p->maxParticles=t->maxParticles;p->atomic=a;p->dataFlags=t->format.flags;p->updateFlags=0;p->numParticles=0;
    t->dirty|=0x800000;t->numParticles=0;
    auto** cursors=static_cast<unsigned char**>(allocate(count*(2*sizeof(void*)+sizeof(int32_t))));
    if(!cursors) return nullptr;
    std::memset(cursors,0,count*(2*sizeof(void*)+sizeof(int32_t)));
    p->input=cursors;p->output=cursors+count;p->strides=reinterpret_cast<int32_t*>(cursors+2*count);return e;
}

} // namespace
namespace csf_rw {
void* particleTankDtor(void* a,int off,int) {
    auto** slot=reinterpret_cast<Tank**>(static_cast<char*>(a)+off);auto* t=*slot;
    // Older lock tests supply borrowed binary prefixes, not owned tanks.
    if(std::find(ownedTanks.begin(),ownedTanks.end(),t)!=ownedTanks.end()) {
        release(t->allocation);forget(ownedTanks,t);release(t);
    }
    *slot=nullptr;return a;
}
bool particleTankQuads(Atomic* a,const rw::Matrix& camera,int first,int count,std::vector<Im3DVertex>& out) {
    auto* t=getTank(a);if(!t || first<0 || count<0 || first>t->maxParticles || count>t->maxParticles-first) return false;
    try { out.resize(size_t(count)*6); } catch(...) { return false; }
    auto read=[&]<class T>(TankLock d,int i,T fallback) {
        return d.data ? payloadGet<T>(static_cast<char*>(d.data)+size_t(i)*d.stride) : fallback;
    };
    rw::V2d center=payloadGet<rw::V2d>(t->tail,12);
    for(int j=0;j<count;j++) {
        int i=first+j;
        rw::V3d pos=read(t->format.position,i,rw::V3d{0,0,0}),right=camera.right,up=camera.up;
        rw::V2d size=read(t->format.size,i,payloadGet<rw::V2d>(t->tail,20));
        float angle=read(t->format.rotation,i,payloadGet<float>(t->tail,28));
        if(t->format.flags&8) { auto m=read(t->format.matrix,i,rw::Matrix{});pos=m.pos;right=m.right;up=m.up;size={1,1}; }
        else if(t->format.flags&0x8000) { auto m=payloadGet<rw::Matrix>(t->tail,84);right=m.right;up=m.up; }
        float c=std::cos(angle),s=std::sin(angle);
        auto r=right,u=up;
        right={size.x*(r.x*c+u.x*s),size.x*(r.y*c+u.y*s),size.x*(r.z*c+u.z*s)};
        up={size.y*(u.x*c-r.x*s),size.y*(u.y*c-r.y*s),size.y*(u.z*c-r.z*s)};
        rw::RGBA color=read(t->format.color,i,payloadGet<rw::RGBA>(t->tail,32));
        rw::RGBA colors[4];std::fill_n(colors,4,color);
        if(t->format.texCoords.data) std::memcpy(colors,static_cast<char*>(t->format.texCoords.data)+size_t(i)*t->format.texCoords.stride,16);
        else if(t->format.flags&0x40000) std::memcpy(colors,t->tail+9,16);
        rw::V2d uv[4];std::memcpy(uv,t->tail+13,32);
        if(t->format.texCoords4.data) std::memcpy(uv,static_cast<char*>(t->format.texCoords4.data)+size_t(i)*t->format.texCoords4.stride,32);
        else if(t->format.colors.data || (t->format.flags&0x80000)) {
            auto* p=t->format.colors.data ? static_cast<char*>(t->format.colors.data)+size_t(i)*t->format.colors.stride : reinterpret_cast<char*>(t->tail+13);
            rw::V2d lo=payloadGet<rw::V2d>(p),hi=payloadGet<rw::V2d>(p,8);
            uv[0]=lo;uv[1]={hi.x,lo.y};uv[2]=hi;uv[3]={lo.x,hi.y};
        }
        constexpr int order[]={0,1,2,0,2,3};
        for(int k=0;k<6;k++) {
            int v=order[k];float x=((v==1 || v==2) ? 0.5f : -0.5f)-center.x;
            float y=(v>=2 ? 0.5f : -0.5f)-center.y;
            auto& o=out[6*j+k];o.position={pos.x+x*right.x+y*up.x,pos.y+x*right.y+y*up.y,pos.z+x*right.z+y*up.z};
            o.normal={-camera.at.x,-camera.at.y,-camera.at.z};
            auto col=colors[v];o.color=(uint32_t(col.alpha)<<24)|(uint32_t(col.red)<<16)|(uint32_t(col.green)<<8)|col.blue;
            o.u=uv[v].x;o.v=uv[v].y;
        }
    }
    return true;
}
} // namespace csf_rw
namespace {
