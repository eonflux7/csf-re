#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetMemberValueD4, 0x00476090, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static unsigned int __fastcall CharActionFsm_CheckPendingActions(
    void* self, void* /*edx*/, uint8_t* hasPendingAction,
    uint8_t* currentActionPending)
{
    *hasPendingAction = 0;
    *currentActionPending = 0;

    const uint32_t begin = CSF_AT(uint32_t, self, 0x18);
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(CSF_AT(uint32_t, self, 0x1C) - begin) >> 2)
        : 0;

    uint32_t index = 0;
    for (; index < count; ++index)
    {
        C_0085e7b8* action =
            CSF_AT(C_0085e7b8**, self, 0x18)[index];

        if (!action)
            continue;

        if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                action->vtbl[0x9C / 4])(action, nullptr))
            continue;

        if (GetMemberValueD4(action, nullptr) != 2 &&
            GetMemberValueD4(action, nullptr) != 1)
            continue;

        if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                action->vtbl[0x64 / 4])(action, nullptr))
        {
            *hasPendingAction = 1;
        }

        int sharesProvider = 0;
        C_0085e7b8* current = CSF_AT(C_0085e7b8*, self, 0x9C);
        if (current &&
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                current->vtbl[0x4C / 4])(current, nullptr) == 10)
        {
            void* currentProvider = GetCachedObjectProvider(
                CSF_AT(C_0085e7b8*, self, 0x9C), nullptr);
            sharesProvider =
                GetCachedObjectProvider(action, nullptr) == currentProvider;
        }

        if (!reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                action->vtbl[0x54 / 4])(action, nullptr, sharesProvider))
        {
            *hasPendingAction = 1;

            C_0085e7b8* selected =
                reinterpret_cast<C_0085e7b8*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr);

            if (action == selected)
                *currentActionPending = 1;
            else
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    action->vtbl[0x70 / 4])(action, nullptr, 1);
        }
    }

    return index;
}

HOOK(0x00485CA0, CharActionFsm_CheckPendingActions, "auto");
