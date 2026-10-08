#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

static int (__cdecl* const CompareNamesIgnoreCase)(const char*, const char*) =
    reinterpret_cast<int (__cdecl*)(const char*, const char*)>(CSF_ADDR_FN(0x007B0C41));

static int __fastcall MenuMpAccounts_FindAccountByName(
    void* self, void* /*edx*/, const char* name)
{
    uint32_t index = 0;
    uint32_t byteOffset = 0;
    uint32_t foundAddress = 0;

    do
    {
        const uint32_t begin = CSF_AT(uint32_t, self, 0x08);
        if (!begin)
            break;

        const int32_t byteCount =
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x0C) - begin);
        const uint32_t entryCount = static_cast<uint32_t>(byteCount / 164);
        if (index >= entryCount)
            break;

        const char* entryName = reinterpret_cast<const char*>(
            CSF_AT(uint32_t, self, 0x08) + byteOffset);
        if (CompareNamesIgnoreCase(name, entryName) == 0)
            foundAddress = CSF_AT(uint32_t, self, 0x08) + byteOffset;

        ++index;
        byteOffset += 164;
    } while (!foundAddress);

    return static_cast<int32_t>(foundAddress);
}

HOOK(0x0055BCB0, MenuMpAccounts_FindAccountByName, "auto");
