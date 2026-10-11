#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));
CSF_FUNC(bool, __fastcall, NetGatheringDdl_WaitForResourceCompletion, 0x00650BA0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, NetSession_SetConnectionState, 0x006510B0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetGatheringDdl_BeginResourceDisconnect, 0x00791150, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(int, __fastcall, NetGatheringDdl_CloseSession, 0x007912F0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetGatheringDdl_CloseTransport, 0x007A06A0, (void* self, void* edx));

static char __fastcall NetGatheringDdl_Disconnect(void* self, void* /*edx*/, char force)
{
    char result = force;
    if (!force)
    {
        const int state = CSF_AT(int, self, 0x08);
        result = static_cast<char>(state);
        if (state != 2 && state != 1 && state != 7)
            return result;
    }

    void* resource = CSF_AT(void*, self, 0x90);
    if (resource)
        result = NetSession_SetConnectionState(resource, nullptr, 4);

    void* object = CSF_AT(void*, self, 0x330);
    if (object)
    {
        result = static_cast<char>(
            ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
                (object, nullptr, 1));
        CSF_AT(void*, self, 0x330) = nullptr;
    }

    object = CSF_AT(void*, self, 0x2C);
    if (object)
    {
        result = static_cast<char>(
            ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
                (object, nullptr, 1));
        CSF_AT(void*, self, 0x2C) = nullptr;
    }

    object = CSF_AT(void*, self, 0x34);
    if (object)
    {
        result = static_cast<char>(
            ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
                (object, nullptr, 1));
        CSF_AT(void*, self, 0x34) = nullptr;
    }

    object = CSF_AT(void*, self, 0x30);
    if (object)
    {
        result = static_cast<char>(
            ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
                (object, nullptr, 1));
        CSF_AT(void*, self, 0x30) = nullptr;
    }

    void* transport = CSF_AT(void*, self, 0x3C);
    if (transport)
    {
        NetGatheringDdl_CloseTransport(transport, nullptr);
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(transport));
        // The original exposes incidental AL from this void call.
        result = 0;
        CSF_AT(void*, self, 0x3C) = nullptr;
    }

    void* session = CSF_AT(void*, self, 0x28);
    if (!session)
    {
        CSF_AT(int, self, 0x08) = 0;
        CSF_AT(int, self, 0x0C) = 0;
        return result;
    }

    if (!CSF_AT(int, self, 0x38))
    {
        NetGatheringDdl_CloseSession(session, nullptr);
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(session));
        CSF_AT(void*, self, 0x28) = nullptr;
        CSF_AT(int, self, 0x08) = 0;
        CSF_AT(int, self, 0x0C) = 0;
        return 0;
    }

    void* storage = NetZ_OperatorNew(112);
    resource = storage ? ConstructResource(storage, nullptr) : nullptr;

    const int disconnectArgument = CSF_AT(int, self, 0x38);
    session = CSF_AT(void*, self, 0x28);
    CSF_AT(void*, self, 0x90) = resource;
    NetGatheringDdl_BeginResourceDisconnect(
        session, nullptr, resource, disconnectArgument);

    result = static_cast<char>(NetGatheringDdl_WaitForResourceCompletion(
        CSF_AT(void*, self, 0x90), nullptr, -1));

    resource = CSF_AT(void*, self, 0x90);
    if (resource)
    {
        result = static_cast<char>(
            ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, resource, 0)[0])
                (resource, nullptr, 1));
        CSF_AT(void*, self, 0x90) = nullptr;
    }

    session = CSF_AT(void*, self, 0x28);
    CSF_AT(int, self, 0x08) = 0;
    if (session)
    {
        NetGatheringDdl_CloseSession(session, nullptr);
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(session));
        result = 0;
        CSF_AT(void*, self, 0x28) = nullptr;
    }
    return result;
}

HOOK(0x006A81E0, NetGatheringDdl_Disconnect, "auto");
