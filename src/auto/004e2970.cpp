#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(char, __fastcall, MpSpawnPoints_UpdateRelatedObject, 0x004E1E40, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0083e8c8_UpdateBase, 0x004E2410, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, C_0083e8c8_HandleParticipantTimeout, 0x004E27B0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, NetSession_FindParticipant, 0x0073E420, (int (__stdcall *participantHandle)(int)));

static void __fastcall C_0083e8c8_Update(
    C_0083e8c8* self, void* /*edx*/, float elapsedTime)
{
    if (self->field_0044 >= g_fZero)
        self->field_0044 = self->field_0044 - elapsedTime;

    if (CSF_AT(uint32_t, self, 0x18) != 1)
    {
        C_0083e8c8_UpdateBase(self, nullptr, elapsedTime);
        return;
    }

    const uint32_t relatedAddress = CSF_AT(
        uint32_t, reinterpret_cast<void*>(self->field_000c), 0x18);

    if (relatedAddress == 0 || relatedAddress == 1712)
    {
        C_0083e8c8_UpdateBase(self, nullptr, elapsedTime);
        return;
    }

    auto* relatedObject = reinterpret_cast<uint8_t*>(relatedAddress - 1712);
    const uint32_t participantHandle = self->field_003c;

    if (participantHandle == UINT32_MAX)
    {
        self->field_0034 = self->field_0034 - elapsedTime;
        if (self->field_0034 < g_fZero)
        {
            MpSpawnPoints_UpdateRelatedObject(self, nullptr);
            self->vSlot9C();
        }

        self->field_003c = CSF_AT(uint32_t, relatedObject, 0x668);
    }
    else if (CSF_AT(uint32_t, relatedObject, 0x668) != participantHandle)
    {
        MpSpawnPoints_UpdateRelatedObject(self, nullptr);
        self->vSlot9C();
    }
    else
    {
        GetGlobalDword_73C020();
        if (!NetSession_FindParticipant(
                reinterpret_cast<int (__stdcall *)(int)>(participantHandle)))
        {
            C_0083e8c8_HandleParticipantTimeout(self, nullptr);
        }
        else
        {
            float participantElapsed;
            std::memcpy(&participantElapsed, &self->field_0038,
                        sizeof(participantElapsed));
            participantElapsed = elapsedTime + participantElapsed;
            std::memcpy(&self->field_0038, &participantElapsed,
                        sizeof(participantElapsed));

            auto* globalState = reinterpret_cast<void*>(GetGlobalValue_558150());
            const float timeout = CSF_AT(float, globalState, 0x198);

            std::memcpy(&participantElapsed, &self->field_0038,
                        sizeof(participantElapsed));
            if (timeout + timeout < participantElapsed)
                C_0083e8c8_HandleParticipantTimeout(self, nullptr);
        }
    }

    C_0083e8c8_UpdateBase(self, nullptr, elapsedTime);
}

HOOK(0x004E2970, C_0083e8c8_Update, "auto");
