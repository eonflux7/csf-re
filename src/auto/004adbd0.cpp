#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30,
         (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CoverProgress_SetEndpoints, 0x004AD8E0,
         (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(void, __fastcall, CoverProgress_Advance, 0x004AD930,
         (void* self, void* edx, float a2, float* a3));

static void __fastcall S_004ad990_UpdateCoverProgress(
    S_004ad990* self, void* /*edx*/, float deltaTime)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    if (self->field_00a8)
    {
        if (static_cast<int32_t>(self->field_0054) >= 0 &&
            CHandleTable_Resolve(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(bytes + 0x4C)))
        {
            void* target = reinterpret_cast<void*>(CHandleTable_Resolve(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(bytes + 0x4C)));
            uint32_t* targetPosition = GetObjectPositionVector(target, nullptr);
            uint32_t* ownerPosition = GetObjectPositionVector(
                reinterpret_cast<void*>(self->field_0000), nullptr);

            CoverProgress_SetEndpoints(
                bytes + 0x5C, nullptr, ownerPosition, targetPosition);
        }
        else
        {
            uint32_t* ownerPosition = GetObjectPositionVector(
                reinterpret_cast<void*>(self->field_0000), nullptr);

            self->field_005c = 0;
            CSF_AT(float, self, 0x70) = self->field_0040;
            CSF_AT(float, self, 0x74) = CSF_AT(float, self, 0x44);
            float destinationZ = CSF_AT(float, self, 0x48);
            self->field_0060 = 1065353216;
            CSF_AT(float, self, 0x78) = destinationZ;

            CSF_AT(uint32_t, self, 0x64) = ownerPosition[0];
            CSF_AT(uint32_t, self, 0x68) = ownerPosition[1];
            CSF_AT(uint32_t, self, 0x6C) = ownerPosition[2];
        }
    }
    else if (self->field_002c != static_cast<uint32_t>(-1))
    {
        bool useAlternateHandle = static_cast<int32_t>(self->field_0054) < 0;
        int32_t handleIndex = useAlternateHandle
            ? CSF_AT(int32_t, self, 0x3C)
            : static_cast<int32_t>(self->field_0054);

        if (handleIndex >= 0)
        {
            uint8_t* handle = bytes + (useAlternateHandle ? 0x34 : 0x4C);
            void* target = reinterpret_cast<void*>(CHandleTable_Resolve(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(handle)));

            if (target)
            {
                uint32_t* targetPosition = GetObjectPositionVector(target, nullptr);
                CoverProgress_Advance(
                    bytes + 0x5C, nullptr, deltaTime,
                    reinterpret_cast<float*>(targetPosition));
            }
        }
    }
}

HOOK(0x004ADBD0, S_004ad990_UpdateCoverProgress, "auto");
