#include <cstdint>
#include <cstddef>
#include <cmath>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, CGuiControl_FindNearbyFacingVector, 0x004504A0, (CGuiControl* self, void* , int source, float maximumDistance, int excludedFirst, int excludedSecond, uint32_t* output));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(int, __fastcall, GfxMatrix_SetYRotation, 0x006AE2A0, (S_006ada10* self, void* , float angle));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_CharacterWidthScale, 0x0082E060);
CSF_GLOBAL(float, g_CharacterFacingDistance, 0x00833854);
CSF_GLOBAL(float, g_ListenerOffsetScale_83BB14, 0x0083BB14);
CSF_GLOBAL(float, g_CharacterDirectionEpsilon, 0x0083F188);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static float CharacterModel_VectorLength(const float* vector)
{
    return std::sqrt(vector[0] * vector[0] +
                     vector[1] * vector[1] +
                     vector[2] * vector[2]);
}

static void CharacterModel_ReadFacing(
    C_008342e8* character, void* matrixStorage, float* output)
{
    void* model = reinterpret_cast<void*>(character->vGetField016c());
    void* transform =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, model, 0)[0x18 / 4])(model, nullptr);
    void* returnedMatrix =
        reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, transform, 0)[0x8C / 4])(
                transform, nullptr, matrixStorage);
    auto* facing = reinterpret_cast<float*>(
        static_cast<std::uint8_t*>(CSF_AT(void*, returnedMatrix, 0)) + 0x20);
    output[0] = facing[0];
    output[1] = facing[1];
    output[2] = facing[2];
    if (CSF_AT(std::uint8_t, matrixStorage, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixStorage, 0));
}

static void CharacterModel_ReadPosition(
    C_008342e8* character, void* scratch, float* output)
{
    if (character->field_0050)
    {
        auto* position =
            reinterpret_cast<float* (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, character, 0)[0xA4 / 4])(
                    character, nullptr, scratch);
        output[0] = position[0];
        output[1] = position[1];
        output[2] = position[2];
    }
    else
    {
        auto* position = reinterpret_cast<float*>(
            GetObjectPositionVector(character, nullptr));
        output[0] = position[0];
        output[1] = position[1];
        output[2] = position[2];
        output[1] += CSF_AT(
            float, reinterpret_cast<void*>(character->field_002c), 0xE4);
    }
}

