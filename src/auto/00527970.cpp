#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Grows the record list when it is full (__thiscall, one stack arg, ret 4).
CSF_FUNC(void*, __fastcall, sub_4DE9B0, 0x004DE9B0, (void* self, void* edx, int a2));

// Appends a 12-byte record (3 dwords copied from `record`) to the list embedded at
// self+0x70 (data ptr +0x70, capacity +0x74, count +0x78, grow argument +0x7C).
// Returns record[1].
static int __fastcall AppendThreeDwordRecord(void* self, void* /*edx*/, const uint32_t* record)
{
    uint8_t* list = static_cast<uint8_t*>(self) + 0x70;

    if (CSF_AT(uint32_t, self, 0x78) >= CSF_AT(uint32_t, self, 0x74))
        sub_4DE9B0(list, nullptr, CSF_AT(int, self, 0x7C));

    uint32_t index = CSF_AT(uint32_t, self, 0x78);
    uint32_t* slot = reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x70)) + static_cast<uintptr_t>(index) * 12u);
    CSF_AT(uint32_t, self, 0x78) = index + 1;

    slot[0] = record[0];
    uint32_t result = record[1];
    slot[1] = result;
    slot[2] = record[2];
    return static_cast<int>(result);
}

HOOK(0x527970, AppendThreeDwordRecord, "auto");
