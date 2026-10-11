#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CScene_RemoveObjectFromCategories, 0x004528D0, (CScene* self, void* , int handleAddress, int categoryMask));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __fastcall, Actor_StopActiveState, 0x00493270, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e248_GetActivationThreshold, 0x004E1120, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083e248_CanStartRecovery, 0x004E2D70, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083e248_CompleteActivation, 0x004E36B0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_WeaponPlacementTraceDepth, 0x0082E11C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void C_0083e248_ExitActiveState(C_0083e248* self)
{
    Actor_StopActiveState(reinterpret_cast<void*>(self->field_000c), nullptr);

    void* flagsObject =
        CSF_AT(void*, reinterpret_cast<void*>(self->field_000c), 0x34);
    CSF_AT(uint32_t, flagsObject, 0x18) |= 2u;

    void* member = reinterpret_cast<uint8_t*>(self->field_000c) + 4;
    alignas(8) uint8_t inactiveBytes[1];
    inactiveBytes[0] = 0;
    ((void(__fastcall*)(void*, void*, char*))
        CSF_AT(void**, member, 0)[0x50 / 4])(
            member, nullptr, reinterpret_cast<char*>(inactiveBytes));
}

static void C_0083e248_RemoveOwner(C_0083e248* self)
{
    const int handleAddress = static_cast<int>(self->field_000c + 0x1c);
    self->field_0018 = 9;
    CScene_RemoveObjectSlot(
        reinterpret_cast<CScene*>(g_pCurrentScene), nullptr, handleAddress);
}

static void C_0083e248_RemoveOwnerFromCategories(C_0083e248* self)
{
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_000c), 0x94) & 1)
    {
        CScene_RemoveObjectFromCategories(
            reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
            static_cast<int>(self->field_000c + 0x1c), 1);
    }
}

static void C_0083e248_StartFade(C_0083e248* self)
{
    if (self->field_0018 == 3)
        C_0083e248_ExitActiveState(self);

    self->field_0018 = 8;
    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* owner = reinterpret_cast<void*>(self->field_000c);
    const float duration = CSF_AT(float, settings, 6180);
    self->field_0020 = duration;

    void* component =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
    void* fade =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, component, 0)[0x50 / 4])(component, nullptr);
    ((void(__fastcall*)(void*, void*, float))
        CSF_AT(void**, fade, 0)[0x14 / 4])(fade, nullptr, duration);
}

