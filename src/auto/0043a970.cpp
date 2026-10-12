#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

// Forwards the object position of a2 (plus a3) to the slot-3 virtual of this, when a scene menu context exists.
static void __fastcall C_00830d68_ForwardSceneObjectPosition(C_00830d68* self, void* /*edx*/, C_008342e8* a2, int a3)
{
    if (a2 && a3)
    {
        if (GetSceneMenuContext(g_pCurrentScene, nullptr))
        {
            // The original loads the vtable pointer of this before calling GetObjectPositionVector.
            void** vtblBefore = CSF_AT(void**, self, 0);
            int objectPositionVector = GetObjectPositionVector(a2, nullptr);
            ((void(__fastcall*)(void*, void*, int, int))vtblBefore[3])(self, nullptr, objectPositionVector, a3);
        }
    }
}

HOOK(0x0043A970, C_00830d68_ForwardSceneObjectPosition, "auto");
