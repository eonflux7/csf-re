#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the weapon-state slot array (calling each slot object's deleting
// virtual destructor with flag 1), clears the table fields, and leaves the
// object with the base-state vtables.
static int __fastcall CCharWeaponFsm_ReleaseAndReset(CCharWeaponFsm* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00833228);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083320C);

    for (int i = 0; ; ++i)
    {
        uint32_t slotsAddr = self->field_0018;
        if (!slotsAddr || i >= (static_cast<int>(self->field_001c - slotsAddr) >> 2))
            break;

        void* slotsBase = reinterpret_cast<void*>(static_cast<uintptr_t>(slotsAddr));
        uint32_t element = CSF_AT(uint32_t, slotsBase, 4 * i);
        if (element)
        {
            void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(element));
            // Slot 0 of the element's vtable: deleting destructor, flag = 1.
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0])(obj, nullptr, 1);

            // Reload the table base after the call; the callee may have changed it.
            void* slotsNow = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0018));
            CSF_AT(uint32_t, slotsNow, 4 * i) = 0;
        }
    }

    self->field_0040 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0082D410);
    self->field_0030 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0082D410);

    if (self->field_0018)
        Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0018)));

    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0082D410);
    self->vtbl = CSF_VTBL(0x0082D410);
    return 0;
}

HOOK(0x00481790, CCharWeaponFsm_ReleaseAndReset, "auto");
