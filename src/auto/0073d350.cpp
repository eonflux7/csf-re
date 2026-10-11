#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(char, __fastcall, NetSession_SetTransportState, 0x006510B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_ConstructTransport, 0x00651770, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Sub_6687B0, 0x006687B0, (void* self, void* edx));
CSF_FUNC(char, __cdecl, NetSession_StartTransport, 0x00669190, (int64_t a1, char a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, NetSession_BeginSessionRequest, 0x0066F180, (void* self, void* edx, void** a2));
CSF_FUNC(unsigned int, __fastcall, C_0085839c_UpdateFlagFromNetResult, 0x0073C850, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_NetSessionFlag, 0x008BBF98);

static uint32_t* __fastcall C_0085839c_StartSession(
    C_0085839c* self, void* /*edx*/, char** arguments)
{
    void* transport = CSF_AT(void*, self, 0xD0);
    if (transport) {
        if (CSF_AT(uint32_t, transport, 0xC) == 1)
            NetSession_SetTransportState(transport, nullptr, 4);

        transport = CSF_AT(void*, self, 0xD0);
        if (transport) {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, transport, 0)[0])(transport, nullptr, 1);
            CSF_AT(uint32_t, self, 0xD0) = 0;
        }
    }

    void* endpoint = reinterpret_cast<uint8_t*>(self) + 0xAC;
    Sub_6687B0(endpoint, nullptr);

    void* storage = NetZ_OperatorNew(92);
    int newTransport = storage
        ? NetSession_ConstructTransport(storage, nullptr)
        : 0;
    CSF_AT(int, self, 0xD0) = newTransport;

    // The supplied binding represents the two pointer words as one int64_t.
    const int64_t transportAndEndpoint = static_cast<int64_t>(
        static_cast<uint64_t>(static_cast<uint32_t>(newTransport)) |
        (static_cast<uint64_t>(reinterpret_cast<uintptr_t>(endpoint)) << 32));

    if (NetSession_StartTransport(transportAndEndpoint, 1, 1000)) {
        CSF_AT(uint32_t, self, 0x98) = 5;
        return NetSession_BeginSessionRequest(
            reinterpret_cast<uint8_t*>(self) + 0x160,
            nullptr, reinterpret_cast<void**>(arguments));
    }

    unsigned int result = C_0085839c_UpdateFlagFromNetResult(self, nullptr);
    CSF_AT(uint32_t, self, 0x98) = 2;
    g_NetSessionFlag = 0;
    return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(result));
}

HOOK(0x0073D350, C_0085839c_StartSession, "auto");
