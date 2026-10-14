#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __stdcall, SceneLoader_SerializeRecordBase, 0x0043FAF0, (void* a1, int a2));

static int __fastcall ProfilesSavegames_SerializeRecord(
    char* self, void* /*edx*/, int recordAddress)
{
    void* record = reinterpret_cast<void*>(recordAddress);
    const int savedMode = CSF_AT(int, record, 0x1C);

    bool failed = false;
    if (CSF_AT(std::uint8_t, record, 0) == 0)
    {
        void* stream = CSF_AT(void*, record, 4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, stream, 0)[0x80 / 4])(stream, nullptr, 0, record);
    }

    CSF_AT(std::uint8_t, record, 0) = failed;
    CSF_AT(int, record, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(std::uint8_t, record, 0x15) = 0;
        CSF_AT(std::uint8_t, record, 0x16) = 0;
        SceneLoader_SerializeRecordBase(self, recordAddress);
    }

    bool canContinue = CSF_AT(std::uint8_t, record, 0) == 0;
    CSF_AT(int, record, 0x1C) = 0;
    if (canContinue)
    {
        CSF_AT(std::uint8_t, record, 0x15) = 0;
        CSF_AT(std::uint8_t, record, 0x16) = 0;
        SceneLoader_SerializeRecordBase(self + 28, recordAddress);
    }

    canContinue = CSF_AT(std::uint8_t, record, 0) == 0;
    CSF_AT(int, record, 0x1C) = 0;
    if (canContinue)
    {
        void* stream = CSF_AT(void*, record, 4);
        CSF_AT(std::uint8_t, record, 0x15) = 0;
        CSF_AT(std::uint8_t, record, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, char*, void*)>(
                CSF_AT(void**, stream, 0)[0x48 / 4])(
                    stream, nullptr, self + 56, record);
        CSF_AT(std::uint8_t, record, 0) = result == -1;
    }

    canContinue = CSF_AT(std::uint8_t, record, 0) == 0;
    CSF_AT(int, record, 0x1C) = 0;
    if (canContinue)
    {
        void* stream = CSF_AT(void*, record, 4);
        CSF_AT(std::uint8_t, record, 0x15) = 0;
        CSF_AT(std::uint8_t, record, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, char*, void*)>(
                CSF_AT(void**, stream, 0)[0x48 / 4])(
                    stream, nullptr, self + 60, record);
        CSF_AT(std::uint8_t, record, 0) = result == -1;
    }

    CSF_AT(int, record, 0x1C) = savedMode;
    if (CSF_AT(std::uint8_t, record, 0) == 0)
    {
        void* stream = CSF_AT(void*, record, 4);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[0x7C / 4])(
                    stream, nullptr, record);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, record, 0) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, record, 0) = 1;
    return 1;
}

HOOK(0x00514390, ProfilesSavegames_SerializeRecord, "auto");
