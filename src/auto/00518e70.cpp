#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(unsigned int, __fastcall, ProfileEntry_Destroy, 0x0069AE00, (void* self, void* edx));

// Re-reads the entry array base pointer (this+0x0C) at each use, as the original does.
static uint8_t* ReadEntryArrayBase(int* self)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x0C)));
}

// Destroys every entry object in the array, frees the array block, and clears the base pointer.
static int __fastcall NetProfile_ReleaseEntryArray(int* self, void* /*edx*/)
{
    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x14));

    if (CSF_AT(uint32_t, self, 0x14) != 0)
    {
        uint32_t index = 0;
        do
        {
            uint8_t* entryBase = ReadEntryArrayBase(self);
            void* entryObject = *reinterpret_cast<void**>(entryBase + index * 24);
            if (entryObject != nullptr)
            {
                ProfileEntry_Destroy(entryObject, nullptr);
                Mem_Free(entryObject);
                uint8_t* reloadedBase = ReadEntryArrayBase(self);
                *reinterpret_cast<void**>(reloadedBase + index * 24) = nullptr;
            }
            ++index;
            result = static_cast<int>(index);
        } while (index < CSF_AT(uint32_t, self, 0x14));
    }

    uint8_t* headerBase = ReadEntryArrayBase(self);
    if (headerBase != nullptr)
    {
        // Element count is stored in the dword just before the array block.
        int32_t elementCount = *reinterpret_cast<int32_t*>(headerBase - 4);
        for (int32_t i = elementCount - 1; i >= 0; --i)
        {
            *reinterpret_cast<void**>(headerBase + 24 * i + 12) = CSF_VTBL(0x0082D410);
        }
        result = Mem_Free(headerBase - 4);
        CSF_AT(uint32_t, self, 0x0C) = 0;
    }

    return result;
}

HOOK(0x00518E70, NetProfile_ReleaseEntryArray, "auto");
