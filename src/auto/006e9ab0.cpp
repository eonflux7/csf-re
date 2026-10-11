#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee: returns a child node object (vtable slot 10 takes a float* output vector).
CSF_FUNC(int, __fastcall, GetBoneNodeForIndex, 0x006E8A50, (void* self, void* edx, int a2));
// Callee: returns a pointer to the 3-float offset block of the node (offset +0x10 is the translation).
CSF_FUNC(int, __fastcall, GetBoneOffsetBlock, 0x006E9880, (void* self, void* edx, int a2));

// Adds the vector a3 (via the node's virtual transform query) onto the translation block of the node.
static float* __fastcall C_00861e98_AddTranslatedVector(C_00861e98* self, void* /*edx*/, int a2, float* a3)
{
    // First: resolve the node object and ask it to write its vector into a3.
    int nodeObj = GetBoneNodeForIndex(self, nullptr, a2);
    void* node = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(nodeObj)));
    ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, node, 0)[10])(node, nullptr, a3);

    // Then: fetch the offset block and accumulate a3 into its translation (+0x10).
    int blockAddr = GetBoneOffsetBlock(self, nullptr, a2);
    uint8_t* block = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(blockAddr)));
    float* translation = reinterpret_cast<float*>(block + 0x10);

    translation[0] = translation[0] + a3[0];
    translation[1] = translation[1] + a3[1];
    translation[2] = translation[2] + a3[2];
    return translation;
}

HOOK(0x006E9AB0, C_00861e98_AddTranslatedVector, "auto");
