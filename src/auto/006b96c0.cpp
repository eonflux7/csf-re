#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, S_006b9130_FindEntryIndex, 0x006B9130,
         (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, S_006b9130_RemoveEntry, 0x006B95E0,
         (void* self, void* edx, uint32_t* a2, uint32_t* a3));

static int __fastcall S_006b9130_ReleaseReference(
    S_006b9130* self, void* /*edx*/, int* handle)
{
    // The packet also classifies this direct-call address as a vtable.
    (void)CSF_VTBL(0x006B9130);

    int* const originalHandle = handle;
    const int index = S_006b9130_FindEntryIndex(self, nullptr, *handle);
    if (index == -1)
    {
        *originalHandle = 0;
        return -1;
    }

    const uint32_t byteOffset = 8u * static_cast<uint32_t>(index);
    auto* entry = reinterpret_cast<uint8_t*>(self->field_0004) + byteOffset;
    CSF_AT(uint32_t, entry, 4) -= 1;

    entry = reinterpret_cast<uint8_t*>(self->field_0004) + byteOffset;
    const int remainingReferences = CSF_AT(int, entry, 4);
    if (remainingReferences != 0)
    {
        *originalHandle = 0;
        return remainingReferences;
    }

    void* const stringObject = CSF_AT(void*, entry, 0);
    if (stringObject)
    {
        if (CSF_AT(uint32_t, stringObject, 0x18) >= 0x10u)
        {
            void* const buffer = CSF_AT(void*, stringObject, 4);
            if (buffer)
                Mem_FreeDebug(buffer);
        }

        CSF_AT(uint32_t, stringObject, 0x18) = 15;
        CSF_AT(uint32_t, stringObject, 0x14) = 0;
        CSF_AT(uint8_t, stringObject, 4) = 0;
        Mem_Free(stringObject);

        entry = reinterpret_cast<uint8_t*>(self->field_0004) + byteOffset;
        CSF_AT(uint32_t, entry, 0) = 0;
    }

    entry = reinterpret_cast<uint8_t*>(self->field_0004) + byteOffset;
    uint32_t argumentSlot = reinterpret_cast<uint32_t>(handle);
    S_006b9130_RemoveEntry(
        self, nullptr, &argumentSlot, reinterpret_cast<uint32_t*>(entry));

    *originalHandle = 0;
    return remainingReferences;
}

HOOK(0x006B96C0, S_006b9130_ReleaseReference, "auto");
