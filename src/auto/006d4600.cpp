#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __cdecl, GfxClumpDict_ResourcesMatch, 0x006C0EE0, (int a1, int a2));

static int __fastcall GfxClumpDict_FindResourceIndex(
    void* self, void* /*edx*/, int resource)
{
    uint32_t entryIndex = 0;
    uint32_t entryOffset = 0;

    for (;;)
    {
        const uint32_t begin = CSF_AT(uint32_t, self, 0x08);
        if (!begin)
            break;

        const uint32_t end = CSF_AT(uint32_t, self, 0x0C);
        const int32_t byteCount = static_cast<int32_t>(end - begin);
        if (entryIndex >= static_cast<uint32_t>(byteCount / 60))
            break;

        uint32_t resourceIndex = 0;
        for (;;)
        {
            const uint32_t currentBegin = CSF_AT(uint32_t, self, 0x08);
            void* entry = reinterpret_cast<void*>(currentBegin + entryOffset);

            if (resourceIndex >= CSF_AT(uint32_t, entry, 0x28))
                break;

            const uint32_t resources = CSF_AT(uint32_t, entry, 0x20);
            const int candidate = CSF_AT(
                int, reinterpret_cast<void*>(resources), 4 * resourceIndex);

            if (resource == candidate ||
                GfxClumpDict_ResourcesMatch(resource, candidate) != 0)
            {
                return static_cast<int>(entryIndex);
            }

            ++resourceIndex;
        }

        ++entryIndex;
        entryOffset += 60;
    }

    return -1;
}

HOOK(0x006D4600, GfxClumpDict_FindResourceIndex, "auto");
