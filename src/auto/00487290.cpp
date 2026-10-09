#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_485A20, 0x00485A20, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, CharActionFsm_SetFlag, 0x00485080, (void* self, void* edx, uint32_t a2));

static void __fastcall CCharActionFsm_SetActionSlotValue(CCharActionFsm* self, void* /*edx*/, int value)
{
    const int slotIndex = sub_485A20(self, nullptr, 28);
    if (slotIndex != -1)
    {
        uint32_t* slotTable = reinterpret_cast<uint32_t*>(self->field_0078);
        uint8_t* slotItem = reinterpret_cast<uint8_t*>(slotTable[slotIndex]);
        CSF_AT(int, slotItem, 0x80) = value;

        if (value)
        {
            if (self->field_0090 == 0xFFFFFFFFu)
                CharActionFsm_SetFlag(self, nullptr, static_cast<uint32_t>(slotIndex));
        }
    }
}

HOOK(0x00487290, CCharActionFsm_SetActionSlotValue, "auto");
