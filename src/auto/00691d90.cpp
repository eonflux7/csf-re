#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, LookupRecordByKey, 0x00691860, (void* self, void* edx, unsigned int a2));

// __thiscall(this, key, a3, a4) with 12 bytes of stack args: ecx = this, edx unused.
static char* __fastcall S_00691860_RemoveRecordByKey(S_00691860* self, void* /*edx*/, int a2, unsigned int a3, int a4)
{
    (void)a4;

    char* result = LookupRecordByKey(self, nullptr, a3);
    if (result != nullptr && CSF_AT(uint32_t, result, 0) == a3) {
        if (CSF_AT(uint8_t, result, 9) != 0) {
            uint8_t* list = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, result, 0xC));
            result = nullptr;
            const uint32_t count = CSF_AT(uint16_t, list, 4);
            if (count != 0) {
                uint32_t* cursor = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, list, 0));
                uint32_t slot = 0;
                while (*cursor != static_cast<uint32_t>(a2)) {
                    ++slot;
                    cursor += 2;
                    if (slot >= count) {
                        return reinterpret_cast<char*>(static_cast<uintptr_t>(slot));
                    }
                }

                // Found at 'slot': shrink the list and move the last entry into the hole.
                --CSF_AT(uint16_t, list, 4);
                const uint32_t last = CSF_AT(uint16_t, list, 4);
                if (last > slot) {
                    uint32_t* entries = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, list, 0));
                    entries[slot * 2] = entries[last * 2];
                    entries[slot * 2 + 1] = entries[last * 2 + 1];
                }
                return reinterpret_cast<char*>(static_cast<uintptr_t>(slot));
            }
        } else {
            CSF_AT(uint32_t, result, 0xC) = 0;
        }
    }
    return result;
}

HOOK(0x00691D90, S_00691860_RemoveRecordByKey, "auto");
