#include <cstdint>
#include "globals.h"
#include <cstring>

#include "hook.h"

CSF_FUNC(int, __cdecl, ObjectProps_ReadEntryFirstDword, 0x006B8430, (int entryIndex));
CSF_FUNC(int, __cdecl, GfxObjectProps_FreeSlotBlock, 0x006B8400, (int slotOffset));

static int __cdecl GfxWorld_ApplyEntryBytesAndFreeSlots(int a1)
{
    uint8_t* obj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a1));

    int entryFirstDword = ObjectProps_ReadEntryFirstDword(a1);
    if (entryFirstDword)
    {
        const uint8_t* entryBytes = reinterpret_cast<const uint8_t*>(static_cast<uintptr_t>(entryFirstDword));
        int index = 0;
        if (CSF_AT(uint16_t, obj, 0x84) != 0)
        {
            do
            {
                uint8_t* slot = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, obj, 4)))
                    + 8 * index + 6;
                uint8_t entryByte = entryBytes[index];
                index++;
                uint8_t lowByte = slot[0];
                uint16_t packed = static_cast<uint16_t>((static_cast<uint16_t>(entryByte) << 8) | lowByte);
                std::memcpy(slot, &packed, sizeof(packed));
            } while (index < static_cast<int>(CSF_AT(uint16_t, obj, 0x84)));
        }
        GfxObjectProps_FreeSlotBlock(a1);
    }
    return a1;
}

HOOK(0x006B6B70, GfxWorld_ApplyEntryBytesAndFreeSlots, "auto");
