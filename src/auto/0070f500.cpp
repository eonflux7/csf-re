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

// Callees (bound by address, original signatures)
CSF_FUNC(int, __cdecl, RtDictFindNamedEntry, 0x007DBD20, (int a1, int a2));
CSF_FUNC(int, __cdecl, RtDictGetEntry, 0x007DBD70, (int a1, int a2));
CSF_FUNC(int, __cdecl, RtDictGetNumEntries, 0x007DBD80, (int a1));
CSF_FUNC(int, __cdecl, RtDictInsertEntry, 0x007DBD90, (int a1, int a2));

// Copies every entry of the source dictionary (a2) whose name is not yet
// present in the destination dictionary (a1). Returns the source entry count
// when both dictionaries are non-null, otherwise the destination pointer value.
static int __stdcall AudioEffects_MergeMissingDictEntries(int a1, int a2)
{
    int result = a1;
    if (a1)
    {
        if (a2)
        {
            result = RtDictGetNumEntries(a2);
            int entryCount = result;
            int index = 0;
            while (index < entryCount)
            {
                int entry = RtDictGetEntry(a2, index);
                if (entry)
                {
                    // Entry+4 holds the entry's name pointer.
                    int entryName = CSF_AT(int, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(entry))), 4);
                    if (!RtDictFindNamedEntry(a1, entryName))
                    {
                        RtDictInsertEntry(a1, entry);
                    }
                }
                result = entryCount;
                ++index;
            }
        }
    }
    return result;
}

HOOK(0x70F500, AudioEffects_MergeMissingDictEntries, "auto");
