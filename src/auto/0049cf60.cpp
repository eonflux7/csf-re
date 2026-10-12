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

CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* /*edx*/, int a2));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Clears the "active" flag of the object, detaches it from the current scene's
// object slot list and returns the scene's removal result.
static int __fastcall C_00834050_ClearFlagAndRemoveSlot(C_00834050* self, void* /*edx*/)
{
    // Ask the object for its linked child (slot +0x28); it is of unknown type here.
    int childValue = self->vGetField016c();
    void* child = reinterpret_cast<void*>(childValue);

    if (childValue) {
        // Slot +0xA8: query on the child; a nonzero byte means "notify it".
        void** childVtbl = CSF_AT(void**, child, 0);
        uint8_t needsNotify =
            ((uint8_t(__fastcall*)(void*, void*))childVtbl[0xA8 / 4])(child, nullptr);
        if (needsNotify) {
            // Slot +0xA4: notify the child.
            childVtbl = CSF_AT(void**, child, 0);
            ((void(__fastcall*)(void*, void*))childVtbl[0xA4 / 4])(child, nullptr);
        }
    }

    self->field_0198 = 0;

    return CScene_RemoveObjectSlot(
        reinterpret_cast<CScene*>(g_pCurrentScene),
        nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_001c)));
}

HOOK(0x0049CF60, C_00834050_ClearFlagAndRemoveSlot, "auto");
