#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (float* self, void* , float* a2, float* a3));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_TransformVector3, 0x004069F0, (S_00526d40* self, void* , uint32_t* out, int* in));
CSF_FUNC(int, __fastcall, GfxMatrix_AreVectorsDifferent, 0x0040FB20, (float* self, void* , float* other));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, C_00831658_InitializeEventRecord, 0x004747B0, (C_00831658* self, void* , int a2));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(float*, __fastcall, C_00833b88_CopyAndNormalizeVector3, 0x004781A0, (C_00833b88* self, void* , const uint32_t* source));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008342e8_GetDrawState, 0x0049CF10, (C_008342e8* self, void*));
CSF_FUNC(void, __fastcall, WeaponFirearms_ForwardDirectionToChild, 0x0049CF40, (void* self, void* edx, int direction));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(int, __fastcall, GfxMatrix_CreateQuaternionMatrix, 0x006AE6F0, (float* self, void* , int outputAddress));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionFromAxisAngle, 0x006AE8C0, (float* self, void* , S_00526d40* axis, float angle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_FireAngleToRadians, 0x008318C0);

static int __fastcall C_008317c8_ApplyFireDirection(
    C_008317c8* self, void* /*edx*/, float* aim, char applySpread, char alternateFire)
{
    alignas(8) uint8_t aimBytes[0x18];
    alignas(8) uint8_t directionBytes[0xC];
    alignas(8) uint8_t scratchBytes[0xC];
    alignas(8) uint8_t axisBytes[0xC];
    alignas(8) uint8_t quaternionBytes[0x10];
    alignas(8) uint8_t transformedBytes[0xC];
    alignas(8) uint8_t eventBytes[0x60];

    std::memcpy(aimBytes, aim, 0x18);

    int result = static_cast<int32_t>(self->field_006c);
    if (result < 0)
        return result;

    result = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x64));
    if (!result)
        return result;

    auto* projectile = reinterpret_cast<C_008342e8*>(result);

    if (!self->vSlotE8() &&
        C_008319c0_GetSubObjectField74(
            reinterpret_cast<C_008319c0*>(self), nullptr) == 10)
    {
        auto* owner = reinterpret_cast<uint8_t*>(self->field_0004);
        if (!CSF_AT(uint32_t, owner, 0x50) &&
            CSF_AT(int32_t, owner, 0x278) >= 0 &&
            CHandleTable_Resolve(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(owner + 0x270)) &&
            static_cast<uint8_t>(GfxMatrix_AreVectorsDifferent(
                reinterpret_cast<float*>(owner + 0x280), nullptr, &g_ZeroVectorX)))
        {
            auto* position = reinterpret_cast<float*>(
                GetObjectPositionVector(projectile, nullptr));
            BuildFacingVectorBetween(
                reinterpret_cast<float*>(owner + 0x280), nullptr,
                reinterpret_cast<float*>(scratchBytes), position);
            NormalizeVector3(
                reinterpret_cast<C_00833b88*>(scratchBytes), nullptr);

            position = reinterpret_cast<float*>(
                GetObjectPositionVector(projectile, nullptr));
            std::memcpy(aimBytes + 0xC, position, 0xC);
            C_00833b88_CopyAndNormalizeVector3(
                reinterpret_cast<C_00833b88*>(aimBytes), nullptr,
                reinterpret_cast<const uint32_t*>(scratchBytes));
        }
    }

    float magnitude;
    auto* overrideHandle = reinterpret_cast<void*>(self->field_0070);
    if (overrideHandle)
    {
        auto* parameters = reinterpret_cast<void*>(
            Handle_GetValuePointer(overrideHandle, nullptr));
        magnitude = alternateFire
            ? CSF_AT(float, parameters, 0x1B0)
            : CSF_AT(float, parameters, 0x1A8);
    }
    else
    {
        auto* drawState = reinterpret_cast<void*>(
            C_008342e8_GetDrawState(projectile, nullptr));
        magnitude = alternateFire
            ? CSF_AT(float, drawState, 0x1AC)
            : CSF_AT(float, drawState, 0x1A4);
    }

    std::memcpy(directionBytes, aimBytes, 0xC);
    auto& direction = *reinterpret_cast<S_00526d40*>(directionBytes);
    auto& scratch = *reinterpret_cast<S_00526d40*>(scratchBytes);
    auto& axis = *reinterpret_cast<S_00526d40*>(axisBytes);

    if (applySpread)
    {
        CSF_AT(float, scratchBytes, 0x0) = CSF_AT(float, aimBytes, 0x8);
        CSF_AT(float, scratchBytes, 0x4) = 0.0f;
        CSF_AT(float, scratchBytes, 0x8) = -CSF_AT(float, directionBytes, 0x0);
        S_00526d40_ConvertDirection(
            &scratch, nullptr, reinterpret_cast<uint32_t*>(axisBytes));

        auto* drawState = reinterpret_cast<void*>(
            C_008342e8_GetDrawState(projectile, nullptr));
        float spreadAngle = alternateFire
            ? CSF_AT(float, drawState, 0x1B0)
            : CSF_AT(float, drawState, 0x1A8);
        GfxMatrix_SetQuaternionFromAxisAngle(
            reinterpret_cast<float*>(quaternionBytes), nullptr,
            &axis, spreadAngle * g_FireAngleToRadians);

        auto* matrix = reinterpret_cast<int*>(GfxMatrix_CreateQuaternionMatrix(
            reinterpret_cast<float*>(quaternionBytes), nullptr,
            reinterpret_cast<int>(scratchBytes + 0x4)));
        uint32_t* transformed = S_00526d40_TransformVector3(
            &direction, nullptr, reinterpret_cast<uint32_t*>(transformedBytes), matrix);
        std::memcpy(directionBytes, transformed, 0xC);

        if (CSF_AT(uint8_t, scratchBytes, 0x8))
            RwMatrixDestroy(reinterpret_cast<uint32_t*>(
                CSF_AT(uint32_t, scratchBytes, 0x4)));
    }

    CSF_AT(float, scratchBytes, 0x0) =
        CSF_AT(float, directionBytes, 0x0) * magnitude;
    CSF_AT(float, scratchBytes, 0x4) =
        CSF_AT(float, directionBytes, 0x4) * magnitude;
    CSF_AT(float, scratchBytes, 0x8) =
        CSF_AT(float, directionBytes, 0x8) * magnitude;
    WeaponFirearms_ForwardDirectionToChild(
        projectile, nullptr, reinterpret_cast<int>(scratchBytes));

    result = GetGlobalState_50D190();
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xD0))
        return result;

    auto& event = *reinterpret_cast<C_00831658*>(eventBytes);
    C_00831658_InitializeEventRecord(&event, nullptr, 17);

    auto* owner = reinterpret_cast<void*>(self->field_0004);
    event.field_000c = CSF_AT(uint32_t, owner, 0x20);
    event.field_0010 = CSF_AT(uint32_t, owner, 0x24);
    event.field_0024 = CSF_AT(
        uint32_t, reinterpret_cast<void*>(self->field_0040), 0x74);

    auto* position = reinterpret_cast<uint32_t*>(
        GetObjectPositionVector(projectile, nullptr));
    event.field_0034 = position[0];
    event.field_0038 = position[1];
    event.field_003c = position[2];
    event.field_0040 = direction.field_0000;
    std::memcpy(&event.field_0044, directionBytes + 0x4, 4);
    std::memcpy(&event.field_0048, directionBytes + 0x8, 4);
    event.field_004c = reinterpret_cast<uint32_t>(projectile);

    auto* registry = reinterpret_cast<void*>(CSF_AT(
        uint32_t, reinterpret_cast<void*>(GetValue8B0554()), 8128));
    auto* value = reinterpret_cast<uint32_t*>(
        Handle_GetValuePointer(self, nullptr));
    event.field_0050 =
        reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, registry, 0)[0xC / 4])(registry, nullptr, *value);

    CSF_AT(float, scratchBytes, 0x0) =
        CSF_AT(float, directionBytes, 0x0) * magnitude;
    event.field_0054 = scratch.field_0000;
    CSF_AT(float, scratchBytes, 0x4) =
        CSF_AT(float, directionBytes, 0x4) * magnitude;
    std::memcpy(&event.field_0058, scratchBytes + 0x4, 4);
    CSF_AT(float, scratchBytes, 0x8) =
        CSF_AT(float, directionBytes, 0x8) * magnitude;
    std::memcpy(&event.field_005c, scratchBytes + 0x8, 4);

    auto* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    return static_cast<int>(DispatchRecordToGlobalObject(
        dispatcher, nullptr, &event, 1));
}

HOOK(0x00479760, C_008317c8_ApplyFireDirection, "auto");
