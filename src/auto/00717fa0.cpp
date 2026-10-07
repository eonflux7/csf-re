#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_SetShown(
    CGuiControl* self,
    void* /*edx*/,
    char shown,
    char secondaryShownFlag,
    char synchronizeRelatedControl)
{
    self->m_bShown = static_cast<std::uint8_t>(shown);
    self->field_0015 = static_cast<std::uint8_t>(secondaryShownFlag);

    if (synchronizeRelatedControl
        && ((void* (__fastcall*)(void*, void*))
                self->vtbl[0x10C / 4])(self, nullptr)
        && !((std::uint8_t (__fastcall*)(void*, void*))
                self->vtbl[0x6C / 4])(self, nullptr)
        && ((std::uint8_t (__fastcall*)(void*, void*))
                self->vtbl[0xBC / 4])(self, nullptr))
    {
        void* relatedControlForQuery =
            ((void* (__fastcall*)(void*, void*))
                self->vtbl[0x10C / 4])(self, nullptr);

        void* relatedControlForUpdate =
            ((void* (__fastcall*)(void*, void*))
                self->vtbl[0x10C / 4])(self, nullptr);

        void** queryVtable =
            CSF_AT(void**, relatedControlForQuery, 0);
        void** updateVtable =
            CSF_AT(void**, relatedControlForUpdate, 0);

        int relatedState =
            ((int (__fastcall*)(void*, void*))
                queryVtable[0x15C / 4])(relatedControlForQuery, nullptr);

        ((void (__fastcall*)(void*, void*, int))
            updateVtable[0x158 / 4])(
                relatedControlForUpdate, nullptr, relatedState);
    }

    return ((int (__fastcall*)(void*, void*))
        self->vtbl[0x214 / 4])(self, nullptr);
}

HOOK(0x00717FA0, CGuiControl_SetShown, "auto");
