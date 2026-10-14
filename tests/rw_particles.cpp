#include "globals.h"
#include "boundary/rw/particles.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cmath>
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
CSF_FUNC(ParticleClass*,__cdecl,advParticle,0x005785B0,(uint32_t,const int32_t*));
CSF_FUNC(ParticleClass*,__cdecl,stdEmitter,0x0057FA90,(uint32_t));
CSF_FUNC(ParticleClass*,__cdecl,stdParticle,0x0057F970,(uint32_t));
CSF_FUNC(ParticleClass*,__cdecl,setCallbacks,0x0057CE10,(ParticleClass*,int,const uintptr_t*));
CSF_FUNC(ParticleClass*,__cdecl,setParticleCallbacks,0x0057FCF0,(ParticleClass*,int,const uintptr_t*));
CSF_FUNC(ParticleClass*,__cdecl,setProperties,0x0057CE90,(ParticleClass*,ParticlePropTable*));
CSF_FUNC(ParticleClass*,__cdecl,getProperties,0x0057CED0,(ParticleClass*,ParticlePropTable**));
CSF_FUNC(ParticlePropTable*,__cdecl,createTable,0x0057C9A0,(int,const int32_t*,const int32_t*));
CSF_FUNC(int,__cdecl,destroyTable,0x0057C770,(ParticlePropTable*));
CSF_FUNC(int,__cdecl,getOffset,0x0057C8B0,(ParticlePropTable*,int));
CSF_FUNC(int,__cdecl,destroyClass,0x0057CD60,(ParticleClass*));
CSF_FUNC(int,__cdecl,destroyParticleClass,0x0057FC60,(ParticleClass*));
CSF_FUNC(ParticleEmitter*,__cdecl,createCB,0x0057ED00,(Atomic*,ParticleEmitter*,void*));
CSF_FUNC(ParticleEmitter*,__cdecl,endCB,0x0057F0B0,(Atomic*,ParticleEmitter*,void*));
CSF_FUNC(ParticleEmitter*,__cdecl,beginCB,0x0057EF90,(Atomic*,ParticleEmitter*,const float*));
CSF_FUNC(ParticleEmitter*,__cdecl,scheduleCB,0x0057F050,(Atomic*,ParticleEmitter*,void*));
CSF_FUNC(ParticleEmitter*,__cdecl,allBatches,0x00580160,(ParticleEmitter*,BatchCallback,void*));
CSF_FUNC(ParticleBatch*,__cdecl,newBatch,0x00580230,(ParticleEmitter*));
CSF_FUNC(ParticleEmitter*,__cdecl,setParticleClass,0x005802A0,(ParticleEmitter*,ParticleClass*,int));
CSF_FUNC(ParticleEmitter*,__cdecl,createEmitter,0x00580100,(ParticleClass*,void*));
CSF_FUNC(ParticleEmitter*,__cdecl,getEmitter,0x0057C290,(Atomic*));
CSF_FUNC(int,__cdecl,isEmitter,0x0057C220,(Atomic*));
CSF_FUNC(int,__cdecl,lock,0x005821D0,(Atomic*,TankLock*,uint32_t,uint32_t));
CSF_FUNC(Atomic*,__cdecl,unlock,0x005823E0,(Atomic*));
CSF_FUNC(ParticleEmitter*,__cdecl,emitCB,0x0057DBA0,(Atomic*,ParticleEmitter*,void*));
CSF_FUNC(ParticleEmitter*,__cdecl,sphereCB,0x0057BBD0,(Atomic*,ParticleEmitter*,void*));
CSF_FUNC(ParticleBatch*,__cdecl,updateCB,0x0057CFE0,(ParticleEmitter*,ParticleBatch*,const float*));
CSF_FUNC(int,__cdecl,compressFloat,0x0061F5E0,(float));
CSF_FUNC(Atomic*,__stdcall,createAtomic,0x0058F5C0,());
CSF_FUNC(int,__cdecl,destroyAtomic,0x0058F060,(Atomic*));
CSF_FUNC(Frame*,__stdcall,createFrame,0x006235D0,());
CSF_FUNC(int,__cdecl,destroyFrame,0x00623B10,(Frame*));
CSF_FUNC(Atomic*,__cdecl,setFrame,0x0058F300,(Atomic*,Frame*));
CSF_FUNC(Clump*,__cdecl,cloneClump,0x0058FC60,(Clump*));
CSF_FUNC(int,__cdecl,destroyClump,0x0058F740,(Clump*));
static void* failMalloc(size_t,rw::uint32) { return nullptr; }
static int renderCalls;
static bool renderFail;
static Atomic* __cdecl render(Atomic* a) { ++renderCalls;return renderFail ? nullptr : a; }
static int visits;
static ParticleBatch* stopAt;
static ParticleBatch* __cdecl visit(ParticleEmitter*,ParticleBatch* b,void* p) {
    assert(p==&visits); ++visits; return b==stopAt ? nullptr : b;
}
static int creates,destroys;
static bool batchFail;
static ParticleBatch* __cdecl batchCreate(ParticleEmitter* e,ParticleBatch* b,void*) {
    assert(!e);++creates;return batchFail ? nullptr : b;
}
static ParticleBatch* __cdecl batchDestroy(ParticleEmitter* e,ParticleBatch* b,void*) {
    assert(!e);++destroys;return b;
}
static void checkSchema(ParticleClass* c,const int* ids,const int* sizes,int n) {
    assert(c && c->refCount==1); auto* t=c->properties; assert(t->numProperties==n && t->refCount==1);
    int offset=0;for(int i=0;i<n;i++) {
        assert(t->ids[i]==ids[i] && t->sizes[i]==sizes[i]);
        assert(t->offsets[i]==offset && getOffset(t,ids[i])==offset);offset+=sizes[i];
    }
    assert(c->objectSize==offset && t->offsets[n]==offset);
    assert(getOffset(t,123456)==-1);
}
static void simulation(bool matrixMode) {
    auto* ec=advEmitter(0x3007f,nullptr);auto* pc=stdParticle(matrixMode ? 255 : 117);assert(ec && pc);
    uintptr_t callbacks[11]{};callbacks[5]=CSF_ADDR_FN(0x0057ED00);callbacks[6]=CSF_ADDR_FN(0x0057EDA0);
    assert(setCallbacks(ec,1,callbacks));
    auto* e=createEmitter(ec,nullptr);assert(e && setParticleClass(e,pc,8));
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));
    rw::Matrix ltm;ltm.setIdentity();ltm.pos={10,20,30};s->ltm=&ltm;
    s->currentTime=1;s->nextEmit=0;s->emitCount=3;s->life=2;s->lifeBias=.1f;
    s->speed=2;s->direction={0,0,1};s->position={1,2,3};
    float* sphere=static_cast<float*>(particleProperty(e,0x1000008));sphere[0]=2;sphere[1]=1;
    int radial=1;std::memcpy(sphere+2,&radial,4);
    auto* tank=static_cast<EmitterTank*>(particleProperty(e,6));
    rw::V3d positions[8]{};rw::RGBA colors[8]{};rw::V2d sizes[8]{};float rotations[8]{};float uvs[8][4]{};
    rw::Matrix matrices[8];for(auto& m:matrices) m.setIdentity();
    unsigned char* input[]={reinterpret_cast<unsigned char*>(positions),reinterpret_cast<unsigned char*>(colors),reinterpret_cast<unsigned char*>(sizes),reinterpret_cast<unsigned char*>(rotations),reinterpret_cast<unsigned char*>(uvs)};
    unsigned char* output[5];std::memcpy(output,input,sizeof(input));int strides[]={12,4,8,4,16};
    tank->dataFlags=0xa7;tank->flags=0xa7;tank->platformFlags=0xa7;tank->maxParticles=8;
    if(matrixMode) {
        input[0]=output[0]=reinterpret_cast<unsigned char*>(matrices);strides[0]=64;
        tank->dataFlags=tank->flags=tank->platformFlags=8;
        auto* axes=static_cast<float*>(particleProperty(e,8));axes[18]=1;axes[23]=1;
    }
    tank->input=input;tank->output=output;tank->strides=strides;
    assert(emitCB(nullptr,e,nullptr)==e);auto* b=e->activeBatches;
    assert(b && b->numParticles==3 && b->newParticle==0 && e->numParticles==3 && tank->numParticles==3);
    assert(sphereCB(nullptr,e,nullptr)==e);
    for(int i=0;i<3;i++) if(!matrixMode) { assert(std::isfinite(positions[i].x));assert(positions[i].x!=11 || positions[i].y!=22); }
    auto* data=reinterpret_cast<unsigned char*>(b)+b->dataOffset;
    auto* dead=reinterpret_cast<float*>(data+pc->objectSize);dead[1]=dead[2];
    // Reset fixture-owned cursors after emission.
    input[0]=output[0]=matrixMode ? reinterpret_cast<unsigned char*>(matrices) : reinterpret_cast<unsigned char*>(positions);
    input[1]=output[1]=reinterpret_cast<unsigned char*>(colors);
    input[2]=output[2]=reinterpret_cast<unsigned char*>(sizes);
    input[3]=output[3]=reinterpret_cast<unsigned char*>(rotations);
    input[4]=output[4]=reinterpret_cast<unsigned char*>(uvs);
    tank->numParticles=0;float dt=.25f;
    assert(updateCB(e,b,&dt)==b && b->numParticles==2 && tank->numParticles==2);
    assert(reinterpret_cast<float*>(data)[1]==.25f);
    auto* base=matrixMode ? reinterpret_cast<unsigned char*>(matrices) : reinterpret_cast<unsigned char*>(positions);
    assert(output[0]==base+2*strides[0] && input[0]==base+3*strides[0]);
    assert(compressFloat(0)==0 && compressFloat(-0.0f)==0x8000 && (compressFloat(1)&0xffff)==0x7800);
    tank->input=nullptr;tank->output=nullptr;tank->strides=nullptr;
    assert(particleEmitterDestroy(e) && destroyClass(ec) && destroyParticleClass(pc));
}
int main() {
    assert(particlesEmpty());
    for(int repeat=0;repeat<2;repeat++) {
        assert(init(nullptr,0,1024)); assert(ptAttach() && prtAttach() && advAttach());
        assert(openEngine(nullptr) && start());
        simulation(false);simulation(true);
        // These arrays are independently asserted against the original image
        // by tests/rw_particles_original.py, not guessed SDK flag definitions.
        int ids0[]={0,1,2,6,5,4,0x1000007}, sizes0[]={40,172,64,52,32,16,20};
        int ids2[]={0,1,2,6,8,5,0x1000007}, sizes2[]={40,172,64,52,116,32,20};
        auto* c0=advEmitter(131131,nullptr);checkSchema(c0,ids0,sizes0,7);
        assert(c0->numCallbackSets==2 && c0->callbacks[0]==CSF_ADDR_FN(0x0057DBA0));
        assert(c0->callbacks[11]==CSF_ADDR_FN(0x0057B640));
        auto* c2=advEmitter(131187,nullptr);checkSchema(c2,ids2,sizes2,7);
        assert(advEmitter(131187,nullptr)==c2 && c2->refCount==2);assert(destroyClass(c2));
        ParticlePropTable* borrowed=nullptr;assert(getProperties(c2,&borrowed)==c2);
        assert(borrowed==c2->properties && borrowed->refCount==1);
        int idsExtended[]={0,1,2,6,8,5,0x1000007,10}, sizesExtended[]={40,172,64,52,116,32,20,8};
        auto* extended=createTable(8,idsExtended,sizesExtended);assert(extended);
        assert(setProperties(c2,extended)==c2 && extended->refCount==2);
        assert(destroyTable(extended) && c2->properties->refCount==1);
        auto* c3=advEmitter(131187,nullptr);assert(c3 && c3!=c2);checkSchema(c3,ids2,sizes2,7);
        assert(setProperties(c2,c2->properties)==c2 && c2->properties->refCount==1);
        int pi0[]={0,6,2,5,4}, ps0[]={16,12,32,32,8};
        int pi2[]={0,6,2,5}, ps2[]={16,12,32,32};
        int pi3[]={0,6,2}, ps3[]={16,12,32};
        int pi1[]={0,1}, ps1[]={16,12};
        auto* p0=stdParticle(117);checkSchema(p0,pi0,ps0,5);
        auto* p2=stdParticle(101);checkSchema(p2,pi2,ps2,4);
        auto* p3=advParticle(69,nullptr);checkSchema(p3,pi3,ps3,3);
        auto* p1=stdParticle(2);checkSchema(p1,pi1,ps1,2);
        assert(p1->callbacks[0]==CSF_ADDR_FN(0x0057CFE0));
        assert(stdParticle(2)==p1 && p1->refCount==2);assert(destroyParticleClass(p1));
        assert(!advEmitter(0x201,nullptr)); // variable advanced count needs parameter array
        int oddIds[]={42,43}, oddSizes[]={1,5}, reverseIds[]={43,42}, reverseSizes[]={8,4};
        auto* odd=createTable(2,oddIds,oddSizes); assert(odd && odd->sizes[0]==4 && odd->offsets[2]==12);
        auto* canonical=createTable(2,reverseIds,reverseSizes);assert(canonical==odd && odd->refCount==2);
        assert(destroyTable(canonical) && destroyTable(odd));
        assert(!createTable(-1,oddIds,oddSizes) && !destroyTable(nullptr));
        auto alloc=rw::Engine::memfuncs.rwmalloc;
        rw::Engine::memfuncs.rwmalloc=failMalloc;
        assert(!stdEmitter(7)); // property allocation failure rolls back
        uintptr_t cb[11]{}; auto* oldCallbacks=c0->callbacks;
        assert(!setCallbacks(c0,1,cb) && c0->callbacks==oldCallbacks);
        rw::Engine::memfuncs.rwmalloc=alloc;
        assert(!stop()); // classes/property tables cannot outlive their allocator

        auto* ec=stdEmitter(33);assert(ec);
        cb[1]=CSF_ADDR_FN(0x0057EF90);cb[2]=CSF_ADDR_FN(0x0057F050);cb[3]=CSF_ADDR_FN(0x0057F0B0);
        cb[5]=CSF_ADDR_FN(0x0057ED00);cb[6]=CSF_ADDR_FN(0x0057EDA0);
        assert(setCallbacks(ec,1,cb)==ec);
        auto* e=createEmitter(ec,nullptr);assert(e && ec->refCount==2 && e->flags==0x7f);
        auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));
        assert(s && s->seed==1 && s->maxParticles==0 && s->size.x==1 && s->size.y==1);
        assert(s->color.red==255 && s->color.alpha==255 && s->uv1.x==1 && !s->texture && !s->ltm);
        auto* et=static_cast<EmitterTank*>(particleProperty(e,6));
        assert(et && et->flags==-1 && et->srcBlend==2 && et->dstBlend==2 && et->vertexAlpha==1);
        assert(!particleProperty(e,999));
        auto* a=createAtomic();auto* f=createFrame();assert(a && f);assert(setFrame(a,f)==a);
        float dt=0.25f; s->emitGap=0.5f;s->emitGapBias=0.125f;
        assert(beginCB(a,e,&dt)==e && s->currentTime==0.25f && s->ltm==&f->ltm);
        assert(scheduleCB(a,e,nullptr)==e && s->seed==0x41cce7a0u);
        assert(s->previousEmit==0.25f && s->nextEmit==0.68925821781158447266f);
        auto seed=s->seed;assert(scheduleCB(a,e,nullptr)==e && s->seed==seed);
        auto* noTank=stdEmitter(1);assert(noTank && setCallbacks(noTank,1,cb)==noTank);
        auto* bare=createEmitter(noTank,nullptr);assert(bare);bare->numParticles=1;
        assert(!endCB(nullptr,bare,nullptr));assert(particleEmitterDestroy(bare) && destroyClass(noTank));
        renderCalls=0;renderFail=false;a->renderCB=render;
        assert(endCB(nullptr,e,nullptr)==e && renderCalls==0);
        e->numParticles=1;et->numParticles=1;et->atomic=a;
        assert(endCB(nullptr,e,nullptr)==e && renderCalls==1);
        renderFail=true;assert(!endCB(nullptr,e,nullptr) && renderCalls==2);
        et->numParticles=0;assert(endCB(nullptr,e,nullptr)==e && renderCalls==2);
        // The emitter now owns this atomic/frame and destroys both later.
        uintptr_t pcb[7]{};pcb[2]=reinterpret_cast<uintptr_t>(batchCreate);pcb[3]=reinterpret_cast<uintptr_t>(batchDestroy);
        assert(setParticleCallbacks(p0,1,pcb)==p0);
        assert(setParticleClass(e,p0,3)==e && p0->refCount==2);
        creates=destroys=0;batchFail=false;
        auto* b0=newBatch(e);auto* b1=newBatch(e);assert(b0 && b1 && creates==2);
        assert(b0->maxParticles==3 && b0->dataOffset==sizeof(ParticleBatch) && b0->numParticles==0);
        assert(e->activeBatches==b1 && b1->next==b0 && b0->next==b1);
        visits=0;stopAt=nullptr;assert(allBatches(e,visit,&visits)==e && visits==2);
        visits=0;stopAt=b0;assert(!allBatches(e,visit,&visits) && visits==1);
        batchFail=true;auto* b2=newBatch(e);assert(b2 && creates==3 && e->activeBatches==b2);batchFail=false;
        assert(setParticleClass(e,p0,2)==e && destroys==3 && p0->refCount==2);
        assert(!e->activeBatches && !e->inactiveBatches && e->numParticles==0);
        b0=newBatch(e);assert(b0);e->activeBatches=nullptr;e->inactiveBatches=b0;
        assert(newBatch(e)==b0 && !e->inactiveBatches && creates==5);
        // PTank prefix supplied by a fixture, NOT a pretend particle backend.
        Tank tank{};tank.format.flags=0x27;
        rw::V3d positions[3]{};rw::RGBA colors[3]{};rw::V2d sizes[3]{};float rotations[3]{};
        tank.format.position={positions,12};tank.format.color={colors,4};tank.format.size={sizes,8};tank.format.rotation={rotations,4};
        int off=plugins(Registry::Atomic).getPluginOffset(303);assert(off>=0);
        *reinterpret_cast<Tank**>(reinterpret_cast<char*>(a)+off)=&tank;
        TankLock l{};assert(lock(a,&l,1,0x40000000) && l.data==positions && l.stride==12);
        assert(lock(a,&l,4,0x40000000) && l.data==sizes && l.stride==8);
        assert(tank.locked==0x40000005 && unlock(a)==a && !tank.locked && tank.dirty==0x40000005);
        assert(lock(a,&l,2,0) && l.data==colors && l.stride==4 && !tank.locked);
        assert(lock(a,&l,32,0) && l.data==rotations);
        float extra[5][3*16]{};
        tank.format.flags=0x1ff;
        tank.format.matrix={extra[0],64};tank.format.normal={extra[1],12};
        tank.format.texCoords={extra[2],16};tank.format.colors={extra[3],16};tank.format.texCoords4={extra[4],32};
        TankLock descriptors[]={tank.format.position,tank.format.color,tank.format.size,tank.format.matrix,
            tank.format.normal,tank.format.rotation,tank.format.texCoords,tank.format.colors,tank.format.texCoords4};
        for(int i=0;i<9;i++) {
            assert(lock(a,&l,1u<<i,0x40000000));
            assert(l.data==descriptors[i].data && l.stride==descriptors[i].stride);
        }
        assert(tank.locked==0x400001ff && unlock(a)==a && !tank.locked && tank.dirty==0x400001ff);
        tank.format.flags=0x27;
        tank.format.stride=28;assert(lock(a,&l,1,0) && l.stride==28);
        assert(!lock(a,&l,8,0) && !l.data && !l.stride);
        l={sizes,99};assert(lock(a,&l,3,0) && l.data==sizes && l.stride==28);
        *reinterpret_cast<Tank**>(reinterpret_cast<char*>(a)+off)=nullptr;
        assert(!lock(a,&l,1,0) && !l.data && !l.stride);
        auto* host=createAtomic();assert(host && !isEmitter(host) && !getEmitter(host));
        auto poff=plugins(Registry::Atomic).getPluginOffset(304);
        *reinterpret_cast<ParticleEmitter**>(reinterpret_cast<char*>(host)+poff)=e;e->atomic=host;
        assert(isEmitter(host) && getEmitter(host)==e);
        assert(!particleAtomicCopy(a,host,poff,sizeof(void*))); // no aliased owner
        auto* clump=clumpCreate();auto* root=createFrame();assert(clump && root);
        clumpSetFrame(clump,root);assert(setFrame(host,root)==host && clumpAddAtomic(clump,host));
        assert(!cloneClump(clump)); // PluginList::copy's ignored return is preflighted
        assert(destroyClump(clump) && destroys==4 && ec->refCount==1 && p0->refCount==1);
        assert(destroyClass(ec) && destroyClass(c0) && destroyClass(c2) && destroyClass(c3));
        assert(destroyParticleClass(p0) && destroyParticleClass(p1) && destroyParticleClass(p2) && destroyParticleClass(p3));
        assert(particlesEmpty());assert(stop() && closeEngine() && term());
    }
    std::puts("PASS particle schemas/ownership/defaults/schedule/batch rings, emission/sphere/update/compression and PTank lock stride/dirty flags");
}
