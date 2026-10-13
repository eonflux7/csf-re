#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall HudPortraits_Serialize(void* self, void* /*edx*/,
                                           S_00691860* archive)
{
    auto* objectBytes = static_cast<uint8_t*>(self);
    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, S_00691860*)>(
            CSF_AT(void**, backend, 0)[0x80 / 4])(backend, nullptr, 0, archive);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, objectBytes + 0x1C, archive) == -1;
    }

    bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(objectBytes + 4), archive);
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(objectBytes + 0x10), archive);
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[0x54 / 4])(
                    backend, nullptr, objectBytes + 0x20, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[0x54 / 4])(
                    backend, nullptr, objectBytes + 0x24, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[0x54 / 4])(
                    backend, nullptr, objectBytes + 0x28, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[0x7C / 4])(backend, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x004580F0, HudPortraits_Serialize, "auto");
