#include "globals.h"
#include "boundary/rw/toolkit9.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <vector>
using O=csf_rw::ToolkitObject;
using D=csf_rw::ToolkitDescriptor;
using H=csf_rw::ToolkitHandle;
CSF_FUNC(void,__cdecl,noop,0x007E4970,(uint32_t));
CSF_FUNC(int,__cdecl,destroy,0x007F7900,(O*));
CSF_FUNC(int,__cdecl,dispatch,0x007F7BF0,(O*,rw::Matrix*,int,void*));
CSF_FUNC(D*,__cdecl,transform,0x007F8250,(O*,const rw::Matrix*));
CSF_FUNC(uintptr_t,__cdecl,setCB,0x007F8320,(O*,uintptr_t));
CSF_FUNC(uintptr_t,__cdecl,getCB,0x007F8330,(O*));
CSF_FUNC(void*,__cdecl,offset,0x007F8340,(O*,uint32_t));
CSF_FUNC(void*,__cdecl,multiplier,0x007F8390,(O*,uint32_t));
CSF_FUNC(O*,__cdecl,box,0x007F83E0,(O*,const void*));
CSF_FUNC(void*,__cdecl,dimensions,0x007F8410,(O*,void*));
CSF_FUNC(O*,__cdecl,sphere,0x007F9860,(O*,uint32_t));
CSF_FUNC(double,__cdecl,radius,0x007F9880,(O*));
CSF_FUNC(O*,__cdecl,capsule,0x007F9B40,(O*,uint32_t,uint32_t));
CSF_FUNC(void*,__cdecl,vector,0x007FAFE0,(void*,const void*));
CSF_FUNC(void*,__cdecl,budget8,0x007FB000,(void*,uint32_t));
CSF_FUNC(void*,__cdecl,budget4,0x007FB010,(void*,uint32_t));
CSF_FUNC(void*,__cdecl,config12,0x007FB020,(void*,float));
CSF_FUNC(void*,__cdecl,config20,0x007FB030,(void*,float));
CSF_FUNC(void*,__cdecl,config16,0x007FB040,(void*,float));
CSF_FUNC(void*,__cdecl,config24,0x007FB050,(void*,float));
CSF_FUNC(int,__cdecl,destroyH,0x007FD9C0,(H*));
namespace {
D boxD{}, sphereD{}, capsuleD{};
std::vector<void*> freed;
void __cdecl freeRecord(void* p) { freed.push_back(p); std::free(p); }
int destructed=0;
void __cdecl objectDtor(O*) { ++destructed; }
rw::Matrix received;
rw::Matrix* expectedIdentity;
int __cdecl output(O* o,rw::Matrix* m,int mode,void* dst) {
    assert(o && mode==17 && dst==&received);
    if(expectedIdentity) assert(m==expectedIdentity);
    if(m) received=*m;
    return 123;
}
uint32_t bits(float f) { uint32_t v; std::memcpy(&v,&f,4); return v; }
template<class T> T read(const void* p,size_t n) { T t;std::memcpy(&t,static_cast<const char*>(p)+n,sizeof t);return t; }
template<class T> void write(void* p,size_t n,T t) {std::memcpy(static_cast<char*>(p)+n,&t,sizeof t);}
void* alignedBlock(size_t n,void** base) {
    *base=std::malloc(n+32); assert(*base);
    auto raw=reinterpret_cast<uintptr_t>(*base);
    auto p=(raw+16)&~uintptr_t(15);
    reinterpret_cast<unsigned char*>(p)[-1]=static_cast<unsigned char>(p-raw);
    return reinterpret_cast<void*>(p);
}
}
template<> unsigned char* const csf::gv<0x00884E38>::d=reinterpret_cast<unsigned char*>(&boxD);
template<> unsigned char* const csf::gv<0x00884EF8>::d=reinterpret_cast<unsigned char*>(&sphereD);
template<> unsigned char* const csf::gv<0x00884F88>::d=reinterpret_cast<unsigned char*>(&capsuleD);

