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

// Binding for the original edit-control setter (__fastcall, ecx = self, one stack arg, pops 4).
CSF_FUNC(int, __fastcall, SetEditIntegerProperty, 0x0044F890, (void* self, void* edx, int value));

// Current scene object pointer.
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Reads a float from the nested structure reached through the stdcall argument and
// forwards it, truncated to an integer, to SetEditIntegerProperty on the current scene.
static int __stdcall SetSceneEditIntegerFromNestedFloat(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    // a3 -> object; object+0 -> inner; inner+4 -> mid; mid+4 -> float value.
    void* argument_block = reinterpret_cast<void*>(static_cast<uintptr_t>(a3));
    uint32_t inner_ptr = CSF_AT(uint32_t, argument_block, 0);
    uint32_t mid_ptr = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(inner_ptr)), 4);
    float float_value = CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(mid_ptr)), 4);

    // Float -> 64-bit integer conversion; the low 32 bits are passed on.
    int64_t wide_value = static_cast<int64_t>(float_value);
    int value = static_cast<int>(wide_value);

    return SetEditIntegerProperty(g_pCurrentScene, nullptr, value);
}

HOOK(0x0045BA10, SetSceneEditIntegerFromNestedFloat, "auto");
