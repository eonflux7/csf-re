#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_0069f3b0_LoadVersion2Directory, 0x0069F3B0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_0069f3b0_LoadVersion3Or4Directory, 0x0069F520, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_0069f3b0_LoadVersion5Directory, 0x0069F6C0, (void* self, void* edx, int a2));
CSF_FUNC(void**, __cdecl, GetSharedLogWindow, 0x006A3110, ());

CSF_GLOBAL(std::uint32_t, g_ArchiveVersionFlags, 0x008BB3FC);

static char __fastcall S_0069f3b0_LoadArchiveHeader(
    S_0069f3b0* self, void* /*edx*/, int streamAddress)
{
    alignas(8) std::uint8_t signature[0x100];
    std::memset(signature, 0, sizeof(signature));
    std::uint32_t version = 0;

    self->field_0004 = static_cast<std::uint32_t>(streamAddress);
    void* stream = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(static_cast<std::uint32_t>(streamAddress)));

    ((void(__fastcall*)(void*, void*, void*, int))
        CSF_AT(void**, stream, 0)[8 / 4])(
            stream, nullptr, signature, 4);

    if (std::memcmp(signature, CSF_STR(0x0085EBC4, "PAKC"), 5) == 0)
    {
        self->field_003c = 1;
        self->field_0040 =
            reinterpret_cast<std::uintptr_t>(GetSharedLogWindow());
    }
    else if (std::memcmp(signature, CSF_STR(0x0085EBBC, "PAKA"), 5) != 0)
    {
        return 0;
    }

    stream = reinterpret_cast<void*>(self->field_0004);
    ((void(__fastcall*)(void*, void*, void*, int))
        CSF_AT(void**, stream, 0)[8 / 4])(
            stream, nullptr, &version, 4);

    g_ArchiveVersionFlags = 0;
    if (version >= 4)
        g_ArchiveVersionFlags = 13;

    switch (version)
    {
    case 2:
        // Also listed as a vtable reference in the packet.
        (void)CSF_VTBL(0x0069F3B0);
        return S_0069f3b0_LoadVersion2Directory(self, nullptr, streamAddress);
    case 3:
    case 4:
        return S_0069f3b0_LoadVersion3Or4Directory(self, nullptr, streamAddress);
    case 5:
        return S_0069f3b0_LoadVersion5Directory(self, nullptr, streamAddress);
    default:
        return 0;
    }
}

HOOK(0x0069F850, S_0069f3b0_LoadArchiveHeader, "auto");
