#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(void**, __fastcall, NetGatheringDdl_SetRequestString, 0x00798E80, (void* self, void* edx, void** a2));
CSF_FUNC(int, __fastcall, StartRequest, 0x00799220, (void* self, void* edx, void* request, void* requestData, void* parameters));

static void __fastcall NetGatheringDdl_SubmitGatheringRequest(
    void* self, void* /*edx*/, std::int32_t source,
    std::uint32_t firstOption, std::uint32_t secondOption, char* description)
{
    if (CSF_AT(std::int32_t, self, 0x08) != 7)
        return;

    // The original uses overlapping regions of its incoming argument block
    // as temporary strings. Preserve those writes and subsequent reads.
    alignas(8) std::uint8_t argumentBytes[16];
    CSF_AT(std::int32_t, argumentBytes, 0x00) = source;
    CSF_AT(std::uint32_t, argumentBytes, 0x04) = firstOption;
    CSF_AT(std::uint32_t, argumentBytes, 0x08) = secondOption;
    CSF_AT(char*, argumentBytes, 0x0C) = description;

    InitializeTemporaryString(argumentBytes, nullptr, source);
    NetGatheringDdl_SetRequestString(
        CSF_AT(void*, self, 0x88), nullptr,
        reinterpret_cast<void**>(argumentBytes));
    DestroyTemporaryString(argumentBytes, nullptr);

    void* requestData = CSF_AT(void*, self, 0x88);
    const std::uint8_t firstByte = CSF_AT(std::uint8_t, argumentBytes, 0x04);
    const std::uint8_t secondByte = CSF_AT(std::uint8_t, argumentBytes, 0x08);
    CSF_AT(std::uint8_t, requestData, 0x30) = firstByte;
    CSF_AT(std::uint8_t, requestData, 0x32) = secondByte;
    CSF_AT(std::uint8_t, requestData, 0x31) = 0;

    InitializeTemporaryString(argumentBytes + 0x04, nullptr, source);
    NetGatheringDdl_SetRequestString(
        requestData, nullptr,
        reinterpret_cast<void**>(argumentBytes + 0x04));
    DestroyTemporaryString(argumentBytes + 0x04, nullptr);

    char* text = CSF_AT(char*, argumentBytes, 0x0C);
    CSF_AT(std::uint16_t, requestData, 0x12) = 0;
    char* destination = reinterpret_cast<char*>(requestData) + 0x54;
    char character;
    do
    {
        character = *text++;
        *destination++ = character;
    } while (character != 0);

    void* oldRequest = CSF_AT(void*, self, 0x90);
    if (oldRequest)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, oldRequest, 0)[0])(oldRequest, nullptr, 1);
        CSF_AT(void*, self, 0x90) = nullptr;
    }

    void* allocation = NetZ_OperatorNew(112);
    void* request = allocation ? ConstructResource(allocation, nullptr) : nullptr;
    void* requestOwner = CSF_AT(void*, self, 0x2C);
    CSF_AT(void*, self, 0x90) = request;

    const int result = StartRequest(
        requestOwner, nullptr, request,
        reinterpret_cast<std::uint8_t*>(self) + 0x2B4,
        reinterpret_cast<std::uint8_t*>(self) + 0x84);

    if (static_cast<std::uint8_t>(result) != 0)
    {
        CSF_AT(std::int32_t, self, 0x08) = 10;
    }
    else
    {
        void* failedRequest = CSF_AT(void*, self, 0x90);
        CSF_AT(std::uint32_t, self, 0x2B0) = 1229732148;
        if (failedRequest)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, failedRequest, 0)[0])(failedRequest, nullptr, 1);
            CSF_AT(void*, self, 0x90) = nullptr;
        }
    }
}

HOOK(0x006A8BF0, NetGatheringDdl_SubmitGatheringRequest, "auto");
