#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, NetSession_SetConnectionState, 0x006510B0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_6A78D0, 0x006A78D0, (void* self, void* edx));

// Calls vSlot0(1) on the session object held at +0x90, then clears the slot.
static void NetGathering_ReleaseSessionObject(S_006a7640* self) {
    C_0085e760* session = reinterpret_cast<C_0085e760*>(self->field_0090);
    if (session) {
        session->vSlot0(1);
        self->field_0090 = 0;
    }
}

static void __fastcall NetGathering_TearDownSessionIfConnected(S_006a7640* self, void* /*edx*/) {
    if (self->field_0008 != 5)
        return;

    uint32_t mode = self->field_0020;
    if (mode == 0) {
        C_0085e760* session = reinterpret_cast<C_0085e760*>(self->field_0090);
        if (CSF_AT(uint32_t, session, 0xC) == 1) {
            NetSession_SetConnectionState(session, nullptr, 4);
            NetGathering_ReleaseSessionObject(self);
            self->field_0008 = 0;
        } else {
            sub_6A78D0(self, nullptr);
            NetGathering_ReleaseSessionObject(self);
        }
    } else if (mode == 1) {
        NetSession_SetConnectionState(reinterpret_cast<C_0085e760*>(self->field_0090), nullptr, 4);
        NetGathering_ReleaseSessionObject(self);
        sub_6A78D0(self, nullptr);
    }
}

HOOK(0x006A7AB0, NetGathering_TearDownSessionIfConnected, "auto");
