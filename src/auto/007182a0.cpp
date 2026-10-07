#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_ApplyResolvedControlValue(
    CGuiControl* self, void* /*edx*/)
{
    if (((void* (__fastcall*)(void*, void*))self->vtbl[0x10C / 4])(
            self, nullptr))
    {
        void* source =
            ((void* (__fastcall*)(void*, void*))self->vtbl[0x10C / 4])(
                self, nullptr);
        void* target =
            ((void* (__fastcall*)(void*, void*))self->vtbl[0x10C / 4])(
                self, nullptr);

        void** sourceVtable = CSF_AT(void**, source, 0);
        void** targetVtable = CSF_AT(void**, target, 0);

        int value =
            ((int (__fastcall*)(void*, void*, int))sourceVtable[0x148 / 4])(
                source, nullptr, 1);
        return
            ((int (__fastcall*)(void*, void*, int))targetVtable[0x14C / 4])(
                target, nullptr, value);
    }

    void** controlVtable = self->vtbl;
    int value =
        ((int (__fastcall*)(void*, void*, int))controlVtable[0x148 / 4])(
            self, nullptr, 1);
    return
        ((int (__fastcall*)(void*, void*, int))controlVtable[0x14C / 4])(
            self, nullptr, value);
}

HOOK(0x007182A0, CGuiControl_ApplyResolvedControlValue, "auto");
