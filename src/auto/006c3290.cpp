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

// RtAnim keyframe compression: loads an animation from a source blob, converts
// it to compressed form when its first dword is 1, and, when the flag byte is
// set and the node count matches the table entry for the current index, runs
// the follow-up step at 0x6C2D10.

CSF_FUNC(char*, __cdecl, RtAnimAnimationRead, 0x0061F3C0, (int a1));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorDestroy, 0x0061EE90, (int a1));
CSF_FUNC(int, __cdecl, RtAnimAnimationGetNumNodes, 0x0061EFB0, (uint32_t* a1));
CSF_FUNC(char*, __cdecl, RtCompressedKeyConvertFromStd, 0x007C5D00, (int* a1));
CSF_FUNC(int, __fastcall, C_00861b58_GetTwo, 0x006B3D10, (C_00861b58*, void*));
CSF_FUNC(char, __fastcall, sub_6C2D10, 0x006C2D10, (void* self, void* edx, int a2));

static char __fastcall RecordObject_InitFromSource(S_006c20f0* self, void* /*edx*/,
                                                   uint8_t* sourceBlob, uint8_t* checkFlag)
{
    if (sourceBlob == nullptr)
        return 0;
    if (*sourceBlob == 0)
        return 0;

    int** animation = reinterpret_cast<int**>(RtAnimAnimationRead(reinterpret_cast<int>(sourceBlob)));
    self->field_0000 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(animation));
    if (animation == nullptr)
        return 0;

    if (**animation == 1) {
        uint32_t compressed = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(
            RtCompressedKeyConvertFromStd(reinterpret_cast<int*>(animation))));
        if (compressed != 0) {
            RtAnimInterpolatorDestroy(static_cast<int>(self->field_0000));
            self->field_0000 = compressed;
        }
    }

    if (*checkFlag != 0) {
        int twoIndex = C_00861b58_GetTwo(nullptr, nullptr);
        int expectedNodes = CSF_AT(int, CSF_ADDR_DATA(0x008A7E60), twoIndex * 4);
        int numNodes = RtAnimAnimationGetNumNodes(
            reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_0000)));
        if (numNodes == expectedNodes) {
            sub_6C2D10(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008A7E6C))));
        }
    }

    return 1;
}

HOOK(0x006C3290, RecordObject_InitFromSource, "auto");
