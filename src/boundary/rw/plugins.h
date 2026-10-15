#pragma once
#include "life.h"
#include <array>
#include <vector>
namespace csf_rw {
// Registration is implemented; registration is NOT decoding/animating a
// compressed keyframe. Game stream callbacks may only see facade streams.
struct AnimationScheme {
    uint32_t id, interpolatedSize, keyframeSize;
    uintptr_t apply, blend, interpolate, add, reciprocal, read, write, getSize;
    uint32_t customSize;
};
static_assert(sizeof(void*)!=4 || sizeof(AnimationScheme)==48);
const AnimationScheme* animationScheme(uint32_t);
void pluginBoundaryTerm();
std::vector<uint32_t> pluginStreamIDs(Registry, bool readsOnly=true);
bool readPluginPayload(Registry, uint32_t, Stream*, int length, void* facade);
bool writePluginPayload(Registry, uint32_t, Stream*, int length, void* facade);
int registerABIPluginStream(Registry, uint32_t, uintptr_t read, uintptr_t write=0, uintptr_t size=0);
int pluginPayloadSize(Registry, uint32_t, void* facade); // -1: no game size callback
}
