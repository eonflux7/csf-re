#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall Archive_SerializeEntry(void* self, void* /*edx*/, S_00691860* entry)
{
    const uint32_t savedState = CSF_AT(uint32_t, entry, 0x1C);
    bool failed = false;
    if (!entry->pad_0000[0])
    {
        void* stream = reinterpret_cast<void*>(entry->field_0004);
        failed = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
            CSF_AT(void**, stream, 0)[0x80 / 4])(stream, nullptr, 0, entry);
    }

    entry->pad_0000[0] = failed;
    CSF_AT(uint32_t, entry, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, entry, 0x15) = 0;
        CSF_AT(uint8_t, entry, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(self), entry);
    }

    const bool serializeSecond = entry->pad_0000[0] == 0;
    CSF_AT(uint32_t, entry, 0x1C) = 0;
    if (serializeSecond)
    {
        CSF_AT(uint8_t, entry, 0x15) = 0;
        CSF_AT(uint8_t, entry, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(static_cast<uint8_t*>(self) + 12), entry);
    }

    CSF_AT(uint32_t, entry, 0x1C) = savedState;
    if (!entry->pad_0000[0])
    {
        void* stream = reinterpret_cast<void*>(entry->field_0004);
        const int result = ((int(__fastcall*)(void*, void*, S_00691860*))
            CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, entry);
        if (static_cast<uint8_t>(result))
        {
            entry->pad_0000[0] = 0;
            return result;
        }
    }

    entry->pad_0000[0] = 1;
    return 1;
}

HOOK(0x00448920, Archive_SerializeEntry, "auto");
