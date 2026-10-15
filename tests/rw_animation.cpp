#include "globals.h"
#include "boundary/rw/animation.h"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace csf_rw;
CSF_FUNC(int,__cdecl,init,0x00627360,(const MemoryFunctions*,char,int));
CSF_FUNC(int,__stdcall,worldAttach,0x0058D0B0,());
CSF_FUNC(int,__stdcall,animInit,0x0061ECD0,());
CSF_FUNC(int,__stdcall,hanimAttach,0x0056DAE0,());
CSF_FUNC(int,__stdcall,skinAttach,0x00587450,());
CSF_FUNC(int,__stdcall,matfxAttach,0x00571FF0,());
CSF_FUNC(int,__stdcall,compressedRegister,0x006205C0,());
CSF_FUNC(int,__cdecl,registerScheme,0x0061ED00,(const AnimationScheme*));
CSF_FUNC(int,__cdecl,openEngine,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,closeEngine,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
CSF_FUNC(Stream*,__cdecl,streamOpen,0x006298E0,(int,int,void*));
CSF_FUNC(int,__cdecl,streamClose,0x00629810,(Stream*,Memory*));
CSF_FUNC(uint32_t,__cdecl,readBytes,0x006293E0,(Stream*,void*,uint32_t));
CSF_FUNC(Animation*,__cdecl,newAnim,0x0061EE10,(int,int,int,float));
CSF_FUNC(int,__cdecl,deleteAnim,0x0061EE90,(void*));
CSF_FUNC(Animation*,__cdecl,readAnimStream,0x0061EEB0,(Stream*));
CSF_FUNC(Animation*,__cdecl,readAnim,0x0061F3C0,(const char*));
CSF_FUNC(int,__cdecl,numNodes,0x0061EFB0,(Animation*));
CSF_FUNC(Interpolator*,__cdecl,newInterp,0x0061EFE0,(int,int));
CSF_FUNC(int,__cdecl,setAnim,0x0061F050,(Interpolator*,Animation*));
CSF_FUNC(int,__cdecl,setTime,0x0061F590,(Interpolator*,float));
CSF_FUNC(int,__cdecl,addTime,0x0061F150,(Interpolator*,float));
CSF_FUNC(int,__cdecl,copyInterp,0x0061F120,(Interpolator*,Interpolator*));
CSF_FUNC(int,__cdecl,blendInterp,0x0061F320,(Interpolator*,Interpolator*,Interpolator*,float));
CSF_FUNC(Interpolator*,__cdecl,newSubInterp,0x0061F380,(Interpolator*,int,int,int));
CSF_FUNC(Animation*,__cdecl,compress,0x007C5D00,(Animation*));
CSF_FUNC(int,__cdecl,encode,0x0061F5E0,(float));
CSF_FUNC(Hierarchy*,__cdecl,newHierarchy,0x0056CC30,(int,int*,int*,int,int));
CSF_FUNC(int,__cdecl,deleteHierarchy,0x0056CD30,(Hierarchy*));
CSF_FUNC(Hierarchy*,__cdecl,copyHierarchy,0x0056CED0,(Hierarchy*,int,int));
CSF_FUNC(Hierarchy*,__cdecl,newSub,0x0056CDB0,(Hierarchy*,int,int,int));
CSF_FUNC(Hierarchy*,__cdecl,detach,0x0056CF50,(Hierarchy*));
CSF_FUNC(Hierarchy*,__cdecl,attach,0x0056DB50,(Hierarchy*));
CSF_FUNC(int,__cdecl,update,0x0056D3F0,(Hierarchy*));
CSF_FUNC(rw::Matrix*,__cdecl,matrixArray,0x0056CF80,(Hierarchy*));
CSF_FUNC(int,__cdecl,frameID,0x0056CFA0,(Frame*));
CSF_FUNC(Hierarchy*,__cdecl,frameHierarchy,0x0056CF90,(Frame*));
CSF_FUNC(Atomic*,__cdecl,setSkinHierarchy,0x00587020,(Atomic*,Hierarchy*));
CSF_FUNC(Hierarchy*,__cdecl,getSkinHierarchy,0x00587040,(Atomic*));
CSF_FUNC(Skin*,__cdecl,getSkin,0x00587050,(Geometry*));
CSF_FUNC(int,__cdecl,isSplit,0x005870E0,(Skin*));
CSF_FUNC(int,__cdecl,skinType,0x005870F0,(Atomic*));
CSF_FUNC(int,__cdecl,atomicEffects,0x005712A0,(Atomic*));
CSF_FUNC(int,__cdecl,materialEffects,0x005712B0,(Material*));
CSF_FUNC(Material*,__cdecl,envFrame,0x005715D0,(Material*,Frame*));
CSF_FUNC(Material*,__cdecl,envAlpha,0x00571610,(Material*,int));
CSF_FUNC(Material*,__cdecl,uvMatrices,0x00571920,(Material*,rw::Matrix*,rw::Matrix*));
CSF_FUNC(Clump*,__cdecl,readClump,0x0058FFC0,(Stream*));
CSF_FUNC(Clump*,__cdecl,cloneClump,0x0058FC60,(Clump*));
CSF_FUNC(int,__cdecl,deleteClump,0x0058F740,(Clump*));
using KF=rw::HAnimKeyFrame;
using IF=rw::HAnimInterpFrame;
static void near(float a,float b,float eps=0.001f) { assert(std::isfinite(a) && std::fabs(a-b)<eps); }
template<class T> static T extension(void* p,Registry k,int id) {
    T t{}; int off=plugins(k).getPluginOffset(id); assert(off>=0);
    std::memcpy(&t,static_cast<char*>(p)+off,sizeof(t)); return t;
}
static Animation* animation(int nodes=2) {
    auto* a=newAnim(1,4*nodes,5,3); assert(a);
    auto* keys=static_cast<KF*>(a->keyframes);
    for(int t=0;t<4;t++) for(int i=0;i<nodes;i++) {
        int n=t*nodes+i; keys[n].time=float(t); keys[n].q={0,0,0,1};
        keys[n].t={float(t*10+i),float(t*4+i*2),float(t*6+i*3)};
        keys[n].prev=keys+(t?t-1:3)*nodes+i;
    }
    assert(numNodes(a)==nodes); return a;
}
static void word(std::vector<uint8_t>& out,uint32_t v) {
    auto n=out.size();out.resize(n+4);std::memcpy(out.data()+n,&v,4);
}
static std::vector<uint8_t> serialized(Animation* a) {
    std::vector<uint8_t> bytes;word(bytes,256);word(bytes,a->scheme->id);
    word(bytes,a->numFrames);word(bytes,a->flags);uint32_t d;std::memcpy(&d,&a->duration,4);word(bytes,d);
    bool compressed=a->scheme->id==2;
    for(int i=0;i<a->numFrames;i++) {
        auto* p=static_cast<uint8_t*>(a->keyframes)+size_t(i)*a->scheme->keyframeSize;
        // Serialized time, quaternion/translation (or seven compressed words),
        // then a 32-bit offset in the original in-memory stride.
        auto n=bytes.size();auto len=compressed?18u:32u;bytes.resize(n+len);
        std::memcpy(bytes.data()+n,p+sizeof(void*),len);
        void* prev;std::memcpy(&prev,p,sizeof(prev));
        auto delta=prev?static_cast<uint8_t*>(prev)-static_cast<uint8_t*>(a->keyframes):0;
        word(bytes,uint32_t(delta/a->scheme->keyframeSize)*(compressed?24:36));
    }
    if(compressed) {
        auto n=bytes.size();bytes.resize(n+24);std::memcpy(bytes.data()+n,a->customData,24);
    }
    return bytes;
}
static Animation* decode(std::vector<uint8_t>& bytes) {
    Memory m{bytes.data(),uint32_t(bytes.size())};auto* s=streamOpen(3,1,&m);assert(s);
    auto* a=readAnimStream(s);assert(streamClose(s,nullptr));return a;
}
static int timed,looped;
static int __cdecl timeCB(Interpolator* a,void* p) { assert(a==p);++timed;return 0; }
static int __cdecl loopCB(Interpolator* a,void* p) { assert(a==p);++looped;return 1; }
static int customReads,customInterps,pluginReads,pluginDtors;
static bool failPlugin;
static decltype(rw::Engine::memfuncs.rwmalloc) savedMalloc;
static int failAllocation;
static void* failMalloc(size_t n,rw::uint32 hint) { return failAllocation--==0?nullptr:savedMalloc(n,hint); }
static void customApply(void* out,void* in) {
    auto* m=static_cast<rw::Matrix*>(out);m->setIdentity();m->pos=static_cast<IF*>(in)->t;
}
static void customInterpolate(void* out,void* a,void* b,float t,void* data) {
    ++customInterps;auto* x=static_cast<KF*>(a);auto* y=static_cast<KF*>(b);auto* f=static_cast<IF*>(out);
    f->q=x->q;f->t=rw::lerp(x->t,y->t,t);f->t.x+=*static_cast<float*>(data);
}
static Animation* __cdecl customRead(Stream* s,Animation* a) {
    ++customReads;assert(readBytes(s,a->customData,4)==4);
    auto* k=static_cast<KF*>(a->keyframes);k[0]={k+1,0,{0,0,0,1},{0,0,0}};k[1]={k,1,{0,0,0,1},{10,0,0}};
    return a;
}
static Stream* __cdecl gameRead(Stream* s,int length,void* p,int off,int size) {
    assert(length==4 && size==4);assert(readBytes(s,static_cast<char*>(p)+off,4)==4);++pluginReads;
    auto* f=static_cast<Frame*>(p);assert(frameID(f)==10 || frameID(f)==20);
    if(frameID(f)==10) {
        auto* h=frameHierarchy(f);assert(h && h->parentFrame==f && h->interpolator && h->numNodes==2);
        assert(h->nodes[0].id==10 && h->nodes[1].id==20 && matrixArray(h)==h->matrices);
    }
    return failPlugin?nullptr:s;
}
static void* pluginDtor(void* p,int,int) { ++pluginDtors;return p; }
static rw::Stream* nativeWrite(rw::Stream* s,int,void*,int,int) { s->writeI32(123);return s; }
static int nativeSize(void*,int,int) { return 4; }
static std::vector<uint8_t> clumpFixture() {
    auto* c=rw::Clump::create();auto* root=rw::Frame::create();auto* child=rw::Frame::create();root->addChild(child);c->setFrame(root);
    int flags[2]={0,1},ids[2]={10,20};
    auto* h=rw::HAnimHierarchy::create(2,flags,ids,0x5000,36);h->parentFrame=root;
    rw::HAnimData::get(root)->id=10;rw::HAnimData::get(root)->hierarchy=h;rw::HAnimData::get(child)->id=20;
    auto* g=rw::Geometry::create(3,1,rw::Geometry::POSITIONS|rw::Geometry::NORMALS);
    auto* m=rw::Material::create();rw::MatFX::setEffects(m,rw::MatFX::ENVMAP);
    g->matList.appendMaterial(m);m->destroy();g->triangles[0]={{0,1,2},0};
    for(int i=0;i<3;i++) { g->morphTargets[0].vertices[i]={float(i),0,0};g->morphTargets[0].normals[i]={0,1,0}; }
    g->morphTargets[0].boundingSphere={{0,0,0},2};g->unlock();
    auto* skin=static_cast<rw::Skin*>(rw::Engine::memfuncs.rwmalloc(sizeof(rw::Skin),0));std::memset(skin,0,sizeof(*skin));
    skin->init(2,2,3);skin->numWeights=1;skin->usedBones[0]=0;skin->usedBones[1]=1;
    for(int i=0;i<2;i++) { rw::Matrix mat;mat.setIdentity();std::memcpy(skin->inverseMatrices+i*16,&mat,64); }
    for(int i=0;i<3;i++) { skin->indices[i*4]=i%2;skin->weights[i*4]=1; }
    rw::Skin::set(g,skin);
    auto* a=rw::Atomic::create();a->setGeometry(g,0);g->destroy();a->setFrame(root);c->addAtomic(a);
    rw::Skin::setHierarchy(a,h);rw::MatFX::enableEffects(a);rw::Skin::setPipeline(a,1);
    std::vector<uint8_t> bytes(c->streamGetSize()+12);rw::StreamMemory out;out.open(bytes.data(),uint32_t(bytes.size()));
    assert(c->streamWrite(&out));out.close();c->destroy();return bytes;
}
int main(int argc,char** argv) {
    assert(!newAnim(1,2,0,1) && !newInterp(1,36));
    for(int repeat=0;repeat<2;repeat++) {
        assert(init(nullptr,0,1024) && worldAttach() && animInit() && hanimAttach() && skinAttach() && matfxAttach() && compressedRegister());
        AnimationScheme custom{273,sizeof(IF),sizeof(KF),uintptr_t(customApply),0,uintptr_t(customInterpolate),0,0,uintptr_t(customRead),0,0,4};
        assert(registerScheme(&custom));
        assert(registerABIPlugin(Registry::Frame,4,0xffffff01,nullptr,pluginDtor)>=0);
        assert(registerABIPluginStream(Registry::Frame,0xffffff01,uintptr_t(gameRead))>=0);
        assert(rw::Frame::registerPlugin(0,0xffffff01,nullptr,nullptr,nullptr)>=0);
        rw::Frame::registerPluginStream(0xffffff01,nullptr,nativeWrite,nativeSize);
        assert(openEngine(nullptr) && start());
        auto* a=animation();auto* i=newInterp(2,sizeof(IF));assert(i && setAnim(i,a));
        auto* frames=static_cast<IF*>(i->frames());near(frames[0].t.x,0);assert(i->currentAnim==a && i->nextFrame);
        assert(setTime(i,1.5f));near(frames[0].t.x,15);near(frames[1].t.y,8);
        assert(setTime(i,0.5f));near(frames[0].t.x,5);assert(!i->nextFrame);
        assert(setTime(i,2.5f));near(frames[0].t.x,25);
        i->callbackTime=2.75f;i->timeCB=timeCB;i->timeData=i;i->loopCB=loopCB;i->loopData=i;
        timed=looped=0;assert(addTime(i,1));near(i->currentTime,0.5f);near(frames[0].t.x,5);
        assert(timed==1 && looped==1 && !i->timeCB);
        assert(setTime(i,3));near(i->currentTime,3);near(frames[0].t.x,30);
        auto* j=newInterp(2,sizeof(IF));assert(setAnim(j,a));assert(setTime(j,1));
        assert(copyInterp(j,i));near(j->currentTime,1);assert(j->currentAnim==a);near(static_cast<IF*>(j->frames())[0].t.x,30);
        assert(setTime(i,0));assert(blendInterp(j,j,i,0.5f));near(static_cast<IF*>(j->frames())[0].t.x,15);
        auto* sub=newSubInterp(i,1,1,-1);assert(sub && sub->parent==i && sub->offset==1 && sub->isSub);deleteAnim(sub);
        auto bytes=serialized(a);auto* read=decode(bytes);assert(read && numNodes(read)==2);assert(setAnim(j,read) && setTime(j,1.25f));near(static_cast<IF*>(j->frames())[0].t.x,12.5f);
        for(size_t n:{size_t(0),size_t(19),bytes.size()-1}) { auto shortBytes=bytes;shortBytes.resize(n);assert(!decode(shortBytes)); }
        auto bad=bytes;uint32_t invalid=3;std::memcpy(bad.data()+20+2*36+32,&invalid,4);assert(!decode(bad));
        auto* compressed=compress(a);assert(compressed && compressed->scheme->id==2 && numNodes(compressed)==2);
        assert(setAnim(j,compressed) && setTime(j,1.25f));near(static_cast<IF*>(j->frames())[0].t.x,12.5f,0.02f);
        auto packed=serialized(compressed);auto* reread=decode(packed);assert(reread && setAnim(j,reread) && setTime(j,1.25f));near(static_cast<IF*>(j->frames())[0].t.x,12.5f,0.02f);
        auto* flat=animation(1);for(int n=0;n<flat->numFrames;n++) static_cast<KF*>(flat->keyframes)[n].t={2,3,4};
        auto* flatPacked=compress(flat);auto* single=newInterp(1,sizeof(IF));assert(setAnim(single,flatPacked) && setTime(single,0.5f));near(static_cast<IF*>(single->frames())->t.z,4);
        auto* turning=animation(1);static_cast<KF*>(turning->keyframes)[1].q={0,0,0.70710678f,0.70710678f};
        assert(setAnim(single,turning) && setTime(single,0.5f));
        auto* pose=static_cast<IF*>(single->frames());near(pose->q.z,0.38268343f);near(pose->q.w,0.92387953f);
        rw::Matrix rotated;rotated.setIdentity();single->apply(&rotated,pose);near(rotated.right.x,0.70710678f);near(rotated.right.y,0.70710678f);
        auto* turningPacked=compress(turning);assert(setAnim(single,turningPacked) && setTime(single,0.5f));near(pose->q.z,0.38268343f);
        IF other=*pose;other.q=rw::scale(pose->q,-1);single->blend(pose,pose,&other,0.5f);near(other.q.z,pose->q.z);
        assert(encode(0)==0 && uint16_t(encode(1))==0x7800 && uint16_t(encode(-1))==0xf800 && uint16_t(encode(0.5f))==0x7000);
        std::vector<uint8_t> customBytes;for(uint32_t w:{256u,273u,2u,0u,0x3f800000u,0x40000000u}) word(customBytes,w);
        auto* ca=decode(customBytes);auto* ci=newInterp(1,sizeof(IF));assert(ca && customReads && setAnim(ci,ca) && setTime(ci,0.5f));near(static_cast<IF*>(ci->frames())->t.x,7);assert(customInterps>=2);
        int nf[2]={0,1},ids[2]={10,20};auto* h=newHierarchy(2,nf,ids,0x4000,sizeof(IF));assert(h && setAnim(h->interpolator,a));
        assert(update(h));near(matrixArray(h)[1].pos.x,1);
        auto* hc=copyHierarchy(h,2|0x1000,sizeof(IF));assert(hc && !hc->matrices && hc->nodes!=h->nodes && hc->nodes[1].id==20);
        auto* hs=newSub(h,1,0x4000,-1);assert(hs && hs->numNodes==1 && hs->nodes==h->nodes+1 && hs->matrices==h->matrices+1 && hs->rootParentOffset==0);
        assert(hs->interpolator->parent==h->interpolator);
        assert(setTime(h->interpolator,0.5f) && update(h));near(h->matrices[0].pos.x,5);
        assert(setAnim(hs->interpolator,flat) && update(hs));near(h->matrices[1].pos.x,7);near(h->matrices[0].pos.x,5);
        deleteHierarchy(hs);deleteHierarchy(hc);deleteHierarchy(h);
        auto clumpBytes=clumpFixture();Memory mem{clumpBytes.data()+12,uint32_t(clumpBytes.size()-12)};auto* stream=streamOpen(3,1,&mem);
        int beforeReads=pluginReads;auto* clump=readClump(stream);assert(clump && pluginReads==beforeReads+2);assert(streamClose(stream,nullptr));
        auto* root=static_cast<Frame*>(clump->object.parent);auto* hierarchy=frameHierarchy(root);assert(hierarchy && attach(hierarchy)==hierarchy);
        assert(hierarchy->nodes[0].frame==root && hierarchy->nodes[1].frame==root->child);
        assert(setAnim(hierarchy->interpolator,a) && setTime(hierarchy->interpolator,0.5f) && update(hierarchy));
        near(root->matrix.pos.x,5);near(root->child->matrix.pos.x,6);near(hierarchy->matrices[1].pos.x,11);
        auto* atomic=reinterpret_cast<Atomic*>(reinterpret_cast<char*>(clump->atomics.next)-offsetof(Atomic,inClump));
        assert(getSkin(atomic->geometry));auto* skin=getSkin(atomic->geometry);assert(skin->numBones==2 && skin->numUsedBones==2 && !isSplit(skin));
        auto* nativeSkin=rw::Skin::get(geometryNative(atomic->geometry));assert(skin->inverseMatrices==nativeSkin->inverseMatrices && skin->indices==nativeSkin->indices && skin->weights==nativeSkin->weights);
        int8_t remap=0;skin->remapIndices=&remap;assert(isSplit(skin));skin->remapIndices=nullptr;
        skin->inverseMatrices[12]=7;near(nativeSkin->inverseMatrices[12],7);skin->inverseMatrices[12]=0;
        assert(extension<Skin*>(atomic->geometry,Registry::Geometry,278)==skin);
        assert(setSkinHierarchy(atomic,hierarchy)==atomic && getSkinHierarchy(atomic)==hierarchy);
        assert(extension<Hierarchy*>(atomic,Registry::Atomic,278)==hierarchy && rw::Skin::getHierarchy(atomicNative(atomic))==hierarchyNative(hierarchy));
        assert(skinType(atomic)==1 && atomicEffects(atomic));auto* material=atomic->geometry->matList.materials[0];assert(materialEffects(material)==2);
        assert(envFrame(material,root)==material && envAlpha(material,1)==material);
        auto* fx=rw::MatFX::get(materialNative(material));assert(fx->getEnvFrame()==frameNative(root) && fx->getEnvFBAlpha());
        rw::MatFX::setEffects(materialNative(material),5);rw::Matrix uv;uv.setIdentity();assert(uvMatrices(material,&uv,nullptr)==material);
        rw::Matrix *base,*dual;fx->getUVTransformMatrices(&base,&dual);assert(base==&uv && !dual);
        auto* clone=cloneClump(clump);assert(clone);auto* cloneRoot=static_cast<Frame*>(clone->object.parent);auto* cloneH=frameHierarchy(cloneRoot);
        assert(cloneH && cloneH!=hierarchy && cloneH->parentFrame==cloneRoot && cloneH->nodes!=hierarchy->nodes);
        auto* cloneAtomic=reinterpret_cast<Atomic*>(reinterpret_cast<char*>(clone->atomics.next)-offsetof(Atomic,inClump));
        assert(getSkinHierarchy(cloneAtomic)==cloneH);
        assert(detach(hierarchy)==hierarchy && !hierarchy->nodes[0].frame && attach(hierarchy)==hierarchy);
        int childOffset=plugins(Registry::Frame).getPluginOffset(286),duplicateID=10,originalID=20;
        std::memcpy(reinterpret_cast<char*>(root->child)+childOffset,&duplicateID,4);hierarchy->nodes[1].id=10;
        assert(detach(hierarchy) && attach(hierarchy));assert(hierarchy->nodes[0].frame==root->child && hierarchy->nodes[1].frame==root->child);
        std::memcpy(reinterpret_cast<char*>(root->child)+childOffset,&originalID,4);hierarchy->nodes[1].id=20;assert(attach(hierarchy));
        // NOMATRICES still composes LTMs and respects update flags.
        auto* noMatrices=copyHierarchy(hierarchy,2|0x3000,sizeof(IF));noMatrices->parentFrame=root;
        for(int n=0;n<2;n++) noMatrices->nodes[n].frame=hierarchy->nodes[n].frame;
        assert(setAnim(noMatrices->interpolator,a) && update(noMatrices));near(root->child->ltm.pos.x,1);deleteHierarchy(noMatrices);
        assert(deleteClump(clump) && frameHierarchy(cloneRoot)==cloneH && getSkinHierarchy(cloneAtomic)==cloneH);assert(attach(cloneH)==cloneH);assert(deleteClump(clone));
        failPlugin=true;stream=streamOpen(3,1,&mem);assert(!readClump(stream));assert(streamClose(stream,nullptr));failPlugin=false;
        // Existing direct interpolators survive failed native graph publication.
        assert(setTime(i,0.5f));near(frames[0].t.x,5);
        for(int file=1;file<argc;file++) {
            auto* real=readAnim(argv[file]);assert(real);int nodes=numNodes(real);assert(nodes>0);
            auto* ri=newInterp(nodes,sizeof(IF));assert(ri && setAnim(ri,real));
            for(float fraction:{0.25f,0.75f,1.0f,0.5f,0.0f,1.5f}) {
                assert(setTime(ri,real->duration*fraction));
                for(int node=0;node<nodes;node++) {
                    auto& f=static_cast<IF*>(ri->frames())[node];
                    assert(std::isfinite(f.q.x) && std::isfinite(f.q.y) && std::isfinite(f.q.z) && std::isfinite(f.q.w));
                    assert(std::isfinite(f.t.x) && std::isfinite(f.t.y) && std::isfinite(f.t.z));
                }
            }
            deleteAnim(ri);deleteAnim(real);
        }
        for(auto* p:{i,j,single,ci}) deleteAnim(p);
        for(auto* anim:{a,read,compressed,reread,flat,flatPacked,turning,turningPacked,ca}) deleteAnim(anim);
        assert(animationFacadesEmpty());
        savedMalloc=rw::Engine::memfuncs.rwmalloc;rw::Engine::memfuncs.rwmalloc=failMalloc;
        failAllocation=0;assert(!newAnim(1,2,0,1));
        for(int point=0;point<2;point++) { failAllocation=point;assert(!newInterp(2,sizeof(IF))); }
        rw::Engine::memfuncs.rwmalloc=savedMalloc;assert(animationFacadesEmpty());
        assert(!readAnim("/nonexistent/csf-animation-fixture.anm"));
        assert(stop() && closeEngine() && term());
    }
    assert(pluginReads>=4 && pluginDtors>=8);
    std::puts("rw animation: streams, compressed keys, time/copy/blend, hierarchy/skin/MatFX/plugin/clone ABI ok");
}
