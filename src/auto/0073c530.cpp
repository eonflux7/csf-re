#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, NetSession_GetTransportRegistry, 0x006487D0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_FindTransport, 0x00648910,
         (void* self, void* edx, char* String2));
CSF_FUNC(int, __fastcall, NetSession_GetPrimaryEndpoint, 0x006486C0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_GetSecondaryEndpoint, 0x006486D0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_SetEndpointValue, 0x00658240,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_SetEndpointAttribute, 0x0068F740,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_ConfigureEndpoint, 0x00648770,
         (void* self, void* edx, int a2));

static int __stdcall NetSession_ConfigurePrudpEndpoints(
    int secondaryOption,
    int secondaryAttribute,
    int secondaryValue,
    int primaryOption,
    int primaryAttribute,
    int primaryValue)
{
    void* registry =
        reinterpret_cast<void*>(NetSession_GetTransportRegistry(nullptr, nullptr));
    void* transport = reinterpret_cast<void*>(
        NetSession_FindTransport(
            registry, nullptr, const_cast<char*>(CSF_STR(0x0084468C, "prudp"))));

    void* endpoint =
        reinterpret_cast<void*>(NetSession_GetPrimaryEndpoint(transport, nullptr));
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, endpoint, 0)[1])(endpoint, nullptr);

    endpoint =
        reinterpret_cast<void*>(NetSession_GetSecondaryEndpoint(transport, nullptr));
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, endpoint, 0)[1])(endpoint, nullptr);

    endpoint =
        reinterpret_cast<void*>(NetSession_GetPrimaryEndpoint(transport, nullptr));
    NetSession_SetEndpointValue(endpoint, nullptr, primaryValue);

    endpoint =
        reinterpret_cast<void*>(NetSession_GetPrimaryEndpoint(transport, nullptr));
    NetSession_SetEndpointAttribute(endpoint, nullptr, primaryAttribute);

    endpoint =
        reinterpret_cast<void*>(NetSession_GetPrimaryEndpoint(transport, nullptr));
    NetSession_ConfigureEndpoint(endpoint, nullptr, primaryOption);

    endpoint =
        reinterpret_cast<void*>(NetSession_GetSecondaryEndpoint(transport, nullptr));
    NetSession_SetEndpointValue(endpoint, nullptr, secondaryValue);

    endpoint =
        reinterpret_cast<void*>(NetSession_GetSecondaryEndpoint(transport, nullptr));
    NetSession_SetEndpointAttribute(endpoint, nullptr, secondaryAttribute);

    endpoint =
        reinterpret_cast<void*>(NetSession_GetSecondaryEndpoint(transport, nullptr));
    return NetSession_ConfigureEndpoint(endpoint, nullptr, secondaryOption);
}

HOOK(0x0073C530, NetSession_ConfigurePrudpEndpoints, "auto");
