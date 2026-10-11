#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Win32 message pump bound through the game's imported-function pointer slots.
// MSG is 0x1C bytes on x86: hwnd, message, wParam, lParam, time, pt.
using TranslateMessageFn = int(__stdcall*)(void* msg);
using PeekMessageAFn = int(__stdcall*)(void* msg, void* hwnd, uint32_t filterMin, uint32_t filterMax, uint32_t removeFlags);
using DispatchMessageAFn = int(__stdcall*)(void* msg);

CSF_GLOBAL(TranslateMessageFn, g_TranslateMessageFn, 0x0082D29C);
CSF_GLOBAL(PeekMessageAFn, g_PeekMessageAFn, 0x0082D2A0);
CSF_GLOBAL(DispatchMessageAFn, g_DispatchMessageAFn, 0x0082D2A4);

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

static constexpr uint32_t kWmQuit = 0x12;
static constexpr std::size_t kMsgOffsetMessage = 0x04;

// Pumps the Windows message queue. Dispatches pending messages; when the queue is
// empty, runs the global state's per-frame update. Exits when the update reports
// done or when a WM_QUIT message is received.
static int __cdecl App_RunMessageLoop() {
    alignas(8) uint8_t msgBytes[0x1C];
    int result = 0;

    do {
        if (g_PeekMessageAFn(msgBytes, nullptr, 0, 0, 1u)) {
            g_TranslateMessageFn(msgBytes);
            result = g_DispatchMessageAFn(msgBytes);
        } else {
            int globalState = GetGlobalState_50D190();
            void* stateObj = reinterpret_cast<void*>(globalState);
            // Slot 0x14 / 4 = 5 of the state object's vtable; __thiscall with no args.
            using StateUpdateFn = bool(__fastcall*)(void* self, void* edx);
            bool stateDone = reinterpret_cast<StateUpdateFn>(CSF_AT(void**, stateObj, 0)[0x14 / 4])(stateObj, nullptr);
            result = stateDone ? 1 : 0;
            if (stateDone) {
                return result;
            }
        }
    } while (CSF_AT(uint32_t, msgBytes, kMsgOffsetMessage) != kWmQuit);

    return result;
}

HOOK(0x00508CE0, App_RunMessageLoop, "auto");
