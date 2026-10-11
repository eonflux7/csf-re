#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(char, __fastcall, NetSession_SetConnectionState, 0x006510B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_ConstructConnection, 0x00651770, (void* self, void* edx));
CSF_FUNC(char, __cdecl, NetSession_StartConnection, 0x00669AC0, (int a1, void** a4));
CSF_FUNC(uint32_t*, __fastcall, NetSession_UpdateSessionDetails, 0x0066F180, (void* self, void* edx, void** a2));
CSF_FUNC(unsigned int, __fastcall, NetSession_ResetConnection, 0x0073C850, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_NetSessionConnectionFlag, 0x008BBF98);

static int __fastcall C_0085839c_StartConnection(
    C_0085839c* self, void* /*edx*/, char** details)
{
    void* connection = CSF_AT(void*, self, 0xCC);
    if (connection)
    {
        if (CSF_AT(int, connection, 0xC) == 1)
            NetSession_SetConnectionState(connection, nullptr, 4);

        connection = CSF_AT(void*, self, 0xCC);
        if (connection)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, connection, 0)[0])(connection, nullptr, 1);
            CSF_AT(int, self, 0xCC) = 0;
        }
    }

    void* storage = NetZ_OperatorNew(92);
    int newConnection = storage
        ? NetSession_ConstructConnection(storage, nullptr)
        : 0;
    CSF_AT(int, self, 0xCC) = newConnection;

    if (NetSession_StartConnection(newConnection, reinterpret_cast<void**>(details)))
    {
        int result = 5;
        if (CSF_AT(int, self, 0x98) != 5)
        {
            CSF_AT(int, self, 0x98) = 5;
            result = reinterpret_cast<int>(NetSession_UpdateSessionDetails(
                reinterpret_cast<uint8_t*>(self) + 0x160,
                nullptr, reinterpret_cast<void**>(details)));
        }
        CSF_AT(uint8_t, self, 0x104) = 0;
        return result;
    }

    int result = static_cast<int>(NetSession_ResetConnection(self, nullptr));
    CSF_AT(int, self, 0x98) = 2;
    g_NetSessionConnectionFlag = 0;
    return result;
}

HOOK(0x0073CB80, C_0085839c_StartConnection, "auto");
