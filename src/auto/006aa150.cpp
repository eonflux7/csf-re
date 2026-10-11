#include <cstdint>
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
CSF_FUNC(char*, __fastcall, NetGatheringDdl_ConstructAccountClient, 0x00790C70, (void* self, void* edx));
CSF_FUNC(void**, __cdecl, NetGatheringDdl_ConfigureAccountClient, 0x00790CD0, (void** a1));
CSF_FUNC(bool, __fastcall, NetGatheringDdl_BeginAccountCreation, 0x00791280,
         (void* self, void* edx, void* resource, void* response,
          const char* account, const char* password, const char* server,
          unsigned int port, void* context, int flags));
CSF_FUNC(int, __fastcall, NetGatheringDdl_ConstructAccountHelper, 0x0079E340, (void* self, void* edx));

static char __fastcall NetGatheringDdl_CreateAccount(
    char* self, void* /*edx*/, const char* name,
    const char* password, const char* email)
{
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x24 / 4])(self, nullptr);

    const int state = CSF_AT(int, self, 0x8);
    if (state)
    {
        CSF_AT(int, self, 0x2B0) = 1229730611;
        return static_cast<char>(state);
    }

    auto* settings = reinterpret_cast<char*>(
        static_cast<std::uintptr_t>(GetGlobalValue_75FDF0()));
    const char* configuration =
        CSF_AT(unsigned int, settings, 0x8C) < 0x10
            ? settings + 0x78
            : CSF_AT(const char*, settings, 0x78);

    alignas(8) std::uint8_t temporaryBytes[0x4];
    InitializeTemporaryString(
        temporaryBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(configuration)));
    NetGatheringDdl_ConfigureAccountClient(
        reinterpret_cast<void**>(temporaryBytes));
    DestroyTemporaryString(temporaryBytes, nullptr);

    void* allocation = NetZ_OperatorNew(272);
    CSF_AT(void*, self, 0x28) = allocation
        ? NetGatheringDdl_ConstructAccountClient(allocation, nullptr)
        : nullptr;

    allocation = NetZ_OperatorNew(92);
    CSF_AT(int, self, 0x34) = allocation
        ? NetGatheringDdl_ConstructAccountHelper(allocation, nullptr)
        : 0;

    allocation = NetZ_OperatorNew(112);
    CSF_AT(void*, self, 0x90) = allocation
        ? ConstructResource(allocation, nullptr)
        : nullptr;

    auto* portSettings = reinterpret_cast<char*>(
        static_cast<std::uintptr_t>(GetGlobalValue_75FDF0()));
    auto* serverSettings = reinterpret_cast<char*>(
        static_cast<std::uintptr_t>(GetGlobalValue_75FDF0()));

    const char* server =
        CSF_AT(unsigned int, serverSettings, 0x70) < 0x10
            ? serverSettings + 0x5C
            : CSF_AT(const char*, serverSettings, 0x5C);
    const unsigned int port = CSF_AT(std::uint16_t, portSettings, 0x90);

    const bool started = NetGatheringDdl_BeginAccountCreation(
        CSF_AT(void*, self, 0x28), nullptr,
        CSF_AT(void*, self, 0x90), self + 0x94,
        CSF_STR(0x0085FF9C, "**CuentaCrearCSFv1**"),
        CSF_STR(0x0085FF9C, "**CuentaCrearCSFv1**"),
        server, port, self + 0x38, 0);

    if (started)
    {
        AssignStringRange_429B60(
            self + 0x2CC, nullptr, const_cast<char*>(name),
            static_cast<unsigned int>(std::strlen(name)));
        AssignStringRange_429B60(
            self + 0x2E8, nullptr, const_cast<char*>(password),
            static_cast<unsigned int>(std::strlen(password)));
        void* result = AssignStringRange_429B60(
            self + 0x304, nullptr, const_cast<char*>(email),
            static_cast<unsigned int>(std::strlen(email)));
        CSF_AT(int, self, 0x8) = 5;
        CSF_AT(int, self, 0x20) = 0;
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(result));
    }

    char result = 0;
    void* resource = CSF_AT(void*, self, 0x90);
    if (resource)
    {
        result = static_cast<char>(
            reinterpret_cast<std::uintptr_t(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, resource, 0)[0])(resource, nullptr, 1));
        CSF_AT(void*, self, 0x90) = nullptr;
    }
    CSF_AT(int, self, 0x2B0) = 1229730612;
    return result;
}

HOOK(0x006AA150, NetGatheringDdl_CreateAccount, "auto");
