#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, ReadSerializedByte, 0x00655E20, (void* self, void* edx, char* a2));
CSF_FUNC(int, __fastcall, ReadCounterWord, 0x00655E40, (void* self, void* edx, char* a2));
CSF_FUNC(int, __fastcall, SharedDispatch_65E950, 0x0065E950, (void* self, void* edx, int a1, int a2, int a3));
CSF_FUNC(char, __fastcall, C_00858180_HandleByteAndCounter, 0x007481B0, (void* self, void* edx, char a2, int a3));
CSF_FUNC(char, __fastcall, C_00858180_HandleByte, 0x00748260, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, Character_7483B0, 0x007483B0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MpSpawnPoints_RefreshStats, 0x00748AE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMpStats_HandleSerializedEvent, 0x0074CC60, (void* self, void* edx, void* arg0));
CSF_FUNC(int, __fastcall, C_00858180_HandleSerializedEvent, 0x0074CD10, (void* self, void* edx, void* arg0));
CSF_FUNC(char, __fastcall, CMenuMpStats_HandleSerializedUpdate, 0x0074CDC0, (void* self, void* edx, void* a2));

static char __fastcall NetGame_DispatchEvent(
    NetGame* self, void* /*edx*/,
    CMenuMpStats* recipient, int eventId, C_008622e8* reader)
{
    if (static_cast<uint16_t>(eventId) == self->field_0020)
    {
        char value = 0;
        uint16_t counter = 0;
        ReadSerializedByte(reader, nullptr, &value);
        ReadCounterWord(reader, nullptr, reinterpret_cast<char*>(&counter));
        C_00858180_HandleByteAndCounter(recipient, nullptr, value, counter);
        return 1;
    }
    else if (static_cast<uint16_t>(eventId) == self->field_0022)
    {
        char value = 0;
        ReadSerializedByte(reader, nullptr, &value);
        C_00858180_HandleByte(recipient, nullptr, value);
        return 1;
    }
    else if (static_cast<uint16_t>(eventId) == self->field_0024)
    {
        CMenuMpStats_HandleSerializedEvent(recipient, nullptr, reader);
        return 1;
    }
    else if (static_cast<uint16_t>(eventId) == self->field_0026)
    {
        C_00858180_HandleSerializedEvent(recipient, nullptr, reader);
        return 1;
    }
    else if (static_cast<uint16_t>(eventId) == self->field_0028)
    {
        CMenuMpStats_HandleSerializedUpdate(recipient, nullptr, reader);
        return 1;
    }
    else if (static_cast<uint16_t>(eventId) == self->field_002a)
    {
        Character_7483B0(recipient, nullptr);
        return 1;
    }
    else if (static_cast<uint16_t>(eventId) == self->field_002c)
    {
        MpSpawnPoints_RefreshStats(recipient, nullptr);
        return 1;
    }

    return static_cast<char>(SharedDispatch_65E950(
        self, nullptr, reinterpret_cast<int>(recipient),
        eventId, reinterpret_cast<int>(reader)));
}

HOOK(0x0074D050, NetGame_DispatchEvent, "auto");
