#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies the source string at [source+0] into self (count -1 = whole string).
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
// Processes the embedded entry block at the object it is called on (this = self+0x1C).
CSF_FUNC(int*, __fastcall, BddTables_ProcessEntry, 0x00519F80,
         (S_00519c10* self, void* /*edx*/, const int* source));

// Resets self's string state (capacity 15, length 0, flag 0), copies source's string,
// then runs BddTables_ProcessEntry on self+0x1C with source+0x1C as the source.
static int* __stdcall C_0082d410_InitFromSourceAndProcessEntry(C_0082d410* self, C_0082d410* source)
{
    int* result = nullptr;
    if (self)
    {
        CSF_AT(int, self, 0x18) = 15;
        CSF_AT(std::uint32_t, self, 0x14) = 0;
        CSF_AT(std::uint8_t, self, 0x04) = 0;

        AssignStringSubstring(self, nullptr, reinterpret_cast<const void*>(source), 0u, 0xFFFFFFFFu);

        S_00519c10* selfEntry = reinterpret_cast<S_00519c10*>(reinterpret_cast<uint8_t*>(self) + 0x1C);
        const int* sourceEntry = reinterpret_cast<const int*>(reinterpret_cast<uint8_t*>(source) + 0x1C);
        result = BddTables_ProcessEntry(selfEntry, nullptr, sourceEntry);
    }
    return result;
}

HOOK(0x00776930, C_0082d410_InitFromSourceAndProcessEntry, "auto");
