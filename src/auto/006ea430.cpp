#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resolves the sub-hierarchy index for a given id; the return value is later
// stored in the record and passed to RpHAnimHierarchyCreateSubHierarchy.
CSF_FUNC(int, __cdecl, HAnimResolveSubHierarchyIndex, 0x006E8120, (int a1, int a2));
// Creates an RpHAnim sub-hierarchy from the root hierarchy.
CSF_FUNC(int*, __cdecl, RpHAnimHierarchyCreateSubHierarchy, 0x0056CDB0, (int a1, int a2, int a3, int a4));
// Grows the record array (called when the record count reaches the capacity).
CSF_FUNC(char, __fastcall, HAnimGrowSubHierarchyRecords, 0x006E8EA0, (void* self, void* edx, int a2));

// Appends a 9-dword sub-hierarchy record to the object's record array.
// Field layout (not declared in auto_classes.h):
//   +0x04 record array base, +0x08 capacity, +0x0C record count,
//   +0x10 growth argument, +0x14 root hierarchy pointer, +0x1C enabled flag.
static int __fastcall C_00861e98_AppendSubHierarchyRecord(C_00861e98* self, void* /*edx*/, int a2)
{
    int result = CSF_AT(int, self, 0x1c);
    if (result != 0)
    {
        // The root hierarchy pointer is stored as a plain value at +0x14 and is
        // passed to the callees as a 32-bit integer argument.
        int rootHierarchyValue = CSF_AT(int, self, 0x14);
        int resolvedIndex = HAnimResolveSubHierarchyIndex(rootHierarchyValue, a2);

        int* rootHierarchy = reinterpret_cast<int*>(static_cast<uintptr_t>(
            static_cast<uint32_t>(CSF_AT(int, self, 0x14))));
        uint32_t record[9];
        std::memset(record, 0, sizeof(record));
        int rootFirstWord = rootHierarchy[0];
        record[6] = static_cast<uint32_t>(resolvedIndex);
        uint32_t subHierarchy = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(
            RpHAnimHierarchyCreateSubHierarchy(CSF_AT(int, self, 0x14), resolvedIndex, rootFirstWord, -1)));
        record[0] = subHierarchy;
        record[5] = 546;

        uint32_t capacity = CSF_AT(uint32_t, self, 0x08);
        if (CSF_AT(uint32_t, self, 0x0c) >= capacity)
        {
            HAnimGrowSubHierarchyRecords(reinterpret_cast<uint8_t*>(self) + 0x04, nullptr,
                                         CSF_AT(int, self, 0x10));
        }

        uint32_t recordIndex = CSF_AT(uint32_t, self, 0x0c);
        uint32_t* recordBase = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x04));
        uint32_t* destination = recordBase + 9 * recordIndex;
        result = static_cast<int>(recordIndex + 1);
        CSF_AT(uint32_t, self, 0x0c) = static_cast<uint32_t>(result);
        std::memcpy(destination, record, 36);
    }
    return result;
}

HOOK(0x006EA430, C_00861e98_AppendSubHierarchyRecord, "auto");
