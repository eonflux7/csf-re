#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, TextDatabase_SceneEntryContainsPosition, 0x007707E0,
         (void* self, void* edx, float* a2));

static int __fastcall TextDatabase_FindSceneEntryAtPosition(
    void* self, void* /*edx*/, S_00770760* position, char category)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x08);
    uint32_t count = 0;
    if (begin != 0)
    {
        const uint32_t end = CSF_AT(uint32_t, self, 0x0C);
        count = static_cast<uint32_t>(static_cast<int32_t>(end - begin) / 104);
    }

    const uint32_t mask = uint32_t{1} << (static_cast<uint8_t>(category) & 31);
    uint32_t byteOffset = 0;

    for (uint32_t index = 0; index < count; ++index, byteOffset += 104)
    {
        const uint32_t entryAddress =
            CSF_AT(uint32_t, self, 0x08) + byteOffset;
        void* entry = reinterpret_cast<void*>(entryAddress);

        if ((CSF_AT(uint32_t, entry, 0x58) & mask) == mask)
        {
            const int contains = TextDatabase_SceneEntryContainsPosition(
                entry, nullptr, reinterpret_cast<float*>(position->pad_0000));
            if (static_cast<uint8_t>(contains) != 0)
                return static_cast<int32_t>(index);
        }
    }

    return -1;
}

HOOK(0x004EF6E0, TextDatabase_FindSceneEntryAtPosition, "auto");
