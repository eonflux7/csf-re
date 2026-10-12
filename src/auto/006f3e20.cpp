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

CSF_FUNC(int, __cdecl, RwFrameTransform, 0x00623860, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(uint32_t*, __fastcall, RpPrtStd_UpdateAtomicChild, 0x006F3A30, (void* self, void* edx, int* a2));

// Applies the frame transform of the atomic's frame, updates its frame objects,
// then runs the child update step.
static uint32_t* __fastcall C_00862748_ApplyFrameTransform(C_00862748* self, void* /*edx*/, int* a2)
{
    uint32_t frameField = self->field_0024;
    self->field_0028 = 0;

    uint8_t* frameBase = reinterpret_cast<uint8_t*>(frameField);
    int frame = static_cast<int>(CSF_AT(uint32_t, frameBase, 4));

    RwFrameTransform(frame, *a2, 0);
    RwFrameUpdateObjects(frame);
    return RpPrtStd_UpdateAtomicChild(self, nullptr, a2);
}

HOOK(0x006F3E20, C_00862748_ApplyFrameTransform, "auto");
