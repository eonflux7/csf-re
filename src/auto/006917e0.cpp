#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int*, __stdcall, sub_6915D0, 0x006915D0, (unsigned int* a1, unsigned int* a2, unsigned int a3));

// Walks the segment table of a C_0085e5c4 object. Each table entry points to a
// block whose first dword is the segment start and whose +8 dword is the element
// count. Searches each segment with sub_6915D0 and returns the first hit whose
// value equals the key, or nullptr when no segment matches.
static unsigned int* __fastcall C_0085e5c4_FindMatchingEntry(C_0085e5c4* self, void* /*edx*/, unsigned int searchKey)
{
    if (self->field_000c == 0)
        return nullptr;

    uint32_t segmentIndex = 0;
    while (true)
    {
        uint32_t* segmentTable = reinterpret_cast<uint32_t*>(self->field_0004);
        uint8_t* segment = reinterpret_cast<uint8_t*>(segmentTable[segmentIndex]);
        if (CSF_AT(uint32_t, segment, 0x8) == 0)
            return nullptr;

        uint32_t segmentStart = CSF_AT(uint32_t, segment, 0x0);
        uint32_t segmentCount = CSF_AT(uint32_t, segment, 0x8);
        uint32_t lastElementAddr = segmentStart + segmentCount * 4 - 4;

        unsigned int* found = sub_6915D0(
            reinterpret_cast<unsigned int*>(segmentStart),
            reinterpret_cast<unsigned int*>(lastElementAddr),
            searchKey);

        if (found <= reinterpret_cast<unsigned int*>(lastElementAddr) && *found == searchKey)
            return found;

        uint32_t tableCount = self->field_000c;
        if (segmentIndex == tableCount - 1)
            return (found == reinterpret_cast<unsigned int*>(lastElementAddr + 4)) ? nullptr : found;

        ++segmentIndex;
        if (segmentIndex >= tableCount)
            return nullptr;
    }
}

HOOK(0x6917E0, C_0085e5c4_FindMatchingEntry, "auto");
