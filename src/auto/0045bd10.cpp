#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, MenuProgressInput_UpdateChildSlot, 0x0045A980, (void* self, void* edx));

static constexpr uint32_t kFloatBitsOne = 0x3F800000u;
static constexpr int kChildSlotCount = 3;
static constexpr size_t kChildSlotStride = 0x4C;

// Resets the three 0x4C-byte child slots that follow the object's header,
// then refreshes each slot starting from the object base. Returns this.
static void* __fastcall MenuProgressInput_ResetSlotsAndUpdate(void* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    for (int slotIndex = 0; slotIndex < kChildSlotCount; ++slotIndex)
    {
        uint8_t* slot = base + kChildSlotStride * slotIndex;

        CSF_AT(uint32_t, slot, 0x08) = 0;
        CSF_AT(uint32_t, slot, 0x0C) = 0;
        CSF_AT(uint32_t, slot, 0x10) = 0;
        CSF_AT(uint32_t, slot, 0x14) = kFloatBitsOne;
        CSF_AT(uint32_t, slot, 0x18) = 0;
        CSF_AT(uint32_t, slot, 0x1C) = 0;
        CSF_AT(uint32_t, slot, 0x20) = 0;
        CSF_AT(uint32_t, slot, 0x24) = kFloatBitsOne;
        CSF_AT(uint32_t, slot, 0x2C) = 0;
        CSF_AT(uint32_t, slot, 0x30) = 0;
        CSF_AT(uint32_t, slot, 0x34) = 0;
        CSF_AT(uint32_t, slot, 0x38) = kFloatBitsOne;
        CSF_AT(uint32_t, slot, 0x3C) = 0;
        CSF_AT(uint32_t, slot, 0x40) = 0;
        CSF_AT(uint32_t, slot, 0x44) = 0;
        CSF_AT(uint32_t, slot, 0x48) = kFloatBitsOne;

        MenuProgressInput_UpdateChildSlot(slot, nullptr);
    }

    for (int slotIndex = 0; slotIndex < kChildSlotCount; ++slotIndex)
    {
        MenuProgressInput_UpdateChildSlot(base + kChildSlotStride * slotIndex, nullptr);
    }

    return self;
}

HOOK(0x0045BD10, MenuProgressInput_ResetSlotsAndUpdate, "auto");
