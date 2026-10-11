#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, Sub_6687B0, 0x006687B0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Session_WaitForSignal, 0x00669280, (int a1, char a2, int a3));
CSF_GLOBAL(uint8_t, g_SessionWaitFailedFlag, 0x008BBF98);

static void __fastcall GlobalObject_ResetState(void* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0x98) != 2)
        return;

    void* waitObject = reinterpret_cast<uint8_t*>(self) + 0x9c;
    Sub_6687B0(waitObject, nullptr);
    if (!Session_WaitForSignal(static_cast<int>(reinterpret_cast<uintptr_t>(waitObject)), 1, 1000))
        g_SessionWaitFailedFlag = 0;
}

HOOK(0x0073D310, GlobalObject_ResetState, "auto");
