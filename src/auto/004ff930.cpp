#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// C_00833908::<unnamed> at 0x490D20: __thiscall, 2 args, ret 8.
CSF_FUNC(unsigned int, __fastcall, C_00833908_SetDoorStateFlags, 0x490D20,
         (C_00833908* self, void* edx, uint32_t a2, char a3));

CSF_GLOBAL(void*, g_pCurrentScene, 0x8AFC90);

// Clears the per-scene flag at entry sceneIdx (stride 88, offset 80) and then
// closes every object of the current scene whose type is 26 or 27 and that
// reports itself as matching through its secondary object.
// Returns g_pCurrentScene.
static CGuiControl* __fastcall CSceneLoader_ResetSlotAndCloseOpenDoors(CSceneLoader* self, void* /*edx*/, int sceneIdx)
{
    // Reset the flag of entry sceneIdx in the table at this->field_0030.
    uint8_t* entryTableBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x30)));
    CSF_AT(int, entryTableBase + 88 * sceneIdx, 80) = 0;

    // Walk every object of the current scene. Count and element table are
    // re-read from the global on each iteration, as the original does.
    for (uint32_t i = 0; i < CSF_AT(uint32_t, g_pCurrentScene, 0x18); ++i) {
        auto* v4 = reinterpret_cast<C_00833908*>(CSF_AT(uint32_t*, g_pCurrentScene, 0x10)[i]);
        if (!v4)
            continue;
        if (!v4->field_0050)
            continue;
        if (!v4->vGetField016c())
            continue;

        auto v5 = v4->vGetField016c();
        void* obj = reinterpret_cast<void*>(v5);
        // Virtual slot 64 / 4 = 16 on the object returned by vGetField016c.
        bool matches = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[16])(obj, nullptr);
        if (matches) {
            uint32_t objectType = v4->field_0050;
            if (objectType == 26 || objectType == 27)
                C_00833908_SetDoorStateFlags(v4, nullptr, 0, 1);
        }
    }

    return reinterpret_cast<CGuiControl*>(g_pCurrentScene);
}

HOOK(0x004FF930, CSceneLoader_ResetSlotAndCloseOpenDoors, "auto");
