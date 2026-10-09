#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CharActionFsm_SetCharacterItem, 0x00494E90, (void* self, void* edx, int a4));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(unsigned int, __fastcall, Character_AssignItemSlot, 0x0049B030, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, CCharActionFsm_SetActionSlotValue, 0x00487290, (CCharActionFsm* self, void* edx, int value));

static void __fastcall C_00833ee8_SwitchCharacterItem(C_00833ee8* self, void* /*edx*/, int a2)
{
    if (a2 == self->field_03a0)
        return;

    CharActionFsm_SetCharacterItem(nullptr, nullptr, 0);

    if (self->field_039c != 0)
    {
        void* globalInt = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
        int sceneController = SceneLoader_GetSceneController(globalInt, nullptr);
        void* scene = reinterpret_cast<void*>(static_cast<uintptr_t>(sceneController));

        // vtable slot 24 / 4 = 6
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, scene, 0)[6])(scene, nullptr, self->field_039c);
        self->field_039c = 0;
    }

    int v5 = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554())), 8136);
    void* slotTable = reinterpret_cast<void*>(static_cast<uintptr_t>(v5));

    // vtable slot 16 / 4 = 4
    int v6 = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, slotTable, 0)[4])(slotTable, nullptr, a2);
    if (v6 != -1)
    {
        // vtable slot 8 / 4 = 2
        int v7 = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, slotTable, 0)[2])(slotTable, nullptr, v6);
        void* item = reinterpret_cast<void*>(static_cast<uintptr_t>(v7));

        Character_AssignItemSlot(self, nullptr, v7);
        CCharActionFsm_SetActionSlotValue(
            reinterpret_cast<CCharActionFsm*>(self->field_0044),
            nullptr,
            CSF_AT(int, item, 188));
    }
}

HOOK(0x0049BAF0, C_00833ee8_SwitchCharacterItem, "auto");
