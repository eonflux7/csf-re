#pragma once
#include "object.h"
#include "plugins.h"
namespace csf_rw {
// Original x86 headers, not librw owning headers. Pointer-width widening is
// solely for native tests; production offsets are asserted below.
struct Animation {
    const AnimationScheme* scheme;
    int numFrames, flags;
    float duration;
    void *keyframes, *customData;
};
struct Interpolator {
    Animation* currentAnim;
    float currentTime;
    void* nextFrame;
    int (__cdecl *timeCB)(Interpolator*,void*);
    void* timeData;
    float callbackTime;
    int (__cdecl *loopCB)(Interpolator*,void*);
    void* loopData;
    int maxSize, frameSize, keySize, numNodes, isSub, offset;
    Interpolator* parent;
    rw::AnimInterpolatorInfo::ApplyCB apply;
    rw::AnimInterpolatorInfo::BlendCB blend;
    rw::AnimInterpolatorInfo::InterpCB interpolate;
    rw::AnimInterpolatorInfo::AddCB add;
    void* frames() { return this+1; }
};
struct HAnimNode { int id,index,flags; Frame* frame; };
struct Hierarchy {
    int flags,numNodes;
    rw::Matrix* matrices;
    void* unaligned;
    HAnimNode* nodes;
    Frame* parentFrame;
    Hierarchy* parent;
    int rootParentOffset;
    Interpolator* interpolator;
};
struct CompressedKey { CompressedKey* prev; float time; uint16_t value[7], pad; };
struct Skin {
    int numBones,numUsedBones;
    uint8_t* usedBones;
    float* inverseMatrices;
    int numWeights;
    uint8_t* indices;
    float* weights;
    int boneLimit,numMeshes,rleSize;
    int8_t* remapIndices;
    rw::Skin::RLEcount* rleCount;
    rw::Skin::RLE* rle;
    void *platformData,*reserved,*data;
};
struct HAnimExtension { int id; Hierarchy* hierarchy; };
// Native-plugin publication/teardown seam. Called after native stream decoding,
// before game extension callbacks. No native owning header enters ABI tails.
void animationPublish(Registry,void* facade,void* native);
void animationForget(Registry,void* facade);
void* animationCopyFrame(void* dst,void* src,int offset,int size);
const AnimationScheme* builtinAnimationScheme(uint32_t);
rw::HAnimHierarchy* hierarchyNative(Hierarchy*);
bool animationFacadesEmpty(); // ownership audit for offline tests
#if UINTPTR_MAX == 0xffffffff
static_assert(sizeof(Animation)==24 && offsetof(Animation,keyframes)==0x10);
static_assert(sizeof(Interpolator)==76 && offsetof(Interpolator,frameSize)==36);
static_assert(offsetof(Interpolator,numNodes)==44 && offsetof(Interpolator,apply)==60);
static_assert(sizeof(Hierarchy)==36 && offsetof(Hierarchy,interpolator)==32);
static_assert(sizeof(HAnimNode)==16 && sizeof(CompressedKey)==24);
static_assert(sizeof(Skin)==64 && offsetof(Skin,indices)==20 && offsetof(Skin,remapIndices)==40);
#endif
}
