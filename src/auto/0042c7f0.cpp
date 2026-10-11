#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00830578_ComputeAxisMovement, 0x0042B150, (void* self, void* edx, float* a2));
CSF_FUNC(void, __fastcall, C_00830578_ComputeTimedMovement, 0x0042C000, (void* self, void* edx, float* a2, float a3));
CSF_FUNC(void, __fastcall, DispatchSelectedCharacterCommand, 0x00450BF0, (void* self, void* , void* command));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CharacterModel_AdjustClampedRotation, 0x004F4F50, (void* self, void* , float horizontalDelta, float verticalDelta));
CSF_FUNC(void, __fastcall, CharacterModel_AccumulateOffset, 0x004F50A0, (float* self, void* , float deltaX, float deltaY));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MinimumRotation, 0x008305EC);
CSF_GLOBAL(float, g_MaximumRotation, 0x008305F0);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVector2X, 0x008BB448);
CSF_GLOBAL(float, g_ZeroVector2Y, 0x008BB44C);

static void __fastcall C_00830578_UpdateControlledCharacterMotion(
    C_00830578* self, void* /*edx*/, float elapsedTime)
{
    // The command's trailing coordinates overlap the movement scratch space.
    alignas(8) uint8_t commandAndMovementBytes[0x18];
    void* command = commandAndMovementBytes;
    float* movement = reinterpret_cast<float*>(commandAndMovementBytes + 0x8);
    movement[0] = 0.0f;
    movement[1] = 0.0f;

    void* source = reinterpret_cast<void*>(self->field_00e0);
    movement[3] =
        ((double(__fastcall*)(void*, void*))CSF_AT(void**, source, 0)[0x38 / 4])(
            source, nullptr);

    source = reinterpret_cast<void*>(self->field_00e0);
    movement[2] =
        ((double(__fastcall*)(void*, void*))CSF_AT(void**, source, 0)[0x34 / 4])(
            source, nullptr);
    self->field_0034 =
        movement[2] < movement[3] ? movement[2] : movement[3];

    source = reinterpret_cast<void*>(self->field_00e0);
    movement[3] =
        ((double(__fastcall*)(void*, void*))CSF_AT(void**, source, 0)[0x4C / 4])(
            source, nullptr);

    source = reinterpret_cast<void*>(self->field_00e0);
    movement[2] =
        ((double(__fastcall*)(void*, void*))CSF_AT(void**, source, 0)[0x48 / 4])(
            source, nullptr);
    self->field_0038 =
        movement[2] < movement[3] ? movement[2] : movement[3];

    source = reinterpret_cast<void*>(self->field_00e0);
    self->field_003d = static_cast<uint8_t>(
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, source, 0)[0x28 / 4])(
            source, nullptr));

    if (self->field_002c == g_fZero && self->field_0030 == g_fZero)
        C_00830578_ComputeTimedMovement(self, nullptr, movement, elapsedTime);
    else
        C_00830578_ComputeAxisMovement(self, nullptr, movement);

    if (movement[0] == g_ZeroVector2X && movement[1] == g_ZeroVector2Y)
        return;

    int collection = GetGlobalValue_4FBDE0();
    int entry = GetFirstElement(reinterpret_cast<void*>(collection), nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(entry), nullptr);

    switch (ResolveEntryLookupValue(provider, nullptr))
    {
    case 4:
    case 5:
    {
        collection = GetGlobalValue_4FBDE0();
        entry = GetFirstElement(reinterpret_cast<void*>(collection), nullptr);
        provider = GetCachedObjectProvider(reinterpret_cast<void*>(entry), nullptr);
        void* referencedObject = GetReferencedObject(provider, nullptr);
        void* rotation =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, referencedObject, 0)[0x10 / 4])(
                    referencedObject, nullptr);

        ((void(__fastcall*)(void*, void*, float))
            CSF_AT(void**, rotation, 0)[0x3C / 4])(
                rotation, nullptr, movement[0]);
        ((void(__fastcall*)(void*, void*, float))
            CSF_AT(void**, rotation, 0)[0x40 / 4])(
                rotation, nullptr, movement[1]);

        if (((double(__fastcall*)(void*, void*))
                CSF_AT(void**, rotation, 0)[0x24 / 4])(
                    rotation, nullptr) > g_MaximumRotation)
        {
            ((void(__fastcall*)(void*, void*, float))
                CSF_AT(void**, rotation, 0)[0x30 / 4])(
                    rotation, nullptr, 89.0f);
        }

        if (((double(__fastcall*)(void*, void*))
                CSF_AT(void**, rotation, 0)[0x24 / 4])(
                    rotation, nullptr) < g_MinimumRotation)
        {
            ((void(__fastcall*)(void*, void*, float))
                CSF_AT(void**, rotation, 0)[0x30 / 4])(
                    rotation, nullptr, -89.0f);
        }
        break;
    }

    case 6:
        collection = GetGlobalValue_4FBDE0();
        entry = GetFirstElement(reinterpret_cast<void*>(collection), nullptr);
        provider = GetCachedObjectProvider(reinterpret_cast<void*>(entry), nullptr);
        CharacterModel_AdjustClampedRotation(
            provider, nullptr, movement[0], movement[1]);
        break;

    case 7:
        return;

    case 8:
        collection = GetGlobalValue_4FBDE0();
        entry = GetFirstElement(reinterpret_cast<void*>(collection), nullptr);
        provider = GetCachedObjectProvider(reinterpret_cast<void*>(entry), nullptr);
        CharacterModel_AccumulateOffset(
            reinterpret_cast<float*>(provider), nullptr, movement[0], movement[1]);
        break;

    default:
        CSF_AT(float, command, 0x4) = movement[0];
        movement[0] = movement[1];
        CSF_AT(uint32_t, command, 0x0) = 1;
        movement[1] = 0.0f;
        DispatchSelectedCharacterCommand(g_pCurrentScene, nullptr, command);
        break;
    }
}

HOOK(0x0042C7F0, C_00830578_UpdateControlledCharacterMotion, "auto");
