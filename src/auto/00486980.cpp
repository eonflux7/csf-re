#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_unknown_GetLinkedDword, 0x00476080, (void* self, void*));
CSF_FUNC(int, __fastcall, CharacterItem_GetIdentifier, 0x00479310, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CCharActionFsm_ResetEmptyItem, 0x004846D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_FinishState8, 0x004864D0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, Character_IsActionReady, 0x004950F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Character_SetActionFlag, 0x00498F50, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

static char __fastcall CCharActionFsm_PrepareStateChange(
    CCharActionFsm* self, void* /*edx*/, int nextState)
{
    if (self->vSlotD8())
    {
        void* linkedObject = reinterpret_cast<void*>(self->vSlotD8());
        if (C_unknown_GetLinkedDword(linkedObject, nullptr) == 2)
            CSF_AT(uint8_t, reinterpret_cast<void*>(self->vSlotD8()), 0x16) = 0;
    }

    if (self->field_0010 == 8)
        CCharActionFsm_FinishState8(self, nullptr);

    if (nextState != 5)
    {
        void* item = reinterpret_cast<void*>(self->field_009c);
        if (item)
        {
            int itemType =
                ((int(__fastcall*)(void*, void*))CSF_AT(void**, item, 0)[0x4C / 4])(
                    item, nullptr);

            if (itemType == 11)
            {
                int identifier = CharacterItem_GetIdentifier(
                    reinterpret_cast<void*>(self->field_009c), nullptr);
                void* currentItem = reinterpret_cast<void*>(self->vSlotEC(identifier));
                if (currentItem)
                {
                    if (!GetCurrentItemCount(currentItem, nullptr))
                        CCharActionFsm_ResetEmptyItem(self, nullptr);
                }
            }
        }
    }

    if (nextState != 16 && nextState != 5 && nextState != 15)
    {
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x2E4) &= ~0x10u;
        Character_SetActionFlag(
            reinterpret_cast<void*>(self->field_0008), nullptr, 16, 0);
    }

    char result = Character_IsActionReady(
        reinterpret_cast<void*>(self->field_0008), nullptr);
    if (!result)
    {
        void* character = reinterpret_cast<void*>(self->field_0008);
        return static_cast<char>(
            ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, character, 0)[0x6C / 4])(
                character, nullptr, 0));
    }
    return result;
}

HOOK(0x00486980, CCharActionFsm_PrepareStateChange, "auto");
