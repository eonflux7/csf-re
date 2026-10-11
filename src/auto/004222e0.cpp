#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, FacingTurn_IsFinished, 0x0041E460, (void* self, void* edx));
CSF_FUNC(double, __fastcall, FacingTurn_Advance, 0x0041E490, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, FacingTurn_StartRotation, 0x0041EA70, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, int payload));
CSF_FUNC(double, __fastcall, ComputeScalarForResolvedObject, 0x0048F220, (void* self, void* edx));

CSF_GLOBAL(float, g_AdvanceScale, 0x0082E894);
CSF_GLOBAL(float, g_DegreesToRadians, 0x0082E898);

// Advances a facing/turn state machine owned by this object. While the turn is
// still in progress (or the task is blocked) the turn is advanced and the scene
// object is dispatched; otherwise a rotation is started using the resolved angle.
static void __fastcall AiTask_UpdateFacingOrDispatch(void* self, void* /*edx*/, float deltaSeconds)
{
    bool useSlowPath = CSF_AT(uint8_t, self, 0x204) != 0;

    if (!useSlowPath) {
        void* taskPtr = CSF_AT(void*, CSF_AT(void*, self, 0x14), 0x48);
        int progress = ((int(__fastcall*)(void*, void*))CSF_AT(void**, taskPtr, 0)[25])(taskPtr, nullptr);
        if (progress == -1) {
            useSlowPath = true;
        } else {
            void* taskState = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, taskPtr, 0)[26])(taskPtr, nullptr);
            if (CSF_AT(uint8_t, taskState, 0x5A) == 0) {
                useSlowPath = true;
            }
        }
    }

    void* turnState = static_cast<uint8_t*>(self) + 0x244;

    if (!useSlowPath) {
        float turnAngle = static_cast<float>(ComputeScalarForResolvedObject(CSF_AT(void*, self, 0x14), nullptr))
                          * g_DegreesToRadians;
        FacingTurn_StartRotation(turnState, nullptr, deltaSeconds, turnAngle);
        return;
    }

    if (!FacingTurn_IsFinished(turnState, nullptr)) {
        double advanced = FacingTurn_Advance(turnState, nullptr, deltaSeconds);
        float scaled = static_cast<float>(advanced * g_AdvanceScale);
        int payloadBits = 0;
        std::memcpy(&payloadBits, &scaled, sizeof payloadBits);
        DispatchSceneObjectWithPayload(CSF_AT(void*, self, 0x14), nullptr, payloadBits);
    }
}

HOOK(0x004222E0, AiTask_UpdateFacingOrDispatch, "auto");
