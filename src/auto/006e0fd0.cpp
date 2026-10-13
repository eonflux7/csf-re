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

// Callees (bound by address, as listed in the packet)
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200,
         (void* string, void* edx, unsigned int position, unsigned int count,
          const char* other, unsigned int otherCount));
CSF_FUNC(void, __fastcall, AddObjectReference, 0x006923B0, (void* self, void* edx));

// Searches the table of entry pointers the object's first dword points to
// (the original reads them from [[this] + index*4] and bounds them by field_0008).
// Each non-null entry has a name-like sub-object at +8 and a selector at +0x1C.
// The first entry whose text does not mismatch the request (CompareStringSubstring
// returns 0) gets an extra reference and its value at +0x24 is returned; otherwise 0.
static int __fastcall C_0082d410_FindEntryByName(C_0082d410* self, void* /*edx*/, uint32_t* request)
{
    const int entryCount = static_cast<int>(self->field_0008);
    int index = 0;

    if (entryCount <= 0)
        return 0;

    for (;;)
    {
        // The object's first dword points to the entry table (original reads [[this] + index*4]).
        const uint32_t entryAddress = CSF_AT(uint32_t*, self, 0)[index];

        if (entryAddress != 0)
        {
            uint8_t* entry = reinterpret_cast<uint8_t*>(entryAddress);

            // request->field_0014 is the length/count of the other text.
            const uint32_t otherCount = request[5];
            // Inline buffer when the request capacity is below 16, otherwise heap pointer at +4.
            const char* otherText = (request[6] < 0x10u)
                ? reinterpret_cast<const char*>(request + 1)
                : reinterpret_cast<const char*>(request[1]);

            const uint32_t selector = CSF_AT(uint32_t, entry, 0x1C);

            const int mismatch = CompareStringSubstring(
                entry + 8,
                reinterpret_cast<void*>(static_cast<uintptr_t>(selector)),
                0,
                selector,
                otherText,
                otherCount);

            if (mismatch == 0)
            {
                AddObjectReference(reinterpret_cast<void*>(entry), nullptr);
                const uint32_t matchedEntry = CSF_AT(uint32_t*, self, 0)[index];
                return static_cast<int>(CSF_AT(uint32_t,
                                               reinterpret_cast<uint8_t*>(matchedEntry),
                                               0x24));
            }
        }

        if (++index >= entryCount)
            return 0;
    }
}

HOOK(0x6E0FD0, C_0082d410_FindEntryByName, "auto");

