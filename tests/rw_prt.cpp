#include "globals.h"
#include "boundary/rw/particles.h"
#include "boundary/rw/world.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <array>
using namespace csf_rw;
CSF_FUNC(int,__cdecl,init,0x00627360,(const MemoryFunctions*,char,int));
CSF_FUNC(int,__cdecl,openEngine,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,closeEngine,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
CSF_FUNC(int,__stdcall,ptAttach,0x005816E0,());
CSF_FUNC(int,__stdcall,prtAttach,0x0057C4A0,());
CSF_FUNC(int,__stdcall,advAttach,0x005784A0,());
CSF_FUNC(ParticleClass*,__cdecl,advEmitter,0x005784E0,(uint32_t,const int32_t*));
CSF_FUNC(int,__cdecl,destroyClass,0x0057CD60,(ParticleClass*));
CSF_FUNC(Stream*,__cdecl,streamOpen,0x006298E0,(int,int,void*));
CSF_FUNC(int,__cdecl,streamClose,0x00629810,(Stream*,Memory*));
CSF_FUNC(Atomic*,__cdecl,tankCreate,0x00582400,(int,uint32_t,uint32_t));
CSF_FUNC(int,__cdecl,tankDestroy,0x005821A0,(Atomic*));
CSF_FUNC(int32_t*,__cdecl,tankFormat,0x005821B0,(Atomic*));
CSF_FUNC(int,__cdecl,lock,0x005821D0,(Atomic*,TankLock*,uint32_t,uint32_t));
CSF_FUNC(Atomic*,__cdecl,unlock,0x005823E0,(Atomic*));
CSF_FUNC(ParticleEmitter*,__cdecl,setTank,0x00581250,(ParticleEmitter*,Atomic*));
CSF_FUNC(ParticleClass*,__cdecl,stdEmitter,0x0057FA90,(uint32_t));
CSF_FUNC(ParticleClass*,__cdecl,advParticle,0x005785B0,(uint32_t,const int32_t*));
CSF_FUNC(int,__cdecl,destroyParticleClass,0x0057FC60,(ParticleClass*));
CSF_FUNC(ParticleEmitter*,__cdecl,setParticleClass,0x005802A0,(ParticleEmitter*,ParticleClass*,int));
CSF_FUNC(ParticleBatch*,__cdecl,newBatch,0x00580230,(ParticleEmitter*));
CSF_FUNC(Atomic*,__cdecl,atomicCreate,0x0057C5D0,(ParticleClass*));
CSF_FUNC(Atomic*,__cdecl,atomicUpdate,0x0057C2D0,(Atomic*,void*));
CSF_FUNC(ParticleEmitter*,__cdecl,atomicEmitter,0x0057C290,(Atomic*));
CSF_FUNC(int,__cdecl,atomicDestroy,0x0058F060,(Atomic*));
CSF_FUNC(Frame*,__stdcall,frameCreate,0x006235D0,());
CSF_FUNC(Atomic*,__cdecl,setFrame,0x0058F300,(Atomic*,Frame*));
CSF_FUNC(int,__cdecl,frameDestroy,0x00623B10,(Frame*));
CSF_FUNC(World*,__cdecl,registerWorld,0x005865A0,(World*));
CSF_FUNC(Image*,__cdecl,pngRead,0x007C9050,(const char*));
CSF_FUNC(int,__cdecl,imageDestroy,0x00625F10,(Image*));
CSF_FUNC(Image*,__cdecl,findFormat,0x00633070,(Image*,int,int*,int*,int*,int*));
CSF_FUNC(Raster*,__cdecl,rasterCreate,0x00627770,(int,int,int,int));
CSF_FUNC(Raster*,__cdecl,rasterFromImage,0x00633040,(Raster*,Image*));
CSF_FUNC(Texture*,__cdecl,textureCreate,0x00631780,(Raster*));
CSF_FUNC(int,__cdecl,textureDestroy,0x006317E0,(Texture*));
CSF_FUNC(Material*,__cdecl,materialTexture,0x00591560,(Material*,Texture*));
CSF_FUNC(ParticleClass*,__cdecl,setCallbacks,0x0057CE10,(ParticleClass*,int,const uintptr_t*));
CSF_GLOBAL(int,pvsOffset,0x008BE7A4);
CSF_GLOBAL(World*,pvsCurrent,0x008BE7A0);

template<class T> T get(void* p,int off=0) { T v;std::memcpy(&v,static_cast<char*>(p)+off,sizeof(v));return v; }
template<class T> void put(void* p,int off,T v) { std::memcpy(static_cast<char*>(p)+off,&v,sizeof(v)); }
using SizeCB=ParticleEmitter* (__cdecl *)(Atomic*,ParticleEmitter*,int*);
using StreamCB=ParticleEmitter* (__cdecl *)(Atomic*,ParticleEmitter*,Stream*);
using CloneCB=ParticleEmitter* (__cdecl *)(Atomic*,ParticleEmitter*,ParticleEmitter*);
static void roundtrip(ParticleClass* c,int property,int header,int elements,int bytes,int row=1) {
    auto* a=particleEmitterCreate(c,nullptr);auto* b=particleEmitterCreate(c,nullptr);assert(a && b);
    auto* p=static_cast<char*>(particleProperty(a,property));assert(p);
    void* data=header ? get<void*>(p,4) : p;
    for(int i=0;i<elements*bytes;i++) static_cast<unsigned char*>(data)[i]=uint8_t(i*17+3);
    int size=-1;auto sz=reinterpret_cast<SizeCB>(c->callbacks[row*11+9]);assert(sz && sz(nullptr,a,&size)==a);
    auto wr=reinterpret_cast<StreamCB>(c->callbacks[row*11+8]);auto rd=reinterpret_cast<StreamCB>(c->callbacks[row*11+7]);
    auto cl=reinterpret_cast<CloneCB>(c->callbacks[row*11+10]);assert(wr && rd && cl);
    Memory mem{};auto* s=streamOpen(3,2,&mem);assert(s && wr(nullptr,a,s)==a && streamClose(s,&mem));
    assert(int(mem.length)==size);
    s=streamOpen(3,1,&mem);assert(s && rd(nullptr,b,s)==b && streamClose(s,nullptr));
    auto* q=static_cast<char*>(particleProperty(b,property));void* copy=header ? get<void*>(q,4) : q;
    // Fixed shapes serialize a selected boolean canonically; their clone is
    // byte-preserving. Multi-key payloads roundtrip entirely bit-for-bit.
    if(header) assert(std::memcmp(data,copy,elements*bytes)==0);
    assert(cl(nullptr,a,b)==b);
    copy=header ? get<void*>(q,4) : q;
    assert(data!=copy && std::memcmp(data,copy,elements*bytes)==0);
    Memory shortMem{mem.start,mem.length-1};s=streamOpen(3,1,&shortMem);
    assert(s && !rd(nullptr,b,s) && streamClose(s,nullptr));
    rw::Engine::memfuncs.rwfree(mem.start);
    assert(particleEmitterDestroy(a) && particleEmitterDestroy(b));
}
static void properties() {
    int params[]={4,1,3,3,3};
    // Independently counted inline payloads, including a second copy with its
    // rebased pointer; native host pointers must never appear in stream bytes.
    for(int id:{0x1000002,0x1000003,0x1000004,0x1000005}) {
        uint32_t flag=id==0x1000002 ? 0x200 : id==0x1000003 ? 0x400 : id==0x1000004 ? 0x800 : 0x1000;
        auto* c=advEmitter(1|flag,params);assert(c);
        roundtrip(c,id,(id==0x1000003 || id==0x1000004) ? 28 : 20,3,id==0x1000005 ? 24 : 40);
        assert(destroyClass(c));
    }
    for(auto entry:{std::array<int,3>{0x20000,0x1000007,20},{0x30000,0x1000008,12}}) {
        auto* c=advEmitter(1|entry[0],params);assert(c);roundtrip(c,entry[1],0,1,entry[2]);assert(destroyClass(c));
    }
    // Multi-point owns two inline vector arrays, with independent presence
    // markers on disk and boolean normalization at read/write.
    auto* c=advEmitter(0x10001,params);assert(c);auto* a=particleEmitterCreate(c,nullptr);auto* b=particleEmitterCreate(c,nullptr);assert(a && b);
    auto* p=particleProperty(a,0x1000006);assert(get<int>(p)==4);put<int>(p,8,1);put<int>(p,12,1);
    auto* pos=get<rw::V3d*>(p,16);auto* normal=get<rw::V3d*>(p,20+sizeof(void*)-4);
    for(int i=0;i<4;i++) { pos[i]={float(i),float(i+1),float(i+2)};normal[i]={0,1,0}; }
    auto wr=reinterpret_cast<StreamCB>(c->callbacks[11+8]);auto rd=reinterpret_cast<StreamCB>(c->callbacks[11+7]);
    Memory mem{};auto* s=streamOpen(3,2,&mem);assert(s && wr(nullptr,a,s)==a && streamClose(s,&mem) && mem.length==116);
    s=streamOpen(3,1,&mem);assert(s && rd(nullptr,b,s)==b && streamClose(s,nullptr));
    auto* q=particleProperty(b,0x1000006);assert(std::memcmp(pos,get<void*>(q,16),48)==0);
    assert(std::memcmp(normal,get<void*>(q,20+sizeof(void*)-4),48)==0);
    rw::Engine::memfuncs.rwfree(mem.start);assert(particleEmitterDestroy(a) && particleEmitterDestroy(b) && destroyClass(c));
}
static void standardStream() {
    auto* c=stdEmitter(1|2|4|8|16|32|64);assert(c);
    auto* a=particleEmitterCreate(c,nullptr);auto* b=particleEmitterCreate(c,nullptr);assert(a && b);
    auto* s=static_cast<EmitterStandard*>(particleProperty(a,1));s->seed=123;s->maxParticles=17;s->force={2,3,4};
    s->size={7,8};s->rotation=0.7f;s->color={13,25,39,255};s->uv0={.1f,.2f};s->uv1={.9f,.8f};
    auto* matrix=static_cast<rw::Matrix*>(particleProperty(a,8));matrix->setIdentity();matrix->pos={10,20,30};
    auto wr=reinterpret_cast<StreamCB>(c->callbacks[8]);auto rd=reinterpret_cast<StreamCB>(c->callbacks[7]);auto sz=reinterpret_cast<SizeCB>(c->callbacks[9]);
    Memory mem{};auto* stream=streamOpen(3,2,&mem);assert(stream && wr(nullptr,a,stream)==a && streamClose(stream,&mem));
    int size;assert(sz(nullptr,a,&size)==a && size==int(mem.length));
    stream=streamOpen(3,1,&mem);assert(stream && rd(nullptr,b,stream)==b && streamClose(stream,nullptr));
    auto* t=static_cast<EmitterStandard*>(particleProperty(b,1));assert(t->seed==123 && t->force.y==3 && t->size.x==7 && t->rotation==.7f && t->color.blue==39);
    auto* m=static_cast<rw::Matrix*>(particleProperty(b,8));assert(m->pos.x==10 && m->pos.z==30);
    rw::Engine::memfuncs.rwfree(mem.start);assert(particleEmitterDestroy(a) && particleEmitterDestroy(b) && destroyClass(c));
}
static void interpolation() {
    int params[]={0,0,2,2,2};
    for(bool step:{false,true}) {
        uint32_t flag=step ? 0x800 : 0x1000;int id=step ? 0x1000004 : 0x1000005,target=step ? 3 : 5;
        auto* ec=advEmitter(1|flag,params);auto* pc=advParticle(1|(step ? 8 : 32)|flag,params);assert(ec && pc);
        auto* e=particleEmitterCreate(ec,nullptr);assert(e && setParticleClass(e,pc,4));auto* b=newBatch(e);assert(b);
        b->numParticles=1;b->newParticle=0;auto* p=reinterpret_cast<unsigned char*>(b)+b->dataOffset;
        put<float>(p,4,0);put<float>(p,8,10);
        int off=-1,interp=-1;for(int i=0;i<pc->properties->numProperties;i++) {
            if(pc->properties->ids[i]==target) off=pc->properties->offsets[i];
            if(pc->properties->ids[i]==id) interp=pc->properties->offsets[i];
        }
        assert(off>=0 && interp>=0);int dims=step ? 4 : 2;
        for(int d=0;d<dims;d++) { put<float>(p,off+4*d,0);put<float>(p,off+4*(d+dims),8); }
        void* config=particleProperty(e,id);auto* keys=get<unsigned char*>(config,4);int stride=step ? 40 : 24;
        std::memset(keys,0,2*stride);
        for(int k=0;k<2;k++) {
            put<float>(keys,k*stride,.25f+float(k)*.25f);
            for(int d=0;d<dims;d++) { int base=2+(d/2)*4+d%2;put<float>(keys,k*stride+base*4,float(2+2*k)); }
        }
        auto emit=reinterpret_cast<EmitterCallback>(ec->callbacks[11]);assert(emit(nullptr,e,nullptr)==e);
        assert(get<float>(p,interp+4)==2.5f && get<float>(p,interp+8)==5);
        assert(get<int>(p,interp)==0 && get<float>(p,off+dims*4)==(step ? 0 : 8));
        // First segment switch happens only once, even for a giant dt.
        auto update=reinterpret_cast<ParticleBatch* (__cdecl *)(ParticleEmitter*,ParticleBatch*,const float*)>(pc->callbacks[7]);
        float delta=9;assert(update(e,b,&delta)==b && get<int>(p,interp)==1);
        assert(update(e,b,&delta)==b && get<int>(p,interp)==2);
        assert(particleEmitterDestroy(e) && destroyClass(ec) && destroyParticleClass(pc));
    }
}
static void dispatch() {
    auto* c=stdEmitter(1|32);auto* pc=advParticle(1|64|2,nullptr);assert(c && pc);
    auto* a=atomicCreate(c);assert(a && a->geometry);auto* e=atomicEmitter(a);assert(e && e->atomic==a);
    assert(setParticleClass(e,pc,8));auto* frame=frameCreate();assert(frame && setFrame(a,frame));
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));s->maxParticles=8;s->emitCount=2;s->life=2;s->speed=1;s->direction={1,0,0};s->emitGap=1;
    // Auto-creation path: the dispatcher locks real production clusters, not
    // buffers supplied by the test; standard emission and compaction use them.
    float dt=.1f;assert(atomicUpdate(a,&dt)==a && e->numParticles==2);
    auto* tank=static_cast<EmitterTank*>(particleProperty(e,6));assert(tank->atomic && tank->numParticles==2);
    auto* f=tankFormat(tank->atomic);assert(f && f[0]==1);
    assert(atomicUpdate(a,&dt)==a && e->numParticles==2 && a->boundingSphere.radius>0);
    // Rendering must fail explicitly on the NULL platform, never report a draw.
    assert(!a->renderCB(a));
    assert(setFrame(a,nullptr) && frameDestroy(frame) && atomicDestroy(a));
    assert(destroyClass(c) && destroyParticleClass(pc));
}
static int sectorCalls;
static Sector* __cdecl sectorRender(Sector* s) { sectorCalls++;return s; }
static void registration() {
    int saved=pvsOffset;pvsOffset=sizeof(World);
    std::vector<uintptr_t> storage((sizeof(World)+48+sizeof(uintptr_t)-1)/sizeof(uintptr_t));auto* w=reinterpret_cast<World*>(storage.data());
    w->sectorRender=sectorRender;auto* data=reinterpret_cast<char*>(w)+pvsOffset;
    assert(registerWorld(w)==w && w->sectorRender==sectorRender);
    put<int>(data,0,1);assert(registerWorld(w)==w && w->sectorRender!=sectorRender && get<int>(data,20)==1);
    auto cb=w->sectorRender;assert(registerWorld(w)==w && cb==w->sectorRender);
    pvsCurrent=w;Sector sector{};assert(w->sectorRender(&sector)==&sector && sectorCalls>0);pvsCurrent=nullptr;pvsOffset=saved;
}
static void tanks() {
    assert(!tankCreate(0,1,0) && !tankCreate(-1,1,0) && !tankCreate(INT32_MAX,0x1ff,0));
    for(uint32_t layout:{0x10000000u,0x20000000u}) {
        auto* a=tankCreate(8,layout|1|2|4|32|128,0);assert(a);
        auto* f=tankFormat(a);assert(f && f[0]==5 && f[1]==(layout==0x20000000 ? 44 : 0) && uint32_t(f[2])==(layout|1|2|4|32|128));
        TankLock p{},color{},size{},uv{};
        assert(lock(a,&p,1,0x40000000) && lock(a,&color,2,0x40000000) && lock(a,&size,4,0x40000000) && lock(a,&uv,128,0x40000000));
        assert(p.stride==(layout==0x20000000 ? 44 : 12));
        put(p.data,0,rw::V3d{10,20,30});put(color.data,0,rw::RGBA{10,20,30,40});put(size.data,0,rw::V2d{4,6});
        put(uv.data,0,rw::V2d{0.1f,0.2f});put(uv.data,8,rw::V2d{0.8f,0.9f});assert(unlock(a)==a);
        rw::Matrix camera;camera.setIdentity();std::vector<Im3DVertex> out;
        assert(particleTankQuads(a,camera,0,1,out) && out.size()==6);
        assert(out[0].position.x==8 && out[0].position.y==17 && out[0].position.z==30);
        assert(out[2].position.x==12 && out[2].position.y==23);
        assert(out[0].color==0x280a141e && out[0].u==0.1f && out[2].v==0.9f);
        assert(!particleTankQuads(a,camera,7,2,out));
        assert(a->geometry && a->geometry->matList.numMaterials==1 && tankDestroy(a));
    }
    // The game's direct single-quad tanks use constant UV2, not UV4; expand
    // the two corners into a rectangle, including non-default atlas ranges.
    auto* constant=tankCreate(1,0x100a0001,0);assert(constant);
    int offset=plugins(Registry::Atomic).getPluginOffset(303);
    auto* header=*reinterpret_cast<Tank**>(reinterpret_cast<char*>(constant)+offset);
    put(header->tail,52,rw::V2d{.25f,.5f});put(header->tail,60,rw::V2d{.5f,.75f});
    rw::Matrix camera;camera.setIdentity();std::vector<Im3DVertex> vertices;
    assert(particleTankQuads(constant,camera,0,1,vertices) && vertices[1].u==.5f && vertices[1].v==.5f && vertices[2].v==.75f && tankDestroy(constant));
    auto* matrixTank=tankCreate(2,0x2008000a,0);assert(matrixTank);TankLock matrices{};
    assert(lock(matrixTank,&matrices,8,0x40000000) && matrices.stride==68);
    rw::Matrix matrix;matrix.setIdentity();matrix.right={2,0,0};matrix.up={0,4,0};matrix.pos={10,20,30};put(matrices.data,0,matrix);
    assert(unlock(matrixTank) && particleTankQuads(matrixTank,camera,0,1,vertices));
    assert(vertices[0].position.x==9 && vertices[0].position.y==18 && vertices[2].position.x==11 && vertices[2].position.y==22 && tankDestroy(matrixTank));
    // Emitter takes ownership of the atomic and cursor block, final teardown
    // invokes the PTank plugin destructor through the normal atomic boundary.
    auto* c=stdEmitter(1|32);auto* e=particleEmitterCreate(c,nullptr);auto* a=tankCreate(4,1|2|4,0);assert(c && e && a);
    assert(setTank(e,a)==e && !setTank(e,a));auto* p=static_cast<EmitterTank*>(particleProperty(e,6));
    assert(p->maxParticles==4 && p->atomic==a && p->input && p->output==p->input+3 && p->strides);
    assert(particleEmitterDestroy(e) && destroyClass(c));
}
static void realSmokeTexture() {
    // Extracted retail data is optional on other machines; no fixture copies
    // or SDK material are checked in. This exercises a real smoke texture's
    // raster/material lifetime through the production PTank owner.
    const char* path="/mnt/e/dev/csf-paks/GlobalEK/Gfx/Textures/humo02.png";
    auto* file=std::fopen(path,"rb");if(!file) { std::puts("SKIP extracted smoke PNG unavailable");return; }std::fclose(file);
    auto* image=pngRead(path);assert(image && image->pixels);
    int w,h,d,f;assert(findFormat(image,4,&w,&h,&d,&f));auto* raster=rasterCreate(w,h,d,f);
    assert(raster && rasterFromImage(raster,image) && imageDestroy(image));auto* texture=textureCreate(raster);assert(texture);
    auto* a=tankCreate(4,1|2|4|128,0);assert(a);
    auto* material=a->geometry->matList.materials[0];assert(materialTexture(material,texture)==material && texture->refCount==2);
    assert(textureDestroy(texture) && material->texture->refCount==1 && tankDestroy(a));
    std::printf("retail smoke PNG -> PTank material: %dx%d\n",w,h);
}
static void* (*savedMalloc)(size_t,rw::uint32);
static int failAfter;
static void* failMalloc(size_t n,rw::uint32 hint) { return failAfter--==0 ? nullptr : savedMalloc(n,hint); }
static void tankFailures() {
    savedMalloc=rw::Engine::memfuncs.rwmalloc;int failures=0;
    for(int i=0;i<64;i++) {
        failAfter=i;rw::Engine::memfuncs.rwmalloc=failMalloc;auto* a=tankCreate(8,1|2|4,0);rw::Engine::memfuncs.rwmalloc=savedMalloc;
        if(a) { assert(tankDestroy(a));break; }
        failures++;assert(particlesEmpty() && objectsEmpty());
    }
    assert(failures>=5 && failures<64);
    auto* c=stdEmitter(1|32);auto* e=particleEmitterCreate(c,nullptr);auto* a=tankCreate(8,1,0);assert(c && e && a);
    failAfter=0;rw::Engine::memfuncs.rwmalloc=failMalloc;assert(!setTank(e,a));rw::Engine::memfuncs.rwmalloc=savedMalloc;
    auto* p=static_cast<EmitterTank*>(particleProperty(e,6));assert(p->atomic==a && !p->input && !p->output && !p->strides);
    assert(particleEmitterDestroy(e) && destroyClass(c) && particlesEmpty() && objectsEmpty());
}
static int childEmits;
static ParticleEmitter* __cdecl recordChild(Atomic* a,ParticleEmitter* child,void*) {
    auto* s=static_cast<EmitterStandard*>(particleProperty(child,1));assert(s && s->currentTime==1 && s->nextEmit==1);
    auto* frame=static_cast<Frame*>(a->object.object.parent);assert(frame->matrix.pos.x==5 && frame->matrix.pos.z==7);childEmits++;return child;
}
static void nestedEmitter() {
    int params[]={0,0,0,0,0};auto* ec=advEmitter(1|32|0x100,params);auto* pc=advParticle(1|2|64|0x100,params);
    auto* cc=stdEmitter(1);auto* a=atomicCreate(ec);assert(ec && pc && cc && a);
    auto* e=atomicEmitter(a);auto* child=particleEmitterCreate(cc,nullptr);assert(child && setParticleClass(e,pc,4) && setParticleClass(child,pc,4));
    child->flags=2;child->atomic=a;child->next=e;e->next=child;
    uintptr_t callbacks[11]{};callbacks[0]=reinterpret_cast<uintptr_t>(recordChild);assert(setCallbacks(cc,1,callbacks));
    auto* s=static_cast<EmitterStandard*>(particleProperty(child,1));s->currentTime=2;s->nextEmit=3;
    auto* frame=frameCreate();assert(frame && setFrame(a,frame));frame->matrix.pos={11,22,33};auto saved=frame->matrix;
    auto* b=newBatch(e);assert(b);b->numParticles=1;auto* p=reinterpret_cast<unsigned char*>(b)+b->dataOffset;
    put<float>(p,4,1);put<float>(p,8,10);put(p,16,rw::V3d{0,1,0});
    int sub=-1;for(int i=0;i<pc->properties->numProperties;i++) if(pc->properties->ids[i]==0x1000001) sub=pc->properties->offsets[i];
    assert(sub>=0);put<float>(p,sub,.25f);put<float>(p,sub+4,.5f);
    auto* tank=tankCreate(4,1,0);assert(tank && setTank(e,tank));TankLock position{};assert(lock(tank,&position,1,0x40000000));
    put(position.data,0,rw::V3d{5,6,7});auto* t=static_cast<EmitterTank*>(particleProperty(e,6));t->input[0]=static_cast<unsigned char*>(position.data);t->strides[0]=position.stride;
    auto cb=reinterpret_cast<BatchCallback>(pc->callbacks[7]);float dt=.1f;int before=childEmits;
    assert(cb(e,b,&dt)==b && childEmits==before+1 && get<float>(p,sub)==.75f);
    assert(s->currentTime==2 && s->nextEmit==3 && child->flags==0x22 && std::memcmp(&saved,&frame->matrix,sizeof(saved))==0);
    assert(unlock(tank) && setFrame(a,nullptr) && frameDestroy(frame) && atomicDestroy(a));
    assert(destroyClass(ec) && destroyClass(cc) && destroyParticleClass(pc));
}
int main() {
    for(int pass=0;pass<2;pass++) {
        assert(init(nullptr,0,1024) && ptAttach() && prtAttach() && advAttach() && openEngine(nullptr) && start());
        properties();standardStream();interpolation();tanks();tankFailures();dispatch();nestedEmitter();registration();realSmokeTexture();assert(particlesEmpty());assert(stop() && closeEngine() && term());
    }
    std::puts("PASS advanced property streams/clones/truncation, PTank ownership/format/locks/CPU quads (two lifecycles)");
}
