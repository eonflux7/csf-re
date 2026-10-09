#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t*, __fastcall, AudioSoundManager_InitializeRequest, 0x0040FE30, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(void, __fastcall, Object_SetEffectIntensity, 0x00420140, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, SceneSelection_CycleEligibleObject, 0x00451000, (unsigned int* self, void* , char backward));
CSF_FUNC(int, __fastcall, CharActionFsm_CommitPendingTransition, 0x00483450, (void* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_UpdateFlagsAndDispatch, 0x00484910, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_TryBeginTargetedAction, 0x00487610, (CCharActionFsm* self, void*));
CSF_FUNC(char, __fastcall, CCharActionFsm_TryBeginActionState, 0x00487650, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, Character_SetSpecialMode, 0x00492760, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Character_DispatchPendingEffect, 0x00496B30, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, C_00833ee8_BeginRequestedMode, 0x00499590, (C_00833ee8* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834050_BeginTransitionMode, 0x0049AE30, (C_00834050* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* , void* request, float volume));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall C_00834050_DispatchRequestedFlags(
    C_00834050* self, void* /*edx*/)
{
    alignas(4) uint8_t soundRequest[0x14]{};
    alignas(4) uint8_t effectRequest[0x2C]{};

    if (CSF_AT(uint32_t, self, 0x1C4) != CSF_AT(uint32_t, self, 0x3EC) ||
        ((CSF_AT(uint32_t, self, 0x2E8) ^
          CSF_AT(uint32_t, self, 0x2E0)) & 7) != 0)
    {
        const uint32_t requested = CSF_AT(uint32_t, self, 0x2E8);
        const uint32_t retained = CSF_AT(uint32_t, self, 0x2E0) & 0xFFFFFFFC;
        CSF_AT(uint32_t, self, 0x2E0) = retained;

        if ((requested & 1) != 0)
        {
            CSF_AT(uint32_t, self, 0x2E0) = retained | 1;
            if ((requested & 2) != 0)
            {
                CSF_AT(uint32_t, self, 0x2E0) = retained | 3;
                Character_SetSpecialMode(self, nullptr, 1);
                ((int(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, self, 0)[0xB4 / 4])(self, nullptr, 0);
            }
            else
            {
                Character_SetSpecialMode(self, nullptr, 0);
                const int enabled =
                    (CSF_AT(uint8_t, self, 0x2E8) & 4) != 0;
                ((int(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, self, 0)[0xB4 / 4])(self, nullptr, enabled);
            }
        }
        else
        {
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, self, 0)[0xB4 / 4])(
                    self, nullptr, (requested & 4) != 0);
        }

        const uint32_t state = CSF_AT(uint32_t, self, 0x1C4);
        if (state == CSF_AT(uint32_t, self, 0x3EC))
        {
            if (state == 1 && (CSF_AT(uint32_t, self, 0x2E0) & 4) == 0)
            {
                CSF_AT(uint32_t, self, 0x2E0) |= 4;
            }
            else if (state == 0)
            {
                const uint32_t flags = CSF_AT(uint32_t, self, 0x2E0);
                if ((flags & 4) != 0)
                    CSF_AT(uint32_t, self, 0x2E0) = flags & 0xFFFFFFFB;
            }
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0030), 0x20) = 0;
        }
    }

    uint32_t allowed = CSF_AT(uint32_t, self, 0x2E4);
    uint32_t pending = CSF_AT(uint32_t, self, 0x2E8) & allowed;
    CSF_AT(uint32_t, self, 0x2E8) = pending;

    if ((pending & 8) != 0 && (CSF_AT(uint32_t, self, 0x2E0) & 8) == 0)
    {
        if ((CSF_AT(uint32_t, self, 0x2E0) & 4) != 0)
        {
            CSF_AT(uint32_t, self, 0x2E4) = allowed & 0xFFFFFFF3;
        }
        else
        {
            const bool alternateClock =
                CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) != 0;
            void* clock = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            const float remaining = CSF_AT(float, self, 0x2F0) -
                (alternateClock ? CSF_AT(float, clock, 0x1894)
                                : CSF_AT(float, clock, 0x1868));
            CSF_AT(float, effectRequest, 0x28) = remaining;

            if (!(remaining < g_fZero))
            {
                CSF_AT(uint32_t, self, 0x2E0) |= 8;
                const bool alternateSettings =
                    CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) != 0;
                void* settings = reinterpret_cast<void*>(GetGlobalValue_753A30());
                const float intensity =
                    alternateSettings ? CSF_AT(float, settings, 0x1B0)
                                      : CSF_AT(float, settings, 0x184);
                CSF_AT(float, effectRequest, 0x24) = intensity;
                Object_SetEffectIntensity(
                    reinterpret_cast<void*>(self->field_0030), nullptr, intensity);

                void* sceneServices = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                const int resource = SceneLoader_GetResourceById(
                    static_cast<uint8_t*>(sceneServices) + 0x22A4, nullptr, 11);
                if (resource != -1)
                {
                    AudioSoundManager_InitializeRequest(
                        soundRequest, nullptr, 4, resource, 4, 0, -1);
                    S_00521df0_PlaySoundRequest(
                        reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0()),
                        nullptr, soundRequest, -1.0f);
                }
                CSF_AT(float, self, 0x2F0) = remaining;
            }
            else
            {
                CSF_AT(uint32_t, self, 0x2E4) &= ~8u;
            }
        }
    }
    else if ((pending & 8) == 0)
    {
        const uint32_t flags = CSF_AT(uint32_t, self, 0x2E0);
        if ((flags & 8) != 0)
            CSF_AT(uint32_t, self, 0x2E0) = flags & 0xFFFFFFF7;
    }

    allowed = CSF_AT(uint32_t, self, 0x2E4);
    pending = CSF_AT(uint32_t, self, 0x2E8) & allowed;
    CSF_AT(uint32_t, self, 0x2E8) = pending;
    if ((pending & 0x10) != 0 &&
        (CSF_AT(uint32_t, self, 0x2E0) & 0x10) == 0)
    {
        if ((CSF_AT(uint32_t, self, 0x2E0) & 0x2000) != 0)
        {
            Character_DispatchPendingEffect(self, nullptr);
        }
        else
        {
            for (uint32_t index = 0; index < self->field_00a4; ++index)
            {
                void* child = reinterpret_cast<void**>(self->field_009c)[index];
                ((void(__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, child, 0)[0x58 / 4])(
                        child, nullptr, reinterpret_cast<uint8_t*>(self) + 0x31C);
            }
        }
    }
    else if (CSF_AT(uint8_t, self, 0x3DD) != 0)
    {
        CSF_AT(uint32_t, self, 0x2E4) = allowed & 0xFFFFFFEF;
        const uint32_t count = self->field_00a4;
        CSF_AT(uint8_t, self, 0x3DD) = 0;
        CSF_AT(uint32_t, effectRequest, 0x00) = 2;
        CSF_AT(uint32_t, effectRequest, 0x04) = 0;
        CSF_AT(void**, effectRequest, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(int32_t, effectRequest, 0x0C) = -1;
        CSF_AT(int32_t, effectRequest, 0x10) = -1;
        std::memset(effectRequest + 0x14, 0, 12);
        CSF_AT(uint8_t, effectRequest, 0x20) = 1;
        CSF_AT(uint8_t, effectRequest, 0x21) = 0;
        CSF_AT(uint8_t, effectRequest, 0x22) = 1;
        if (count != 0)
        {
            uint32_t index = 0;
            do
            {
                void* child = reinterpret_cast<void**>(self->field_009c)[index];
                ((void(__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, child, 0)[0x58 / 4])(child, nullptr, effectRequest);
                ++index;
            } while (index < self->field_00a4);
        }
    }

    pending = CSF_AT(uint32_t, self, 0x2E4) & CSF_AT(uint32_t, self, 0x2E8);
    const uint32_t toggle20 = pending & 0x20;
    CSF_AT(uint32_t, self, 0x2E8) = pending;
    if (toggle20 != 0 && (CSF_AT(uint8_t, self, 0x2E0) & 0x20) == 0)
    {
        for (uint32_t index = 0; index < self->field_00a4; ++index)
        {
            void* child = reinterpret_cast<void**>(self->field_009c)[index];
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x60 / 4])(child, nullptr);
        }
    }
    else if (toggle20 == 0 && (CSF_AT(uint8_t, self, 0x2E0) & 0x20) != 0)
    {
        for (uint32_t index = 0; index < self->field_00a4; ++index)
        {
            void* child = reinterpret_cast<void**>(self->field_009c)[index];
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x64 / 4])(child, nullptr);
        }
    }

    pending = CSF_AT(uint32_t, self, 0x2E4) & CSF_AT(uint32_t, self, 0x2E8);
    const uint32_t toggle40 = pending & 0x40;
    CSF_AT(uint32_t, self, 0x2E8) = pending;
    if (toggle40 != 0 && (CSF_AT(uint8_t, self, 0x2E0) & 0x40) == 0)
    {
        for (uint32_t index = 0; index < self->field_00a4; ++index)
        {
            void* child = reinterpret_cast<void**>(self->field_009c)[index];
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x74 / 4])(child, nullptr);
        }
    }
    else if (toggle40 == 0 && (CSF_AT(uint8_t, self, 0x2E0) & 0x40) != 0)
    {
        for (uint32_t index = 0; index < self->field_00a4; ++index)
        {
            void* child = reinterpret_cast<void**>(self->field_009c)[index];
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x78 / 4])(child, nullptr);
        }
    }

    pending = CSF_AT(uint32_t, self, 0x2E4) & CSF_AT(uint32_t, self, 0x2E8);
    CSF_AT(uint32_t, self, 0x2E8) = pending;
    if ((pending & 0x80) != 0 && CSF_AT(int8_t, self, 0x2E0) >= 0)
    {
        for (uint32_t index = 0; index < self->field_00a4; ++index)
        {
            void* child = reinterpret_cast<void**>(self->field_009c)[index];
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x70 / 4])(child, nullptr);
        }
    }

    allowed = CSF_AT(uint32_t, self, 0x2E4);
    pending = allowed & CSF_AT(uint32_t, self, 0x2E8);
    CSF_AT(uint32_t, self, 0x2E8) = pending;
    uint32_t begin = CSF_AT(uint32_t, self, 0x300);
    if (begin != 0 &&
        (static_cast<int32_t>(CSF_AT(uint32_t, self, 0x304) - begin) >> 3) != 0)
    {
        if ((pending & 0x200) != 0)
        {
            for (;;)
            {
                begin = CSF_AT(uint32_t, self, 0x300);
                if (begin == 0 ||
                    (static_cast<int32_t>(CSF_AT(uint32_t, self, 0x304) - begin) >> 3) == 0)
                    break;
                for (uint32_t index = 0; index < self->field_00a4; ++index)
                {
                    void* child = reinterpret_cast<void**>(self->field_009c)[index];
                    void* entry = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x304) - 8);
                    ((void(__fastcall*)(void*, void*, void*))
                        CSF_AT(void**, child, 0)[0x68 / 4])(child, nullptr, entry);
                }
                begin = CSF_AT(uint32_t, self, 0x300);
                if (begin != 0 &&
                    (static_cast<int32_t>(CSF_AT(uint32_t, self, 0x304) - begin) >> 3) != 0)
                    CSF_AT(uint32_t, self, 0x304) -= 8;
            }
        }
        else
        {
            CSF_AT(uint32_t, self, 0x2E4) = allowed & 0xFFFFFDFF;
            if (CSF_AT(uint32_t, self, 0x300) != 0)
                Mem_FreeDebug(CSF_AT(void*, self, 0x300));
            CSF_AT(uint32_t, self, 0x300) = 0;
            CSF_AT(uint32_t, self, 0x304) = 0;
            CSF_AT(uint32_t, self, 0x308) = 0;
        }
    }

    allowed = CSF_AT(uint32_t, self, 0x2E4);
    pending = allowed & CSF_AT(uint32_t, self, 0x2E8);
    CSF_AT(uint32_t, self, 0x2E8) = pending;
    begin = CSF_AT(uint32_t, self, 0x310);
    if (begin != 0 &&
        (static_cast<int32_t>(CSF_AT(uint32_t, self, 0x314) - begin) >> 3) != 0)
    {
        if ((pending & 0x400) != 0)
        {
            for (;;)
            {
                begin = CSF_AT(uint32_t, self, 0x310);
                if (begin == 0 ||
                    (static_cast<int32_t>(CSF_AT(uint32_t, self, 0x314) - begin) >> 3) == 0)
                    break;
                for (uint32_t index = 0; index < self->field_00a4; ++index)
                {
                    void* child = reinterpret_cast<void**>(self->field_009c)[index];
                    void* entry = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x314) - 8);
                    ((void(__fastcall*)(void*, void*, void*))
                        CSF_AT(void**, child, 0)[0x6C / 4])(child, nullptr, entry);
                }
                begin = CSF_AT(uint32_t, self, 0x310);
                if (begin != 0 &&
                    (static_cast<int32_t>(CSF_AT(uint32_t, self, 0x314) - begin) >> 3) != 0)
                    CSF_AT(uint32_t, self, 0x314) -= 8;
            }
        }
        else
        {
            CSF_AT(uint32_t, self, 0x2E4) = allowed & 0xFFFFFBFF;
            if (CSF_AT(uint32_t, self, 0x310) != 0)
                Mem_FreeDebug(CSF_AT(void*, self, 0x310));
            CSF_AT(uint32_t, self, 0x310) = 0;
            CSF_AT(uint32_t, self, 0x314) = 0;
            CSF_AT(uint32_t, self, 0x318) = 0;
        }
    }

    pending = CSF_AT(uint32_t, self, 0x2E4) & CSF_AT(uint32_t, self, 0x2E8);
    CSF_AT(uint32_t, self, 0x2E8) = pending;
    if ((pending & 0x100) != 0 && (CSF_AT(uint32_t, self, 0x2E0) & 0x100) == 0)
    {
        for (uint32_t index = 0; index < self->field_00a4; ++index)
        {
            void* child = reinterpret_cast<void**>(self->field_009c)[index];
            ((void(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, child, 0)[0x5C / 4])(
                    child, nullptr, reinterpret_cast<uint8_t*>(self) + 0x340);
        }
    }

    pending = CSF_AT(uint32_t, self, 0x2E4) & CSF_AT(uint32_t, self, 0x2E8);
    CSF_AT(uint32_t, self, 0x2E8) = pending;
    if ((pending & 0x20000) != 0 &&
        (CSF_AT(uint32_t, self, 0x2E0) & 0x20000) == 0)
        CCharActionFsm_TryBeginTargetedAction(
            reinterpret_cast<CCharActionFsm*>(self->field_0044), nullptr);

    pending = CSF_AT(uint32_t, self, 0x2E4) & CSF_AT(uint32_t, self, 0x2E8);
    CSF_AT(uint32_t, self, 0x2E8) = pending;
    if ((pending & 0x40000) != 0 &&
        (CSF_AT(uint32_t, self, 0x2E0) & 0x40000) == 0)
    {
        CCharActionFsm_TryBeginActionState(
            reinterpret_cast<CCharActionFsm*>(self->field_0044), nullptr);
    }
    else if ((pending & 0x40000) == 0 &&
             (CSF_AT(uint32_t, self, 0x2E0) & 0x40000) != 0)
    {
        CCharActionFsm_UpdateFlagsAndDispatch(
            reinterpret_cast<CCharActionFsm*>(self->field_0044), nullptr);
    }

    pending = CSF_AT(uint32_t, self, 0x2E4) & CSF_AT(uint32_t, self, 0x2E8);
    CSF_AT(uint32_t, self, 0x2E8) = pending;
    if ((pending & 0x2000) != 0 &&
        (CSF_AT(uint32_t, self, 0x2E0) & 0x2000) == 0)
        C_00833ee8_BeginRequestedMode(reinterpret_cast<C_00833ee8*>(self), nullptr);

    pending = CSF_AT(uint32_t, self, 0x2E4) & CSF_AT(uint32_t, self, 0x2E8);
    CSF_AT(uint32_t, self, 0x2E8) = pending;
    if ((pending & 0x10000) != 0 &&
        (CSF_AT(uint32_t, self, 0x2E0) & 0x10000) == 0)
    {
        C_00834050_BeginTransitionMode(self, nullptr);
    }
    else if ((pending & 0x10000) == 0)
    {
        const uint32_t flags = CSF_AT(uint32_t, self, 0x2E0);
        if ((flags & 0x10000) != 0)
        {
            void* actionFsm = reinterpret_cast<void*>(self->field_0044);
            CSF_AT(uint32_t, self, 0x2E0) = flags & 0xFFFEFFFF;
            CharActionFsm_CommitPendingTransition(actionFsm, nullptr);
            self->vSlot6C(1);
        }
    }

    pending = CSF_AT(uint32_t, self, 0x2E4) & CSF_AT(uint32_t, self, 0x2E8);
    CSF_AT(uint32_t, self, 0x2E8) = pending;
    if ((pending & 0x80000) != 0)
    {
        if ((CSF_AT(uint32_t, self, 0x2E0) & 0x80000) != 0)
        {
            const uint32_t result = CSF_AT(uint32_t, self, 0x2E4);
            CSF_AT(uint32_t, self, 0x2E8) &= result;
            return static_cast<int>(result);
        }
        SceneSelection_CycleEligibleObject(
            reinterpret_cast<unsigned int*>(g_pCurrentScene), nullptr, 0);
    }

    const uint32_t result =
        CSF_AT(uint32_t, self, 0x2E4) & CSF_AT(uint32_t, self, 0x2E8);
    CSF_AT(uint32_t, self, 0x2E8) = result;
    return static_cast<int>(result);
}

HOOK(0x0049B2C0, C_00834050_DispatchRequestedFlags, "auto");
