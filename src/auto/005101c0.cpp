#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(std::uintptr_t, __fastcall, MenuManager_TransitionToGameOrIntro, 0x0050CE60, (float* self, void* , int));
CSF_FUNC(int, __fastcall, MenuSystem_UpdateFrame, 0x0050DAF0, (void* self, void*));
CSF_FUNC(int, __fastcall, MenuSystem_ResetTransitionState, 0x0050EAC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuSystem_HideActivePanels, 0x0050ED40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareGlobalState_50FF30, 0x0050FF30, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue_8B0574, 0x00518980, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(char, __fastcall, MenuSystem_ApplyStateFlag, 0x00712DF0, (void* self, void* edx, char a2));

CSF_GLOBAL(int, g_Value_8B0568, 0x008B0568);
CSF_GLOBAL(void*, g_pMenuManager, 0x008B0570);

// Typed view of a vtable slot (slot = byte offset / 4).
template <typename Fn>
static Fn SlotFn(void* obj, int slot)
{
    return reinterpret_cast<Fn>(CSF_AT(void**, obj, 0)[slot]);
}

static void* IntToPtr(int value)
{
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(value));
}

// GetGlobalInt() -> LookupObjectById() -> virtual call on the result.
static void CallSlotOfGlobalObject(int slot)
{
    void* obj = LookupObjectById(IntToPtr(GetGlobalInt()), nullptr);
    SlotFn<void(__fastcall*)(void*, void*)>(obj, slot)(obj, nullptr);
}

// Shared epilogue: done only when both status getters on the scene object return 0.
static char CMenuManager_IsSceneFinished(void* scene)
{
    const int blockedFlag = SlotFn<int(__fastcall*)(void*, void*)>(scene, 0x68 / 4)(scene, nullptr);
    if (blockedFlag != 0)
        return 0;
    const int pendingFlag = SlotFn<int(__fastcall*)(void*, void*)>(scene, 0x6C / 4)(scene, nullptr);
    return pendingFlag == 0 ? 1 : 0;
}

// Phase 0: start/intro transition (the jump target at 0x50FFB0, inlined).
static char CMenuManager_TickIdlePhase(CMenuManager* self)
{
    if (self->field_0070 == 0 && self->field_00d0 == 0)
        return 0;

    MenuSystem_HideActivePanels(self, nullptr);

    if (self->field_0070 != 0)
    {
        GetValue_8B0574();
        void* guiRoot = reinterpret_cast<void*>(self->m_pGuiRoot);
        SlotFn<void(__fastcall*)(void*, void*, uint32_t)>(guiRoot, 0x0C / 4)(
            guiRoot, nullptr, self->field_00a0);
        GetValue_8B0574();
        MenuSystem_ApplyStateFlag(reinterpret_cast<void*>(self->m_pGuiRoot), nullptr, 1);
        if (self->field_0070 != 0)
            GetValue_8B0574();
    }

    // Choose the entry pointer from the scene's list (count at +0x94, inline array at +0x80).
    void* scene = reinterpret_cast<void*>(self->field_0014);
    const uint32_t entryCount = CSF_AT(uint32_t, scene, 0x94);
    const uint32_t entryPtr = (entryCount < 0x10)
        ? static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(scene) + 0x80)
        : CSF_AT(uint32_t, scene, 0x80);

    void* fader = reinterpret_cast<void*>(self->m_pFader);
    SlotFn<void(__fastcall*)(void*, void*, uint32_t)>(fader, 0x04 / 4)(fader, nullptr, entryPtr);
    fader = reinterpret_cast<void*>(self->m_pFader);
    SlotFn<void(__fastcall*)(void*, void*, uint32_t)>(fader, 0x0C / 4)(
        fader, nullptr, self->field_00a0);

    MenuManager_TransitionToGameOrIntro(
        reinterpret_cast<float*>(self->field_006c), nullptr, static_cast<int>(self->field_00a0));
    MenuSystem_UpdateFrame(self, nullptr);

    if (self->field_0070 != 0)
    {
        void* gameValue = IntToPtr(GetGlobalValue_70DDE0());
        SlotFn<void(__fastcall*)(void*, void*, uint32_t)>(gameValue, 0x3C / 4)(
            gameValue, nullptr, self->field_00a0);

        if (g_Value_8B0568)
        {
            void* globalValue = IntToPtr(GetGlobalValue_8B0568());
            SlotFn<void(__fastcall*)(void*, void*)>(globalValue, 0x20 / 4)(globalValue, nullptr);
        }

        SceneLoader_PrepareGlobalState_50FF30(self, nullptr);

        const int remainingCount = static_cast<int>(self->field_010c);
        if (CSF_AT(uint8_t, g_pMenuManager, 0xD0) != 0 && remainingCount > 0)
        {
            self->field_010c = static_cast<uint32_t>(remainingCount - 1);
        }
        else
        {
            CallSlotOfGlobalObject(0x2C / 4);
        }

        if (g_Value_8B0568)
        {
            void* globalValue = IntToPtr(GetGlobalValue_8B0568());
            SlotFn<void(__fastcall*)(void*, void*)>(globalValue, 0x24 / 4)(globalValue, nullptr);
        }

        void* sceneObj = reinterpret_cast<void*>(self->field_0014);
        const bool sceneReady =
            SlotFn<unsigned char(__fastcall*)(void*, void*)>(sceneObj, 0x98 / 4)(sceneObj, nullptr) != 0
            && SlotFn<int(__fastcall*)(void*, void*)>(
                   reinterpret_cast<void*>(self->field_0014), 0x80 / 4)(
                   reinterpret_cast<void*>(self->field_0014), nullptr) != 0;

        if (self->field_00c8 != 0 || sceneReady)
        {
            CallSlotOfGlobalObject(0x34 / 4);
        }
    }

    return CMenuManager_IsSceneFinished(reinterpret_cast<void*>(self->field_0014));
}

// Phase 1: frame-counted fade step.
static char CMenuManager_TickFadePhase(CMenuManager* self)
{
    const uint32_t nextFrame = self->field_000c + 1;

    if (nextFrame < self->field_0008)
    {
        self->field_000c = nextFrame;
        MenuSystem_HideActivePanels(self, nullptr);
        MenuSystem_UpdateFrame(self, nullptr);
    }
    else
    {
        self->field_000c = 0;
        MenuSystem_HideActivePanels(self, nullptr);
        MenuSystem_ResetTransitionState(self, nullptr);
        MenuSystem_UpdateFrame(self, nullptr);
        SceneLoader_PrepareGlobalState_50FF30(self, nullptr);
        CallSlotOfGlobalObject(0x2C / 4);
    }

    return CMenuManager_IsSceneFinished(reinterpret_cast<void*>(self->field_0014));
}

static char __fastcall CMenuManager_TickTransitionState(CMenuManager* self, void* /*edx*/)
{
    const int phase = self->field_0004;
    if (phase == 0)
        return CMenuManager_TickIdlePhase(self);
    if (phase == 1)
        return CMenuManager_TickFadePhase(self);
    return 1;
}

HOOK(0x005101C0, CMenuManager_TickTransitionState, "auto");
