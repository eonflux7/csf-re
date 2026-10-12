#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall Archive_SerializeRecord(void* self, void* /*edx*/, S_00691860* archive)
{
    auto* record = static_cast<std::uint8_t*>(self);
    const std::uint32_t savedContext = CSF_AT(std::uint32_t, archive, 0x1C);

    bool failed = false;
    if (CSF_AT(std::uint8_t, archive, 0x00) == 0)
    {
        void* stream = reinterpret_cast<void*>(archive->field_0004);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, S_00691860*)>(
            CSF_AT(void**, stream, 0)[0x80 / 4])(stream, nullptr, 0, archive);
    }

    CSF_AT(std::uint8_t, archive, 0x00) = failed;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(record), archive);
    }

    bool ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, stream, 0)[0x58 / 4])(
                    stream, nullptr, record + 0x0C, archive);
        CSF_AT(std::uint8_t, archive, 0x00) = result == -1;
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(record + 0x10), archive);
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, stream, 0)[0x54 / 4])(
                    stream, nullptr, record + 0x1C, archive);
        CSF_AT(std::uint8_t, archive, 0x00) = result == -1;
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, stream, 0)[0x54 / 4])(
                    stream, nullptr, record + 0x20, archive);
        CSF_AT(std::uint8_t, archive, 0x00) = result == -1;
    }

    CSF_AT(std::uint32_t, archive, 0x1C) = savedContext;
    if (CSF_AT(std::uint8_t, archive, 0x00) != 0)
    {
        CSF_AT(std::uint8_t, archive, 0x00) = 1;
        return 1;
    }

    void* stream = reinterpret_cast<void*>(archive->field_0004);
    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, S_00691860*)>(
            CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, archive);
    if (static_cast<std::uint8_t>(result) == 0)
    {
        CSF_AT(std::uint8_t, archive, 0x00) = 1;
        return 1;
    }

    CSF_AT(std::uint8_t, archive, 0x00) = 0;
    return result;
}

HOOK(0x00448F30, Archive_SerializeRecord, "auto");
