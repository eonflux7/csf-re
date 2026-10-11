#include <cstdint>
#include "globals.h"
#include <cstring>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, TFncGirar_IsRotationSettled, 0x0041E460, (void* self, void* edx));
CSF_FUNC(double, __fastcall, TFncGirar_ClearRotationFlag, 0x0041E480, (void* self, void* edx));
CSF_FUNC(double, __fastcall, TFncGirar_AdvanceRotation, 0x0041E490, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, int payload));
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_RotationPayloadScale, 0x0082E894);

static void __fastcall TFncGirar_TickRotationAndNotify(TFncGirar* self, void* /*edx*/, float deltaTime)
{
    uint8_t* sceneBase = reinterpret_cast<uint8_t*>(self->field_0008);

    if (!TFncGirar_IsRotationSettled(sceneBase + 0x244, nullptr))
    {
        double step = TFncGirar_AdvanceRotation(sceneBase + 0x244, nullptr, deltaTime);
        float payload = static_cast<float>(step * g_RotationPayloadScale);
        int payloadBits;
        std::memcpy(&payloadBits, &payload, sizeof(payloadBits));
        void* dispatchTarget = CSF_AT(void*, sceneBase, 0x14);
        DispatchSceneObjectWithPayload(dispatchTarget, nullptr, payloadBits);
    }

    uint8_t* sceneBaseAgain = reinterpret_cast<uint8_t*>(self->field_0008);
    if (TFncGirar_IsRotationSettled(sceneBaseAgain + 0x244, nullptr))
    {
        TFncGirar_ClearRotationFlag(reinterpret_cast<uint8_t*>(self->field_0008) + 0x244, nullptr);
        TFncGirar_OnRotationComplete(self, nullptr, 0);
    }
}

HOOK(0x421BD0, TFncGirar_TickRotationAndNotify, "auto");
