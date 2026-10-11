#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, ReturnZero, 0x004B2A30, ());
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));
CSF_FUNC(void*, __cdecl, NetSession_GetRuntimeState, 0x00671260, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, NetSession_DestroySessionState, 0x0073D500, (void* self, void* edx));
CSF_FUNC(void, __cdecl, NetSession_BeginShutdown, 0x0073DE40, ());
CSF_FUNC(int, __cdecl, NetSession_ResetTransportState, 0x007437C0, ());
CSF_FUNC(void, __cdecl, NetSession_StopSessionServices, 0x00743A70, ());
CSF_FUNC(char, __cdecl, NetSession_FinalizeShutdown, 0x0074A770, ());
CSF_FUNC(void, __fastcall, NetSession_DestroyTransport, 0x007A2970, (void* self, void* edx));

CSF_GLOBAL(uint32_t, g_NetSessionOwner, 0x008B99B0);
CSF_GLOBAL(uint8_t*, g_NetSessionEntries, 0x008B9B40);

static char __fastcall S_0073ecc0_Shutdown(S_0073ecc0* self, void* /*edx*/)
{
    if (self->field_0000)
    {
        NetSession_BeginShutdown();
        NetSession_StopSessionServices();

        const int entryIndex = ReturnZero();
        uint8_t* entry;
        if (entryIndex)
            entry = g_NetSessionEntries + 32 * static_cast<uint32_t>(entryIndex);
        else
            entry = reinterpret_cast<uint8_t*>(CSF_ADDR_DATA(0x008B9B48));
        CSF_AT(uint8_t, entry, 0x10) = 0;

        void* runtimeState = NetSession_GetRuntimeState();
        CSF_AT(uint32_t, runtimeState, 0x10) |= 0x4000000u;

        const uint32_t transport = self->field_0000;
        if (transport)
        {
            NetSession_DestroyTransport(reinterpret_cast<void*>(transport), nullptr);
            NetSession_ReleaseTransportStorage(static_cast<int>(transport));
            self->field_0000 = 0;
        }

        self->field_014f = 0;
        self->field_0000 = 0;
        NetSession_ResetTransportState();

        void* object = reinterpret_cast<void*>(self->field_0004);
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
                (object, nullptr, 1);
            self->field_0004 = 0;
        }

        object = reinterpret_cast<void*>(self->field_0008);
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
                (object, nullptr, 1);
            self->field_0008 = 0;
        }
    }

    void* sessionState = reinterpret_cast<void*>(self->field_00c8);
    if (sessionState)
    {
        NetSession_DestroySessionState(sessionState, nullptr);
        Mem_Free(sessionState);
        self->field_00c8 = 0;
    }

    void* object = reinterpret_cast<void*>(self->field_00d0);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
            (object, nullptr, 1);
        self->field_00d0 = 0;
    }

    object = reinterpret_cast<void*>(self->field_00cc);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
            (object, nullptr, 1);
        self->field_00cc = 0;
    }

    NetSession_FinalizeShutdown();

    const uint32_t buffer = self->field_0158;
    char result = static_cast<char>(buffer);
    if (buffer)
    {
        result = static_cast<char>(Mem_Free(reinterpret_cast<void*>(buffer)));
        self->field_0158 = 0;
    }

    if (self->field_015c)
    {
        g_NetSessionOwner = 0;
        object = reinterpret_cast<void*>(self->field_015c);
        if (object)
        {
            result = static_cast<char>(
                ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
                    (object, nullptr, 1));
            self->field_015c = 0;
        }
        self->field_015c = 0;
    }

    return result;
}

HOOK(0x0073ECC0, S_0073ecc0_Shutdown, "auto");
