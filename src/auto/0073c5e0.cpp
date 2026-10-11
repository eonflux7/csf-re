#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, NetSession_GetTransportRegistry, 0x006487D0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_FindTransport, 0x00648910,
         (void* self, void* edx, char* String2));
CSF_FUNC(int, __fastcall, NetSession_GetFirstEndpoint, 0x006486C0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_GetSecondEndpoint, 0x006486D0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_SetEndpointLimit, 0x00658240,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_SetEndpointOption, 0x0068F740,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_SetEndpointState, 0x00648770,
         (void* self, void* edx, int a2));

static void* NetSession_AsObjectPointer(int value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

static int __cdecl NetSession_ConfigurePrudpEndpoints()
{
    void* registry = NetSession_GetTransportRegistry(nullptr, nullptr);
    void* transport = NetSession_AsObjectPointer(
        NetSession_FindTransport(
            registry, nullptr, const_cast<char*>(CSF_STR(0x0084468C, "prudp"))));

    NetSession_SetEndpointLimit(
        NetSession_AsObjectPointer(NetSession_GetFirstEndpoint(transport, nullptr)),
        nullptr, 0x200000);
    NetSession_SetEndpointOption(
        NetSession_AsObjectPointer(NetSession_GetFirstEndpoint(transport, nullptr)),
        nullptr, 0);
    NetSession_SetEndpointState(
        NetSession_AsObjectPointer(NetSession_GetFirstEndpoint(transport, nullptr)),
        nullptr, 0);

    NetSession_SetEndpointLimit(
        NetSession_AsObjectPointer(NetSession_GetSecondEndpoint(transport, nullptr)),
        nullptr, 0x200000);
    NetSession_SetEndpointOption(
        NetSession_AsObjectPointer(NetSession_GetSecondEndpoint(transport, nullptr)),
        nullptr, 0);
    return NetSession_SetEndpointState(
        NetSession_AsObjectPointer(NetSession_GetSecondEndpoint(transport, nullptr)),
        nullptr, 0);
}

HOOK(0x0073C5E0, NetSession_ConfigurePrudpEndpoints, "auto");
