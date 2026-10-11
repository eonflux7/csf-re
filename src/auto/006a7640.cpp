#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));
CSF_FUNC(char, __fastcall, NetSession_SetConnectionState, 0x006510B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetGatheringDdl_DestroyTransport, 0x007912F0, (void* self, void* edx));

static void __fastcall S_006a7640_ResetConnection(
    S_006a7640* self, void* /*edx*/, int /*unused*/)
{
    C_0085e760* session;
    if (self->field_02c8)
    {
        session = reinterpret_cast<C_0085e760*>(self->field_0090);
        const uint32_t state = CSF_AT(uint32_t, session, 0x0C);
        if (state == 1)
            return;

        if (state == 3)
            self->field_02b0 = 1229730613;

        if (session)
        {
            session->vSlot0(1);
            self->field_0090 = 0;
        }
    }

    session = reinterpret_cast<C_0085e760*>(self->field_0090);
    self->field_0008 = 0;
    if (session)
        NetSession_SetConnectionState(session, nullptr, 4);

    session = reinterpret_cast<C_0085e760*>(self->field_0090);
    if (session)
    {
        session->vSlot0(1);
        self->field_0090 = 0;
    }

    session = reinterpret_cast<C_0085e760*>(self->field_0090);
    if (session)
        NetSession_SetConnectionState(session, nullptr, 4);

    session = reinterpret_cast<C_0085e760*>(self->field_0090);
    if (session)
    {
        session->vSlot0(1);
        self->field_0090 = 0;
    }

    session = reinterpret_cast<C_0085e760*>(self->field_0090);
    if (session)
        NetSession_SetConnectionState(session, nullptr, 4);

    session = reinterpret_cast<C_0085e760*>(self->field_0090);
    if (session)
    {
        session->vSlot0(1);
        self->field_0090 = 0;
    }

    void* transport = reinterpret_cast<void*>(self->field_0028);
    if (transport)
    {
        NetGatheringDdl_DestroyTransport(transport, nullptr);
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(transport));
        self->field_0028 = 0;
    }
}

HOOK(0x006A7640, S_006a7640_ResetConnection, "auto");
