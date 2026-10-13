#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082e090_UpdateGroundAdjustedFocus, 0x004162E0, (C_0082e090* self, void* , float* position, float* direction, int traceState));
CSF_FUNC(void, __fastcall, C_0082e090_PlayActivitySound, 0x00416980, (C_0082e090* self, void* , uint32_t* eventData));
CSF_FUNC(void, __fastcall, C_0082e090_HandleCollision, 0x00416AE0, (C_0082e090* self, void* , const float* displacement, uint32_t* collision, int));
CSF_FUNC(void*, __fastcall, InitializePositionRecord_4460A0, 0x004460A0, (void* self, void* , int identifier, const void* source, const void* position, int parameter));
CSF_FUNC(uint32_t*, __fastcall, S_00445eb0_SubmitSceneRequest, 0x00446F10, (S_00445eb0* self, void* , int requestAddress));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void*));
CSF_FUNC(char, __fastcall, C_008342e8_SetObjectPlacement, 0x00491190, (C_008342e8* self, void* , uint32_t* position, float* direction));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_0082e090_ProcessMovementCollision(
    C_0082e090* self, void* /*edx*/,
    const float* displacement, uint32_t* collision, int collisionMode)
{
    float* direction =
        reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(collision) + 0x2C);

    // The original also takes the collision path for unordered comparisons.
    if (CSF_AT(uint32_t, collision, 0x4C) != 0 ||
        !(std::fabs(CSF_AT(float, collision, 0x2C)) <
          CSF_AT(float, collision, 0x30)) ||
        !(std::fabs(CSF_AT(float, collision, 0x34)) <
          CSF_AT(float, collision, 0x30)))
    {
        C_0082e090_HandleCollision(
            self, nullptr, displacement, collision, collisionMode);
    }
    else
    {
        C_0082e090_UpdateGroundAdjustedFocus(
            self, nullptr,
            reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(collision) + 8),
            direction, 1);
    }

    if (!self->field_0077)
    {
        C_008342e8_SetObjectPlacement(
            reinterpret_cast<C_008342e8*>(self->field_0004), nullptr,
            reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(collision) + 8),
            direction);
        C_0082e090_PlayActivitySound(self, nullptr, collision);

        if (CSF_AT(uint32_t,
                   reinterpret_cast<void*>(self->field_0004), 0x50) == 21)
        {
            const int globalAddress = GetGlobalValue_8BC1E0();
            const int parameter =
                CSF_AT(int, reinterpret_cast<void*>(globalAddress), 0xA70);
            auto* object = reinterpret_cast<C_008342e8*>(self->field_0004);
            const void* source = &object->field_001c;

            // The preceding pushed parameter belongs to InitializePositionRecord,
            // not to this no-stack-argument position getter.
            const int positionAddress = GetObjectPositionVector(object, nullptr);

            alignas(8) uint8_t recordBytes[0x28];
            InitializePositionRecord_4460A0(
                recordBytes, nullptr, 1, source,
                reinterpret_cast<const void*>(positionAddress), parameter);
            S_00445eb0_SubmitSceneRequest(
                CSF_AT(S_00445eb0*, g_pCurrentScene, 0xB0), nullptr,
                reinterpret_cast<int>(recordBytes));
        }
    }
}

HOOK(0x00416DE0, C_0082e090_ProcessMovementCollision, "auto");
