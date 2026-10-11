#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(void, __fastcall, NetSession_BeginIteration, 0x0064B880, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_AdvanceIterator, 0x0064B900, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetSession_DestroyStationReference, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_AssignStationReference, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(unsigned int*, __fastcall, NetSession_InitializeIterator, 0x006530D0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_EndIteration, 0x0065E130, (void* self, void* edx, int a2));

static int __stdcall NetSession_FindStationById(uint32_t* stationId)
{
    alignas(8) uint8_t iteratorBytes[0x28];
    void* iterator = iteratorBytes;
    void* stationReference = iteratorBytes + 0x4;

    NetSession_InitializeIterator(iterator, nullptr);
    uint32_t foundStation = 0;

    if (CSF_AT(uint32_t, iterator, 0))
    {
        NetSession_BeginIteration(iterator, nullptr);
        NetSession_AdvanceIterator(iterator, nullptr, 1);
    }
    else
    {
        CSF_AT(uint32_t, iteratorBytes, 0x24) = 0;
        NetSession_AssignStationReference(
            stationReference, nullptr,
            &CSF_AT(uint32_t, iteratorBytes, 0x24));
    }

    uint32_t hasCurrent = CSF_AT(uint32_t, stationReference, 0x4);
    if (hasCurrent)
    {
        do
        {
            if (foundStation)
                break;

            uint32_t currentStation;
            if (hasCurrent)
            {
                currentStation = CSF_AT(uint32_t, stationReference, 0);
            }
            else
            {
                Game_RaiseStatusNotice(0, 0, static_cast<uint32_t>(-1610612729), 0);
                currentStation = 0;
            }

            uint32_t currentId = CSF_AT(
                uint32_t, reinterpret_cast<void*>(currentStation), 0x2C);
            if ((currentId & 0x3FFFFF) == 0)
            {
                Game_RaiseStatusNotice(0, 0, static_cast<uint32_t>(-536870898), 0);
                currentId = 0;
            }

            if (currentId == *stationId)
            {
                if (CSF_AT(uint32_t, stationReference, 0x4))
                {
                    foundStation = CSF_AT(uint32_t, stationReference, 0);
                }
                else
                {
                    Game_RaiseStatusNotice(0, 0, static_cast<uint32_t>(-1610612729), 0);
                    foundStation = 0;
                }
            }

            NetSession_AdvanceIterator(iterator, nullptr, 0);
            hasCurrent = CSF_AT(uint32_t, stationReference, 0x4);
        } while (hasCurrent);
    }

    NetSession_EndIteration(iterator, nullptr, 0);
    stationId = nullptr;
    NetSession_AssignStationReference(
        stationReference, nullptr, reinterpret_cast<uint32_t*>(&stationId));
    NetSession_DestroyStationReference(stationReference, nullptr);
    return static_cast<int32_t>(foundStation);
}

HOOK(0x0073E830, NetSession_FindStationById, "auto");
