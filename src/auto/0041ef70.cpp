#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, sub_44D9B0, 0x0044D9B0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));

// Referenced by the original (call site at 0x41EFAE binds the S_0041e5f0 helper).
[[maybe_unused]] static const auto s_ButtonArrayVtable = CSF_VTBL(0x0041E5F0);

// Clears the pending flag, looks up the scene index held at this+0x14 and,
// when the lookup succeeds, appends it to the current scene's index list.
static int __fastcall FlushPendingSceneIndex(uint8_t* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 1))
        CSF_AT(uint8_t, self, 2) = 0;

    C_008311f0* scene = reinterpret_cast<C_008311f0*>(CSF_AT(uint32_t, g_pCurrentScene, 0x130));
    unsigned int pendingIndex = CSF_AT(uint32_t, self, 0x14);

    int result = sub_44D9B0(scene, nullptr, pendingIndex);
    if (result != -1)
    {
        // Index list lives embedded at scene+0x14:
        //   +0x00 data pointer, +0x08 count, +0x0C growth step.
        uint8_t* list = reinterpret_cast<uint8_t*>(scene) + 0x14;

        uint32_t count = CSF_AT(uint32_t, scene, 0x1C);
        uint32_t capacity = CSF_AT(uint32_t, scene, 0x18);
        if (count >= capacity)
            GrowButtonArray(list, nullptr, static_cast<int>(CSF_AT(uint32_t, list, 0x0C)));

        uint32_t index = CSF_AT(uint32_t, list, 0x08);
        uint32_t* elements = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, list, 0x00));

        result = static_cast<int>(index + 1);
        CSF_AT(uint32_t, list, 0x08) = static_cast<uint32_t>(result);
        elements[index] = pendingIndex;
    }

    CSF_AT(uint32_t, self, 0x14) = 0;
    return result;
}

HOOK(0x0041EF70, FlushPendingSceneIndex, "auto");
