#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharacterMovement_SetDestination, 0x00425200,
         (void* self, void* edx, float* a2, void* a3, int a4));
CSF_FUNC(int, __fastcall, CPlayerCharacter_SetInteractionState, 0x0048EEC0, (CPlayerCharacter* self, void* , int state));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_0083b118_HandleTaskEvent(
    C_0083b118* self, void* /*edx*/, int eventAddress)
{
    auto* event = reinterpret_cast<uint8_t*>(eventAddress);

    switch (CSF_AT(uint32_t, event, 0))
    {
    case 0:
    {
        self->field_0018 |= 1u;
        const int request = CSF_AT(int, event, 0x10);
        if (request != -1)
        {
            auto* character = reinterpret_cast<C_00833908*>(self->field_0014);
            reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
                character->vtbl[0x90 / 4])(
                character, nullptr, request, 0, -1, 0);

            void* requestManager = reinterpret_cast<void*>(
                reinterpret_cast<C_00833908*>(self->field_0014)->field_0048);
            if (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, requestManager, 0)[0x64 / 4])(
                    requestManager, nullptr) != -1)
            {
                requestManager = reinterpret_cast<void*>(
                    reinterpret_cast<C_00833908*>(self->field_0014)->field_0048);
                void* currentRequest =
                    reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, requestManager, 0)[0x68 / 4])(
                        requestManager, nullptr);
                self->field_0044 = CSF_AT(uint32_t, currentRequest, 0x30);
            }
        }

        float destinationParameter;
        std::memcpy(&destinationParameter, &self->field_0044,
                    sizeof(destinationParameter));
        if (destinationParameter > g_fZero)
        {
            const uint8_t option = CSF_AT(uint8_t, event, 0x16);
            void* movement = reinterpret_cast<void*>(
                reinterpret_cast<C_00833908*>(self->field_0014)->field_0030);
            CSF_AT(uint32_t, movement, 0x1C) = option != 0;
            CharacterMovement_SetDestination(
                movement, nullptr, reinterpret_cast<float*>(event + 4),
                reinterpret_cast<void*>(self->field_0044), 1);

            self->field_0048 = CSF_AT(uint32_t, event, 4);
            self->field_004c = CSF_AT(uint32_t, event, 8);
            const uint32_t destinationZ = CSF_AT(uint32_t, event, 12);
            self->field_0040 = 2;
            self->field_0050 = destinationZ;
        }
        break;
    }

    case 0x19:
    {
        self->field_0018 |= 1u;
        auto* character = reinterpret_cast<C_00833908*>(self->field_0014);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
            character->vtbl[0x90 / 4])(
            character, nullptr, CSF_AT(int, event, 4),
            CSF_AT(int, event, 8), -1, 0);
        self->field_0040 = 1;
        break;
    }

    case 0x1D:
    {
        CPlayerCharacter_SetInteractionState(
            reinterpret_cast<CPlayerCharacter*>(self->field_0014), nullptr, 0);

        auto* character = reinterpret_cast<C_00833908*>(self->field_0014);
        alignas(8) uint8_t recordBytes[0xC];
        CSF_AT(int, recordBytes, 4) = -1;
        CSF_AT(int, recordBytes, 8) = -1;
        CSF_AT(void**, recordBytes, 0) = CSF_VTBL(0x0082D448);
        character->vSlot58(reinterpret_cast<int>(recordBytes));
        break;
    }

    case 0x26:
        if (self->field_0040 == 2)
        {
            void* movement = reinterpret_cast<void*>(
                reinterpret_cast<C_00833908*>(self->field_0014)->field_0030);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, movement, 0)[0xA0 / 4])(movement, nullptr);
            self->field_0040 = 0;
        }
        break;

    default:
        return;
    }
}

HOOK(0x004CF0F0, C_0083b118_HandleTaskEvent, "auto");