int main() {
    noop(400);
    boxD.type=5; sphereD.type=2; capsuleD.type=3;
    O o; std::memset(&o,0xa5,sizeof o);
    o.matrix.flags=0x400;
    float v[3]={2,3,4}, result[3];
    assert(box(&o,v)==&o && o.descriptor==&boxD);
    assert(dimensions(&o,result)==result && !std::memcmp(v,result,12));
    assert(o.matrix.flags==0x20403 && o.matrix.right.x==1 && o.matrix.pos.z==0);
    assert(o.multiplier==bits(.2f) && o.offset==bits(.2f));
    assert(o.control0==0 && o.control1==0);
    assert(read<uint32_t>(&o.matrix,28)==0xa5a5a5a5); // untouched padding
    assert(sphere(&o,bits(7))==&o && o.descriptor==&sphereD && radius(&o)==7);
    assert(capsule(&o,bits(6),bits(8))==&o && o.descriptor==&capsuleD);
    dimensions(&o,result); assert(result[0]==8 && result[1]==0 && result[2]==0 && radius(&o)==6);
    assert(setCB(&o,0x12345)==0x12345 && getCB(&o)==0x12345);
    rw::Matrix m{}; m.setIdentity(); m.pos={5,6,7}; m.flags=0;
    assert(transform(&o,&m)==&capsuleD && !std::memcmp(&o.matrix,&m,64));
    capsuleD.flags=2; m.pos.x=99;
    assert(transform(&o,&m)==&capsuleD && o.matrix.pos.x==5);
    capsuleD.flags=0;
    capsuleD.methods[3]=reinterpret_cast<void*>(&output);
    expectedIdentity=&o.matrix;
    assert(dispatch(&o,nullptr,17,&received)==123 && received.pos.x==5);
    expectedIdentity=nullptr;
    m.pos={1,2,3};
    assert(dispatch(&o,&m,17,&received)==123);
    assert(received.pos.x==6 && received.pos.y==8 && received.pos.z==10);
    expectedIdentity=&m;
    o.matrix.flags|=0x20000; assert(dispatch(&o,&m,17,&received)==123);
    o.matrix.flags=0; capsuleD.flags=2; assert(dispatch(&o,&m,17,&received)==123);
    capsuleD.flags=0;
    D groupD{};groupD.type=8;
    O leaves[2]{}; leaves[0].descriptor=&boxD;leaves[1].descriptor=&sphereD;
    csf_rw::ToolkitChildren child{leaves,2}, empty{nullptr,0};
    O branch{}; branch.descriptor=&groupD;write(&branch.payload,0,&child);
    csf_rw::ToolkitChildren outerChildren{&branch,1};
    O outer{};outer.descriptor=&groupD;write(&outer.payload,0,&outerChildren);
    assert(multiplier(&outer,bits(.5f))==&outerChildren);
    assert(offset(&outer,bits(.7f))==&outerChildren);
    for(auto& leaf:leaves) assert(leaf.multiplier==bits(.5f) && leaf.offset==bits(.7f));
    assert(outer.multiplier==0 && branch.offset==0);
    write(&branch.payload,0,&empty);assert(offset(&branch,42)==&empty);
    unsigned char config[1060];std::memset(config,0xa5,sizeof config);
    assert(budget8(config,600)==config && budget4(config,400)==config);
    assert(config12(config,7)==config && config20(config,200)==config);
    assert(config16(config,.2f)==config && config24(config,5000)==config);
    assert(vector(config,v)==config);
    assert(read<uint32_t>(config,4)==400 && read<uint32_t>(config,8)==600);
    assert(read<float>(config,12)==7 && read<float>(config,16)==.2f);
    assert(read<float>(config,20)==200 && read<float>(config,24)==5000);
    assert(!std::memcmp(config+76,v,12) && read<uint32_t>(config,28)==0xa5a5a5a5);
    // Use the exact game engine allocator dispatch, recording release order.
    uintptr_t engineSlots[75]{};engineSlots[67]=reinterpret_cast<uintptr_t>(&freeRecord);
    *reinterpret_cast<void**>(csf::gv<0x008B68C0>::d)=engineSlots;
    void* base;auto owned=static_cast<O*>(alignedBlock(sizeof(O),&base));
    *owned=O{};owned->descriptor=&boxD;boxD.methods[0]=reinterpret_cast<void*>(&objectDtor);
    assert(destroy(owned)==1 && destructed==1 && freed.back()==base);
    auto h=static_cast<H*>(std::calloc(1,sizeof(H))); assert(h);
    h->configuration=std::malloc(1060);
    void *base0,*base1,*base2;
    write(h->configuration,1036,alignedBlock(50,&base0));
    write(h->configuration,1048,alignedBlock(60,&base1));
    h->workspace=alignedBlock(100,&base2);
    h->entries=std::malloc(8);h->scratch=std::malloc(8);h->nodes=std::malloc(8);
    h->count=2;
    std::vector<void*> expected={base0,base1,h->configuration,base2,h->entries,h->scratch,h->nodes,h};
    freed.clear();assert(destroyH(h)==1 && freed==expected);
    *reinterpret_cast<void**>(csf::gv<0x008B68C0>::d)=nullptr;
}