static int __fastcall CharacterModel_SetCollisionAdjustedFacing(
    void* self, void* /*edx*/, char reverse, int excludedObject)
{
    if (CSF_AT(int, self, 0x8) == 2)
        return 2;
    CSF_AT(int, self, 0x8) = 2;

    auto* character = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(static_cast<std::uint8_t*>(self) + 0x74)));

    alignas(8) std::uint8_t positionScratch[0xC]{};
    alignas(8) std::uint8_t contactBytes[0x68]{};
    alignas(8) std::uint8_t rayBytes[0x54]{};
    alignas(8) std::uint8_t transformedBytes[0xC]{};
    alignas(8) std::uint8_t nearbyBytes[0xC]{};
    alignas(8) std::uint8_t rotationBytes[0x8]{};
    alignas(8) std::uint8_t matrixBytes[0x8]{};
    alignas(8) std::uint8_t endpointBytes[0xC]{};
    alignas(8) std::uint8_t facingBytes[0x34]{};
    alignas(8) std::uint8_t positionBytes[0xC]{};

    S_006ada10& rotation = *reinterpret_cast<S_006ada10*>(rotationBytes);
    auto* facing = reinterpret_cast<float*>(facingBytes);
    auto* position = reinterpret_cast<float*>(positionBytes);
    auto* transformed = reinterpret_cast<float*>(transformedBytes);
    auto* nearby = reinterpret_cast<float*>(nearbyBytes);
    auto* endpoint = reinterpret_cast<float*>(endpointBytes);
    auto* displacement = reinterpret_cast<float*>(
        static_cast<std::uint8_t*>(self) + 0xAC);
    auto* ray = reinterpret_cast<float*>(rayBytes + 0x18);
    auto* segment = reinterpret_cast<float*>(rayBytes);

    position[0] = g_fZero;
    position[1] = g_fZero;
    position[2] = g_fZero;
    displacement[0] = g_ZeroVectorX;
    displacement[1] = g_ZeroVectorY;
    displacement[2] = g_ZeroVectorZ;

    int attempt = 0;
    while (CharacterModel_VectorLength(displacement) < g_CharacterDirectionEpsilon)
    {
        float angles[8];
        angles[1] = g_fZero;
        angles[3] = g_fZero;
        angles[5] = g_fZero;
        angles[7] = g_fZero;
        if ((!reverse && !attempt) || (reverse && attempt))
        {
            angles[0] = 2.1991148f;
            angles[2] = 2.8274333f;
            angles[4] = 3.4557519f;
            angles[6] = 4.0840702f;
        }
        else
        {
            angles[0] = 5.3407078f;
            angles[2] = 5.9690261f;
            angles[4] = 0.31415927f;
            angles[6] = 0.94247788f;
        }

        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        int stateValue = GetStateValue(state, nullptr, 4);
        rotation.field_0000 =
            reinterpret_cast<decltype(rotation.field_0000)>(RwMatrixCreate());
        CSF_AT(std::uint8_t, rotationBytes, 4) = 1;
        GfxMatrix_SetYRotation(&rotation, nullptr, angles[2 * stateValue]);

        CharacterModel_ReadFacing(character, matrixBytes, facing);
        transformed[0] = 0.0f;
        transformed[1] = 0.0f;
        transformed[2] = 0.0f;
        RwV3dTransformVectors(
            reinterpret_cast<int>(transformed),
            reinterpret_cast<int>(facing), 1,
            reinterpret_cast<int>(
                reinterpret_cast<std::uint32_t*>(rotation.field_0000)));
        facing[0] = transformed[0];
        facing[1] = transformed[1];
        facing[2] = transformed[2];
        NormalizeVector3(reinterpret_cast<C_00833b88*>(facing), nullptr);

        CharacterModel_ReadPosition(character, positionScratch, position);
        ray[3] = position[0];
        ray[4] = position[1];
        ray[5] = position[2];

        displacement[0] = facing[0] * g_CharacterFacingDistance;
        displacement[1] = facing[1] * g_CharacterFacingDistance;
        displacement[2] = facing[2] * g_CharacterFacingDistance;
        CSF_AT(float, self, 0xB0) += g_ListenerOffsetScale_83BB14;

        ray[0] = displacement[0];
        // The original converts this component numerically to a signed integer.
        CSF_AT(int, ray, 4) = static_cast<int>(displacement[1]);
        ray[2] = displacement[2];
        NormalizeVector3(reinterpret_cast<C_00833b88*>(ray), nullptr);

        nearby[0] = 0.0f;
        nearby[1] = 0.0f;
        nearby[2] = 0.0f;
        float maximumDistance = CharacterModel_VectorLength(displacement);
        if (CGuiControl_FindNearbyFacingVector(
                reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
                reinterpret_cast<int>(ray), maximumDistance,
                reinterpret_cast<int>(character), excludedObject,
                reinterpret_cast<std::uint32_t*>(nearby)))
        {
            displacement[0] = (nearby[0] - position[0]) * g_CharacterWidthScale;
            displacement[1] = (nearby[1] - position[1]) * g_CharacterWidthScale;
            displacement[2] = (nearby[2] - position[2]) * g_CharacterWidthScale;
        }
        else
        {
            void* loader = reinterpret_cast<void*>(GetGlobalInt());
            void* controller = reinterpret_cast<void*>(
                SceneLoader_GetSceneController(loader, nullptr));

            segment[0] = position[0];
            segment[1] = position[1];
            segment[2] = position[2];
            segment[3] = position[0] + displacement[0];
            segment[4] = position[1] + displacement[1];
            segment[5] = position[2] + displacement[2];

            void* collisionScene =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, controller, 0)[0x3C / 4])(
                        controller, nullptr);
            void* collision =
                reinterpret_cast<void* (__fastcall*)(void*, void*, int, void*, int)>(
                    CSF_AT(void**, collisionScene, 0)[0x38 / 4])(
                        collisionScene, nullptr, 0, segment, 3);
            if (collision)
            {
                CSF_AT(float, contactBytes, 0x0) = FLT_MAX;
                CSF_AT(float, contactBytes, 0x4) = 0.0f;
                CSF_AT(float, contactBytes, 0x8) = 0.0f;
                CSF_AT(float, contactBytes, 0xC) = 0.0f;
                CSF_AT(int, contactBytes, 0x10) = 0;
                CSF_AT(int, contactBytes, 0x14) = 0;
                CSF_AT(int, contactBytes, 0x18) = 0;
                CSF_AT(int, contactBytes, 0x1C) = -1;
                CSF_AT(std::int8_t, contactBytes, 0x20) = -1;
                CSF_AT(int, contactBytes, 0x24) = 0;
                CSF_AT(std::int8_t, contactBytes, 0x28) = 0;
                CSF_AT(int, contactBytes, 0x2C) = -1;
                CSF_AT(std::int8_t, contactBytes, 0x30) = -1;
                CSF_AT(int, contactBytes, 0x5C) = 0;
                CSF_AT(int, contactBytes, 0x60) = 0;
                CSF_AT(int, contactBytes, 0x64) = 10;
                reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, collision, 0)[0xC / 4])(
                        collision, nullptr, contactBytes);

                displacement[0] =
                    (CSF_AT(float, contactBytes, 0x4) - position[0]) *
                    g_CharacterWidthScale;
                displacement[1] =
                    (CSF_AT(float, contactBytes, 0x8) - position[1]) *
                    g_CharacterWidthScale;
                displacement[2] =
                    (CSF_AT(float, contactBytes, 0xC) - position[2]) *
                    g_CharacterWidthScale;
            }
        }

        ++attempt;
        if (CSF_AT(std::uint8_t, rotationBytes, 4))
            RwMatrixDestroy(
                reinterpret_cast<std::uint32_t*>(rotation.field_0000));
        if (attempt >= 2)
            break;
    }

    if (CharacterModel_VectorLength(displacement) < g_CharacterDirectionEpsilon)
        CharacterModel_ReadFacing(character, matrixBytes, displacement);

    CharacterModel_ReadPosition(character, positionScratch, position);

    void* attached = CSF_AT(void*, self, 0x4);
    void* transform =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, attached, 0)[0x10 / 4])(attached, nullptr);
    endpoint[0] = position[0] + displacement[0];
    endpoint[1] = position[1] + displacement[1];
    endpoint[2] = position[2] + displacement[2];
    reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, transform, 0)[0x14 / 4])(
            transform, nullptr, endpoint);

    attached = CSF_AT(void*, self, 0x4);
    transform =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, attached, 0)[0x10 / 4])(attached, nullptr);
    return reinterpret_cast<int (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, transform, 0)[0x54 / 4])(
            transform, nullptr, position);
}

HOOK(0x004F5910, CharacterModel_SetCollisionAdjustedFacing, "auto");
