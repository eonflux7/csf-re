#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(char, __stdcall, TextDatabase_MatchesSceneAudioEntry, 0x004EF080, (void* a1, int a2));

static int __fastcall TextDatabase_CollectSceneAudioEntries(
    int* self, void* /*edx*/, int sceneId, char category,
    S_0041e5f0* output)
{
    (void)CSF_VTBL(0x0041E5F0);

    output->field_0008 = 0;

    const uint32_t begin = CSF_AT(uint32_t, self, 0x08);
    uint32_t remaining = 0;
    if (begin != 0)
    {
        const int32_t byteCount = static_cast<int32_t>(
            CSF_AT(uint32_t, self, 0x0C) - begin);
        remaining = static_cast<uint32_t>(byteCount / 104);
    }

    if (remaining != 0)
    {
        const uint32_t categoryMask =
            uint32_t{1} << (static_cast<uint32_t>(category) & 31);
        uint32_t entryOffset = 0;

        do
        {
            void* entry = reinterpret_cast<void*>(
                CSF_AT(uint32_t, self, 0x08) + entryOffset);

            if ((CSF_AT(uint32_t, entry, 0x58) & categoryMask) == categoryMask
                && TextDatabase_MatchesSceneAudioEntry(entry, sceneId))
            {
                if (output->field_0008 >= output->field_0004)
                {
                    S_0041e5f0_GrowArrayCapacity(
                        output, nullptr, output->field_000c);
                }

                const uint32_t index = output->field_0008;
                uint32_t* destination = reinterpret_cast<uint32_t*>(
                    output->field_0000 + 4 * index);
                output->field_0008 = index + 1;
                *destination = reinterpret_cast<uint32_t>(entry);
            }

            entryOffset += 104;
            --remaining;
        } while (remaining != 0);
    }

    return output->field_0008 != 0;
}

HOOK(0x004EF620, TextDatabase_CollectSceneAudioEntries, "auto");
