#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, GuiControl_SetInteractionState, 0x0048EEC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CScene_UnlinkSlot, 0x00452800, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, CScene_ReleaseSlotMasked, 0x004528D0, (void* self, void* edx, int a2, int a3));

static int __fastcall CScene_RemoveObjectSlot(C_008312b0* self, void* /*edx*/, int a2)
{
    void* request = reinterpret_cast<void*>(a2);
    int result = CSF_AT(int, request, 0x8);

    if (result >= 0 && result < CSF_AT(int, self, 0x18))
    {
        void* slotArray = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
        void* object = reinterpret_cast<void*>(CSF_AT(uint32_t, slotArray, result * 4));

        if (object != nullptr && CSF_AT(int, object, 0x20) == CSF_AT(int, request, 0x4))
        {
            if ((CSF_AT(uint8_t, object, 0x170) & 0x10) != 0)
                GuiControl_SetInteractionState(object, nullptr, 0);

            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0x6C / 4])(object, nullptr, 0);

            if ((CSF_AT(uint8_t, object, 0x94) & 0x20) != 0)
                CScene_ReleaseSlotMasked(self, nullptr, a2, 0x20);

            if ((CSF_AT(uint8_t, object, 0x94) & 1) != 0)
                CScene_ReleaseSlotMasked(self, nullptr, a2, 1);

            return CScene_UnlinkSlot(self, nullptr, a2, 2);
        }
    }
    return result;
}

HOOK(0x00453390, CScene_RemoveObjectSlot, "auto");
