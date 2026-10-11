#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, sub_4222E0, 0x004222E0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, sub_41E460, 0x0041E460, (void* self, void* edx));
CSF_FUNC(double, __fastcall, sub_41E480, 0x0041E480, (void* self, void* edx));
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, int payload));
CSF_FUNC(void, __fastcall, sub_4DB1D0, 0x004DB1D0, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_DegreesPerRadian, 0x0082E894);

// Advances the turn state of a TFncGirar: ticks the base, and when the turn timer
// has elapsed, dispatches the current heading (in degrees) and issues the follow-up
// command on the owner's controller.
static void __fastcall TFncGirar_UpdateTurnState(TFncGirar* self, void* /*edx*/, float deltaTime)
{
    sub_4222E0(reinterpret_cast<void*>(self->field_0008), nullptr, deltaTime);

    void* turnTimer = reinterpret_cast<uint8_t*>(self->field_0008) + 0x244;
    if (static_cast<uint8_t>(sub_41E460(turnTimer, nullptr)) != 0)
    {
        sub_41E480(reinterpret_cast<uint8_t*>(self->field_0008) + 0x244, nullptr);

        // Turn rate (radians) converted to degrees, passed as a float stack argument.
        float headingDegrees =
            CSF_AT(float, reinterpret_cast<void*>(self->field_0008), 0x250) * g_DegreesPerRadian;
        void* sceneObj = CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x14);
        int payloadBits = 0;
        std::memcpy(&payloadBits, &headingDegrees, sizeof(payloadBits));
        DispatchSceneObjectWithPayload(sceneObj, nullptr, payloadBits);

        // Re-read the owner chain after the dispatch call.
        void* ownerObj = CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x14);
        void* controller = CSF_AT(void*, ownerObj, 0x48);
        bool canCommand =
            ((bool(__fastcall*)(void*, void*, int))CSF_AT(void**, controller, 0)[9])(controller, nullptr, 1);
        if (canCommand)
        {
            void* commandObj = CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x14);
            ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, commandObj, 0)[35])(
                commandObj, nullptr, 0, 0, -1, 0);
        }

        sub_4DB1D0(self, nullptr, 0);
    }
}

HOOK(0x004225F0, TFncGirar_UpdateTurnState, "auto");
