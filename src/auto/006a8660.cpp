#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, NetSession_SetConnectionState, 0x006510B0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, NetGathering_ResetSessionSlot, 0x006A78D0, (void* self, void* edx));

// Releases the session object (vSlot0 with 1, then clears the slot) and resets the gathering state.
static void ReleaseSessionAndReset(S_006a7640* self, C_0085e760* session) {
    if (session) {
        session->vSlot0(1);
        self->field_0090 = 0;
    }
    NetGathering_ResetSessionSlot(self, nullptr);
}

static void __fastcall NetGathering_HandleStateFiveTeardown(S_006a7640* self, void* /*edx*/) {
    if (self->field_0008 != 5)
        return;

    uint32_t mode = self->field_0020;
    if (mode == 0) {
        C_0085e760* session = reinterpret_cast<C_0085e760*>(self->field_0090);
        if (CSF_AT(int, session, 0x0C) != 1) {
            ReleaseSessionAndReset(self, session);
            return;
        }
        NetSession_SetConnectionState(session, nullptr, 4);
        C_0085e760* again = reinterpret_cast<C_0085e760*>(self->field_0090);
        if (again) {
            again->vSlot0(1);
            self->field_0090 = 0;
        }
        self->field_0008 = 0;
        return;
    }

    if (mode != 1)
        return;

    NetSession_SetConnectionState(reinterpret_cast<void*>(self->field_0090), nullptr, 4);
    C_0085e760* session = reinterpret_cast<C_0085e760*>(self->field_0090);
    ReleaseSessionAndReset(self, session);
}

HOOK(0x006A8660, NetGathering_HandleStateFiveTeardown, "auto");
