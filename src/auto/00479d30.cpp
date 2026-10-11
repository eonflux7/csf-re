#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall WeaponFirearms_ResetAssociatedObject(void* self, void* /*edx*/)
{
    void* associatedObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[168 / 4])(self, nullptr);

    if (!associatedObject)
        return 0;

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, associatedObject, 0)[52 / 4])(
            associatedObject, nullptr, 0);

    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, associatedObject, 0)[60 / 4])(
            associatedObject, nullptr, 0);
}

HOOK(0x00479D30, WeaponFirearms_ResetAssociatedObject, "auto");
