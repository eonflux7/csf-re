#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: CScene_RemoveObjectFromCategories (0x4528D0), __fastcall, ret 8
CSF_FUNC(int, __fastcall, CScene_RemoveObjectFromCategories, 0x004528D0, (CScene* self, void* , int handleAddress, int categoryMask));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Removes this object's handle (field at +0x0C, handle offset +0x1C) from the
// categories of the current scene, with category mask 1.
static int __fastcall C_0083e5e8_RemoveHandleFromCurrentScene(C_0083e5e8* self, void* /*edx*/)
{
    return CScene_RemoveObjectFromCategories(
        reinterpret_cast<CScene*>(g_pCurrentScene),
        nullptr,
        static_cast<int>(self->field_000c + 0x1C),
        1);
}

HOOK(0x004E1D10, C_0083e5e8_RemoveHandleFromCurrentScene, "auto");
