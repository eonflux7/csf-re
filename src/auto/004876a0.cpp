#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CCharActionFsm_FindSlotIndex, 0x00485A20, (void* self, void* edx, int a2));

static int __fastcall CCharActionFsm_TryStartSlotAction(CCharActionFsm* self, void* /*edx*/)
{
    int result = CCharActionFsm_FindSlotIndex(self, nullptr, 28);
    if (result != -1)
    {
        uint32_t* slotTable = reinterpret_cast<uint32_t*>(self->field_0078);
        uint32_t* slotEntry = slotTable + result;
        uint32_t entryObject = *slotEntry;
        result = static_cast<int>(reinterpret_cast<uintptr_t>(slotEntry));

        uint8_t* entryPtr = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryObject));
        if (CSF_AT(uint32_t, entryPtr, 0x80) != 0)
        {
            result = static_cast<int>(entryObject);
            if (entryObject != 0)
            {
                if (CSF_AT(uint32_t, entryPtr, 0x80) != 0)
                {
                    self->field_00a4 = entryObject;
                    self->field_009c = entryObject;
                    self->vSlot134(12, 0);
                    result = self->vSlot138(21);
                    self->field_009c = 0;
                }
            }
        }
    }
    return result;
}

HOOK(0x004876A0, CCharActionFsm_TryStartSlotAction, "auto");
