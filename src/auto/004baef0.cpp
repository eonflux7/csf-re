#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_0082d448_Construct, 0x004203B0, (void* self, void* , int initialValue, uint8_t* sourceRecord, uint8_t* sourceVector));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void, __stdcall, AiCore_AdjustPositionToSceneSurface, 0x004AA6D0, (void* a1));
CSF_FUNC(int, __fastcall, AiCore_IsTargetPositionValid, 0x004AEDD0, (S_004aa310* self, void* , int a2));
CSF_FUNC(bool, __fastcall, S_004b0200_CanSeeTargetPosition, 0x004B0200, (S_004b0200* self, void* , int character, int targetPosition));
CSF_FUNC(char, __fastcall, S_004b0200_CallWithFieldAddress, 0x004B05F0, (S_004b0200* self, void* , int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_WeaponPlacementTraceDepth, 0x0082E11C);

static float AiDistracted_FloatFromBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static uint8_t* AiDistracted_GetCharacterObject(void* character)
{
    const int linkedValue = GetLinkedObjectValue_4DB5C0(character, nullptr);
    return linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue - 4)
        : nullptr;
}

static void __fastcall S_004b0200_UpdateDistractedTarget(
    S_004b0200* self, void* /*edx*/, int character, int /*unused*/)
{
    // The packet also lists the visibility-method address as a vtable binding.
    (void)CSF_VTBL(0x004B0200);

    void* characterHandle = reinterpret_cast<void*>(character);
    uint8_t* characterObject = AiDistracted_GetCharacterObject(characterHandle);
    void* sceneObject = CSF_AT(void*, characterObject, 0x2C);
    void* controller = CSF_AT(void*, sceneObject, 0x30);

    alignas(8) uint8_t flagsBytes[0x4];

    if (((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0xB8 / 4])(controller, nullptr))
    {
        Dispatch_NoLinkedTarget(characterHandle, nullptr, 1);
        return;
    }

    uint32_t* flags = ((uint32_t*(__fastcall*)(void*, void*, uint8_t*))
        CSF_AT(void**, controller, 0)[0xBC / 4])(
            controller, nullptr, flagsBytes);

    if ((*flags & 2) == 0)
    {
        Dispatch_NoLinkedTarget(characterHandle, nullptr, 1);
        return;
    }

    bool followLinkedPosition =
        !(AiDistracted_FloatFromBits(self->field_0010) > g_fZero);

    if (!followLinkedPosition)
    {
        characterObject = AiDistracted_GetCharacterObject(characterHandle);
        const int positionAddress = GetObjectPositionVector(
            CSF_AT(void*, characterObject, 0x2C), nullptr);
        void* position = reinterpret_cast<void*>(positionAddress);

        const float dx = AiDistracted_FloatFromBits(self->field_0004)
            - CSF_AT(float, position, 0);
        const float dy = self->field_0008 - CSF_AT(float, position, 4);
        const float dz = CSF_AT(float, self, 0x0C)
            - CSF_AT(float, position, 8);
        const float radius = AiDistracted_FloatFromBits(self->field_0010);

        followLinkedPosition =
            !(radius * radius >= dz * dz + dy * dy + dx * dx);
    }

    if (followLinkedPosition)
    {
        if (!self->field_0014)
            return;

        characterObject = AiDistracted_GetCharacterObject(characterHandle);
        if (CSF_AT(int32_t, characterObject, 0x608) == -1
            && CSF_AT(int32_t, characterObject, 0x60C) == -1)
        {
            return;
        }

        characterObject = AiDistracted_GetCharacterObject(characterHandle);
        const uint32_t targetX = CSF_AT(uint32_t, characterObject, 0x64C);
        const uint32_t targetY = CSF_AT(uint32_t, characterObject, 0x650);
        const uint32_t targetZ = CSF_AT(uint32_t, characterObject, 0x654);

        const float dx = AiDistracted_FloatFromBits(targetX)
            - AiDistracted_FloatFromBits(self->field_0004);
        const float dy = AiDistracted_FloatFromBits(targetY) - self->field_0008;
        const float dz = AiDistracted_FloatFromBits(targetZ)
            - CSF_AT(float, self, 0x0C);

        if (!(dz * dz + dy * dy + dx * dx > g_WeaponPlacementTraceDepth))
            return;

        characterObject = AiDistracted_GetCharacterObject(characterHandle);
        if (static_cast<uint8_t>(AiCore_IsTargetPositionValid(
                reinterpret_cast<S_004aa310*>(characterObject), nullptr,
                reinterpret_cast<int>(&self->field_0004))) == 0)
        {
            return;
        }

        self->field_0004 = targetX;
        std::memcpy(&self->field_0008, &targetY, sizeof(targetY));
        CSF_AT(uint32_t, self, 0x0C) = targetZ;

        GetLinkedObjectValue_4DB5C0(characterHandle, nullptr);
        AiCore_AdjustPositionToSceneSurface(&self->field_0004);
        ((void(__fastcall*)(void*, void*, uint32_t*, int))
            CSF_AT(void**, controller, 0)[0xAC / 4])(
                controller, nullptr, &self->field_0004, 0);
        return;
    }

    if (self->field_0015)
    {
        characterObject = AiDistracted_GetCharacterObject(characterHandle);
        const int positionAddress = GetObjectPositionVector(
            CSF_AT(void*, characterObject, 0x2C), nullptr);

        if (!S_004b0200_CanSeeTargetPosition(
                self, nullptr, character, positionAddress)
            && S_004b0200_CallWithFieldAddress(self, nullptr, character))
        {
            characterObject = AiDistracted_GetCharacterObject(characterHandle);
            alignas(8) uint8_t eventBytes[0x28];

            C_0082d448_Construct(
                eventBytes, nullptr, 2,
                CSF_AT(uint8_t*, characterObject, 0x2C) + 0x1C,
                reinterpret_cast<uint8_t*>(&self->field_0004));

            void* manager = reinterpret_cast<void*>(GetValue8B0554());
            DispatchWithArgBlock(
                CSF_AT(uint8_t*, manager, 0x1FD8) + 0x54,
                nullptr, reinterpret_cast<uint32_t*>(eventBytes));

            self->field_0010 = 0;
            GetLinkedObjectValue_4DB5C0(characterHandle, nullptr);
            AiCore_AdjustPositionToSceneSurface(&self->field_0004);
            ((void(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, controller, 0)[0xAC / 4])(
                    controller, nullptr, &self->field_0004, 0);
            return;
        }
    }

    Dispatch_NoLinkedTarget(characterHandle, nullptr, 0);
}

HOOK(0x004BAEF0, S_004b0200_UpdateDistractedTarget, "auto");
