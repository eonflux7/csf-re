#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, S_006b9130_FindEntryIndex, 0x006B9130, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, S_006b9130_RemoveEntry, 0x006B95E0, (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, S_006b9130_UnregisterObject, 0x006B96C0, (void* self, void* edx, int* a2));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int32_t, g_ObjectRegistryOffset, 0x008BB474);

static int __fastcall S_006b9130_ReleaseObject(
    S_006b9130* self, void* /*edx*/, int* objectHandle)
{
    // The packet also lists this callee address as a vtable reference.
    (void)CSF_VTBL(0x006B9130);

    const int index = S_006b9130_FindEntryIndex(self, nullptr, *objectHandle);
    if (index == -1)
    {
        *objectHandle = 0;
        return -1;
    }

    const uint32_t entryOffset = static_cast<uint32_t>(index) * 8;
    auto* entries = reinterpret_cast<uint8_t*>(self->field_0004);
    --CSF_AT(uint32_t, entries + entryOffset, 0x4);

    entries = reinterpret_cast<uint8_t*>(self->field_0004);
    const int remainingReferences = CSF_AT(int, entries + entryOffset, 0x4);
    if (remainingReferences != 0)
    {
        *objectHandle = 0;
        return remainingReferences;
    }

    alignas(8) uint8_t scratchBytes[0x4];
    uint32_t& scratch = *reinterpret_cast<uint32_t*>(scratchBytes);
    scratch = 0;

    int* object = CSF_AT(int*, entries + entryOffset, 0x0);
    if (object)
    {
        scratch = reinterpret_cast<uint32_t>(object);
        CSF_AT(int, object, 0x4) = -1;

        void* registry = CSF_AT(void*, g_pRwEngineInstance, g_ObjectRegistryOffset);
        S_006b9130_UnregisterObject(registry, nullptr, object);
        Mem_Free(reinterpret_cast<void*>(scratch));

        CSF_AT(uint32_t,
               reinterpret_cast<uint8_t*>(self->field_0004) + entryOffset,
               0x0) = 0;
    }

    S_006b9130_RemoveEntry(
        self, nullptr, &scratch,
        reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(self->field_0004) + entryOffset));

    *objectHandle = 0;
    return remainingReferences;
}

HOOK(0x006B9E10, S_006b9130_ReleaseObject, "auto");
