#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CharActionFsm_StartItemAction, 0x00479120,
         (C_008317c8* self, void* edx, C_00862490* item, uint32_t parameter));
CSF_FUNC(int, __fastcall, CharActionFsm_GetItemActionParameter, 0x00479310,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, CCharActionFsm_FinishItemAction, 0x004846D0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_GetCharacterToggleState, 0x00496DC0,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, CharActionFsm_SetCharacterToggleState, 0x004975E0,
         (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, CharActionFsm_SetCharacterFlag, 0x00498F50,
         (void* self, void* edx, int a2, char a3));

static void __fastcall CCharActionFsm_ProcessPendingItemAction(
    CCharActionFsm* self, void* /*edx*/)
{
    void* item = reinterpret_cast<void*>(self->field_00a4);
    if (!item)
        return;

    const int actionType =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, item, 0)[0x4C / 4])(item, nullptr);

    switch (actionType)
    {
    case 9:
    {
        void* character = reinterpret_cast<void*>(self->field_0008);
        const int state =
            CharActionFsm_GetCharacterToggleState(character, nullptr);
        CharActionFsm_SetCharacterToggleState(
            character, nullptr, static_cast<uint8_t>(state) == 0);
        return;
    }

    case 10:
    {
        if (self->field_009c)
        {
            CCharActionFsm_FinishItemAction(self, nullptr);
            return;
        }

        void* character = reinterpret_cast<void*>(self->field_0008);
        CSF_AT(uint32_t, character, 0x2E4) &= ~0x40u;
        CharActionFsm_SetCharacterFlag(
            reinterpret_cast<void*>(self->field_0008), nullptr, 64, 0);

        void* controller = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x94 / 4])(controller, nullptr);

        item = reinterpret_cast<void*>(self->field_00a4);
        self->field_009c = reinterpret_cast<uint32_t>(item);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, item, 0)[0x38 / 4])(item, nullptr, 1);
        return;
    }

    case 11:
    {
        if (self->field_009c)
        {
            CCharActionFsm_FinishItemAction(self, nullptr);
            return;
        }

        item = reinterpret_cast<void*>(self->field_00a4);
        self->field_009c = reinterpret_cast<uint32_t>(item);
        const int itemParameter =
            CharActionFsm_GetItemActionParameter(item, nullptr);
        const int actionParameter = self->vLookupEntryByIndexOrZero(itemParameter);
        C_00862490* activeItem =
            reinterpret_cast<C_00862490*>(self->field_009c);
        C_008317c8* controller =
            reinterpret_cast<C_008317c8*>(self->vGetIndexedEntryOrZero());
        CharActionFsm_StartItemAction(
            controller, nullptr, activeItem,
            static_cast<uint32_t>(actionParameter));
        return;
    }

    case 12:
    case 13:
    case 14:
    {
        if (self->field_009c)
        {
            CCharActionFsm_FinishItemAction(self, nullptr);
            return;
        }

        if (self->field_0024 != static_cast<uint32_t>(-1))
        {
            void* controller = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, controller, 0)[0x30 / 4])(
                    controller, nullptr, 0);
        }

        item = reinterpret_cast<void*>(self->field_00a4);
        self->field_009c = reinterpret_cast<uint32_t>(item);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, item, 0)[0x38 / 4])(item, nullptr, 0);
        return;
    }

    default:
        return;
    }
}

HOOK(0x004847C0, CCharActionFsm_ProcessPendingItemAction, "auto");
