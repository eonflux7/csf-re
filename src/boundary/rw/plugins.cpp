// Standalone-only plugin/registry ABI bridge. MIT librw, unmodified.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "plugins.h"
#include "particles.h"
#include "startup.h"
#include <cstring>
#include <array>
#include <unordered_map>
#include <cstdio>
CSF_GLOBAL(int, hanimOffset, 0x008BE558);
CSF_GLOBAL(int, skinEngine, 0x008B1240);
CSF_GLOBAL(int, skinAtomic, 0x008B1244);
CSF_GLOBAL(int, skinGeometry, 0x008B1248);
CSF_GLOBAL(int, matfxMaterial, 0x008B0984);
CSF_GLOBAL(int, matfxAtomic, 0x008B0994);
CSF_GLOBAL(int, matfxSector, 0x008B0998);
CSF_GLOBAL(int, ltEngine, 0x008BE560);
CSF_GLOBAL(int, ltAtomic, 0x008BE564);
CSF_GLOBAL(int, ltGeometry, 0x008BE568);
CSF_GLOBAL(int, ltSector, 0x008BE56C);
CSF_GLOBAL(int, ltWorld, 0x008BE570);
CSF_GLOBAL(int, ltMaterial, 0x008BE574);
CSF_GLOBAL(int, mipOffset, 0x008B09DC);
CSF_GLOBAL(int, mipStream, 0x008B09E0);
CSF_GLOBAL(int, adcGeometry, 0x008B0904);
CSF_GLOBAL(int, adcSector, 0x008B0908);
CSF_GLOBAL(int, collEngine, 0x008B0924);
CSF_GLOBAL(int, collAtomic, 0x008B0928);
CSF_GLOBAL(int, ptEngine, 0x008B09F0);
CSF_GLOBAL(int, ptAtomic, 0x008B09EC);
CSF_GLOBAL(int, prtEngine, 0x008BE6A0);
CSF_GLOBAL(int, prtAtomic, 0x008BE6A4);
CSF_GLOBAL(int, prtStream, 0x008BE6A8);
CSF_GLOBAL(int, advEngine, 0x008BE5E0);
CSF_GLOBAL(int, randomOffset, 0x008B0A14);
CSF_GLOBAL(int, patchWorld, 0x008BE7A4);
CSF_GLOBAL(int, patchSector, 0x008BE7A8);
CSF_GLOBAL(int, userGeometry, 0x008B1294);
CSF_GLOBAL(int, userGeometryStream, 0x008B1298);
CSF_GLOBAL(int, userSector, 0x008B129C);
CSF_GLOBAL(int, userSectorStream, 0x008B12A0);
CSF_GLOBAL(int, userFrame, 0x008B12A4);
CSF_GLOBAL(int, userFrameStream, 0x008B12A8);
CSF_GLOBAL(int, userCamera, 0x008B12AC);
CSF_GLOBAL(int, userCameraStream, 0x008B12B0);
CSF_GLOBAL(int, userLight, 0x008B12B4);
CSF_GLOBAL(int, userLightStream, 0x008B12B8);
CSF_GLOBAL(int, userMaterial, 0x008B12BC);
CSF_GLOBAL(int, userMaterialStream, 0x008B12C0);
CSF_GLOBAL(int, userTexture, 0x008B12C4);
CSF_GLOBAL(int, userTextureStream, 0x008B12C8);
CSF_GLOBAL(int, anisoOffset, 0x008B0918);
CSF_GLOBAL(int, anisoStream, 0x008B091C);
CSF_GLOBAL(int, morphAtomic, 0x008B09E4);
CSF_GLOBAL(int, morphGeometry, 0x008B09E8);
CSF_GLOBAL(int, worldCamera, 0x008B1348);
CSF_GLOBAL(int, worldAtomic, 0x008B134C);
CSF_GLOBAL(int, worldClump, 0x008B1350);
CSF_GLOBAL(int, worldLight, 0x008B1354);
CSF_GLOBAL(int, clumpCamera, 0x008B13B0);
CSF_GLOBAL(int, clumpLight, 0x008B13B4);
namespace {
// The game's 16-bit compressed format is not IEEE binary16: eleven mantissa
// bits, a biased four-bit exponent and saturation above magnitude one.
int __cdecl compressedFloat(float value) {
    uint32_t bits; std::memcpy(&bits,&value,4);
    unsigned exponent=(bits>>23)&255;
    if((bits&0xfff)>=0x800 && exponent>12 &&
       (exponent<254 || (bits&0x7ff000)!=0x7ff000)) {
        uint32_t increment=((exponent-12)<<23)|(bits&0x80000000);
        float delta; std::memcpy(&delta,&increment,4);
        value+=delta; std::memcpy(&bits,&value,4);
    }
    bits&=0xfffff000;
    if((bits&0x7f800000)>0x3f800000) bits=(bits&0x80000000)|0x7ffff000;
    if((bits&0x7f800000)<0x38000000) bits&=0x80000000;
    uint32_t result=(bits>>16)&0x8000;
    if(bits&0x7fffffff) result|=((bits>>12)&0x7ff)|(uint32_t((int32_t(bits)>>23)-16)<<11);
    return int(result);
}
using K=csf_rw::Registry;
using Scheme=csf_rw::AnimationScheme;
std::array<Scheme,16> schemes{};
unsigned schemeCount;
void* zero(void* p,int off,int size) { std::memset(static_cast<char*>(p)+off,0,size); return p; }
void* copyBytes(void* d,void* s,int off,int size) {
    std::memcpy(static_cast<char*>(d)+off,static_cast<char*>(s)+off,size); return d;
}
void* hanimCtor(void* p,int off,int n) { zero(p,off,n); int id=-1; std::memcpy(static_cast<char*>(p)+off,&id,4); return p; }
void* anisoCtor(void* p,int off,int n) { zero(p,off,n); static_cast<unsigned char*>(p)[off]=1; return p; }
void* mipCtor(void* p,int off,int n) { zero(p,off,n); uint16_t v=0x0fc0; std::memcpy(static_cast<char*>(p)+off,&v,2); return p; }
void* ltMaterialCtor(void* p,int off,int n) {
    zero(p,off,n); uint32_t values[5]={0,0x3f800000,0xffffffff,0x3f800000,0x3f800000};
    std::memcpy(static_cast<char*>(p)+off,values,sizeof(values)); return p;
}
void* patchSectorCtor(void* p,int off,int n) {
    zero(p,off,n); int id=-1, flags=3;
    std::memcpy(static_cast<char*>(p)+off,&id,4);
    std::memcpy(static_cast<char*>(p)+off+20,&flags,4); return p;
}
int reserve(K k,int n,uint32_t id,rw::Constructor c=zero) {
    return csf_rw::registerABIPlugin(k,n,id,c,nullptr,copyBytes);
}
bool put(int& cell,K k,int n,uint32_t id,rw::Constructor c=zero) {
    cell=reserve(k,n,id,c);
    if(csf_rw::startupTracing()) std::fprintf(stderr,"RW plugin id=%x registry=%u offset=%x bytes=%d\n",id,unsigned(k),cell,n);
    return cell>=0;
}
bool engine(int n,uint32_t id) { return reserve(K::Engine,n,id)>=0; }
bool attachable(K k,uint32_t id) {
    return rw::Engine::state==rw::Engine::Initialized && csf_rw::plugins(k).getPluginOffset(id)<0;
}
// World object and pipeline primitives are core librw classes, not an attach
// function. Rights/mesh/native-data are its optional world-facing components.
int __stdcall worldAttach() {
    csf_rw::startupTrace("0x0058D0B0","RpWorldPluginAttach");
    if(!attachable(K::Engine,1281)) return 0;
    for(auto p: {std::pair<int,uint32_t>{4,1281},{48,1282},{4,1283},{8,1284},{4,1285},{0,1290},{12,1287},{0,1291},{12,1289}})
        if(!engine(p.first,p.second)) return 0;
    if(!put(worldCamera,K::Camera,sizeof(void*)==8?32:28,1289) || !put(worldAtomic,K::Atomic,8,1289) ||
       !put(worldClump,K::Clump,8,1289) || !put(worldLight,K::Light,8,1289) ||
       !put(clumpCamera,K::Camera,3*sizeof(void*),16) || !put(clumpLight,K::Light,3*sizeof(void*),16)) return 0;
    // Zero-size registrations still have IDs/offsets and stream contracts.
    for(uint32_t id:{1294u,1296u}) {
        if(reserve(K::Sector,0,id)<0 || reserve(K::Geometry,0,id)<0) return 0;
    }
    if(reserve(K::Atomic,0,31)<0 || reserve(K::World,0,31)<0 ||
       reserve(K::Geometry,0,31)<0 || reserve(K::Material,0,31)<0) return 0;
    rw::registerMeshPlugin(); rw::registerNativeDataPlugin();
    rw::registerAtomicRightsPlugin(); rw::registerMaterialRightsPlugin();
    return csf_rw::attachStreamImageLifetime();
}
int __stdcall animInit() {
    csf_rw::startupTrace("0x0061ECD0","RtAnimInitialize");
    if(!attachable(K::Engine,439)) return 0;
    schemeCount=0; return engine(0,439);
}
int __stdcall hanimAttach() {
    csf_rw::startupTrace("0x0056DAE0","RpHAnimPluginAttach");
    if(!attachable(K::Engine,286) || !engine(0,286) || !put(hanimOffset,K::Frame,8,286,hanimCtor)) return 0;
    rw::registerHAnimPlugin(); return 1;
}
int __stdcall skinAttach() {
    csf_rw::startupTrace("0x00587450","RpSkinPluginAttach");
    if(!attachable(K::Engine,278) || !put(skinEngine,K::Engine,0,278) ||
       !put(skinAtomic,K::Atomic,4,278) || !put(skinGeometry,K::Geometry,4,278)) return 0;
    rw::registerSkinPlugin(); return 1;
}
int __stdcall matfxAttach() {
    csf_rw::startupTrace("0x00571FF0","RpMatFXPluginAttach");
    if(!attachable(K::Engine,288) || !engine(0,288) || !put(matfxMaterial,K::Material,4,288) ||
       !put(matfxAtomic,K::Atomic,4,288) || !put(matfxSector,K::Sector,4,288)) return 0;
    rw::registerMatFXPlugin(); return 1;
}
int __stdcall ltAttach() {
    csf_rw::startupTrace("0x0056F3C0","RpLtMapPluginAttach (storage only)");
    return attachable(K::Engine,276) && put(ltEngine,K::Engine,0,276) &&
        put(ltAtomic,K::Atomic,28,276) && put(ltGeometry,K::Geometry,8,276) &&
        put(ltSector,K::Sector,28,276) && put(ltWorld,K::World,8,276) && put(ltMaterial,K::Material,20,276,ltMaterialCtor);
}
int __stdcall mipAttach() {
    csf_rw::startupTrace("0x00577060","RpMipmapKLPluginAttach (storage only)");
    if(!attachable(K::Engine,272) || !engine(0,272) || !put(mipOffset,K::Texture,2,272,mipCtor)) return 0;
    mipStream=mipOffset; return 1;
}
int __stdcall adcAttach() {
    csf_rw::startupTrace("0x005663C0","RpADCPluginAttach");
    if(!attachable(K::Engine,308) || !engine(0,308) || !put(adcGeometry,K::Geometry,12,308) || !put(adcSector,K::Sector,12,308)) return 0;
    rw::ps2::registerADCPlugin(); return 1;
}
int __stdcall collisionAttach() {
    csf_rw::startupTrace("0x0056C1B0","RpCollisionPluginAttach (storage only)");
    return attachable(K::Engine,285) && put(collEngine,K::Engine,4,285) && put(collAtomic,K::Atomic,4,285);
}
int __stdcall ptAttach() {
    csf_rw::startupTrace("0x005816E0","RpPTankPluginAttach (storage only)");
    return attachable(K::Engine,303) && put(ptEngine,K::Engine,4,303) && put(ptAtomic,K::Atomic,sizeof(void*),303);
}
int __stdcall prtAttach() {
    csf_rw::startupTrace("0x0057C4A0","RpPrtStdPluginAttach (emitter ownership)");
    if(!attachable(K::Engine,304) || !put(prtEngine,K::Engine,0,304)) return 0;
    prtAtomic=csf_rw::registerABIPlugin(K::Atomic,sizeof(void*),304,csf_rw::particleAtomicCtor,
        csf_rw::particleAtomicDtor,csf_rw::particleAtomicCopy);
    if(prtAtomic<0) return 0;
    prtStream=prtAtomic; return 1;
}
int __stdcall advAttach() {
    csf_rw::startupTrace("0x005784A0","PrtAdv attach (not Rt2d; storage only)");
    return attachable(K::Engine,306) && put(advEngine,K::Engine,0,306);
}
struct RandomState { uint32_t* words; unsigned front,back; void* facade; } randomState{};
void publishRandom() {
#if UINTPTR_MAX == 0xffffffff
    auto& s=randomState;
    uintptr_t pointers[4]={reinterpret_cast<uintptr_t>(s.words),
        reinterpret_cast<uintptr_t>(s.words+s.front),reinterpret_cast<uintptr_t>(s.words+s.back),
        reinterpret_cast<uintptr_t>(s.words+31)};
    std::memcpy(static_cast<char*>(s.facade)+randomOffset,pointers,sizeof(pointers));
#endif
}
uint32_t __stdcall randomNext() {
    auto& s=randomState; if(!s.words) return 0; s.words[s.front]+=s.words[s.back];
    uint32_t v=s.words[s.front]>>1;
    s.front=(s.front+1)%31; s.back=(s.back+1)%31; publishRandom(); return v;
}
uint32_t __cdecl randomSeed(uint32_t v) {
    auto& s=randomState; if(!s.words) return 0; s.words[0]=v;
    for(unsigned i=1;i<31;i++) s.words[i]=s.words[i-1]*1103515245u+12345u;
    s.front=3;s.back=0;
    for(unsigned i=0;i<309;i++) randomNext();
    return randomNext();
}
void* randomCtor(void* p,int off,int n) {
    zero(p,off,n);
    auto* words=static_cast<uint32_t*>(rw::Engine::memfuncs.rwmalloc(124,0));
    if(!words) return nullptr;
    randomState={words,3,0,p}; randomSeed(0x9a319039u); return p;
}
void* randomDtor(void* p,int,int) {
    rw::Engine::memfuncs.rwfree(randomState.words); randomState={}; return p;
}
int __stdcall randomAttach() {
    csf_rw::startupTrace("0x005869E0","RtRandomPluginAttach");
    if(!attachable(K::Engine,268)) return 0;
    randomOffset=csf_rw::registerABIPlugin(K::Engine,16,268,randomCtor,randomDtor,nullptr);
    return randomOffset>=0;
}
int __stdcall patchAttach() {
    csf_rw::startupTrace("0x005865E0","RpPatchPluginAttach (storage only)");
    return attachable(K::World,298) && put(patchWorld,K::World,44,298) && put(patchSector,K::Sector,104,298,patchSectorCtor);
}
int __stdcall userAttach() {
    csf_rw::startupTrace("0x0058C040","RpUserDataPluginAttach");
    if(!attachable(K::Engine,287) || !engine(0,287) ||
       !put(userGeometry,K::Geometry,sizeof(rw::UserDataExtension),287) || !put(userSector,K::Sector,sizeof(rw::UserDataExtension),287) ||
       !put(userFrame,K::Frame,sizeof(rw::UserDataExtension),287) || !put(userCamera,K::Camera,sizeof(rw::UserDataExtension),287) ||
       !put(userLight,K::Light,sizeof(rw::UserDataExtension),287) || !put(userMaterial,K::Material,sizeof(rw::UserDataExtension),287) ||
       !put(userTexture,K::Texture,sizeof(rw::UserDataExtension),287)) return 0;
    userGeometryStream=userGeometry; userSectorStream=userSector; userFrameStream=userFrame;
    userCameraStream=userCamera;userLightStream=userLight;userMaterialStream=userMaterial;userTextureStream=userTexture;
    rw::registerUserDataPlugin(); return 1;
}
int __stdcall anisoAttach() {
    csf_rw::startupTrace("0x005665A0","RpAnisotPluginAttach");
    if(!attachable(K::Engine,295) || !engine(0,295) || !put(anisoOffset,K::Texture,1,295,anisoCtor)) return 0;
    anisoStream=anisoOffset; rw::registerAnisotropyPlugin(); return 1;
}
int __stdcall morphAttach() {
    csf_rw::startupTrace("0x005776E0","RpMorphPluginAttach (storage only)");
    return attachable(K::Engine,261) && engine(0,261) && put(morphAtomic,K::Atomic,12,261) && put(morphGeometry,K::Geometry,12,261);
}
int __cdecl registerScheme(const Scheme* p) {
    csf_rw::startupTrace("0x0061ED00","RtAnimRegisterInterpolationScheme");
    if(rw::Engine::state!=rw::Engine::Initialized || !p || schemeCount==schemes.size() ||
       !p->interpolatedSize || !p->keyframeSize || csf_rw::animationScheme(p->id)) return 0;
    schemes[schemeCount++]=*p; return 1;
}
int __stdcall compressedRegister() {
    csf_rw::startupTrace("0x006205C0","RtCompressedKeyFrameRegister (descriptor only)");
    // Original 12-word ABI; these callbacks are address-resolved game/library
    // identities. They are not invoked or cast to native librw stream callbacks.
    Scheme s{2,36,24,
        reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x0056DB70)),
        reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x0056E0A0)),
        reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x0061F6D0)),
        reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x0056E700)),
        reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x00620070)),
        reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x0061FF00)),
        reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x0061FFB0)),
        reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x00620060)),24};
    return registerScheme(&s);
}
// Retain game stream callback identities separately from librw's native stream
// callbacks. No stream decoder/adoption is installed by registration alone.
struct StreamCallbacks { uintptr_t read,write,size; };
std::unordered_map<uint64_t,StreamCallbacks> streamCallbacks;
int streamRegistration(K k,uint32_t id,uintptr_t r,uintptr_t w,uintptr_t s) {
    if(rw::Engine::state!=rw::Engine::Initialized) return -1;
    int off=csf_rw::plugins(k).getPluginOffset(id); if(off<0) return -1;
    try { streamCallbacks[(uint64_t(unsigned(k))<<32)|id]={r,w,s}; }
    catch(...) { return -1; } return off;
}
#define STREAM_FN(fn,kind,ea) int __cdecl fn(uint32_t id,uintptr_t r,uintptr_t w,uintptr_t s) { \
    csf_rw::startupTrace(ea,"RegisterPluginStream"); return streamRegistration(K::kind,id,r,w,s); }
