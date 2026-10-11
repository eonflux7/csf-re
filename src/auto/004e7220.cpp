#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_4E5BE0, 0x004E5BE0, (void* self, void* edx, int a2));

// Searches one of the four embedded slot tables of C_0083e9d0 (at +0x80, +0x70, +0x60, +0x50)
// for an entry whose +0x8 equals matchId and whose +0x18 equals the value at matchRecord+4.
static char __fastcall C_0083e9d0_FindMatchingEntry(C_0083e9d0* self, void* /*edx*/,
                                                    int slotIndex, int matchId, int matchRecord)
{
    char found = 0;

    if (sub_4E5BE0(self, nullptr, matchRecord)) {
        uint32_t subOffset = 0;
        switch (slotIndex) {
            case 0: subOffset = 0x80; break;
            case 1: subOffset = 0x70; break;
            case 2: subOffset = 0x60; break;
            case 3: subOffset = 0x50; break;
            default:
                return found;
        }

        uint8_t* slotObject = reinterpret_cast<uint8_t*>(self) + subOffset;
        if (slotObject != nullptr) {
            uint32_t entryCount = CSF_AT(uint32_t, slotObject, 0x8);
            if (entryCount != 0) {
                uint32_t* entries = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, slotObject, 0x0));
                for (uint32_t i = 0; i < entryCount; ++i) {
                    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entries[i]));
                    if (CSF_AT(uint32_t, entry, 0x8) == static_cast<uint32_t>(matchId)) {
                        uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(matchRecord));
                        if (CSF_AT(uint32_t, entry, 0x18) == CSF_AT(uint32_t, record, 0x4)) {
                            return 1;
                        }
                    }
                }
            }
        }
    }

    return found;
}

HOOK(0x4E7220, C_0083e9d0_FindMatchingEntry, "auto");
