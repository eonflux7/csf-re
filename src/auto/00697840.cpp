#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(char, __fastcall, CoreFiles_ContainsFile, 0x0069E830,
               (void* self, void* edx, char* a2));
static CSF_FUNC(uint32_t*, __fastcall, CoreFiles_FindFile, 0x0069E910,
               (void* self, void* edx, char* arg0));

static int __cdecl CoreFiles_FindFileInContainers(char* fileName, void* containers)
{
    const uint32_t begin = CSF_AT(uint32_t, containers, 4);
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(CSF_AT(uint32_t, containers, 8) - begin) >> 5)
        : 0;

    for (uint32_t index = 0; index < count; ++index)
    {
        const uint32_t entry = CSF_AT(uint32_t, containers, 4) + index * 32;
        void* container = CSF_AT(void*, reinterpret_cast<void*>(entry), 0x1C);
        if (CoreFiles_ContainsFile(container, nullptr, fileName))
        {
            const uint32_t matchedEntry =
                CSF_AT(uint32_t, containers, 4) + index * 32;
            container = CSF_AT(void*, reinterpret_cast<void*>(matchedEntry), 0x1C);
            return reinterpret_cast<int>(
                CoreFiles_FindFile(container, nullptr, fileName));
        }
    }
    return 0;
}

HOOK(0x00697840, CoreFiles_FindFileInContainers, "auto");
