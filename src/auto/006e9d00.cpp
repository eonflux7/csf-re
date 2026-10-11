#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Destructor of the _Timevec passed in (ecx = timevec, ret 0).
// returns what its inner call returns (Hex-Rays typed it void; this function compares against that eax)
CSF_FUNC(int, __fastcall, sub_6C2040, 0x006C2040, (void* self, void* edx));

// Destroys the timevec argument, then scans the 36-byte entry table at this+4
// (count at this+0x0C) for an entry whose first object has field +4 equal to the
// value left in eax by the destructor. Returns the value at entry[0]->[16]->[0]
// for a match at index > 0, 0 for a match at index 0, or -1 if not found.
static int __fastcall C_00861e98_LookupFirstMatchValue(C_00861e98* self, void* /*edx*/, void* timevec) {
    const int key = sub_6C2040(timevec, nullptr);

    uint32_t entryCount = CSF_AT(uint32_t, self, 0x0C);
    if (entryCount == 0) {
        return -1;
    }

    uint8_t* entryBase = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x04));
    uint32_t index = 0;
    for (;;) {
        uint8_t* entry = entryBase + index * 36;
        uint8_t* obj = *reinterpret_cast<uint8_t**>(entry);
        if (*reinterpret_cast<int*>(obj + 4) == key) {
            break;
        }
        ++index;
        if (index >= entryCount) {
            return -1;
        }
    }

    if (index != 0) {
        uint8_t* entry = entryBase + index * 36;
        uint8_t* obj = *reinterpret_cast<uint8_t**>(entry);
        uint8_t* inner = *reinterpret_cast<uint8_t**>(obj + 16);
        return *reinterpret_cast<int*>(inner);
    }
    return 0;
}

HOOK(0x006E9D00, C_00861e98_LookupFirstMatchValue, "auto");

