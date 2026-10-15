#include "globals.h"
#include "boundary/rw/plugins.h"
#include "boundary/rw/animation.h"
#include "boundary/rw/world.h"
#include <cassert>
#include <cstdio>
#include <cstring>
using namespace csf_rw;
CSF_FUNC(int,__cdecl,init,0x00627360,(const MemoryFunctions*,char,int));
CSF_FUNC(int,__cdecl,openEngine,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,closeEngine,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
#define ATTACH(name,ea) CSF_FUNC(int,__stdcall,name,ea,());
ATTACH(worldAttach,0x0058D0B0)
ATTACH(animInit,0x0061ECD0)
ATTACH(hanimAttach,0x0056DAE0)
ATTACH(skinAttach,0x00587450)
ATTACH(matfxAttach,0x00571FF0)
ATTACH(ltAttach,0x0056F3C0)
ATTACH(mipAttach,0x00577060)
ATTACH(adcAttach,0x005663C0)
ATTACH(collisionAttach,0x0056C1B0)
ATTACH(ptAttach,0x005816E0)
ATTACH(prtAttach,0x0057C4A0)
ATTACH(advAttach,0x005784A0)
ATTACH(randomAttach,0x005869E0)
ATTACH(patchAttach,0x005865E0)
ATTACH(userAttach,0x0058C040)
ATTACH(anisoAttach,0x005665A0)
ATTACH(morphAttach,0x005776E0)
ATTACH(compressedRegister,0x006205C0)
CSF_FUNC(int,__cdecl,schemeRegister,0x0061ED00,(const AnimationScheme*));
CSF_FUNC(int,__cdecl,atomicStream,0x0058F270,(uint32_t,uintptr_t,uintptr_t,uintptr_t));
CSF_FUNC(int,__cdecl,geometryStream,0x00590430,(uint32_t,uintptr_t,uintptr_t,uintptr_t));
CSF_FUNC(int,__cdecl,materialStream,0x005915C0,(uint32_t,uintptr_t,uintptr_t,uintptr_t));
CSF_FUNC(int,__cdecl,frameStream,0x006329D0,(uint32_t,uintptr_t,uintptr_t,uintptr_t));
CSF_FUNC(int,__cdecl,lightStream,0x0059CEF0,(uint32_t,uintptr_t,uintptr_t,uintptr_t));
CSF_FUNC(uint32_t,__stdcall,randomNext,0x005867D0,());
CSF_FUNC(uint32_t,__cdecl,randomSeed,0x00586860,(uint32_t));
CSF_FUNC(World*,__cdecl,worldCreate,0x0058CCB0,(const rw::BBox*));
CSF_FUNC(int,__cdecl,destroyWorld,0x0058CBD0,(World*));
CSF_FUNC(SectorRender,__cdecl,worldRender,0x0058CC80,(World*));
CSF_FUNC(World*,__cdecl,setSectorCallback,0x0058CC90,(World*,Sector* (__cdecl *)(Sector*,void*)));
CSF_FUNC(World*,__cdecl,addCamera,0x0058DD90,(World*,Camera*));
CSF_FUNC(World*,__cdecl,removeCamera,0x0058DDC0,(World*,Camera*));
CSF_FUNC(World*,__cdecl,cameraWorld,0x0058DDF0,(Camera*));
CSF_FUNC(World*,__cdecl,addLight,0x0058DF50,(World*,Light*));
CSF_FUNC(World*,__cdecl,removeLight,0x0058DFA0,(World*,Light*));
CSF_FUNC(World*,__cdecl,lightWorld,0x0058DFD0,(Light*));
CSF_FUNC(Camera*,__stdcall,cameraCreate,0x006290B0,());
CSF_FUNC(int,__cdecl,cameraDestroy,0x00629070,(Camera*));
CSF_FUNC(Light*,__cdecl,lightCreate,0x0059D040,(uint8_t));
CSF_FUNC(int,__cdecl,lightDestroy,0x0059D000,(Light*));
CSF_FUNC(Frame*,__stdcall,frameCreate,0x006235D0,());
CSF_FUNC(int,__cdecl,frameDestroy,0x00623B10,(Frame*));
CSF_FUNC(Texture*,__cdecl,textureCreate,0x00631780,(Raster*));
CSF_FUNC(int,__cdecl,textureDestroy,0x006317E0,(Texture*));
CSF_FUNC(Texture*,__cdecl,anisotropy,0x00566530,(Texture*,char));
static void* __cdecl failMalloc(size_t,rw::uint32) { return nullptr; }
static uint32_t word(void* p,int off) { uint32_t v;std::memcpy(&v,static_cast<char*>(p)+off,4);return v; }
int main() {
    assert(!worldAttach() && !randomNext());
    for(int repeat=0;repeat<2;repeat++) {
        assert(init(nullptr,0,1024));
        auto alloc=rw::Engine::memfuncs.rwmalloc;
        rw::Engine::memfuncs.rwmalloc=failMalloc;
        assert(!worldAttach()); // first descriptor failure leaves no reservation
        rw::Engine::memfuncs.rwmalloc=alloc;
        assert(worldAttach() && animInit() && hanimAttach() && skinAttach() && matfxAttach());
        assert(ltAttach() && mipAttach() && adcAttach() && collisionAttach() && ptAttach());
        assert(prtAttach() && advAttach() && randomAttach() && patchAttach() && userAttach());
        assert(anisoAttach() && morphAttach());
        assert(!worldAttach() && !skinAttach());
        // The game's 006bba20 plugin sizes/sequence, without truncating native
        // fixture callback pointers to the game's uint32_t descriptor fields.
        assert(registerABIPlugin(Registry::Engine,12,uint32_t(-256))>=300);
        for(auto k:{Registry::Atomic,Registry::Sector,Registry::Material,Registry::Frame,Registry::Light})
            assert(registerABIPlugin(k,8,uint32_t(-256))>=0);
        assert(atomicStream(-256,11,12,13)==plugins(Registry::Atomic).getPluginOffset(-256));
        assert(geometryStream(-256,11,12,13)==plugins(Registry::Sector).getPluginOffset(-256));
        assert(materialStream(-256,11,12,13)==plugins(Registry::Material).getPluginOffset(-256));
        assert(frameStream(-256,11,12,13)==plugins(Registry::Frame).getPluginOffset(-256));
        assert(lightStream(-256,11,12,13)==plugins(Registry::Light).getPluginOffset(-256));
        assert(atomicStream(999,0,0,0)==-1);
        assert(compressedRegister() && animationScheme(2)->keyframeSize==sizeof(CompressedKey));
        assert(!compressedRegister());
        AnimationScheme game{273,36,16,1,2,3,4,5,6,7,8,0};
        assert(schemeRegister(&game) && !schemeRegister(&game));
        assert(animationScheme(273)->apply==1 && animationScheme(273)->customSize==0);
#if UINTPTR_MAX == 0xffffffff
        assert(plugins(Registry::Atomic).getPluginOffset(-256)==184);
        assert(plugins(Registry::Sector).getPluginOffset(-256)==292);
        assert(plugins(Registry::Geometry).size==140);
        assert(plugins(Registry::Material).getPluginOffset(-256)==60);
        assert(plugins(Registry::Frame).getPluginOffset(-256)==180);
        assert(plugins(Registry::Light).getPluginOffset(-256)==92);
        assert(plugins(Registry::Camera).size==436 && plugins(Registry::World).size==168);
#endif
        assert(openEngine(nullptr) && start());
        assert(!anisoAttach() && atomicStream(-256,0,0,0)==-1);
        // Independent additive generator reference, original 31-word seed loop.
        uint32_t words[31]; words[0]=123;
        for(int i=1;i<31;i++) words[i]=words[i-1]*1103515245u+12345u;
        unsigned front=3,back=0; uint32_t expected=0;
        auto next=[&]() { words[front]+=words[back];auto r=words[front]>>1;front=(front+1)%31;back=(back+1)%31;return r; };
        for(int i=0;i<310;i++) expected=next();
        assert(randomSeed(123)==expected);
        for(int i=0;i<100;i++) assert(randomNext()==next());
        auto* f=frameCreate();assert(f);
        assert(word(f,plugins(Registry::Frame).getPluginOffset(286))==0xffffffff);
        assert(frameDestroy(f));
        auto* texture=textureCreate(nullptr);assert(texture);
        int aniso=plugins(Registry::Texture).getPluginOffset(295);
        assert(reinterpret_cast<unsigned char*>(texture)[aniso]==1);
        assert(anisotropy(texture,8)==texture && reinterpret_cast<unsigned char*>(texture)[aniso]==8);
        reinterpret_cast<unsigned char*>(texture)[aniso]=4;
        assert(textureNative(texture)->getMaxAnisotropy()==4 && textureDestroy(texture));
        rw::BBox box{rw::makeV3d(100,100,100),rw::makeV3d(-100,-100,-100)};
        auto* w=worldCreate(&box);assert(w && w->object.type==7 && w->root->type==-1);
        assert(w->root->bounds.sup.x==100 && worldRender(w)(w->root)==w->root);
    // Original default sector callback tests triangles, not vertex count.
    w->root->numVertices=1;assert(worldRender(w)(w->root)==w->root);w->root->numVertices=0;
    w->root->numTriangles=1;assert(!worldRender(w)(w->root));w->root->numTriangles=0;
        assert(setSectorCallback(w,nullptr)==w);
        assert(word(w->root,plugins(Registry::Sector).getPluginOffset(298))==0xffffffff);
        assert(word(w->root,plugins(Registry::Sector).getPluginOffset(298)+20)==3);
        auto* c=cameraCreate();auto* l=lightCreate(128);auto* ambient=lightCreate(2);assert(c&&l&&ambient);
        assert(addCamera(w,c)==w && cameraWorld(c)==w && !addCamera(w,c));
        assert(!worldDestroy(w) && !cameraDestroy(c));
        assert(removeCamera(w,c)==w && !cameraWorld(c) && !removeCamera(w,c));
        assert(addLight(w,l)==w && lightWorld(l)==w && w->localLights.next==&l->inWorld);
        assert(addLight(w,ambient)==w && w->globalLights.next==&ambient->inWorld);
        assert(!worldDestroy(w) && !lightDestroy(l) && !stop());
        assert(removeLight(w,l)==w && !lightWorld(l) && removeLight(w,ambient)==w);
        assert(cameraDestroy(c) && lightDestroy(l) && lightDestroy(ambient) && destroyWorld(w));
        assert(stop() && closeEngine() && term());
        assert(!animationScheme(2) && !randomNext());
    }
    std::puts("rw plugins/world: ok");
}