STREAM_FN(atomicStream,Atomic,"0x0058F270")
STREAM_FN(sectorStream,Sector,"0x00590430")
STREAM_FN(geometryStream,Geometry,"0x00590BD0")
STREAM_FN(materialStream,Material,"0x005915C0")
STREAM_FN(lightStream,Light,"0x0059CEF0")
STREAM_FN(frameStream,Frame,"0x006329D0")
}
namespace csf_rw {
const AnimationScheme* animationScheme(uint32_t id) {
    for(unsigned i=0;i<schemeCount;i++) if(schemes[i].id==id) return &schemes[i];
    return nullptr;
}
void pluginBoundaryTerm() { schemeCount=0; streamCallbacks.clear(); randomState={}; }
std::vector<uint32_t> pluginStreamIDs(Registry k,bool readsOnly) {
    std::vector<uint32_t> ids;
    for(auto& p:streamCallbacks) if(unsigned(p.first>>32)==unsigned(k) && (!readsOnly || p.second.read))
        ids.push_back(uint32_t(p.first));
    return ids;
}
int registerABIPluginStream(Registry k,uint32_t id,uintptr_t r,uintptr_t w,uintptr_t s) {
    return streamRegistration(k,id,r,w,s);
}
int pluginPayloadSize(Registry k,uint32_t id,void* object) {
    auto it=streamCallbacks.find((uint64_t(unsigned(k))<<32)|id);
    if(it==streamCallbacks.end() || !it->second.size) return -1;
    FORLIST(l,plugins(k).plugins) {
        auto* p=LLLinkGetData(l,rw::Plugin,inParentList);
        if(p->id==id) {
            using Size=int (__cdecl *)(void*,int,int);
            return reinterpret_cast<Size>(it->second.size)(object,p->offset,p->size);
        }
    }
    return -1;
}
bool readPluginPayload(Registry k,uint32_t id,Stream* s,int length,void* object) {
    auto it=streamCallbacks.find((uint64_t(unsigned(k))<<32)|id);
    if(it==streamCallbacks.end() || !it->second.read) return false;
    FORLIST(l,plugins(k).plugins) {
        auto* p=LLLinkGetData(l,rw::Plugin,inParentList);
        if(p->id==id) {
            using Read=Stream* (__cdecl *)(Stream*,int,void*,int,int);
            return reinterpret_cast<Read>(it->second.read)(s,length,object,p->offset,p->size)!=nullptr;
        }
    }
    return false;
}
}
HOOK(0x0058D0B0,worldAttach,"boundary-rw");
HOOK(0x0061ECD0,animInit,"boundary-rw");
HOOK(0x0056DAE0,hanimAttach,"boundary-rw");
HOOK(0x00587450,skinAttach,"boundary-rw");
HOOK(0x00571FF0,matfxAttach,"boundary-rw");
HOOK(0x0056F3C0,ltAttach,"boundary-rw");
HOOK(0x00577060,mipAttach,"boundary-rw");
HOOK(0x005663C0,adcAttach,"boundary-rw");
HOOK(0x0056C1B0,collisionAttach,"boundary-rw");
HOOK(0x005816E0,ptAttach,"boundary-rw");
HOOK(0x0057C4A0,prtAttach,"boundary-rw");
HOOK(0x005784A0,advAttach,"boundary-rw");
HOOK(0x005869E0,randomAttach,"boundary-rw");
HOOK(0x005865E0,patchAttach,"boundary-rw");
HOOK(0x0058C040,userAttach,"boundary-rw");
HOOK(0x005665A0,anisoAttach,"boundary-rw");
HOOK(0x005776E0,morphAttach,"boundary-rw");
HOOK(0x006205C0,compressedRegister,"boundary-rw");
HOOK(0x0061F5E0,compressedFloat,"boundary-rw");
HOOK(0x0061ED00,registerScheme,"boundary-rw");
HOOK(0x0058F270,atomicStream,"boundary-rw");
HOOK(0x00590430,sectorStream,"boundary-rw");
HOOK(0x00590BD0,geometryStream,"boundary-rw");
HOOK(0x005915C0,materialStream,"boundary-rw");
HOOK(0x0059CEF0,lightStream,"boundary-rw");
HOOK(0x006329D0,frameStream,"boundary-rw");
HOOK(0x005867D0,randomNext,"boundary-rw");
HOOK(0x00586860,randomSeed,"boundary-rw");
#endif
