#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Callee bindings (original addresses)
CSF_FUNC(int, __cdecl, RtDictGetEntry, 0x007DBD70, (int a1, int a2));
CSF_FUNC(int, __cdecl, RtDictGetNumEntries, 0x007DBD80, (int a1));
CSF_FUNC(int, __cdecl, GetAudioEffectsDictionary, 0x007DBE90, ());

// Enumerates every entry of the RW audio effects dictionary and calls
// `callback` once per slot with a 4-dword record built from the entry:
//   [0] = entry+0x04, [1] = entry+0x34, [2] = entry+0x2C, [3] = byte 1 of entry+0x38.
// Returns the number of callback invocations for the last non-empty slot pass.
static int __stdcall AudioEffects_ForEachEntry(void (__cdecl* callback)(uint32_t*))
{
    int result = (int)(uintptr_t)callback;
    int slotIndex = 0;

    if (callback)
    {
        int invokeCount = 0;
        int dictionary = GetAudioEffectsDictionary();
        result = RtDictGetNumEntries(dictionary);
        int slotCount = result;

        if (result > 0)
        {
            do
            {
                int currentDictionary = GetAudioEffectsDictionary();
                int entryValue = RtDictGetEntry(currentDictionary, slotIndex);
                result = entryValue;

                if (entryValue)
                {
                    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryValue));
                    uint32_t entryHeader = CSF_AT(uint32_t, entry, 0x4);

                    uint32_t payload[7];
                    std::memcpy(payload, entry + 0x2C, sizeof(payload));

                    uint32_t record[4];
                    record[0] = entryHeader;
                    record[1] = payload[2];
                    record[2] = payload[0];
                    record[3] = (payload[3] >> 8) & 0xFF;

                    callback(record);

                    result = ++invokeCount;
                }

                ++slotIndex;
            }
            while (slotIndex < slotCount);
        }
    }

    return result;
}

HOOK(0x70FB20, AudioEffects_ForEachEntry, "auto");
