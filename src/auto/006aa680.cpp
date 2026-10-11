#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// NetSession_SetConnectionState(session, 4): thiscall-style, pops its 4-byte argument.
CSF_FUNC(char, __fastcall, NetSession_SetConnectionState, 0x006510B0, (void* self, void* edx, int a2));
// Case 2 handler (original tail-jumps here).
CSF_FUNC(void, __fastcall, S_006a8940_HandleStateTwo, 0x006A8A10, (void* self, void* edx));
// Case 7 handler (original tail-jumps here).
CSF_FUNC(char, __fastcall, S_006a8940_HandleStateSeven, 0x006AA470, (void* self, void* edx));

// Spin-wait handshake flags in the data segment.
CSF_GLOBAL(uint8_t, g_SpinRequestFlag, 0x008BB41C);
CSF_GLOBAL(uint8_t, g_SpinReleaseFlag, 0x008BB41D);

// Shared body of the case 1 and case 5 paths: when the session is in sub-state
// `expectedSubState`, drop the connection, release the owned session object,
// and return the state machine to idle.
static void S_006a8940_ResetSessionAfterDisconnect(S_006a8940* self, uint32_t expectedSubState) {
    if (self->field_0008 == 3 && self->field_000c == expectedSubState) {
        NetSession_SetConnectionState(reinterpret_cast<void*>(self->field_0090), nullptr, 4);

        // The original re-reads field_0090 after the call.
        C_0085e760* session = reinterpret_cast<C_0085e760*>(self->field_0090);
        if (session) {
            // Virtual slot 0 (destructor-style release with argument 1).
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, session, 0)[0])(session, nullptr, 1);
            self->field_0090 = 0;
        }

        self->field_0008 = 2;
        self->field_000c = 0;
        self->field_0334 = 0;
    }
}

static void __fastcall S_006a8940_HandleSessionState(S_006a8940* self, void* /*edx*/) {
    uint32_t state = self->field_0008;
    if (state == 3) {
        switch (self->field_000c) {
            case 1u:
                S_006a8940_ResetSessionAfterDisconnect(self, 1u);
                break;
            case 2u:
                S_006a8940_HandleStateTwo(self, nullptr);
                break;
            case 5u:
                S_006a8940_ResetSessionAfterDisconnect(self, 5u);
                break;
            case 7u:
                S_006a8940_HandleStateSeven(self, nullptr);
                break;
            default:
                return;
        }
    } else if (state == 1 && !g_SpinReleaseFlag) {
        g_SpinRequestFlag = 1;
        while (!g_SpinReleaseFlag) {
        }
    }
}

HOOK(0x006AA680, S_006a8940_HandleSessionState, "auto");
