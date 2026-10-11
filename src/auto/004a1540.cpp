#include <cmath>
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, Matrix_AddPitchAngle, 0x006AE690, (S_006ae570* self, void* , float pitchDelta));
CSF_FUNC(void, __fastcall, Rotation_ApplyYawDelta, 0x006AE6B0, (S_006ae570* self, void* , float yawDelta));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

static int __fastcall HudAlertIcons_ApplyRecoil(void* self, void* /*edx*/, float* recoil)
{
    const bool randomizeYaw = CSF_AT(uint8_t, recoil, 0x08) != 0;
    float yawDelta = -recoil[0];
    float pitchDelta = -recoil[1];

    if (randomizeYaw)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        yawDelta = (GetStateValue(state, nullptr, 200) - 100)
                 * g_ProgressScale * yawDelta;
    }

    if (CSF_AT(uint8_t, recoil, 0x09))
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        pitchDelta = ((GetStateValue(state, nullptr, 40) - 20)
                    * g_ProgressScale + g_TransitionOne) * pitchDelta;
    }

    const double damping = std::exp2(
        -(CSF_AT(float, self, 0x18) * CSF_AT(float, self, 0x14))
        * 1.442695040888963407);
    const float dampedPitch =
        static_cast<float>(damping * CSF_AT(float, self, 0x08));
    CSF_AT(float, self, 0x04) =
        static_cast<float>(damping * CSF_AT(float, self, 0x04));
    CSF_AT(float, self, 0x08) = dampedPitch;

    auto* rotation = reinterpret_cast<S_006ae570*>(
        static_cast<uint8_t*>(self) + 0x04);
    Rotation_ApplyYawDelta(rotation, nullptr, yawDelta);
    Matrix_AddPitchAngle(rotation, nullptr, pitchDelta);

    CSF_AT(float, self, 0x14) = 0.0f;
    CSF_AT(float, self, 0x18) = recoil[3];

    return 0;
}

HOOK(0x004A1540, HudAlertIcons_ApplyRecoil, "auto");
