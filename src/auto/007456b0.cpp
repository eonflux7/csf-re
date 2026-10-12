#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, NetPlayer_DispatchInheritedEvent, 0x0067F520, (void* self, void* edx, void* a2, int a3, void* a4));
CSF_FUNC(char, __fastcall, SpawnPoints_UpdateCharacter, 0x00741150, (C_00858180* self, void* , int a2));
CSF_FUNC(char, __fastcall, CMenuMpStats_RefreshSpawnStats, 0x00741660, (CMenuMpStats* self, void*));

CSF_FUNC(char, __fastcall, NetPlayer_HandleEvent0022, 0x00744EE0, (void* self, void* edx, void* arg0));
CSF_FUNC(char, __fastcall, NetPlayer_HandleEvent0024, 0x00745020, (void* self, void* edx, void* arg0));
CSF_FUNC(char, __fastcall, NetPlayer_HandleEvent0026, 0x00745120, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, NetPlayer_HandleEvent0028, 0x00745230, (void* self, void* edx, void* arg0));
CSF_FUNC(int16_t, __fastcall, NetPlayer_HandleEvent002A, 0x00745290, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, NetPlayer_HandleEvent002C, 0x00745320, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, NetPlayer_HandleEvent0030, 0x007453A0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, NetPlayer_HandleEvent0034, 0x00745490, (void* self, void* edx, void* arg0));
CSF_FUNC(void*, __stdcall, NetPlayer_HandleEvent0036, 0x007454D0, (void* a2));
CSF_FUNC(void, __fastcall, NetPlayer_HandleEvent0038, 0x007454F0, (void* self, void* edx, void* a2));

static char __fastcall NetPlayer_DispatchEvent(
    NetPlayer* self, void* /*edx*/, int* recipient, int eventId,
    C_008622e8* payload)
{
    const uint16_t id = static_cast<uint16_t>(eventId);

    if (id == self->field_0020)
    {
        NetCogibleDdl_ReadIntoSlot(
            payload, nullptr, reinterpret_cast<int>(&payload));
        SpawnPoints_UpdateCharacter(
            reinterpret_cast<C_00858180*>(recipient), nullptr,
            reinterpret_cast<int>(payload));
        return 1;
    }
    else if (id == self->field_0022)
    {
        NetPlayer_HandleEvent0022(recipient, nullptr, payload);
        return 1;
    }
    else if (id == self->field_0024)
    {
        NetPlayer_HandleEvent0024(recipient, nullptr, payload);
        return 1;
    }
    else if (id == self->field_0026)
    {
        NetPlayer_HandleEvent0026(recipient, nullptr, payload);
        return 1;
    }
    else if (id == self->field_0028)
    {
        NetPlayer_HandleEvent0028(recipient, nullptr, payload);
        return 1;
    }
    else if (id == self->field_002a)
    {
        NetPlayer_HandleEvent002A(
            recipient, nullptr, reinterpret_cast<int>(payload));
        return 1;
    }
    else if (id == self->field_002c)
    {
        NetPlayer_HandleEvent002C(recipient, nullptr, payload);
        return 1;
    }
    else if (id == self->field_002e)
    {
        SceneLoader_NoOp();
        return 1;
    }
    else if (id == self->field_0030)
    {
        NetPlayer_HandleEvent0030(recipient, nullptr, payload);
        return 1;
    }
    else if (id == self->field_0032)
    {
        CMenuMpStats_RefreshSpawnStats(
            reinterpret_cast<CMenuMpStats*>(recipient), nullptr);
        return 1;
    }
    else if (id == self->field_0034)
    {
        NetPlayer_HandleEvent0034(recipient, nullptr, payload);
        return 1;
    }
    else if (id == self->field_0036)
    {
        NetPlayer_HandleEvent0036(payload);
        return 1;
    }
    else if (id == self->field_0038)
    {
        NetPlayer_HandleEvent0038(recipient, nullptr, payload);
        return 1;
    }

    return NetPlayer_DispatchInheritedEvent(
        self, nullptr, recipient, eventId, payload);
}

HOOK(0x007456B0, NetPlayer_DispatchEvent, "auto");
