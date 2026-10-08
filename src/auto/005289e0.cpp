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

// GetGlobalState: __cdecl, no args, returns the global state pointer in eax.
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x0050D190, ());

// sub_50E1E0: __thiscall, ecx = global state object.
CSF_FUNC(int, __fastcall, GlobalState_Reset, 0x0050E1E0, (void* self, void* edx));

// RwRasterDestroy: __cdecl, one raster pointer argument.
CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));

// Releases the raster held at +0x9B0, clears the word at +0x90 and resets the raster slot.
// The return value is the RwRasterDestroy result when a raster was held, otherwise the
// (zero) slot value read before the test.
static int __fastcall MenuMessageBox_ReleaseRasterAndReset(void* self, void* /*edx*/)
{
    void* globalState = GetGlobalState();
    GlobalState_Reset(globalState, nullptr);

    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x9B0));
    CSF_AT(uint16_t, self, 0x90) = 0;
    if (result) {
        result = RwRasterDestroy(static_cast<uint32_t>(result));
        CSF_AT(uint32_t, self, 0x9B0) = 0;
    }
    return result;
}

HOOK(0x005289E0, MenuMessageBox_ReleaseRasterAndReset, "auto");
