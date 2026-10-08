#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41,
         (const char* first, const char* second));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_00864508_ReleaseNamedEntryObject(
    C_00864508* self, void* /*edx*/, char* name)
{
    const std::uint32_t begin = self->field_0014;
    const std::uint32_t count = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_0018 - begin) >> 6)
        : 0;

    int result = static_cast<int>(begin);
    for (std::uint32_t index = 0, offset = 0; index < count;
         ++index, offset += 0x40)
    {
        void* entry = reinterpret_cast<void*>(self->field_0014 + offset);
        const std::uint32_t capacity = CSF_AT(std::uint32_t, entry, 0x34);
        const char* entryName = capacity < 0x10
            ? reinterpret_cast<const char*>(
                  reinterpret_cast<std::uint8_t*>(entry) + 0x20)
            : CSF_AT(const char*, entry, 0x20);

        result = __stricmp(entryName, name);
        if (result != 0)
            continue;

        const std::uint32_t matchedOffset = index << 6;
        entry = reinterpret_cast<void*>(self->field_0014 + matchedOffset);
        void* object = CSF_AT(void*, entry, 0x3C);
        result = static_cast<int>(reinterpret_cast<std::uintptr_t>(entry));

        if (object)
        {
            MenuExtras_CloseFile(object, nullptr);
            Mem_Free(object);

            entry = reinterpret_cast<void*>(self->field_0014 + matchedOffset);
            CSF_AT(std::uint32_t, entry, 0x3C) = 0;
            result = static_cast<int>(reinterpret_cast<std::uintptr_t>(entry));
        }
        return result;
    }
    return result;
}

HOOK(0x0071A760, C_00864508_ReleaseNamedEntryObject, "auto");
