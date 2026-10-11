#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(char*, __fastcall, ConstructGatheringClient, 0x00790C70, (void* self, void* edx));
CSF_FUNC(void**, __cdecl, ConfigureGatheringClient, 0x00790CD0, (void** a1));
CSF_FUNC(bool, __fastcall, StartGatheringClient, 0x00791280,
         (void* self, void* edx, void* resource, void* sessionData,
          std::uint32_t firstArgument, int secondArgument, const char* address,
          unsigned int port, void* connectionData, int flags));
CSF_FUNC(int, __fastcall, ConstructGatheringSupport, 0x0079E340, (void* self, void* edx));

static char __fastcall NetGatheringDdl_InitializeConnection(
    void* self, void* /*edx*/, std::uint32_t firstArgument,
    int secondArgument, const char* name,
    int /*unused4*/, int /*unused5*/, int /*unused6*/, int /*unused7*/)
{
    auto* object = static_cast<std::uint8_t*>(self);

    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x24 / 4])(self, nullptr);

    const int state = CSF_AT(int, self, 0x08);
    if (state != 0)
    {
        CSF_AT(int, self, 0x2B0) = 1229730616;
        return static_cast<char>(state);
    }

    auto* configuration =
        reinterpret_cast<std::uint8_t*>(GetGlobalValue_75FDF0());
    const char* configurationText;
    if (CSF_AT(std::uint32_t, configuration, 0x8C) < 0x10)
        configurationText = reinterpret_cast<const char*>(configuration + 0x78);
    else
        configurationText = CSF_AT(const char*, configuration, 0x78);

    alignas(8) std::uint8_t temporaryStringBytes[0x4];
    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(
            reinterpret_cast<std::intptr_t>(configurationText)));
    ConfigureGatheringClient(reinterpret_cast<void**>(temporaryStringBytes));
    DestroyTemporaryString(temporaryStringBytes, nullptr);

    void* allocation = NetZ_OperatorNew(272);
    char* client = allocation
        ? ConstructGatheringClient(allocation, nullptr)
        : nullptr;
    CSF_AT(char*, self, 0x28) = client;

    allocation = NetZ_OperatorNew(92);
    const int support = allocation
        ? ConstructGatheringSupport(allocation, nullptr)
        : 0;
    CSF_AT(int, self, 0x34) = support;

    allocation = NetZ_OperatorNew(112);
    void* resource = allocation
        ? ConstructResource(allocation, nullptr)
        : nullptr;
    CSF_AT(void*, self, 0x90) = resource;

    auto* portConfiguration =
        reinterpret_cast<std::uint8_t*>(GetGlobalValue_75FDF0());
    auto* addressConfiguration =
        reinterpret_cast<std::uint8_t*>(GetGlobalValue_75FDF0());

    const char* address;
    if (CSF_AT(std::uint32_t, addressConfiguration, 0x70) < 0x10)
        address = reinterpret_cast<const char*>(addressConfiguration + 0x5C);
    else
        address = CSF_AT(const char*, addressConfiguration, 0x5C);

    const unsigned int port =
        CSF_AT(std::uint16_t, portConfiguration, 0x90);

    if (StartGatheringClient(
            CSF_AT(void*, self, 0x28), nullptr,
            CSF_AT(void*, self, 0x90), object + 0x94,
            firstArgument, secondArgument, address, port, object + 0x38, 0))
    {
        void* result = AssignStringRange_429B60(
            object + 0x2E8, nullptr, const_cast<char*>(name),
            static_cast<unsigned int>(std::strlen(name)));
        CSF_AT(int, self, 0x08) = 6;
        CSF_AT(int, self, 0x24) = 0;
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(result));
    }

    char result = 0;
    resource = CSF_AT(void*, self, 0x90);
    if (resource)
    {
        result = static_cast<char>(
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, resource, 0)[0])(resource, nullptr, 1));
        CSF_AT(void*, self, 0x90) = nullptr;
    }
    CSF_AT(int, self, 0x2B0) = 1229730615;
    return result;
}

HOOK(0x006AA300, NetGatheringDdl_InitializeConnection, "auto");
