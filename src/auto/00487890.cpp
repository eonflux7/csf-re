#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, CCharActionFsm_ReleaseItemHandle, 0x00487100, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharAction_SetFlag, 0x00498F50, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static int __fastcall CCharActionFsm_ProcessPendingItem(
    CCharActionFsm* self, void* /*edx*/)
{
    auto* handle = reinterpret_cast<uint8_t*>(self) + 0xB0;
    void* item = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr,
                             reinterpret_cast<int>(handle)));

    if (item)
    {
        void* context = reinterpret_cast<void*>(GetValue8B0554());
        void* manager = CSF_AT(void*, context, 0x1FC0);
        uint32_t itemId = CSF_AT(uint32_t, item, 0x184);

        void* definition =
            reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, manager, 0)[8 / 4])(
                    manager, nullptr, itemId);

        int action = self->vSlotD4(CSF_AT(int, definition, 0xDC), 8);
        if (action != -1 && static_cast<uint32_t>(action) == self->field_0024)
        {
            void* character = reinterpret_cast<void*>(self->field_0008);
            CSF_AT(uint32_t, character, 0x2E4) |= 0x200u;

            CharAction_SetFlag(
                reinterpret_cast<void*>(self->field_0008), nullptr, 512, 1);
            return self->vSlot11C(12);
        }

        CCharActionFsm_ReleaseItemHandle(
            self, nullptr, reinterpret_cast<int>(handle));
        CSF_AT(int, self, 0xB4) = -1;
        CSF_AT(int, self, 0xB8) = -1;
    }

    return self->vSlot134(0, 0);
}

HOOK(0x00487890, CCharActionFsm_ProcessPendingItem, "auto");
