#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ComputeChildHandleFromSlot, 0x00515BD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ObjectivePanel_Assign, 0x005150F0, (void* self, void* edx, int value));

static int __fastcall C_00840af8_AssignObjectiveToSlot(C_00840af8* self, void* /*edx*/, int a2)
{
    // Mark the incoming object as assigned (byte at +0x4C).
    CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(a2), 0x4C) = 1;

    // Compute the child slot index, then address the 80-byte slot in the table at field_0008.
    int slotIndex = ComputeChildHandleFromSlot(self, nullptr);
    uint8_t* slotEntry = reinterpret_cast<uint8_t*>(self->field_0008 + 80 * slotIndex);

    return ObjectivePanel_Assign(slotEntry, nullptr, a2);
}

HOOK(0x516A70, C_00840af8_AssignObjectiveToSlot, "auto");
