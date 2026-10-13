#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(bool, __cdecl, CoreFiles_DeleteFile, 0x006976C0, (const char* path));
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));
CSF_FUNC(int, __cdecl, CoreFiles_CheckPageAvailability, 0x00698B20, (const char* path, char bypassList));

static char __fastcall NetProfile_DeleteProfileFile(void* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0x1704) = 0;

    const char* path = CSF_AT(std::uint32_t, self, 0x1724) < 0x10u
        ? reinterpret_cast<const char*>(self) + 0x1710
        : CSF_AT(const char*, self, 0x1710);

    if (!*path)
    {
        CSF_AT(std::uint32_t, self, 0x1704) = 13;
        return 0;
    }

    const char savedOption = GetOptionsByteAt2();
    if (savedOption)
        CoreFiles_SetFileOption(0);

    path = CSF_AT(std::uint32_t, self, 0x1724) < 0x10u
        ? reinterpret_cast<const char*>(self) + 0x1710
        : CSF_AT(const char*, self, 0x1710);

    if (static_cast<std::uint8_t>(CoreFiles_CheckPageAvailability(path, 0)))
        CSF_AT(std::uint32_t, self, 0x1704) = 14;

    path = CSF_AT(std::uint32_t, self, 0x1724) < 0x10u
        ? reinterpret_cast<const char*>(self) + 0x1710
        : CSF_AT(const char*, self, 0x1710);

    if (CoreFiles_DeleteFile(path))
    {
        CSF_AT(std::uint8_t, self, 0x1708) = 1;
        CoreFiles_SetFileOption(savedOption);
        return 1;
    }

    CSF_AT(std::uint32_t, self, 0x1704) = 15;
    CoreFiles_SetFileOption(savedOption);
    return 0;
}

HOOK(0x00517840, NetProfile_DeleteProfileFile, "auto");
