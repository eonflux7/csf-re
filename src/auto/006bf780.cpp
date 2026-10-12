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

CSF_FUNC(char, __fastcall, GfxFrames_ProcessFrameLink, 0x006BF380, (void* self, void* edx, int a2));

static char __fastcall C_00860700_DispatchToChildFrame(C_00860700* self, void* /*edx*/, int a2)
{
    uint8_t* linkedObject = reinterpret_cast<uint8_t*>(self->field_000c);
    uint32_t frameValue = CSF_AT(uint32_t, linkedObject, 4);
    if (frameValue == 0)
        return 0;

    GfxFrames_ProcessFrameLink(reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), nullptr,
                               static_cast<int>(frameValue));
    return 1;
}

HOOK(0x006BF780, C_00860700_DispatchToChildFrame, "auto");
