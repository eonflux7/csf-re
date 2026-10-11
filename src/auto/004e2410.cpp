#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, ForwardToAttachedObject, 0x0048E5F0,
         (void* self, void* edx, C_0083e5e8* controller));
CSF_FUNC(int, __fastcall, RefreshAttachedSpawnState, 0x00493270,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e5e8_GetStateThreshold, 0x004E1120,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083e5e8_ProcessAllowedSpawnChange, 0x004E1F10,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083e5e8_ProcessThresholdReached, 0x004E2070,
         (void* self, void* edx));
CSF_FUNC(bool, __fastcall, MpSpawnPoints_IsSpawnChangeAllowed, 0x004F5740, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static bool IsSpawnChangeCurrentlyAllowed()
{
    int collection = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(collection), nullptr);
    void* provider = GetCachedObjectProvider(
        reinterpret_cast<void*>(firstElement), nullptr);
    return MpSpawnPoints_IsSpawnChangeAllowed(provider, nullptr);
}

static void __fastcall C_0083e5e8_UpdateSpawnState(
    C_0083e5e8* self, void* /*edx*/, float elapsedTime)
{
    switch (CSF_AT(uint32_t, self, 0x18))
    {
    case 1u:
        if (!CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        {
            int threshold = C_0083e5e8_GetStateThreshold(self, nullptr);
            if (static_cast<int32_t>(self->field_0008) > threshold)
                C_0083e5e8_ProcessThresholdReached(self, nullptr);
        }
        break;

    case 2u:
        if (CSF_AT(uint8_t, self, 0x1C))
        {
            if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_000c), 0x228)
                || IsSpawnChangeCurrentlyAllowed())
            {
                reinterpret_cast<void (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, self, 0)[0xD0 / 4])(self, nullptr);
            }
        }
        break;

    case 3u:
        if (!CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_000c), 0x228)
            && IsSpawnChangeCurrentlyAllowed())
        {
            void* attachedObject = reinterpret_cast<void*>(self->field_000c);
            ForwardToAttachedObject(attachedObject, nullptr, self);
            RefreshAttachedSpawnState(attachedObject, nullptr);
            CSF_AT(uint32_t, self, 0x18) = 0;
        }
        break;

    case 4u:
        if (!CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_000c), 0x228)
            && IsSpawnChangeCurrentlyAllowed())
        {
            C_0083e5e8_ProcessAllowedSpawnChange(self, nullptr);
        }
        break;

    default:
        break;
    }

    float remainingTime = CSF_AT(float, self, 0x2C) - elapsedTime;
    CSF_AT(float, self, 0x2C) = remainingTime;
    if (remainingTime <= g_fZero)
    {
        CSF_AT(float, self, 0x2C) = 1.0f;
        CSF_AT(int32_t, self, 0x24) = -1;
        CSF_AT(int32_t, self, 0x28) = -1;
    }
}

HOOK(0x004E2410, C_0083e5e8_UpdateSpawnState, "auto");
