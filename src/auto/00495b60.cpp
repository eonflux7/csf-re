#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CPlayerCharacter_SetPostureParameter, 0x00492500, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, S_004f5420_SetProviderState, 0x004F5610, (S_004f5420* self, void* , int state));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static float CPlayerCharacter_DecodeFloat(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static int __fastcall CPlayerCharacter_SetPosture(
    CPlayerCharacter* self, void* /*edx*/, int posture)
{
    if (self->field_03ec == static_cast<uint32_t>(posture))
        return posture;

    if (posture == 0 || posture == 1 || posture == 2)
    {
        alignas(8) uint8_t targetBytes[0xC];
        float* target = reinterpret_cast<float*>(targetBytes);

        if (posture == 0)
        {
            auto* settings = reinterpret_cast<C_00833908*>(self->field_002c);
            CSF_AT(uint32_t, targetBytes, 0) = settings->field_0160;
            CSF_AT(uint32_t, targetBytes, 4) = settings->field_0164;
            CSF_AT(uint32_t, targetBytes, 8) = settings->field_0168;
        }
        else
        {
            auto* tuning = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
            const int offset = posture == 1 ? 2624 : 2636;
            const float scaleX = CSF_AT(float, tuning + offset, 0);
            const float scaleY = CSF_AT(float, tuning + offset, 4);
            const float scaleZ = CSF_AT(float, tuning + offset, 8);
            auto* settings = reinterpret_cast<C_00833908*>(self->field_002c);

            target[0] = scaleX * CPlayerCharacter_DecodeFloat(settings->field_0160);
            target[1] = scaleY * CPlayerCharacter_DecodeFloat(settings->field_0164);
            target[2] = scaleZ * CPlayerCharacter_DecodeFloat(settings->field_0168);
        }

        const bool isCurrentCharacter =
            GetSceneMenuContext(g_pCurrentScene, nullptr) ==
            reinterpret_cast<uint32_t>(self);

        if (isCurrentCharacter)
        {
            if (posture != 2)
            {
                const int providerState = posture == 1 ? 5 : 4;
                const int collection = GetGlobalValue_4FBDE0();
                const int firstElement =
                    GetFirstElement(reinterpret_cast<void*>(collection), nullptr);
                auto* provider = static_cast<S_004f5420*>(
                    GetCachedObjectProvider(
                        reinterpret_cast<void*>(firstElement), nullptr));
                S_004f5420_SetProviderState(provider, nullptr, providerState);
            }

            void* linkedObject;
            if (posture == 2)
            {
                const float currentParameter = self->field_01d4;
                auto* settings = reinterpret_cast<C_00833908*>(self->field_002c);
                std::memcpy(self->pad_01a4, &currentParameter, sizeof(currentParameter));
                linkedObject = reinterpret_cast<void*>(self->field_004c);
                self->field_0184 = 1;
                self->field_0188 = 0;
                self->field_03ec = 2;
                CSF_AT(uint32_t, self, 0x1A8) = settings->field_00ec;
            }
            else if (posture == 1)
            {
                auto* settings = reinterpret_cast<C_00833908*>(self->field_002c);
                self->field_0184 = 1;
                self->field_03ec = 1;
                const float currentParameter = self->field_01d4;
                self->field_0188 = 0;
                std::memcpy(self->pad_01a4, &currentParameter, sizeof(currentParameter));
                const uint32_t targetParameter = settings->field_00e8;
                linkedObject = reinterpret_cast<void*>(self->field_004c);
                CSF_AT(uint32_t, self, 0x1A8) = targetParameter;
            }
            else
            {
                const float currentParameter = self->field_01d4;
                auto* settings = reinterpret_cast<C_00833908*>(self->field_002c);
                self->field_0184 = 1;
                self->field_0188 = 0;
                self->field_03ec = 0;
                std::memcpy(self->pad_01a4, &currentParameter, sizeof(currentParameter));
                const uint32_t targetParameter = settings->field_00e4;
                linkedObject = reinterpret_cast<void*>(self->field_004c);
                CSF_AT(uint32_t, self, 0x1A8) = targetParameter;
            }

            if (linkedObject)
            {
                void* object =
                    C_0082dd78_DispatchReturnedObject(linkedObject, nullptr);
                auto* currentVector =
                    reinterpret_cast<uint32_t*(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, object, 0)[0x30 / 4])(object, nullptr);

                self->field_018c = currentVector[0];
                self->field_0190 = currentVector[1];
                self->field_0194 = currentVector[2];
                self->field_0198 = CSF_AT(uint32_t, targetBytes, 0);
                self->field_019c = CSF_AT(uint32_t, targetBytes, 4);
                self->field_01a0 = CSF_AT(uint32_t, targetBytes, 8);
            }
        }
        else
        {
            float parameter;
            if (posture == 0)
            {
                auto* settings = reinterpret_cast<C_00833908*>(self->field_002c);
                self->field_01c4 = 0;
                self->field_03ec = 0;
                parameter = CPlayerCharacter_DecodeFloat(settings->field_00e4);
            }
            else
            {
                self->field_01c4 = static_cast<uint32_t>(posture);
                self->field_03ec = static_cast<uint32_t>(posture);
                auto* settings = reinterpret_cast<C_00833908*>(self->field_002c);
                parameter = CPlayerCharacter_DecodeFloat(
                    posture == 1 ? settings->field_00e8 : settings->field_00ec);
            }

            CPlayerCharacter_SetPostureParameter(self, nullptr, parameter);
            void* linkedObject = reinterpret_cast<void*>(self->field_004c);
            if (linkedObject)
            {
                void* object =
                    C_0082dd78_DispatchReturnedObject(linkedObject, nullptr);
                reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
                    CSF_AT(void**, object, 0)[0x2C / 4])(object, nullptr, target);
            }
        }
    }

    void* embeddedObject = &self->field_0004;
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, embeddedObject, 0)[0x30 / 4])(embeddedObject, nullptr);
}

HOOK(0x00495B60, CPlayerCharacter_SetPosture, "auto");
