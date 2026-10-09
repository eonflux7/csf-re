#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, sub_42D750, 0x0042D750, (void* a1, int a2));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));

// Referenced by the original (the grow path calls into S_0041e5f0's code); bound but not stored.
[[maybe_unused]] static auto kButtonArrayVtbl = CSF_VTBL(0x0041E5F0);

// Computes a handle via sub_42D750 and appends it to the array held by a3.
static int __cdecl Hud_AppendWidgetHandle(S_00692b80* a1, int a2, S_0041e5f0* a3)
{
    int handle = sub_42D750(a1, a2);

    // Unsigned compare (jb in the original): grow when count >= capacity.
    if (static_cast<uint32_t>(a3->field_0008) >= static_cast<uint32_t>(a3->field_0004))
        GrowButtonArray(a3, nullptr, static_cast<int>(a3->field_000c));

    // Re-read count and data pointer after a possible grow.
    uint32_t index = static_cast<uint32_t>(a3->field_0008);
    int* slots = reinterpret_cast<int*>(a3->field_0000);
    a3->field_0008 = index + 1;
    slots[index] = handle;
    return handle;
}

HOOK(0x0042FF30, Hud_AppendWidgetHandle, "auto");
