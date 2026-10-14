#pragma once
#include "life.h"
#include <array>
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
}
