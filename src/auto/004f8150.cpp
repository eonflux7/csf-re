#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharActionFsm_IsFlag10000SetAndByteNonZero, 0x00482310, (void* self, void*));
CSF_FUNC(double, __fastcall, GetFloat3D4, 0x004954E0, (void* self, void*));
CSF_FUNC(char, __fastcall, C_00495880_GetField3A5, 0x00495880, (uint8_t* self, void*));
CSF_FUNC(int, __fastcall, CharacterModel_UpdateSpecialCharacter, 0x004958C0, (void* self, void* edx, int a4));
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetProviderPosition, 0x004F4EB0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CharacterModel_UpdateHeadMotion, 0x004F6990, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, CharacterModel_UpdateHeadTransform, 0x004F6AF0, (void* self, void* edx, uint8_t* arg0, float* a3, float a4));
CSF_FUNC(int, __fastcall, CharacterModel_UpdateModeEight, 0x004F7400, (void* self, void* edx, int a2, int _68));
CSF_FUNC(int, __fastcall, CharacterModel_PrepareCharacterUpdate, 0x004F7940, (void* self, void* edx, void* a2, int a3));

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

static char __fastcall C_0085ec18_UpdateCharacterTransform(
    C_0085ec18* self, void* /*edx*/, CPlayerCharacter* character, int timeBits)
{
    const char isPrimaryCharacter = character->field_0050 == 0;
    CPlayerCharacter* primaryCharacter = isPrimaryCharacter ? character : nullptr;

    if (isPrimaryCharacter)
    {
        if (C_00495880_GetField3A5(
                reinterpret_cast<uint8_t*>(primaryCharacter), nullptr))
        {
            return static_cast<char>(CharacterModel_UpdateSpecialCharacter(
                primaryCharacter, nullptr, timeBits));
        }
        CharacterModel_PrepareCharacterUpdate(self, nullptr, character, timeBits);
    }

    alignas(8) uint8_t frameBytes[0x20];
    float* position = reinterpret_cast<float*>(frameBytes);
    float* offsetPosition = reinterpret_cast<float*>(frameBytes + 0x14);
    float* orientation = reinterpret_cast<float*>(frameBytes + 0x18);

    switch (CSF_AT(int, self, 0x8))
    {
    case 0:
    {
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, character, 0)[0x38 / 4])(character, nullptr, reinterpret_cast<int>(orientation));

        void* provider = reinterpret_cast<void*>(self->field_0004);
        void* referencedObject =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, provider, 0)[0x10 / 4])(provider, nullptr);
        reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, referencedObject, 0)[0x4C / 4])(
                referencedObject, nullptr, &g_ZeroVectorX);

        provider = reinterpret_cast<void*>(self->field_0004);
        referencedObject =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, provider, 0)[0x10 / 4])(provider, nullptr);
        reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, referencedObject, 0)[0x50 / 4])(
                referencedObject, nullptr, orientation);

        character->vSlot34(reinterpret_cast<uint32_t*>(position));
        if (primaryCharacter && (primaryCharacter->field_02e0 & 0x10000) != 0)
        {
            if (static_cast<uint8_t>(
                    CharActionFsm_IsFlag10000SetAndByteNonZero(
                        primaryCharacter, nullptr)) != 0)
            {
                if (GetFloat3D4(primaryCharacter, nullptr)
                        - g_fReturnedConstant_0082E030 < position[1])
                {
                    position[1] = static_cast<float>(
                        GetFloat3D4(primaryCharacter, nullptr)
                        - g_fReturnedConstant_0082E030);
                }
            }
            else
            {
                position[1] = static_cast<float>(
                    GetFloat3D4(primaryCharacter, nullptr)
                    + g_fReturnedConstant_0082E030);
            }
        }

        provider = reinterpret_cast<void*>(self->field_0004);
        referencedObject =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, provider, 0)[0x10 / 4])(provider, nullptr);
        reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, referencedObject, 0)[0x14 / 4])(
                referencedObject, nullptr, position);

        float elapsedTime;
        std::memcpy(&elapsedTime, &timeBits, sizeof(elapsedTime));
        CharacterModel_UpdateHeadMotion(self, nullptr, elapsedTime);

        // The supplied binding is void, but the callee's body returns a byte.
        using HeadTransformCall =
            char (__fastcall*)(void*, void*, uint8_t*, float*, float);
        return reinterpret_cast<HeadTransformCall>(
            CharacterModel_UpdateHeadTransform)(
                self, nullptr, reinterpret_cast<uint8_t*>(primaryCharacter),
                orientation, elapsedTime);
    }

    case 1:
    {
        void* provider = reinterpret_cast<void*>(self->field_0004);
        void* referencedObject =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, provider, 0)[0x10 / 4])(provider, nullptr);
        void** referencedVtable = CSF_AT(void**, referencedObject, 0);

        int returnedOrientation =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, character, 0)[0x38 / 4])(character, nullptr, reinterpret_cast<int>(frameBytes + 0xC));
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            referencedVtable[0x50 / 4])(
                referencedObject, nullptr, returnedOrientation);

        character->vSlot34(reinterpret_cast<uint32_t*>(offsetPosition));
        float* providerPosition = reinterpret_cast<float*>(
            MpSpawnPoints_GetProviderPosition(self, nullptr));
        const float offsetScale = CSF_AT(float, self, 0x80);
        provider = reinterpret_cast<void*>(self->field_0004);

        const float offsetX = offsetScale * providerPosition[0];
        const float offsetY = offsetScale * providerPosition[1];
        const float offsetZ = offsetScale * providerPosition[2];
        offsetPosition[0] -= offsetX;
        offsetPosition[1] -= offsetY;
        offsetPosition[2] -= offsetZ;

        referencedObject =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, provider, 0)[0x10 / 4])(provider, nullptr);
        return reinterpret_cast<char (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, referencedObject, 0)[0x14 / 4])(
                referencedObject, nullptr, offsetPosition);
    }

    case 8:
        if (isPrimaryCharacter)
        {
            return static_cast<char>(CharacterModel_UpdateModeEight(
                self, nullptr, reinterpret_cast<int>(primaryCharacter), timeBits));
        }
        break;
    }

    return isPrimaryCharacter;
}

HOOK(0x004F8150, C_0085ec18_UpdateCharacterTransform, "auto");
