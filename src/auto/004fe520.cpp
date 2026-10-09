#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(unsigned int, __fastcall, ApplySceneEntryValue, 0x006B1840, (void* self, void* edx, int a2));

// Releases the scene entry array of a CSceneLoaderBase. For every 68-byte entry whose
// dword at +0x40 is nonzero, the value is passed to ApplySceneEntryValue, with the
// GetGlobalInt() result as `this`. Then the entry count is cleared and the array is freed.
static int __fastcall CSceneLoaderBase_ReleaseSceneEntries(C_0083f578* self, void* /*edx*/)
{
    uint32_t entryIndex = 0;
    if (CSF_AT(uint32_t, self, 0x44) != 0)
    {
        uint32_t entryOffset = 0;
        do
        {
            uintptr_t entryAddress = static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x3C)) + entryOffset;
            uint32_t entryValue = CSF_AT(uint32_t, reinterpret_cast<void*>(entryAddress), 0x40);
            if (entryValue != 0)
            {
                int value = static_cast<int>(entryValue);
                void* handlerThis = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
                ApplySceneEntryValue(handlerThis, nullptr, value);
            }
            ++entryIndex;
            entryOffset += 0x44;
        } while (entryIndex < CSF_AT(uint32_t, self, 0x44));
    }

    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x3C));
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    if (result != 0)
    {
        result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x3C))));
        CSF_AT(uint32_t, self, 0x3C) = 0;
    }
    return result;
}

HOOK(0x004FE520, CSceneLoaderBase_ReleaseSceneEntries, "auto");
