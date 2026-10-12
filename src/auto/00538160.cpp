#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char, __fastcall, sub_537E60, 0x00537E60, (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, sub_536D30, 0x00536D30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_537740, 0x00537740, (void* self, void* edx, int a3));

// Dispatches on the mode field (+0x134). Mode 0 goes to sub_537E60, mode 1 to
// sub_536D30, mode 2 to sub_537740. Any other mode falls through and returns
// the low byte of (mode - 2), as the original does.
static char __fastcall C_00830578_DispatchSceneResultMode(C_00830578* self, void* /*edx*/, int a3)
{
    uint32_t mode = CSF_AT(uint32_t, self, 0x134);

    if (mode == 0)
    {
        return sub_537E60(self, nullptr, a3);
    }
    if (mode == 1)
    {
        return static_cast<char>(sub_536D30(self, nullptr, a3));
    }
    if (mode == 2)
    {
        return static_cast<char>(sub_537740(self, nullptr, a3));
    }
    return static_cast<char>(mode - 2);
}

HOOK(0x538160, C_00830578_DispatchSceneResultMode, "auto");
