#include "globals.h"
#include "boundary/rw/animation.h"
#include "boundary/rw/world.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
using namespace csf_rw;
CSF_FUNC(int,__cdecl,init,0x00627360,(const MemoryFunctions*,char,int));
CSF_FUNC(int,__stdcall,worldAttach,0x0058D0B0,());
CSF_FUNC(int,__stdcall,matfxAttach,0x00571FF0,());
CSF_FUNC(int,__stdcall,morphAttach,0x005776E0,());
CSF_FUNC(int,__stdcall,pvsAttach,0x005865E0,());
CSF_FUNC(int,__cdecl,open,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,close,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
CSF_FUNC(Stream*,__cdecl,streamOpen,0x006298E0,(int,int,void*));
CSF_FUNC(int,__cdecl,streamClose,0x00629810,(Stream*,Memory*));
CSF_FUNC(uint32_t,__cdecl,read,0x006293E0,(Stream*,void*,uint32_t));
CSF_FUNC(Stream*,__cdecl,write,0x00629530,(Stream*,const void*,uint32_t));
CSF_FUNC(int,__cdecl,find,0x00629AE0,(Stream*,uint32_t,uint32_t*,uint32_t*));
CSF_FUNC(Clump*,__cdecl,clumpRead,0x0058FFC0,(Stream*));
CSF_FUNC(Clump*,__cdecl,clumpWrite,0x0058FDF0,(Clump*,Stream*));
CSF_FUNC(uint32_t,__cdecl,clumpSize,0x0058FD10,(Clump*));
CSF_FUNC(int,__cdecl,clumpDestroy,0x0058F740,(Clump*));
CSF_FUNC(TexDictionary*,__cdecl,txdRead,0x00630A20,(Stream*));
CSF_FUNC(TexDictionary*,__cdecl,txdWrite,0x006308E0,(TexDictionary*,Stream*));
CSF_FUNC(int,__cdecl,txdDestroy,0x00632340,(TexDictionary*));
CSF_FUNC(int,__cdecl,textureDestroy,0x006317E0,(Texture*));
CSF_FUNC(Frame*,__stdcall,frameCreate,0x006235D0,());
CSF_FUNC(Atomic*,__stdcall,atomicCreate,0x0058F5C0,());
CSF_FUNC(Atomic*,__cdecl,setGeometry,0x0058EFE0,(Atomic*,Geometry*,unsigned char));
CSF_FUNC(int,__cdecl,geometryDestroy,0x00590FC0,(Geometry*));
CSF_FUNC(Atomic*,__cdecl,enable,0x00571270,(Atomic*));
CSF_FUNC(Material*,__cdecl,effects,0x00571A80,(Material*,unsigned));
CSF_FUNC(Atomic*,__cdecl,morph,0x005774B0,(Atomic*,float));
CSF_FUNC(void,__cdecl,worldMatrix,0x0059B990,(rw::Matrix*));
CSF_FUNC(rw::RawMatrix*,__cdecl,composed,0x0059BA20,(rw::RawMatrix*));
CSF_FUNC(uint32_t,__cdecl,matrixSize,0x0063BD90,(const rw::Matrix*));
CSF_FUNC(rw::Matrix*,__cdecl,matrixWrite,0x0063BDA0,(rw::Matrix*,Stream*));
CSF_FUNC(int,__cdecl,animationSize,0x0061EF90,(Animation*));
CSF_FUNC(int,__cdecl,request,0x006269C0,(void*,int,void*,void*,int));
CSF_FUNC(int,__cdecl,setTexture,0x0062F950,(Texture*,unsigned));
CSF_FUNC(void*,__cdecl,registryRead,0x0063CB30,(void*,Stream*,void*));
CSF_FUNC(Sector*,__cdecl,pvsSet,0x005860C0,(World*,const rw::V3d*));
CSF_FUNC(World*,__cdecl,pvsUnhook,0x005861F0,(World*));
CSF_FUNC(World*,__cdecl,worldCreate,0x0058CCB0,(const rw::BBox*));
CSF_FUNC(int,__cdecl,worldDestroySlot,0x0058CBD0,(World*));
CSF_FUNC(int32_t,__stdcall,capture,0x007F78EC,(const void*,void**,void*));
CSF_GLOBAL(int,morphA,0x008B09E4);
CSF_GLOBAL(int,morphG,0x008B09E8);
CSF_GLOBAL(int,pvsW,0x008BE7A4);
CSF_GLOBAL(int,pvsS,0x008BE7A8);
static constexpr uint32_t id=0x43534601;
static int pluginOffset,reads,writes,geometryOffset,geometryWrites;
static bool failDictionaryRead;
static Stream* gameRead(Stream* s,int len,void* a,int off,int size) {
    assert(len==4 && off==pluginOffset && size==4); ++reads;
    return read(s,static_cast<char*>(a)+off,4)==4 && !failDictionaryRead?s:nullptr;
}
static Stream* gameWrite(Stream* s,int len,void* a,int off,int size) {
    assert(len==4 && off==pluginOffset && size==4); ++writes;
    return write(s,static_cast<char*>(a)+off,4);
}
static int gameSize(void*,int,int) { return 4; }
static Stream* geometryWrite(Stream* s,int len,void* a,int off,int size) {
    assert(len==4 && size==4 && off==geometryOffset);++geometryWrites;
    return write(s,static_cast<char*>(a)+off,4);
}
static Stream* geometryRead(Stream* s,int len,void* a,int off,int size) {
    assert(len==4 && size==4 && off==geometryOffset);
    return read(s,static_cast<char*>(a)+off,4)==4?s:nullptr;
}
static void word(std::vector<uint8_t>& v,uint32_t n) { for(int i=0;i<4;++i) v.push_back(uint8_t(n>>(8*i))); }
static void chunk(std::vector<uint8_t>& v,uint32_t id,const std::vector<uint8_t>& data) {
    word(v,id); word(v,data.size()); word(v,0x1c020037);v.insert(v.end(),data.begin(),data.end());
}
static TexDictionary* decode(std::vector<uint8_t>& bytes) {
    Memory memory{bytes.data(),uint32_t(bytes.size())}; auto* s=streamOpen(3,1,&memory);assert(s);
    assert(find(s,22,nullptr,nullptr)); auto* d=txdRead(s);assert(streamClose(s,nullptr));return d;
}
static void destroyDictionary(TexDictionary* d) {
    while(d->textures.next!=&d->textures) {
        auto* t=reinterpret_cast<Texture*>(reinterpret_cast<char*>(d->textures.next)-offsetof(Texture,inDict));
        assert(textureDestroy(t));
    }
    assert(txdDestroy(d));
}
static float advance(Atomic* a,float t) {
    struct Sequence { int flags; int16_t start,end;float duration; };
    Sequence* seq;std::memcpy(&seq,reinterpret_cast<char*>(a)+morphA,sizeof(seq));
    return t-seq->duration;
}
static void* failAlloc(size_t,rw::uint32) { return nullptr; }
int main(int argc,char** argv) {
    assert(init(nullptr,0,0)); assert(worldAttach() && matfxAttach() && morphAttach() && pvsAttach());
    pluginOffset=registerABIPlugin(Registry::Dictionary,4,id);assert(pluginOffset>=0);
    assert(registerABIPluginStream(Registry::Dictionary,id,uintptr_t(&gameRead),uintptr_t(&gameWrite),uintptr_t(&gameSize))>=0);
    geometryOffset=registerABIPlugin(Registry::Geometry,4,id);assert(geometryOffset>=0);
    assert(registerABIPluginStream(Registry::Geometry,id,uintptr_t(&geometryRead),uintptr_t(&geometryWrite),uintptr_t(&gameSize))>=0);
    assert(open(nullptr) && start());
    void* cap=reinterpret_cast<void*>(1);assert(uint32_t(capture(nullptr,&cap,nullptr))==0x88780078 && !cap);
    uint32_t device=0;assert(request(nullptr,22,&device,nullptr,0) && device==2);
    uint16_t shortDevice[2]={0,0xbeef};assert(request(nullptr,22,shortDevice,nullptr,0) && shortDevice[0]==2 && shortDevice[1]==0xbeef);
    assert(!request(nullptr,12345,&device,nullptr,0));
    assert(setTexture(nullptr,0) && !setTexture(nullptr,8));
    // PVS uses the ABI plugin records, not native world/sector pointers.
    rw::BBox bounds{{10,10,10},{-10,-10,-10}};
    auto* pw=worldCreate(&bounds);assert(pw);auto* ps=pw->root;
    auto* sectorBytes=reinterpret_cast<char*>(ps);auto* worldBytes=reinterpret_cast<char*>(pw);
    std::memcpy(sectorBytes+pvsS+28,&bounds,sizeof(bounds));
    int visible=123;std::memcpy(sectorBytes+pvsS+16,&visible,4);
    rw::V3d pos{1,2,3};assert(pvsSet(pw,&pos)==ps);
    std::memcpy(&visible,sectorBytes+pvsS+16,4);assert(!visible);
    int hooked=1;std::memcpy(worldBytes+pvsW+20,&hooked,4);
    SectorRender callback=nullptr;std::memcpy(worldBytes+pvsW+40,&callback,sizeof(callback));
    assert(pvsUnhook(pw)==pw && pw->sectorRender);
    std::memcpy(&hooked,worldBytes+pvsW+20,4);assert(!hooked);assert(worldDestroySlot(pw));
    auto* material=materialCreate();assert(material);
    for(unsigned i=0;i<=6;++i) { assert(effects(material,i)==material);assert(rw::MatFX::getEffects(materialNative(material))==i); }
    assert(!effects(material,7));assert(materialDestroy(material));
    material=materialCreate();auto saved=rw::Engine::memfuncs.rwmalloc;rw::Engine::memfuncs.rwmalloc=failAlloc;
    assert(!effects(material,1));rw::Engine::memfuncs.rwmalloc=saved;assert(materialDestroy(material));
    Animation a{};AnimationScheme scheme{};a.scheme=&scheme;a.numFrames=3;
    scheme.id=1;assert(animationSize(&a)==128);scheme.id=2;assert(animationSize(&a)==110);
    // Explicit non-symmetric device matrices detect multiplication order and
    // transpose mistakes without relying on the NULL camera's dummy matrices.
    rw::Camera camera{};rw::RawMatrix::setIdentity(&camera.devView);rw::RawMatrix::setIdentity(&camera.devProj);
    camera.devView.pos={2,3,4};camera.devProj.right.x=2;camera.devProj.up.y=3;camera.devProj.at.z=4;
    rw::engine->currentCamera=&camera;
    rw::Matrix m;m.setIdentity();m.pos={5,7,11};worldMatrix(&m);rw::RawMatrix out;
    assert(composed(&out)==&out);assert(out.right.x==2 && out.up.y==3 && out.at.z==4);
    assert(out.rightw==14 && out.upw==30 && out.atw==60 && out.posw==1);
    worldMatrix(nullptr);assert(composed(&out));assert(out.rightw==4 && out.upw==9 && out.atw==16);
    rw::engine->currentCamera=nullptr;
    Memory output{};auto* stream=streamOpen(3,2,&output);assert(matrixSize(&m)==64 && matrixWrite(&m,stream)==&m);
    assert(streamClose(stream,&output) && output.length==76);
    float savedPosition[3];std::memcpy(savedPosition,output.start+60,12);
    assert(savedPosition[0]==5 && savedPosition[1]==7 && savedPosition[2]==11);
    rw::Engine::memfuncs.rwfree(output.start);
    auto* clump=clumpCreate();auto* frame=frameCreate();assert(clump && frame);clumpSetFrame(clump,frame);
    auto* geometry=geometryCreate(3,1,rw::Geometry::POSITIONS);assert(geometry);
    for(int i=0;i<3;++i) geometry->morphTargets[0].vertices[i]={float(i),0,0};
    material=materialCreate();assert(geometryAppendMaterial(geometry,material));assert(materialDestroy(material));
    geometry->triangles[0]={{0,1,2},0};
    for(int i=0;i<2;++i) { auto* atomic=atomicCreate();assert(atomic && setGeometry(atomic,geometry,0));
        atomic->object.object.parent=frame;objectFrameChanged(&atomic->object);assert(clumpAddAtomic(clump,atomic));assert(enable(atomic)==atomic);
        struct Sequence { int flags;int16_t start,end;float duration; } sequence{0,0,0,2};
        struct G { int count;float (*advance)(Atomic*,float);Sequence* sequence; } g{1,advance,&sequence};
        std::memcpy(reinterpret_cast<char*>(geometry)+morphG,&g,sizeof(g));
        assert(morph(atomic,5)==atomic && atomic->interpolator.position==1 && atomic->interpolator.time==2);
        std::memset(reinterpret_cast<char*>(atomic)+morphA,0,3*sizeof(void*));
    }
    std::memset(reinterpret_cast<char*>(geometry)+morphG,0,3*sizeof(void*));assert(geometryDestroy(geometry));
    output={};stream=streamOpen(3,2,&output);assert(clumpWrite(clump,stream)==clump);assert(streamClose(stream,&output));
    assert(geometryWrites==1);
    assert(output.length==12+clumpSize(clump));
    stream=streamOpen(3,1,&output);assert(find(stream,16,nullptr,nullptr));auto* copy=clumpRead(stream);assert(copy);
    assert(clumpNative(copy)->countAtomics()==2);assert(streamClose(stream,nullptr));
    assert(clumpDestroy(copy) && clumpDestroy(clump));rw::Engine::memfuncs.rwfree(output.start);
    // A complete D3D9 native texture dictionary, including pixel payload and
    // a game-owned dictionary extension. No native owning header is published.
    std::vector<uint8_t> native(88);uint32_t platform=rw::PLATFORM_D3D9,format=0x500,d3d=21;
    std::memcpy(native.data(),&platform,4);native[4]=1;std::strcpy(reinterpret_cast<char*>(native.data()+8),"misc-fixture");
    std::memcpy(native.data()+72,&format,4);std::memcpy(native.data()+76,&d3d,4);
    native[80]=2;native[82]=2;native[84]=32;native[85]=1;native[86]=4;native[87]=1;
    word(native,16);for(int i=0;i<16;++i) native.push_back(uint8_t(i));
    std::vector<uint8_t> texture,body,header,extension,payload,bytes;
    chunk(texture,1,native);chunk(texture,3,{});word(header,1);chunk(body,1,header);chunk(body,21,texture);
    word(payload,42);chunk(extension,id,payload);chunk(body,3,extension);chunk(bytes,22,body);
    auto* dict=decode(bytes);assert(dict && reads==1);assert(*reinterpret_cast<int*>(reinterpret_cast<char*>(dict)+pluginOffset)==42);
    auto* nd=dictionaryNative(dict);assert(nd && nd->count()==1);auto* tex=rw::Texture::fromDict(nd->textures.link.next);
    auto* textureHeader=textureFacade(tex);assert(textureHeader && textureHeader->raster && textureHeader->raster->parent==textureHeader->raster);
    auto* pixels=tex->raster->lock(0,rw::Raster::LOCKREAD);assert(pixels && !std::memcmp(pixels,native.data()+92,16));tex->raster->unlock(0);
    output={};stream=streamOpen(3,2,&output);assert(txdWrite(dict,stream)==dict);assert(streamClose(stream,&output) && writes==1);
    std::vector<uint8_t> roundtrip(output.start,output.start+output.length);rw::Engine::memfuncs.rwfree(output.start);
    auto* second=decode(roundtrip);assert(second && reads==2);destroyDictionary(second);
    auto broken=bytes;broken.resize(broken.size()-1);assert(!decode(broken));
    int dictionariesBefore=rw::TexDictionary::numAllocated,texturesBefore=rw::Texture::numAllocated,rastersBefore=rw::Raster::numAllocated;
    failDictionaryRead=true;assert(!decode(bytes));failDictionaryRead=false;
    assert(rw::TexDictionary::numAllocated==dictionariesBefore && rw::Texture::numAllocated==texturesBefore && rw::Raster::numAllocated==rastersBefore);
    saved=rw::Engine::memfuncs.rwmalloc;rw::Engine::memfuncs.rwmalloc=failAlloc;assert(!decode(bytes));rw::Engine::memfuncs.rwmalloc=saved;
    Memory nativeMemory{texture.data(),uint32_t(texture.size())};stream=streamOpen(3,1,&nativeMemory);
    Texture* nativeFacade=nullptr;assert(readNativeTextureABI(stream,&nativeFacade,texture.size()) && nativeFacade && nativeFacade->raster);
    auto* textureToken=reinterpret_cast<void*>(CSF_ADDR_DATA(0x008A6410));
    assert(registryRead(textureToken,stream,nativeFacade)==textureToken);assert(streamClose(stream,nullptr));assert(textureDestroy(nativeFacade));
    // Direct registry slot returns the registry identity, not the object.
    Memory extMemory{body.data()+body.size()-extension.size()-12,uint32_t(extension.size()+12)};
    stream=streamOpen(3,1,&extMemory);
    auto* token=reinterpret_cast<void*>(CSF_ADDR_DATA(0x008A6428));assert(registryRead(token,stream,dict)==token);assert(streamClose(stream,nullptr));
    destroyDictionary(dict);
    // Real retail dictionaries can be supplied without checking them in.
    for(int i=1;i<argc;++i) { stream=streamOpen(2,1,argv[i]);assert(stream && find(stream,22,nullptr,nullptr));dict=txdRead(stream);assert(dict);assert(streamClose(stream,nullptr));destroyDictionary(dict); }
    assert(stop() && close() && term());
    std::puts("rw misc: matrices, MatFX, morph, clump/TXD roundtrips, plugin callbacks and failure returns PASS");
}
