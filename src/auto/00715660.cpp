#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int (__cdecl* const Mem_FreeDebug)(void*) =
    reinterpret_cast<int (__cdecl*)(void*)>(0x00690E60);

static int __fastcall CGuiListBoxBase_ClearStorageAndSendEvent(
    CGuiListBoxBase* self, void* /*edx*/)
{
    void* storage = reinterpret_cast<void*>(self->field_01ac);
    if (storage)
        Mem_FreeDebug(storage);

    self->field_01ac = 0;
    self->field_01b0 = 0;
    self->field_01b4 = 0;

    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, self, 0)[0x138 / 4])(self, nullptr, 23);
}

HOOK(0x00715660, CGuiListBoxBase_ClearStorageAndSendEvent, "auto");
