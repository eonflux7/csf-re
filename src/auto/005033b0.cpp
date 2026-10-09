#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __stdcall, SceneLoader_SerializeRecordBase, 0x0043FAF0, (void* a1, int a2));
CSF_FUNC(int, __stdcall, SceneLoader_SerializeRecordMember, 0x00445620, (void* a1, int a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));

static int __fastcall SceneLoader_SerializeMapRecord(void* self, void* /*edx*/, void* archive)
{
    auto* record = static_cast<std::uint8_t*>(self);
    const std::uint32_t savedContext = CSF_AT(std::uint32_t, archive, 0x1C);

    bool failed = false;
    if (CSF_AT(std::uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, stream, 0x00)[0x80 / 4])(stream, nullptr, 0, archive);
    }

    CSF_AT(std::uint8_t, archive, 0x00) = failed;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        SceneLoader_SerializeRecordBase(self, reinterpret_cast<int>(archive));
    }

    bool ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        SceneLoader_SerializeRecordMember(record + 0x48, reinterpret_cast<int>(archive));
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        const int result = reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
            CSF_AT(void**, stream, 0x00)[0x44 / 4])(stream, nullptr, record + 0x1C, archive);
        CSF_AT(std::uint8_t, archive, 0x00) = result == -1;
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        const int result = reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
            CSF_AT(void**, stream, 0x00)[0x44 / 4])(stream, nullptr, record + 0x1D, archive);
        CSF_AT(std::uint8_t, archive, 0x00) = result == -1;
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        const int result = reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
            CSF_AT(void**, stream, 0x00)[0x48 / 4])(stream, nullptr, record + 0x20, archive);
        CSF_AT(std::uint8_t, archive, 0x00) = result == -1;
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(record + 0x24), archive);
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(record + 0x30), archive);
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(record + 0x3C), archive);
    }

    CSF_AT(std::uint32_t, archive, 0x1C) = savedContext;
    if (CSF_AT(std::uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        const int result = reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, stream, 0x00)[0x7C / 4])(stream, nullptr, archive);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x005033B0, SceneLoader_SerializeMapRecord, "auto");
