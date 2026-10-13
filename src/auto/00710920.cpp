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

CSF_FUNC(int, __cdecl, RtDictGetEntry, 0x007DBD70, (int a1, int a2));
CSF_FUNC(int, __cdecl, RtDictGetNumEntries, 0x007DBD80, (int a1));
CSF_FUNC(int, __cdecl, GetAudioEffectsDictionary, 0x007DBE90, ());

// __thiscall int AudioEffectsDict_SumTotals(void) : sums entry field [2] of every
// dictionary entry into self+0x28 when the dirty flag at self+0x30 is set, then clears the flag.
static int __fastcall AudioEffectsDict_SumTotals(void* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0x30) != 0)
    {
        CSF_AT(int, self, 0x28) = 0;

        int dictHandle = GetAudioEffectsDictionary();
        if (dictHandle != 0)
        {
            int numEntries = RtDictGetNumEntries(dictHandle);
            for (int index = 0; index < numEntries; ++index)
            {
                int entryAddr = RtDictGetEntry(dictHandle, index);
                if (entryAddr != 0)
                {
                    void* entry = reinterpret_cast<void*>(entryAddr);
                    uint32_t entryValues[7];
                    for (int i = 0; i < 7; ++i)
                    {
                        entryValues[i] = CSF_AT(uint32_t, entry, 0x2C + 4 * i);
                    }
                    CSF_AT(int, self, 0x28) += static_cast<int>(entryValues[2]);
                }
            }
        }

        CSF_AT(uint8_t, self, 0x30) = 0;
    }

    return CSF_AT(int, self, 0x28);
}

HOOK(0x710920, AudioEffectsDict_SumTotals, "auto");
