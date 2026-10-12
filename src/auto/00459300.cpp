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

// Bound by address: callees of Hud_ResetSecondaryWidget.
CSF_FUNC(int, __fastcall, sub_458E40, 0x00458E40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Clears the secondary HUD widget: resets the embedded sub-object at +0x08,
// frees the block held at +0x1C (if any), and zeroes +0x1C, +0x20 and +0x24.
static int __fastcall Hud_ResetSecondaryWidget(int* self, void* /*edx*/)
{
    // Sub-object embedded at +8 (lea ecx, [esi+8]); not a pointer load.
    sub_458E40(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 8), nullptr);

    int result = CSF_AT(int, self, 0x1C);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result)));

    CSF_AT(int, self, 0x1C) = 0;
    CSF_AT(int, self, 0x20) = 0;
    CSF_AT(int, self, 0x24) = 0;
    return result;
}

HOOK(0x459300, Hud_ResetSecondaryWidget, "auto");
