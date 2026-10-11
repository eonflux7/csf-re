#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GameConfig_IsStateCodeAllowed, 0x00755810, (std::uint32_t stateCode));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_DegreesToRadians, 0x0083F184);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CharacterModel_ComputeLerpBetweenPoints(
    void* self, void* /*edx*/, int firstHandle, int secondHandle,
    float blend, float* offset, float* heading)
{
    alignas(8) std::uint8_t vectorBytes[0x10];
    float* temporaryVector = reinterpret_cast<float*>(vectorBytes);
    void* firstHandleData = reinterpret_cast<void*>(firstHandle);
    void* secondHandleData = reinterpret_cast<void*>(secondHandle);

    CSF_AT(int, self, 0xD4) = -1;
    CSF_AT(int, self, 0xD8) = -1;
    CSF_AT(int, self, 0xE0) = -1;
    CSF_AT(int, self, 0xE4) = -1;

    if (CSF_AT(int, firstHandleData, 0x8) >= 0)
    {
        auto* firstObject = reinterpret_cast<C_008342e8*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, firstHandle));
        if (firstObject)
        {
            float* effectiveOffset = offset;
            if (!effectiveOffset)
            {
                void* provider = CSF_AT(void*, self, 0x4);
                void* currentObject =
                    ((void*(__fastcall*)(void*, void*))
                        CSF_AT(void**, provider, 0)[0x10 / 4])(provider, nullptr);

                float* objectPosition = reinterpret_cast<float*>(
                    GetObjectPositionVector(firstObject, nullptr));
                float* currentPosition =
                    ((float*(__fastcall*)(void*, void*))
                        CSF_AT(void**, currentObject, 0)[0x10 / 4])(currentObject, nullptr);

                temporaryVector[0] = currentPosition[0] - objectPosition[0];
                temporaryVector[1] = currentPosition[1] - objectPosition[1];
                temporaryVector[2] = currentPosition[2] - objectPosition[2];
                effectiveOffset = temporaryVector;
            }

            CSF_AT(std::uint32_t, self, 0xF4) = CSF_AT(std::uint32_t, effectiveOffset, 0);
            CSF_AT(std::uint32_t, self, 0xF8) = CSF_AT(std::uint32_t, effectiveOffset, 4);
            CSF_AT(std::uint32_t, self, 0xFC) = CSF_AT(std::uint32_t, effectiveOffset, 8);

            double targetHeading;
            if (heading)
                targetHeading = *heading;
            else
                targetHeading = C_008342e8_ComputeResolvedObjectScalar(firstObject, nullptr)
                    * g_DegreesToRadians;

            void* provider = CSF_AT(void*, self, 0x4);
            CSF_AT(float, self, 0x104) = static_cast<float>(targetHeading);
            void* currentObject =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, provider, 0)[0x10 / 4])(provider, nullptr);
            CSF_AT(float, self, 0x100) = static_cast<float>(
                ((double(__fastcall*)(void*, void*))
                    CSF_AT(void**, currentObject, 0)[0x28 / 4])(currentObject, nullptr)
                * g_DegreesToRadians);

            CSF_AT(std::uint32_t, self, 0xD4) = CSF_AT(std::uint32_t, firstHandleData, 4);
            CSF_AT(std::uint32_t, self, 0xD8) = CSF_AT(std::uint32_t, firstHandleData, 8);

            provider = CSF_AT(void*, self, 0x4);
            currentObject =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, provider, 0)[0x10 / 4])(provider, nullptr);
            void* currentPosition =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, currentObject, 0)[0x10 / 4])(currentObject, nullptr);

            CSF_AT(std::uint32_t, self, 0xE8) = CSF_AT(std::uint32_t, currentPosition, 0);
            CSF_AT(std::uint32_t, self, 0xEC) = CSF_AT(std::uint32_t, currentPosition, 4);
            CSF_AT(std::uint32_t, self, 0xF0) = CSF_AT(std::uint32_t, currentPosition, 8);
        }
    }

    if (CSF_AT(int, secondHandleData, 0x8) >= 0)
    {
        auto* secondObject = reinterpret_cast<C_008342e8*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, secondHandle));
        if (secondObject)
        {
            CSF_AT(std::uint32_t, self, 0xE0) = CSF_AT(std::uint32_t, secondHandleData, 4);
            CSF_AT(std::uint32_t, self, 0xE4) = CSF_AT(std::uint32_t, secondHandleData, 8);

            void* position;
            if (static_cast<std::uint8_t>(
                    GameConfig_IsStateCodeAllowed(secondObject->field_0050)) != 0)
            {
                position =
                    ((void*(__fastcall*)(void*, void*, float*))
                        CSF_AT(void**, secondObject, 0)[0xA4 / 4])(
                            secondObject, nullptr, temporaryVector);
            }
            else
            {
                position = reinterpret_cast<void*>(
                    GetObjectPositionVector(secondObject, nullptr));
            }

            CSF_AT(std::uint32_t, self, 0x110) = CSF_AT(std::uint32_t, position, 0);
            CSF_AT(std::uint32_t, self, 0x114) = CSF_AT(std::uint32_t, position, 4);
            CSF_AT(std::uint32_t, self, 0x118) = CSF_AT(std::uint32_t, position, 8);
        }
    }

    if (CSF_AT(int, self, 0xD8) < 0)
    {
        int secondIndex = CSF_AT(int, self, 0xE4);
        if (secondIndex < 0)
            return secondIndex;
    }

    CSF_AT(float, self, 0x10C) = g_TransitionOne - blend;
    return ObjectProvider_SetUpdateMode(self, nullptr, 9);
}

HOOK(0x004F7B40, CharacterModel_ComputeLerpBetweenPoints, "auto");
