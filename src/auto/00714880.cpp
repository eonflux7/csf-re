#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(char*, g_NamedEntryName, 0x008A85E4);

static int __fastcall SetFlagsAndUpdateNamedEntry(
    std::uint8_t* self, void* /*edx*/, int value)
{
    void** initialVtable = CSF_AT(void**, self, 0);
    CSF_AT(std::uint8_t, self, 0x1DD) = 1;
    CSF_AT(std::uint8_t, self, 0x140) = 0;

    void* entry =
        ((void*(__fastcall*)(void*, void*, char*))
            initialVtable[0x1BC / 4])(self, nullptr, g_NamedEntryName);

    if (entry)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, entry, 0)[0x84 / 4])(entry, nullptr, value);
    }
    else
    {
        char* name = g_NamedEntryName;
        void** vtable = CSF_AT(void**, self, 0);
        ((void(__fastcall*)(void*, void*, int, int, char*))
            vtable[0x1F8 / 4])(self, nullptr, value, 0, name);
    }

    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x1AC);
    std::int32_t count = 0;
    if (begin != 0)
    {
        const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x1B0);
        count = static_cast<std::int32_t>(end - begin) >> 2;
    }

    char* name = g_NamedEntryName;
    void** vtable = CSF_AT(void**, self, 0);

    // SETNE AL replaces only the low byte, preserving the upper count bits.
    const std::uint32_t state =
        (static_cast<std::uint32_t>(count) & 0xFFFFFF00u) |
        static_cast<std::uint32_t>(count != 0);

    return ((int(__fastcall*)(void*, void*, int, char*))
        vtable[0x20C / 4])(
            self, nullptr, static_cast<std::int32_t>(state), name);
}

HOOK(0x00714880, SetFlagsAndUpdateNamedEntry, "auto");
