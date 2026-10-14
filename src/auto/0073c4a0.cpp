#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, NetSession_GetTransportRegistry, 0x006487D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_FindTransport, 0x00648910, (void* self, void* edx, char* String2));
CSF_FUNC(void*, __fastcall, NetGetPrudpModeEndpoint, 0x006486E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetPrudpSendModeCommand, 0x00676700, (void* self, void* edx, int a1, int a2));

// Maps the requested session mode (0..4) to the prudp mode code and sends it.
// Returns the command result, or 0 when the session has no transport, the
// prudp protocol is missing, or the mode is out of range.
static int __fastcall NetSession_SetPrudpConnectionMode(void* self, void* /*edx*/, int requestedMode)
{
    int result = 0;

    if (CSF_AT(uint32_t, self, 0x98) != 0)
    {
        void* registry = NetSession_GetTransportRegistry(self, nullptr);
        int protocolReady = NetSession_FindTransport(
            registry, nullptr, const_cast<char*>(CSF_STR(0x84468C, "prudp")));

        if (protocolReady != 0)
        {
            int prudpMode = 0;
            switch (requestedMode)
            {
            case 0:
                prudpMode = 0;
                break;
            case 1:
                prudpMode = 1;
                break;
            case 2:
                prudpMode = 2;
                break;
            case 3:
                prudpMode = 4;
                break;
            case 4:
                prudpMode = 3;
                break;
            default:
                return result;
            }

            void* modeEndpoint = NetGetPrudpModeEndpoint(reinterpret_cast<void*>(static_cast<intptr_t>(protocolReady)), nullptr);
            result = NetPrudpSendModeCommand(modeEndpoint, nullptr, prudpMode, 5);
        }
    }

    return result;
}

HOOK(0x0073C4A0, NetSession_SetPrudpConnectionMode, "auto");
