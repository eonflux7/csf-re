#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Campaign_SerializeRecordBase, 0x0043FAF0, (C_0082d410* record, int stateAddress));

static int __fastcall HudPortraits_Serialize(
    std::uint8_t* self, void* /*edx*/, int stateAddress)
{
    auto* state = reinterpret_cast<std::uint8_t*>(stateAddress);

    bool failedToBegin = false;
    if (CSF_AT(std::uint8_t, state, 0) == 0)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        failedToBegin =
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, serializer, 0)[0x80 / 4])(
                    serializer, nullptr, 0, stateAddress);
    }

    const int savedContext = CSF_AT(int, state, 0x1C);
    CSF_AT(std::uint8_t, state, 0) = failedToBegin;
    CSF_AT(int, state, 0x1C) = 0;

    if (!failedToBegin)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        CSF_AT(std::uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, self + 4, stateAddress) == -1;
    }

    bool canContinue = CSF_AT(std::uint8_t, state, 0) == 0;
    CSF_AT(int, state, 0x1C) = 0;
    if (canContinue)
    {
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        Campaign_SerializeRecordBase(
            reinterpret_cast<C_0082d410*>(self + 8), stateAddress);
    }

    canContinue = CSF_AT(std::uint8_t, state, 0) == 0;
    CSF_AT(int, state, 0x1C) = 0;
    if (canContinue)
    {
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        Campaign_SerializeRecordBase(
            reinterpret_cast<C_0082d410*>(self + 36), stateAddress);
    }

    canContinue = CSF_AT(std::uint8_t, state, 0) == 0;
    CSF_AT(int, state, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        CSF_AT(std::uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, self + 64, stateAddress) == -1;
    }

    canContinue = CSF_AT(std::uint8_t, state, 0) == 0;
    CSF_AT(int, state, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        CSF_AT(std::uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, self + 68, stateAddress) == -1;
    }

    canContinue = CSF_AT(std::uint8_t, state, 0) == 0;
    CSF_AT(int, state, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        CSF_AT(std::uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, self + 72, stateAddress) == -1;
    }

    canContinue = CSF_AT(std::uint8_t, state, 0) == 0;
    CSF_AT(int, state, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        CSF_AT(std::uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr, self + 76, stateAddress) == -1;
    }

    canContinue = CSF_AT(std::uint8_t, state, 0) == 0;
    CSF_AT(int, state, 0x1C) = savedContext;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        // Only the low byte is tested, but the entire result is returned.
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, stateAddress);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, state, 0) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x00457820, HudPortraits_Serialize, "auto");
