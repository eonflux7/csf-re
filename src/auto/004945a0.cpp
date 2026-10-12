#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(long double, __fastcall, Vector_GetLength, 0x00401FA0, (float* self, void*));
CSF_FUNC(float*, __fastcall, AiStatic_AddVectors, 0x00402000, (float* self, void* , float* outVector, float* addVector));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (float* self, void* , float* a2, float* a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GameConfig_IsSceneObjectRemovable, 0x007558C0, (int objectType));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CPlayerCharacter_FinalizeTargetSelection(
    CPlayerCharacter* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0298);
    if (result != 0)
        return result;

    C_00833908* selectedTarget = nullptr;

    if (static_cast<int>(self->field_0248) >= 0)
    {
        auto* candidate = reinterpret_cast<C_00833908*>(
            CHandleTable_Resolve(
                g_pCurrentScene, nullptr,
                reinterpret_cast<int>(
                    reinterpret_cast<uint8_t*>(self) + 0x240)));

        if (candidate)
        {
            void* objectInfo = reinterpret_cast<void*>(candidate->field_0034);
            if (objectInfo &&
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, objectInfo, 0)[0x98 / 4])(
                        objectInfo, nullptr) == 6)
            {
                alignas(8) uint8_t originBytes[0xC];
                alignas(8) uint8_t vectorBytes[0x10];
                float* origin = reinterpret_cast<float*>(originBytes);
                float* vector = reinterpret_cast<float*>(vectorBytes);

                vector[1] = CSF_AT(float, self, 0x1D4) * g_HalfDurationScale;
                vector[0] = 0;
                vector[2] = 0;

                float* position = GetObjectPositionVector(self, nullptr);
                AiStatic_AddVectors(position, nullptr, origin, vector);
                float* targetPosition =
                    GetObjectPositionVector(candidate, nullptr);
                float* facing =
                    BuildFacingVectorBetween(origin, nullptr, vector, targetPosition);
                float length = static_cast<float>(Vector_GetLength(facing, nullptr));
                void* configuration =
                    reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

                if (length < CSF_AT(float, configuration, 0xB10))
                {
                    int objectType = static_cast<int>(candidate->field_0050);
                    void* inventory = reinterpret_cast<void*>(self->field_0044);
                    int selectionId = -1;

                    if (objectType == 22)
                    {
                        selectionId = CSF_AT(int, candidate, 0x184);
                    }
                    else if (static_cast<uint8_t>(
                                 GameConfig_IsSceneObjectRemovable(objectType)))
                    {
                        void* component =
                            reinterpret_cast<void*>(candidate->field_004c);
                        if (component &&
                            !reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                                CSF_AT(void**, component, 0)[0x4C / 4])(
                                    component, nullptr))
                        {
                            selectionId = CSF_AT(int, candidate, 0x194);
                        }
                    }

                    if (selectionId != -1 &&
                        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                            CSF_AT(void**, inventory, 0)[0xB0 / 4])(
                                inventory, nullptr, selectionId) == 2)
                    {
                        selectedTarget = candidate;
                    }
                }
            }
        }
    }

    if (!selectedTarget)
    {
        result = static_cast<int>(self->field_02d0);
        if (result < 0)
            return result;

        result = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(
                reinterpret_cast<uint8_t*>(self) + 0x2C8));
        if (!result)
            return result;

        selectedTarget = reinterpret_cast<C_00833908*>(result);
    }

    self->field_0298 = 8;
    self->field_02a0 = selectedTarget->field_0020;
    self->field_02a4 = selectedTarget->field_0024;
    return reinterpret_cast<int>(selectedTarget);
}

HOOK(0x004945A0, CPlayerCharacter_FinalizeTargetSelection, "auto");
