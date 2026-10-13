#include <cstddef>
#include "globals.h"
#include <cstdint>

#include "hook.h"

// Callees, bound by address exactly as the packet gives them.
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));
CSF_FUNC(int, __cdecl, Rt2dObjectDestroy, 0x007F7900, (int a1));
CSF_FUNC(int, __cdecl, RtDictRemoveEntry, 0x007FDA80, (int a1, int a2));

// __thiscall(this, key), ret 4. Finds the 8-byte {key, object} entry whose key equals `key`
// in the entry array at +0x40, removes it from the dictionary at +0x3C, destroys the
// object, then shifts the following entries down and decrements the count at +0x48.
static void __fastcall SceneLoader_RemoveRenderWorldByKey(void* self, void* /*edx*/, int key)
{
    const uint32_t initialCount = CSF_AT(uint32_t, self, 0x48);
    if (initialCount == 0)
        return;

    uint32_t index = 0;
    uint32_t* entry = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x40));
    while (entry[0] != static_cast<uint32_t>(key))
    {
        ++index;
        entry += 2;
        if (index >= initialCount)
            return;
    }

    const int worldObject = static_cast<int>(entry[1]);

    RtDictRemoveEntry(static_cast<int>(CSF_AT(uint32_t, self, 0x3C)), worldObject);
    Rt2dObjectDestroy(worldObject);

    // Count and array are re-read after the calls, as the original does.
    const uint32_t newCount = CSF_AT(uint32_t, self, 0x48) - 1;
    CSF_AT(uint32_t, self, 0x48) = newCount;

    if (index != newCount)
    {
        uint8_t* entries = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x40));
        _memcpy(entries + 8 * index,
                entries + 8 * index + 8,
                static_cast<std::size_t>(8u * (newCount - index)));
    }
}

HOOK(0x6D6A40, SceneLoader_RemoveRenderWorldByKey, "auto");
