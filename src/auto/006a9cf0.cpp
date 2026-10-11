#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(char, __stdcall, GlobalObject_ApplyMode, 0x0073C7F0, (char mode));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(char*, __fastcall, NetGatheringDdl_ConstructConnection, 0x00790C70, (void* self, void* edx));
CSF_FUNC(void**, __cdecl, NetGatheringDdl_ConfigureConnectionName, 0x00790CD0, (void** a1));
CSF_FUNC(bool, __fastcall, NetGatheringDdl_StartConnection, 0x00791280,
         (void* self, void* edx, void* resource, void* state,
          std::uint32_t firstArgument, int secondArgument, const char* address,
          unsigned int port, void* output, int flags));
CSF_FUNC(int, __fastcall, NetGatheringDdl_ConstructSession, 0x00799800, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetGatheringDdl_ConstructSessionHelper, 0x0079CA60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetGatheringDdl_ConstructSessionState, 0x007A0730, (void* self, void* edx));
CSF_FUNC(uintptr_t, __cdecl, __beginthread, 0x007B07F5, (void (__cdecl* startAddress)(void*), unsigned stackSize, void* arg));

CSF_GLOBAL(std::uint8_t, g_GatheringThreadFlag, 0x008BB41C);
CSF_GLOBAL(std::uint8_t, g_GatheringThreadExitFlag, 0x008BB41D);

static char __fastcall NetGatheringDdl_StartGathering(
    void* self, void* /*edx*/, std::uint32_t firstArgument, int secondArgument)
{
    auto* bytes = static_cast<std::uint8_t*>(self);
    void** vtable = CSF_AT(void**, self, 0);
    CSF_AT(std::uint32_t, self, 0x338) = 0;
    reinterpret_cast<void (__fastcall*)(void*, void*)>(vtable[0x24 / 4])(
        self, nullptr);

    if (CSF_AT(void*, self, 0x28))
    {
        reinterpret_cast<char (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, self, 0)[0x44 / 4])(self, nullptr, 0);
    }

    auto* settings = reinterpret_cast<std::uint8_t*>(GetGlobalValue_75FDF0());
    const char* connectionName;
    if (CSF_AT(std::uint32_t, settings, 0x8C) < 0x10)
        connectionName = reinterpret_cast<const char*>(settings + 0x74 + 4);
    else
        connectionName = CSF_AT(const char*, settings + 0x74, 4);

    alignas(8) std::uint8_t temporaryString[0x4];
    InitializeTemporaryString(
        temporaryString, nullptr, reinterpret_cast<std::int32_t>(connectionName));
    NetGatheringDdl_ConfigureConnectionName(
        reinterpret_cast<void**>(temporaryString));
    DestroyTemporaryString(temporaryString, nullptr);

    CSF_AT(int, self, 0x8) = 1;

    void* allocation = NetZ_OperatorNew(272);
    CSF_AT(char*, self, 0x28) =
        allocation ? NetGatheringDdl_ConstructConnection(allocation, nullptr) : nullptr;

    allocation = NetZ_OperatorNew(160);
    CSF_AT(int, self, 0x2C) =
        allocation ? NetGatheringDdl_ConstructSession(allocation, nullptr) : 0;

    allocation = NetZ_OperatorNew(40);
    CSF_AT(int, self, 0x30) =
        allocation ? NetGatheringDdl_ConstructSessionHelper(allocation, nullptr) : 0;

    allocation = NetZ_OperatorNew(232);
    int sessionState =
        allocation ? NetGatheringDdl_ConstructSessionState(allocation, nullptr) : 0;

    void* connection = CSF_AT(void*, self, 0x28);
    CSF_AT(int, self, 0x3C) = sessionState;
    CSF_AT(int, self, 0x38) = 0;

    if (!connection || !CSF_AT(int, self, 0x2C) ||
        !CSF_AT(int, self, 0x30) || !sessionState)
    {
        vtable = CSF_AT(void**, self, 0);
        CSF_AT(std::uint32_t, self, 0x2B0) = 1229729840;
        return reinterpret_cast<char (__fastcall*)(void*, void*, int)>(
            vtable[0x44 / 4])(self, nullptr, 0);
    }

    allocation = NetZ_OperatorNew(112);
    CSF_AT(void*, self, 0x90) =
        allocation ? ConstructResource(allocation, nullptr) : nullptr;

    auto* portSettings =
        reinterpret_cast<std::uint8_t*>(GetGlobalValue_75FDF0());
    settings = reinterpret_cast<std::uint8_t*>(GetGlobalValue_75FDF0());

    const char* address;
    if (CSF_AT(std::uint32_t, settings, 0x70) < 0x10)
        address = reinterpret_cast<const char*>(settings + 0x58 + 4);
    else
        address = CSF_AT(const char*, settings + 0x58, 4);

    unsigned int port = CSF_AT(std::uint16_t, portSettings, 0x90);
    void* resource = CSF_AT(void*, self, 0x90);
    bool started = NetGatheringDdl_StartConnection(
        CSF_AT(void*, self, 0x28), nullptr, resource, bytes + 0x94,
        firstArgument, secondArgument, address, port, bytes + 0x38, 0);

    if (started)
    {
        GetGlobalDword_73C020();
        GlobalObject_ApplyMode(0);
        g_GatheringThreadFlag = 0;
        g_GatheringThreadExitFlag = 0;
        return static_cast<char>(__beginthread(
            reinterpret_cast<void (__cdecl*)(void*)>(CSF_ADDR_FN(0x006A9890)),
            0, self));
    }

    resource = CSF_AT(void*, self, 0x90);
    if (resource)
    {
        reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, resource, 0)[0])(resource, nullptr, 1);
        CSF_AT(void*, self, 0x90) = nullptr;
    }

    vtable = CSF_AT(void**, self, 0);
    CSF_AT(std::uint32_t, self, 0x2B0) = 1229729842;
    return reinterpret_cast<char (__fastcall*)(void*, void*, int)>(
        vtable[0x44 / 4])(self, nullptr, 0);
}

HOOK(0x006A9CF0, NetGatheringDdl_StartGathering, "auto");
