#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(int, __cdecl, IsReactionSuppressed, 0x00761090, (int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __fastcall C_0083e5e8_ApplyImpactReaction(
    C_0083e5e8* self, void* /*edx*/, int eventAddress)
{
    const void* event = reinterpret_cast<const void*>(eventAddress);

    // The direction, saved receiver, and feedback payload share the original frame.
    alignas(8) uint8_t localBytes[0x20];
    float* displacement = reinterpret_cast<float*>(localBytes + 0x04);
    float* direction = reinterpret_cast<float*>(localBytes + 0x10);
    float* feedback = reinterpret_cast<float*>(localBytes);

    CSF_AT(void*, localBytes, 0x1C) =
        CSF_AT(void*, reinterpret_cast<void*>(self->field_000c), 0x4C);
    direction[0] = 0.0f;
    direction[1] = 0.0f;
    direction[2] = 0.0f;

    int result = IsReactionSuppressed(CSF_AT(int, event, 0x0C));
    if (static_cast<uint8_t>(result))
        return result;

    if (CSF_AT(int, event, 0x08) < 0 || CSF_AT(int, event, 0x24) == 4)
    {
        float* position = GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_000c), nullptr);
        displacement[0] = position[0] - CSF_AT(float, event, 0x18);
        displacement[1] = position[1] - CSF_AT(float, event, 0x1C);
        displacement[2] = position[2] - CSF_AT(float, event, 0x20);
        direction[0] = displacement[0];
        direction[1] = displacement[1];
        direction[2] = displacement[2];
    }
    else
    {
        int sourceAddress = CHandleTable_Resolve(
            g_pCurrentScene, nullptr, eventAddress);
        if (sourceAddress)
        {
            float* sourcePosition = GetObjectPositionVector(
                reinterpret_cast<void*>(sourceAddress), nullptr);
            float* position = GetObjectPositionVector(
                reinterpret_cast<void*>(self->field_000c), nullptr);
            displacement[0] = position[0] - sourcePosition[0];
            displacement[1] = position[1] - sourcePosition[1];
            displacement[2] = position[2] - sourcePosition[2];
            direction[0] = displacement[0];
            direction[1] = displacement[1];
            direction[2] = displacement[2];
        }
    }

    if (direction[0] != g_ZeroVectorX ||
        direction[1] != g_ZeroVectorY ||
        direction[2] != g_ZeroVectorZ)
    {
        NormalizeVector3(reinterpret_cast<C_00833b88*>(direction), nullptr);
        void* object = reinterpret_cast<void*>(self->field_000c);
        if (!static_cast<uint8_t>(IsField1BCNonNegative(object, nullptr)) &&
            !CSF_AT(uint8_t, object, 0x228))
        {
            double strength = static_cast<double>(CSF_AT(int, event, 0x28));
            void* receiver = CSF_AT(void*, localBytes, 0x1C);
            auto applyImpulse =
                reinterpret_cast<void(__fastcall*)(void*, void*, float*, float*, int)>(
                    CSF_AT(void**, receiver, 0)[0x54 / 4]);
            displacement[0] = static_cast<float>(direction[0] * strength);
            displacement[1] = static_cast<float>(direction[1] * strength);
            displacement[2] = static_cast<float>(direction[2] * strength);
            applyImpulse(receiver, nullptr, displacement, &g_ZeroVectorX, 0);
        }
    }

    uint32_t context = GetSceneMenuContext(g_pCurrentScene, nullptr);
    result = static_cast<int>(context);
    if (context == self->field_000c)
    {
        result = GetGlobalState_50D190();
        void* feedbackReceiver =
            CSF_AT(void*, reinterpret_cast<void*>(result), 0x74);
        if (feedbackReceiver)
        {
            feedback[3] = static_cast<float>(CSF_AT(int, event, 0x10));
            int reactionType = CSF_AT(int, event, 0x24);
            feedback[0] = direction[0];
            feedback[2] = direction[2];
            feedback[1] = direction[1];

            int feedbackType;
            switch (reactionType)
            {
            case 4: feedbackType = 3; break;
            case 5: feedbackType = 4; break;
            case 6: feedbackType = 6; break;
            default: feedbackType = 0; break;
            }

            return reinterpret_cast<int(__fastcall*)(void*, void*, int, float*)>(
                CSF_AT(void**, feedbackReceiver, 0)[0x04 / 4])(
                    feedbackReceiver, nullptr, feedbackType, feedback);
        }
    }
    return result;
}

HOOK(0x004E1B50, C_0083e5e8_ApplyImpactReaction, "auto");
