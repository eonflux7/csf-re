#include <cstdint>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CScene_RemoveObjectFromCategories, 0x004528D0, (CScene* self, void* , int handleAddress, int categoryMask));
CSF_FUNC(int, __fastcall, C_0082f9f8_TryAcquire, 0x0048E5D0, (void* self, void* edx, C_0083e248* requester, int flags, int priority));
CSF_FUNC(int, __fastcall, ReleaseCurrentInteraction, 0x00493270, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e248_SetState, 0x004E30B0, (C_0083e248* self, void* edx, int state));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void C_0083e248_LeaveInteractionState(C_0083e248* self)
{
    if (self->field_0018 == 3)
    {
        ReleaseCurrentInteraction(
            reinterpret_cast<void*>(self->field_000c), nullptr);

        void* actor = reinterpret_cast<void*>(self->field_000c);
        void* interaction = CSF_AT(void*, actor, 0x34);
        CSF_AT(std::uint32_t, interaction, 0x18) |= 2u;

        void* component =
            reinterpret_cast<std::uint8_t*>(self->field_000c) + 4;
        alignas(8) std::uint8_t enabledBytes[0x1];
        enabledBytes[0] = 0;
        reinterpret_cast<void(__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, component, 0)[0x50 / 4])(
                component, nullptr, reinterpret_cast<char*>(enabledBytes));
    }
}

static int __fastcall C_0083e248_UpdateInteractionState(
    C_0083e248* self, void* /*edx*/)
{
    const bool canAcquire =
        reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            self->vtbl[0xB4 / 4])(self, nullptr);

    if (!canAcquire && self->field_002c)
        return C_0083e248_SetState(self, nullptr, 6);

    void* actor = reinterpret_cast<void*>(self->field_000c);
    void* controller =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, actor, 0)[0x28 / 4])(actor, nullptr);
    void* target =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x78 / 4])(controller, nullptr);

    const bool targetAvailable =
        target &&
        reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, target, 0)[0x28 / 4])(target, nullptr);

    if (!targetAvailable)
    {
        if (!canAcquire)
            return C_0083e248_SetState(self, nullptr, 4);

        C_0083e248_LeaveInteractionState(self);

        actor = reinterpret_cast<void*>(self->field_000c);
        self->field_0018 = 4;
        if (CSF_AT(std::uint8_t, actor, 0x94) & 1)
        {
            CScene_RemoveObjectFromCategories(
                reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                static_cast<int>(self->field_000c + 0x1C), 1);
        }

        actor = reinterpret_cast<void*>(self->field_000c);
        self->field_0020 = FLT_MAX;
        self->field_0024 = 0.0f;
        C_0082f9f8_TryAcquire(actor, nullptr, self, 0, 100);

        actor = reinterpret_cast<void*>(self->field_000c);
        return reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int, int)>(
            CSF_AT(void**, actor, 0)[0x8C / 4])(
                actor, nullptr, 10, 0, -1, 0);
    }

    C_0083e248_LeaveInteractionState(self);

    actor = reinterpret_cast<void*>(self->field_000c);
    self->field_0018 = 5;
    reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, actor, 0)[0x8C / 4])(
            actor, nullptr, 10, 0, -1, 0);

    actor = reinterpret_cast<void*>(self->field_000c);
    int result = static_cast<int>(reinterpret_cast<std::uintptr_t>(actor));
    if (CSF_AT(std::uint8_t, actor, 0x94) & 1)
    {
        result = CScene_RemoveObjectFromCategories(
            reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
            static_cast<int>(self->field_000c + 0x1C), 1);
    }

    self->field_0020 = FLT_MAX;
    self->field_0024 = 0.0f;
    return result;
}

HOOK(0x004E3400, C_0083e248_UpdateInteractionState, "auto");
