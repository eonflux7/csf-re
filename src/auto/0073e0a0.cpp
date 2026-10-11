#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __fastcall, NetSession_IsWellKnown, 0x0064AF40, (void* self, void* edx));
CSF_FUNC(const char*, __fastcall, NetSession_GetStationName, 0x0064B110, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_GetMasterId, 0x0064B2F0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, NetSession_IsDuplicate, 0x0064B5C0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_PrepareStationIterator, 0x0064B880, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_AdvanceStationIterator, 0x0064B900, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetSession_DestroyStationReference, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_AssignStationReference, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, NetSession_ReleaseStationIterator, 0x0065E130, (void* self, void* edx, int a2));
CSF_FUNC(int*, __fastcall, NetSession_GetStationIterator, 0x0073D710, (void* self, void* edx));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

static void __stdcall NetSession_FormatStationEntry(char* buffer, char* stationIndex)
{
    char* referenceText = buffer + 180;
    buffer[490] = 0;
    *referenceText = 0;
    buffer[190] = 0;
    buffer[170] = 0;
    buffer[160] = 0;
    buffer[150] = 0;
    buffer[100] = 0;
    buffer[50] = 0;
    buffer[0] = 0;

    // The iterator and its station reference occupy contiguous stack storage.
    alignas(8) uint8_t iteratorBytes[0x28];
    void* iterator = iteratorBytes;
    void* stationReference = iteratorBytes + 0x4;
    uint32_t* emptyReference =
        reinterpret_cast<uint32_t*>(iteratorBytes + 0x24);

    NetSession_GetStationIterator(iterator, nullptr);
    C_00858180* selectedStation = nullptr;
    uint32_t currentIndex = 0;

    if (CSF_AT(int, iterator, 0))
    {
        NetSession_PrepareStationIterator(iterator, nullptr);
        NetSession_AdvanceStationIterator(iterator, nullptr, 1);
    }
    else
    {
        *emptyReference = 0;
        NetSession_AssignStationReference(stationReference, nullptr, emptyReference);
    }

    int hasStation = CSF_AT(int, stationReference, 0x4);
    if (hasStation)
    {
        while (!selectedStation)
        {
            if (currentIndex == reinterpret_cast<uintptr_t>(stationIndex))
            {
                if (hasStation)
                    selectedStation = CSF_AT(C_00858180*, stationReference, 0);
                else
                    Game_RaiseStatusNotice(0, 0, -1610612729, 0);
            }

            NetSession_AdvanceStationIterator(iterator, nullptr, 0);
            hasStation = CSF_AT(int, stationReference, 0x4);
            ++currentIndex;
            if (!hasStation)
                break;
        }

        if (selectedStation)
        {
            const char* name = NetSession_GetStationName(selectedStation, nullptr);
            _sprintf(buffer, CSF_STR(0x008315DC, "%s"), name);

            uint32_t stationId = selectedStation->field_002c;
            if ((stationId & 0x3FFFFF) == 0)
            {
                Game_RaiseStatusNotice(0, 0, -536870898, 0);
                stationId = 0;
            }
            _sprintf(buffer + 50, CSF_STR(0x0083090C, "%d"), stationId);

            int masterId = NetSession_GetMasterId(selectedStation, nullptr);
            _sprintf(buffer + 100, CSF_STR(0x0083090C, "%d"), masterId);

            bool isCore =
                (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    selectedStation->vtbl[0x34 / 4]))(selectedStation, nullptr);
            const char* coreText = isCore
                ? CSF_STR(0x0086770C, "CORE")
                : CSF_STR(0x00867704, "NO CORE");
            _sprintf(buffer + 150, CSF_STR(0x008315DC, "%s"), coreText);

            bool isDuplicate = NetSession_IsDuplicate(selectedStation, nullptr);
            const char* duplicateText = isDuplicate
                ? CSF_STR(0x008676FC, "DUPLICA")
                : CSF_STR(0x008676F4, "MASTER");
            _sprintf(buffer + 160, CSF_STR(0x008315DC, "%s"), duplicateText);

            bool isWellKnown =
                static_cast<uint8_t>(NetSession_IsWellKnown(selectedStation, nullptr)) != 0;
            const char* wellKnownText = isWellKnown
                ? CSF_STR(0x008676E8, "WELLKNOWN")
                : CSF_STR(0x008676E0, "NO WK");
            _sprintf(buffer + 170, CSF_STR(0x008315DC, "%s"), wellKnownText);

            _sprintf(referenceText, CSF_STR(0x0083090C, "%d"),
                     static_cast<int>(static_cast<uint16_t>(selectedStation->field_0004)));
            _sprintf(buffer + 190,
                     CSF_STR(0x008676B4, "%20s %10s MId: %3s %7s %7s %10s RefC: %4s"),
                     buffer, buffer + 50, buffer + 100, buffer + 150,
                     buffer + 160, buffer + 170, referenceText);
            buffer[490] = 1;
        }
    }

    NetSession_ReleaseStationIterator(iterator, nullptr, 0);
    uint32_t nullReference = 0;
    NetSession_AssignStationReference(stationReference, nullptr, &nullReference);
    NetSession_DestroyStationReference(stationReference, nullptr);
}

HOOK(0x0073E0A0, NetSession_FormatStationEntry, "auto");
