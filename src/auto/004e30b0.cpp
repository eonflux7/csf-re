#include <cstdint>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CScene_RemoveObjectFromCategories, 0x004528D0, (CScene* self, void* , int handleAddress, int categoryMask));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(int, __fastcall, C_0082f9f8_TryAcquire, 0x0048E5D0, (void* self, void* edx, C_0083e248* requester, int flags, int priority));
CSF_FUNC(int, __fastcall, ReleaseCurrentInteraction, 0x00493270, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(double, __fastcall, SceneMissionSetup_EvaluateAnimationTime, 0x0050DD80, (void* self, void* edx, float a2));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_AnimationTimeOffset, 0x0083E7A4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Each helper represents one fixed virtual operation, not a runtime slot lookup.
static int PlayInteractionAnimation(void* object, int animation)
{
    return ((int(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, object, 0)[0x8C / 4])(
            object, nullptr, animation, 0, -1, 0);
}

static int __fastcall C_0083e248_SetInteractionState(
    C_0083e248* self, void* /*edx*/, int state)
{
    if (self->field_0018 == 3)
    {
        ReleaseCurrentInteraction(
            reinterpret_cast<void*>(self->field_000c), nullptr);

        void* owner = reinterpret_cast<void*>(self->field_000c);
        void* flagsObject = CSF_AT(void*, owner, 0x34);
        CSF_AT(uint32_t, flagsObject, 0x18) |= 2u;

        void* control = reinterpret_cast<uint8_t*>(self->field_000c) + 4;
        alignas(8) uint8_t flagBytes[1];
        flagBytes[0] = 0;
        ((void(__fastcall*)(void*, void*, char*))
            CSF_AT(void**, control, 0)[0x50 / 4])(
                control, nullptr, reinterpret_cast<char*>(flagBytes));
    }

    self->field_0018 = static_cast<uint32_t>(state);
    int result = static_cast<int>(static_cast<uint32_t>(state) - 1u);

    switch (state)
    {
    case 1:
        return PlayInteractionAnimation(
            reinterpret_cast<void*>(self->field_000c), 48);

    case 2:
    {
        void* globals = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        self->field_001c = static_cast<float>(
            CSF_AT(uint32_t, globals, 0x182C));

        void* owner = reinterpret_cast<void*>(self->field_000c);
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, owner, 0)[0x54 / 4])(owner, nullptr);

        owner = reinterpret_cast<void*>(self->field_000c);
        void* flagsObject = CSF_AT(void*, owner, 0x34);
        CSF_AT(uint32_t, flagsObject, 0x18) &= ~2u;

        void* control = reinterpret_cast<uint8_t*>(self->field_000c) + 4;
        reinterpret_cast<uint8_t*>(&state)[3] = 1;
        ((void(__fastcall*)(void*, void*, char*))
            CSF_AT(void**, control, 0)[0x50 / 4])(
                control, nullptr, reinterpret_cast<char*>(&state) + 3);

        return PlayInteractionAnimation(
            reinterpret_cast<void*>(self->field_000c), 49);
    }

    case 3:
        return PlayInteractionAnimation(
            reinterpret_cast<void*>(self->field_000c), 50);

    case 4:
    case 6:
    {
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_000c), 0x94) & 1)
        {
            CScene_RemoveObjectFromCategories(
                reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                static_cast<int>(self->field_000c + 0x1C), 1);
        }

        void* owner = reinterpret_cast<void*>(self->field_000c);
        self->field_0020 = FLT_MAX;
        self->field_0024 = 0.0f;
        C_0082f9f8_TryAcquire(owner, nullptr, self, 0, 100);

        result = PlayInteractionAnimation(
            reinterpret_cast<void*>(self->field_000c), 10);
        if (state == 4)
            return result;

        owner = reinterpret_cast<void*>(self->field_000c);
        void* character = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
        void* values = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, character, 0)[0x68 / 4])(character, nullptr);
        void* value = ((void*(__fastcall*)(void*, void*, int))
            CSF_AT(void**, values, 0)[0x20 / 4])(values, nullptr, 0);

        float scale = value
            ? static_cast<float>(C_Unknown_GetValue(value, nullptr))
            : 0.0f;

        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        double animationTime = SceneMissionSetup_EvaluateAnimationTime(
            globalState, nullptr, 0.30000001f);
        self->field_0028 = static_cast<float>(
            (animationTime + g_AnimationTimeOffset) * scale);

        // The double-returning callee supplies no defined integer result.
        return 0;
    }

    case 5:
        result = PlayInteractionAnimation(
            reinterpret_cast<void*>(self->field_000c), 10);
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_000c), 0x94) & 1)
        {
            result = CScene_RemoveObjectFromCategories(
                reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                static_cast<int>(self->field_000c + 0x1C), 1);
        }
        self->field_0020 = FLT_MAX;
        self->field_0024 = 0.0f;
        return result;

    case 7:
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_000c), 0x94) & 1)
        {
            result = CScene_RemoveObjectFromCategories(
                reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                static_cast<int>(self->field_000c + 0x1C), 1);
        }
        self->field_0024 = 0.0f;
        return result;

    case 8:
    {
        void* globals = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        void* owner = reinterpret_cast<void*>(self->field_000c);
        float duration = CSF_AT(float, globals, 0x1824);
        self->field_0020 = duration;

        void* character = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
        void* controller = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, character, 0)[0x50 / 4])(character, nullptr);
        return ((int(__fastcall*)(void*, void*, float))
            CSF_AT(void**, controller, 0)[0x14 / 4])(
                controller, nullptr, duration);
    }

    case 9:
        return CScene_RemoveObjectSlot(
            reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
            static_cast<int>(self->field_000c + 0x1C));

    default:
        return result;
    }
}

HOOK(0x004E30B0, C_0083e248_SetInteractionState, "auto");
