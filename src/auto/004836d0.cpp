#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_452320, 0x00452320, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_483540, 0x00483540, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, C_00495140_ForwardInnerObjectVirtualCall, 0x00495140, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, sub_4FC3B0, 0x004FC3B0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CCharActionFsm_ResetItemAndEnterState12(
    CCharActionFsm* self, void* /*edx*/)
{
    int itemState = 0;
    if (self->field_009c)
    {
        void* item = reinterpret_cast<void*>(self->field_009c);
        itemState = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, item, 0)[0x4C / 4])(item, nullptr);
    }

    if (itemState == 12 || itemState == 13)
    {
        C_00495140_ForwardInnerObjectVirtualCall(
            reinterpret_cast<void*>(self->field_0008), nullptr);

        if (self->field_0024 != static_cast<uint32_t>(-1))
        {
            void* weapon = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, weapon, 0)[0x30 / 4])(weapon, nullptr, 0);
        }

        // The earlier push of 1 belongs to this call, not to the forwarder.
        void* item = reinterpret_cast<void*>(self->field_009c);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, item, 0)[0x3C / 4])(item, nullptr, 1);

        item = reinterpret_cast<void*>(self->field_009c);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, item, 0)[0x38 / 4])(item, nullptr, 0);

        item = reinterpret_cast<void*>(self->field_009c);
        if (item &&
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, item, 0)[0x4C / 4])(item, nullptr) == 13)
        {
            sub_452320(g_pCurrentScene, nullptr, 0);
        }

        if (itemState == 13)
        {
            void* manager = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
            sub_4FC3B0(manager, nullptr, -1);
        }
    }

    void* value = reinterpret_cast<void*>(
        Handle_GetValuePointer(reinterpret_cast<void*>(self->field_009c), nullptr));
    sub_483540(self, nullptr, CSF_AT(float, value, 0x1C0));
    return self->vUpdateWeaponSelection(12);
}

HOOK(0x004836D0, CCharActionFsm_ResetItemAndEnterState12, "auto");
