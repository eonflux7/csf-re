#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, AiAlert_IsLinkedAlertReady, 0x004AD810, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

static int __fastcall TFncBusqueda_InitializeFromLinkedObject(
    TFncBusqueda* self, void* /*edx*/, TAiState* state)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    void* linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);

    self->field_0010 = CSF_AT(uint32_t, linkedObject, 0x64C);
    CSF_AT(uint32_t, self, 0x14) = CSF_AT(uint32_t, linkedObject, 0x650);
    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, linkedObject, 0x654);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);

    bool useAlternateState =
        CSF_AT(int, linkedObject, 0x608) == -1 &&
        CSF_AT(int, linkedObject, 0x60C) == -1;

    if (!useAlternateState)
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
        linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);

        if (CSF_AT(int, linkedObject, 0x608) != -1)
        {
            linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
            linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);

            useAlternateState = AiAlert_IsLinkedAlertReady(
                reinterpret_cast<uint8_t*>(linkedObject) + 0x604, nullptr);
        }
    }

    if (useAlternateState)
    {
        self->field_001c = 2;
        return CoverAttack_SetState_4DC150(
            state, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 8));
    }

    self->field_001c = 0;
    return CoverAttack_SetState_4DC150(
        state, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4));
}

HOOK(0x004B5710, TFncBusqueda_InitializeFromLinkedObject, "auto");
