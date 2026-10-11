#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint32_t, g_dword_8BC098, 0x008BC098);

CSF_FUNC(void, __fastcall, NetSession_AdvanceStationIterator, 0x0064B900, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetSession_DestroyStationReference, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_ResetStationReference, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(unsigned int*, __fastcall, NetSession_InitStationIterator, 0x006530D0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_ResetStationIterator, 0x0065E130, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetSession_AcceptStation, 0x0067FB50, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_RejectStation, 0x0067FB60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_GetStationObject, 0x0067FC00, (void* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620, (std::uint16_t* source, char* destination, unsigned int length));

static int __cdecl NetSession_CheckStationAdmission(int station)
{
    alignas(8) uint8_t frameBytes[0xAC];
    char* sessionName = reinterpret_cast<char*>(frameBytes);
    void* iterator = frameBytes + 0x80;
    void* stationReference = frameBytes + 0x84;
    uint32_t* emptyReference = reinterpret_cast<uint32_t*>(frameBytes + 0xA4);

    CSF_AT(uint8_t, frameBytes, 0xAB) = 0;

    if (g_dword_8BC098)
    {
        int stationCount = 0;
        NetSession_InitStationIterator(iterator, nullptr);

        while (CSF_AT(int, iterator, 0x08))
        {
            if (reinterpret_cast<uintptr_t>(
                    LookupObjectById(CSF_AT(void*, iterator, 0x04), nullptr)) != 4)
            {
                ++stationCount;
            }
            NetSession_AdvanceStationIterator(iterator, nullptr, 0);
        }

        std::memset(sessionName, 0, 128);

        void* session = reinterpret_cast<void*>(g_dword_8BC098);
        std::uint16_t* wideName;
        if (CSF_AT(uint32_t, session, 0x94) < 8)
            wideName = reinterpret_cast<std::uint16_t*>(
                reinterpret_cast<uint8_t*>(session) + 0x80);
        else
            wideName = reinterpret_cast<std::uint16_t*>(
                CSF_AT(uint32_t, session, 0x80));

        ConvertWideTextToNarrow(wideName, sessionName, 128);

        void* stationObject = reinterpret_cast<void*>(
            NetSession_GetStationObject(reinterpret_cast<void*>(station), nullptr));

        if (reinterpret_cast<uintptr_t>(
                LookupObjectById(stationObject, nullptr)) == 4)
        {
            CSF_AT(uint8_t, frameBytes, 0xAB) = 1;
        }
        else
        {
            session = reinterpret_cast<void*>(g_dword_8BC098);
            if (stationCount < CSF_AT(int, session, 0x100) &&
                CSF_AT(uint8_t, session, 0x104))
            {
                CSF_AT(uint8_t, frameBytes, 0xAB) = 1;
            }
        }

        NetSession_ResetStationIterator(iterator, nullptr, 0);
        *emptyReference = 0;
        NetSession_ResetStationReference(stationReference, nullptr, emptyReference);
        NetSession_DestroyStationReference(stationReference, nullptr);

        if (CSF_AT(uint8_t, frameBytes, 0xAB))
        {
            NetSession_AcceptStation(reinterpret_cast<void*>(station), nullptr);
            return 0;
        }
    }

    NetSession_RejectStation(reinterpret_cast<void*>(station), nullptr);
    return 0;
}

HOOK(0x0073DB20, NetSession_CheckStationAdmission, "auto");
