#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Scans the slot array of S_0046f130 (begin at +0x004, end at +0x008) for the first
// entry whose object has the 0xFFFF marker at +2, a non-null child at +4 whose +8 id is
// valid (!= -1), and that id equals the id stored at +8 of the child referenced by a2+4.
// Entries equal to a2 are skipped. Returns the slot index, or -1 when nothing matches.
static int __fastcall MissionScript_FindMatchingSlotIndex(S_0046f130* self, void* /*edx*/, int a2)
{
    uint32_t* slotBegin = reinterpret_cast<uint32_t*>(self->field_0004);
    uint32_t slotCount = 0;
    if (slotBegin) {
        slotCount = static_cast<uint32_t>(static_cast<int32_t>(self->field_0008 - self->field_0004) >> 2);
    }

    uint8_t* requestObj = reinterpret_cast<uint8_t*>(a2);

    for (uint32_t slotIndex = 0; slotIndex < slotCount; ++slotIndex) {
        uint8_t* entryObj = reinterpret_cast<uint8_t*>(slotBegin[slotIndex]);
        if (entryObj && entryObj != requestObj
            && CSF_AT(uint16_t, entryObj, 2) == 0xFFFF) {
            uint32_t childPtr = CSF_AT(uint32_t, entryObj, 4);
            if (childPtr) {
                int32_t childId = CSF_AT(int32_t, reinterpret_cast<uint8_t*>(childPtr), 8);
                if (childId != -1) {
                    uint32_t requestChildPtr = CSF_AT(uint32_t, requestObj, 4);
                    int32_t requestChildId = CSF_AT(int32_t, reinterpret_cast<uint8_t*>(requestChildPtr), 8);
                    if (childId == requestChildId) {
                        return static_cast<int>(slotIndex);
                    }
                }
            }
        }
    }
    return -1;
}

HOOK(0x46F1A0, MissionScript_FindMatchingSlotIndex, "auto");
