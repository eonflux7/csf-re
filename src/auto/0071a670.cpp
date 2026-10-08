#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (unsigned int size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* memory));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770,
         (void* self, void* edx, const char* name));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41,
         (const char* first, const char* second));

static const char* GetEntryName(void* entry)
{
    if (CSF_AT(uint32_t, entry, 0x34) < 16u)
        return reinterpret_cast<const char*>(entry) + 0x20;

    return CSF_AT(const char*, entry, 0x20);
}

static int __fastcall C_00864508_GetOrLoadNamedResource(
    C_00864508* self, void* /*edx*/, char* name)
{
    const uint32_t begin = self->field_0014;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0018 - begin) >> 6)
        : 0u;

    if (count == 0)
        return 0;

    uint32_t index = 0;
    uint32_t offset = 0;
    for (;;)
    {
        void* entry = reinterpret_cast<void*>(self->field_0014 + offset);
        if (__stricmp(GetEntryName(entry), name) == 0)
            break;

        ++index;
        offset += 0x40;
        if (index >= count)
            return 0;
    }

    void* entry = reinterpret_cast<void*>(self->field_0014 + (index << 6));
    if (CSF_AT(uint32_t, entry, 0x3C) == 0)
    {
        void* memory = Mem_Alloc(0x3C);
        void* resource = memory ? MenuExtras_InitializeFile(memory, nullptr) : nullptr;
        CSF_AT(void*, entry, 0x3C) = resource;

        if (!MenuExtras_LoadFile(resource, nullptr, GetEntryName(entry)))
        {
            void* failedResource = CSF_AT(void*, entry, 0x3C);
            if (failedResource)
            {
                MenuExtras_CloseFile(failedResource, nullptr);
                Mem_Free(failedResource);
                CSF_AT(uint32_t, entry, 0x3C) = 0;
            }
        }
    }

    return static_cast<int>(CSF_AT(uint32_t, entry, 0x3C));
}

HOOK(0x0071A670, C_00864508_GetOrLoadNamedResource, "auto");
