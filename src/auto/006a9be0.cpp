#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, NetSession_SetConnectionState, 0x006510B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_652810, 0x00652810, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_7A14C0, 0x007A14C0, (void* self, void* edx));
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Tears down the connection and transport resources held by this net-gathering DDL object.
static void __fastcall NetGatheringDdl_ReleaseConnectionAndTransport(S_006a9be0* self, void* /*edx*/)
{
    // Connection object at +0x38: move it to state 4, then drop it through its vtable slot 0.
    if (self->field_0038)
    {
        NetSession_SetConnectionState(reinterpret_cast<void*>(self->field_0038), nullptr, 4);

        if (self->field_0038)
        {
            void* connection = reinterpret_cast<void*>(self->field_0038);
            CSF_AT(void**, connection, 0)[0] == nullptr
                ? (void)0
                : (void)((void (__fastcall*)(void*, void*, int))CSF_AT(void**, connection, 0)[0])(connection, nullptr, 1);
            self->field_0038 = 0;
        }
    }

    // Embedded sub-object at +0x08 (lea ecx, [esi+8]).
    sub_652810(static_cast<uint8_t*>(static_cast<void*>(self)) + 8, nullptr);

    // Transport object at +0x28.
    void* transport = reinterpret_cast<void*>(self->field_0028);
    if (transport)
    {
        sub_7A14C0(transport, nullptr);
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(transport));
        self->field_0028 = 0;
    }

    self->field_0004 = 0;
}

HOOK(0x6A9BE0, NetGatheringDdl_ReleaseConnectionAndTransport, "auto");
