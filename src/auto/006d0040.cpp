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

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));

// Scans this object's entry table; returns true if any non-null entry's name
// compares equal (CompareStringSubstring returns 0) to the text in a2.
static char __fastcall S_0041e5f0_HasMatchingEntryName(S_0041e5f0* self, void* /*edx*/, uint32_t* a2)
{
    char matched = 0;
    int entryCount = static_cast<int>(self->field_0008);
    int index = 0;
    if (entryCount > 0)
    {
        do
        {
            uint32_t entry = reinterpret_cast<uint32_t*>(self->field_0000)[index];
            if (entry != 0)
            {
                uint32_t entryNameLength = CSF_AT(uint32_t, entry, 0x1C);
                const char* text;
                if (a2[6] < 0x10u)
                    text = reinterpret_cast<const char*>(a2 + 1);
                else
                    text = reinterpret_cast<const char*>(a2[1]);

                void* nameObject = reinterpret_cast<void*>(entry + 8);
                if (!CompareStringSubstring(nameObject, reinterpret_cast<void*>(entryNameLength), 0,
                                            entryNameLength, text, a2[5]))
                    matched = 1;
            }
            ++index;
        } while (index < entryCount);
        return matched;
    }
    return matched;
}

HOOK(0x006D0040, S_0041e5f0_HasMatchingEntryName, "auto");
