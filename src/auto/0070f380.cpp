#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00863ba8_RunActivationSequence(
    C_00863ba8* self, void* /*edx*/)
{
    void* associatedObject = reinterpret_cast<void*>(self->field_0014);
    if (associatedObject)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, associatedObject, 0)[5])(
                associatedObject, nullptr);
    }

    reinterpret_cast<void(__fastcall*)(C_00863ba8*, void*)>(
        self->vtbl[14])(self, nullptr);

    reinterpret_cast<void(__fastcall*)(C_00863ba8*, void*)>(
        self->vtbl[8])(self, nullptr);

    void** finalVtable = self->vtbl;
    self->field_0030 = 1;
    self->field_0031 = 0;

    return reinterpret_cast<int(__fastcall*)(C_00863ba8*, void*, int)>(
        finalVtable[23])(self, nullptr, 0);
}

HOOK(0x0070F380, C_00863ba8_RunActivationSequence, "auto");
