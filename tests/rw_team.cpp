// Historical name of the job. Binary contracts here are physics, not RpTeam.
#include "globals.h"
#include "paths.h"
#include "boundary/rw/toolkit9.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <fstream>
using namespace csf_rw;
CSF_FUNC(double,__cdecl,massProperties,0x007FA460,(PhysicsMass*,float,int));
CSF_FUNC(ToolkitObject*,__cdecl,defaultShape,0x007FA5B0,(ToolkitObject*));
CSF_FUNC(float*,__cdecl,timeStep,0x007FAFD0,(float*,float));
CSF_FUNC(void*,__cdecl,dictionary,0x007FDAD0,(ToolkitHandle*));
CSF_FUNC(uint32_t*,__cdecl,bitMatrix,0x007FDB00,(uint32_t,uint32_t));
CSF_FUNC(int,__cdecl,freeMatrix,0x007FDB40,(void*));
CSF_FUNC(PhysicsDictionary*,__cdecl,enable,0x007FEE10,(PhysicsDictionary*,PhysicsBody*,int));
CSF_FUNC(PhysicsBody*,__cdecl,activate,0x007FEE60,(PhysicsDictionary*,PhysicsBody*));
CSF_FUNC(PhysicsBody*,__cdecl,deactivate,0x007FEF10,(PhysicsDictionary*,PhysicsBody*));
CSF_FUNC(void*,__cdecl,boundsQuery,0x007FEDC0,(PhysicsDictionary*,PhysicsBody*,void*));
CSF_FUNC(PhysicsDictionary*,__cdecl,collisionMatrix,0x007FF0A0,(PhysicsDictionary*,void*));
CSF_FUNC(void*,__cdecl,links,0x007FF0C0,(PhysicsBody*));
CSF_FUNC(uint32_t*,__cdecl,parameters,0x007FFB90,(uint32_t*));
CSF_FUNC(uint32_t*,__cdecl,limits,0x007FFBF0,(uint32_t*,uint32_t,uint32_t,uint32_t,uint32_t));
CSF_FUNC(uint32_t*,__cdecl,pair,0x007FFC20,(uint32_t*,uint32_t));
CSF_FUNC(float*,__cdecl,scaleBounds,0x008006D0,(float*,float));
CSF_FUNC(uintptr_t,__cdecl,word,0x008013C0,(void*));
CSF_FUNC(ToolkitObject*,__cdecl,child,0x008013D0,(ToolkitChildren*,uint32_t));
CSF_FUNC(void*,__cdecl,target,0x00801520,(ToolkitObject*));
CSF_FUNC(PhysicsElement*,__cdecl,force,0x00802410,(PhysicsElement*,const rw::V3d*,const rw::V3d*));
CSF_FUNC(double,__cdecl,dim68,0x00802650,(ToolkitObject*));
CSF_FUNC(double,__cdecl,dim64,0x00802660,(ToolkitObject*));
CSF_FUNC(void*,__cdecl,transformPart,0x00803770,(PhysicsBody*,uint16_t));
namespace {
ToolkitDescriptor boxDescriptor{},sphereDescriptor{},capsuleDescriptor{};
int starts,stops;bool canStart=true,allocFail=false;void* freed;double volume=8;
int __cdecl start(PhysicsBody*) { ++starts;return canStart; }
void __cdecl stop(PhysicsBody*) { ++stops; }
void* __cdecl allocate(uint32_t n,uint32_t size,uint32_t tag) { assert(tag==198912);return allocFail?nullptr:std::calloc(n,size); }
void __cdecl release(void* p) { freed=p;std::free(p); }
double __cdecl shapeProperties(ToolkitObject*,PhysicsShapeProperties* p,rw::V3d* c) {
    p->inertia={2,3,4};p->rotation={0,0,0,1};*c={1,2,3};return volume;
}
}
template<> unsigned char* const csf::gv<0x00884E38>::d=reinterpret_cast<unsigned char*>(&boxDescriptor);
template<> unsigned char* const csf::gv<0x00884EF8>::d=reinterpret_cast<unsigned char*>(&sphereDescriptor);
template<> unsigned char* const csf::gv<0x00884F88>::d=reinterpret_cast<unsigned char*>(&capsuleDescriptor);
int main() {
    uintptr_t engine[75]{};engine[69]=reinterpret_cast<uintptr_t>(&allocate);engine[67]=reinterpret_cast<uintptr_t>(&release);
    *reinterpret_cast<void**>(csf::gv<0x008B68C0>::d)=engine;
    auto bits=bitMatrix(33,3);assert(bits && bits[0]==33 && bits[1]==3 && bits[2]==4);
    for(int i=3;i<7;++i) assert(bits[i]==0);
    bits[3]|=1u<<4;assert(freeMatrix(bits)==1 && freed==bits);
    auto zero=bitMatrix(0,3);assert(zero && zero[2]==0);freeMatrix(zero);
    allocFail=true;assert(!bitMatrix(3,3));allocFail=false;
    uint32_t p[18];for(auto& v:p) v=0xabababab;
    assert(parameters(p)==p && p[0]==~0u && p[1]==3 && p[7]==100 && p[12]==1000 && p[16]==0xabababab);
    assert(limits(p,1,2,3,4)==p && p[13]==1 && p[12]==2 && p[14]==3 && p[15]==4);
    assert(pair(p,150)==p && p[10]==150 && p[11]==150);
    float dt=4;assert(timeStep(&dt,.125f)==&dt && dt==.125f);
    ToolkitHandle h{};h.workspace=p;h.configuration=&dt;
    assert(dictionary(&h)==p && word(&h)==reinterpret_cast<uintptr_t>(&dt));
    ToolkitObject objects[3]{};ToolkitChildren children{objects,3};
    assert(word(&children)==3 && child(&children,2)==objects+2);
    void* value=&children;std::memcpy(objects[0].payload,&value,sizeof value);
    assert(target(objects)==value);
    float dimensions[]={4,9};std::memcpy(objects[0].payload,dimensions,sizeof dimensions);
    assert(dim64(objects)==4 && dim68(objects)==9);
    float bounds[]={-2,-4,-6,99,2,4,6,88};
    assert(scaleBounds(bounds,2)==bounds && bounds[0]==-4 && bounds[6]==12 && bounds[3]==99 && bounds[7]==88);
    scaleBounds(bounds,0);assert(bounds[0]==0 && bounds[6]==0);
    ToolkitObject o;std::memset(&o,0xa5,sizeof o);o.matrix.flags=0;
    assert(defaultShape(&o)==&o && o.matrix.flags==0x20003 && o.matrix.pos.x==0);
    assert(o.payload[0]==0xa5 && o.payload[12]==0 && o.control0==0);
    ToolkitDescriptor desc{};desc.methods[2]=reinterpret_cast<void*>(&shapeProperties);
    o.descriptor=&desc;o.matrix.setIdentity();o.matrix.pos={10,20,30};
    PhysicsMass bodyMass{};bodyMass.shape=&o;
    assert(massProperties(&bodyMass,-2,1)==2 && bodyMass.mass==16);
    assert(bodyMass.center.x==11 && bodyMass.center.z==33 && bodyMass.inertia.z==64 && bodyMass.flags==2);
    assert(massProperties(&bodyMass,4,0)==.5 && bodyMass.sphericalInertia==16 && bodyMass.flags==0);
    bodyMass.sphericalInertia=91;bodyMass.flags=2;
    massProperties(&bodyMass,0,0);assert(bodyMass.sphericalInertia==91 && bodyMass.flags==2);
    massProperties(&bodyMass,1.1754944e-38f,0);assert(bodyMass.sphericalInertia==91 && bodyMass.flags==2);
    massProperties(&bodyMass,1.1754944e-38f,1);assert(bodyMass.inertia.z==4);
    desc.flags=2;massProperties(&bodyMass,1,1);assert(bodyMass.center.x==1);
    volume=0;assert(massProperties(&bodyMass,1,1)==double(3.4028234663852886e38f));volume=8;

    uint32_t enabled[3]{},active[3]{};PhysicsDictionary d{};
    d.enabled=enabled;d.active=active;d.activeLimit=1;
    void* callbacks[7]{};callbacks[4]=reinterpret_cast<void*>(&start);callbacks[5]=reinterpret_cast<void*>(&stop);
    PhysicsBody b{};b.methods=callbacks;b.id=33;b.owner=&d;b.links=p;
    assert(links(&b)==p && enable(&d,&b,1)==&d && enabled[1]==2);
    enable(&d,&b,0);assert(enabled[1]==0);
    assert(activate(&d,&b)==&b && d.activeCount==1 && active[1]==2 && starts==1);
    assert(activate(&d,&b)==&b && starts==1);
    assert(deactivate(&d,&b)==&b && d.activeCount==0 && active[1]==0 && stops==1);
    deactivate(&d,&b);assert(stops==1);
    canStart=false;assert(!activate(&d,&b) && starts==2 && d.activeCount==0);
    assert(*reinterpret_cast<int*>(csf::gv<0x008BE0D4>::d)==1);
    d.activeLimit=0;assert(!activate(&d,&b) && starts==2);
    d.activeLimit=1;canStart=true;assert(activate(&d,&b)==&b);
    assert(*reinterpret_cast<int*>(csf::gv<0x008BE0D4>::d)==0);
    PhysicsSpatial spatial{};uint32_t bounded[3]{};d.spatial=&spatial;d.bounded=bounded;
    assert(collisionMatrix(&d,p)==&d && spatial.collisionMatrix==p);
    float queried[8]={0,0,0,19,0,0,0,29};
    assert(!boundsQuery(&d,&b,queried) && queried[3]==19);
    float stored[8]={1,2,3,0,4,5,6,0};spatial.margin=.5f;
    std::memcpy(spatial.storage+32*b.id,stored,sizeof stored);bounded[1]=2;
    assert(boundsQuery(&d,&b,queried)==queried && queried[0]==.5f && queried[6]==6.5f);
    assert(queried[3]==19 && queried[7]==29);

    rw::Matrix matrices[2];matrices[0].setIdentity();matrices[1].setIdentity();matrices[1].pos={1,2,3};
    rw::Quat rotations[2]{};float forces[16]{};PhysicsState state{};
    state.matrices=matrices;state.rotations=rotations;state.forces=forces;
    PhysicsAggregate aggregate{};aggregate.state=&state;PhysicsElement e{&aggregate,1,{4,5,6}};
    rw::V3d f{2,0,0},at{1,3,3};assert(force(&e,&f,&at)==&e);
    assert(forces[8]==2 && forces[14]==-2 && forces[0]==0 && forces[11]==0 && forces[15]==0);
    force(&e,&f,&at);assert(forces[8]==4 && forces[14]==-4);
    rw::Matrix source;source.setIdentity();source.pos={10,20,30};
    PhysicsPart part{0,&e,&source};b.parts=&part;
    assert(transformPart(&b,0)==&e && matrices[1].pos.x==14 && matrices[1].pos.z==36);
    assert(std::fabs(rotations[1].w-1)<1e-6);
    // Real mission data: feed the first clump's serialized frame matrices
    // through the physics-part adapter. This is not a world/clump reader test.
    const char* frames=std::getenv("CSF_RW_TEAM_FRAMES");
    const std::string fixture=frames?frames:paksPath("Ransom/Maps/FR01/FR01.rws");
    std::ifstream asset(fixture,std::ios::binary);
    if(asset) {
        uint32_t header[16]{};asset.read(reinterpret_cast<char*>(header),sizeof header);
        assert(asset && header[0]==0x10 && header[9]==0xe && header[12]==1);
        unsigned count=header[15];assert(count>0 && count<10000);
        for(unsigned i=0;i<count;++i) {
            float basis[12];uint32_t tail[2];
            asset.read(reinterpret_cast<char*>(basis),sizeof basis);
            asset.read(reinterpret_cast<char*>(tail),sizeof tail);assert(asset);
            source.right={basis[0],basis[1],basis[2]};source.up={basis[3],basis[4],basis[5]};
            source.at={basis[6],basis[7],basis[8]};source.pos={basis[9],basis[10],basis[11]};source.flags=0;
            assert(transformPart(&b,0)==&e);
            for(int axis=0;axis<3;++axis) {
                float expected=basis[9+axis]+4*basis[axis]+5*basis[3+axis]+6*basis[6+axis];
                assert(std::fabs(reinterpret_cast<float*>(&matrices[1].pos)[axis]-expected)<.005f);
            }
            assert(std::fabs(rw::normsq(rotations[1])-1)<.01f);
        }
        std::printf("physics part transforms: %u FR01 serialized frames passed\n",count);
    } else std::puts("FR01 frame fixture unavailable (synthetic tests still run)");
    deactivate(&d,&b);assert(transformPart(&b,0)==active);
    b.owner=nullptr;assert(!transformPart(&b,0));
    puts("physics boundary: 23 slots passed");
}