static void __fastcall C_0083e248_UpdateState(
    C_0083e248* self, void* /*edx*/, float deltaTime)
{
    const uint32_t state = self->field_0018;
    switch (state)
    {
    case 1u:
    case 2u:
    {
        const int threshold =
            C_0083e248_GetActivationThreshold(self, nullptr);
        if (static_cast<int32_t>(self->field_0008) >= threshold)
        {
            C_0083e248_CompleteActivation(self, nullptr);
        }
        else
        {
            const float remaining = self->field_001c - deltaTime;
            self->field_001c = remaining;
            if (remaining <= g_fZero)
                self->vSlot9C();
        }
        break;
    }

    case 4u:
    {
        const float remaining = self->field_0020 - deltaTime;
        self->field_0020 = remaining;
        self->field_0024 = deltaTime + self->field_0024;
        if (remaining <= g_fZero)
        {
            if (state == 3)
                C_0083e248_ExitActiveState(self);
            C_0083e248_RemoveOwner(self);
        }
        break;
    }

    case 5u:
    {
        void* owner = reinterpret_cast<void*>(self->field_000c);
        self->field_0024 = deltaTime + self->field_0024;
        void* component =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
        void* motion =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, component, 0)[0x78 / 4])(component, nullptr);

        if (!((bool(__fastcall*)(void*, void*))
                CSF_AT(void**, motion, 0)[0x34 / 4])(motion, nullptr))
        {
            self->field_0020 = 0.0f;
        }

        alignas(8) uint8_t velocityBytes[0x13];
        void* velocity =
            ((void*(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, motion, 0)[0x50 / 4])(
                    motion, nullptr, velocityBytes);
        const float z = CSF_AT(float, velocity, 8);
        const float y = CSF_AT(float, velocity, 4);
        const float x = CSF_AT(float, velocity, 0);
        if (x * x + y * y + z * z > g_WeaponPlacementTraceDepth)
            self->field_0024 = self->field_0024 - (deltaTime + deltaTime);

        if (self->field_0020 <= g_fZero ||
            self->field_0024 > g_MessageBoxAnimationScale)
        {
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, motion, 0)[0x24 / 4])(motion, nullptr);
            if (self->field_0018 == 3)
                C_0083e248_ExitActiveState(self);
            self->field_0018 = 7;
            C_0083e248_RemoveOwnerFromCategories(self);
            self->field_0024 = 0.0f;
        }

        if (CSF_AT(uint8_t, g_pCurrentScene, 425))
        {
            owner = reinterpret_cast<void*>(self->field_000c);
            component =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
            const uint8_t sceneFlag =
                CSF_AT(uint8_t, CSF_AT(void*, g_pCurrentScene, 192), 9);
            const float height = CSF_AT(float, component, 120);
            void* child =
                CSF_AT(void*, reinterpret_cast<void*>(self->field_000c), 68);
            const int type =
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, child, 0)[0xf4 / 4])(child, nullptr);

            if (height < g_fZero && !sceneFlag && type == 9)
            {
                ((void(__fastcall*)(void*, void*))
                    CSF_AT(void**, motion, 0)[0x24 / 4])(motion, nullptr);
                if (self->field_0018 == 3)
                    C_0083e248_ExitActiveState(self);
                C_0083e248_RemoveOwner(self);
            }
        }
        break;
    }

    case 6u:
    {
        const float remaining = self->field_0020 - deltaTime;
        self->field_0020 = remaining;
        self->field_0024 = deltaTime + self->field_0024;
        if (remaining <= g_fZero)
        {
            if (state == 3)
                C_0083e248_ExitActiveState(self);
            C_0083e248_RemoveOwner(self);
        }

        if (self->field_002c && self->field_0024 >= self->field_0028)
        {
            if (C_0083e248_CanStartRecovery(self, nullptr))
            {
                if (self->field_0018 == 3)
                    C_0083e248_ExitActiveState(self);

                void* owner = reinterpret_cast<void*>(self->field_000c);
                self->field_0018 = 5;
                ((void(__fastcall*)(void*, void*, int, int, int, int))
                    CSF_AT(void**, owner, 0)[0x8c / 4])(
                        owner, nullptr, 10, 0, -1, 0);
                C_0083e248_RemoveOwnerFromCategories(self);
                self->field_0020 = 3.4028235e38f;
                self->field_0024 = 0.0f;
            }
            self->field_002c = 0;
        }
        break;
    }

    case 7u:
        if (static_cast<uint8_t>(IsField1BCNonNegative(
                reinterpret_cast<void*>(self->field_000c), nullptr)))
        {
            C_0083e248_StartFade(self);
        }
        self->field_0024 = deltaTime + self->field_0024;
        {
            void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            if (self->field_0024 >= CSF_AT(float, settings, 6176) ||
                CSF_AT(uint8_t, g_pCurrentScene, 425))
            {
                C_0083e248_StartFade(self);
            }
        }
        break;

    case 8u:
    {
        const float remaining = self->field_0020 - deltaTime;
        self->field_0020 = remaining;
        if (remaining <= g_fZero)
        {
            if (state == 3)
                C_0083e248_ExitActiveState(self);
            C_0083e248_RemoveOwner(self);
        }
        break;
    }

    default:
        break;
    }

    if (!self->field_002d)
        self->field_002d = 1;

    if (self->field_003c)
    {
        const float remaining = self->field_0030 - deltaTime;
        self->field_0030 = remaining;
        if (remaining < g_fZero)
        {
            self->field_0030 = 5.0f;
            self->field_003c = 0;
        }
    }
}

HOOK(0x004E3710, C_0083e248_UpdateState, "auto");
