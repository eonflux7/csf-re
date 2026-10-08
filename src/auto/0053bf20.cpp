#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0053BCA0: handles listbox event id 13 for the profiles dialog.
// Original prototype: int __thiscall sub_53BCA0(char *this, int a2)  (ret 4)
CSF_FUNC(int, __fastcall, MenuProfiles_HandleEvent, 0x53BCA0,
         (CMenuProfiles* self, void* /*edx*/, int a2));

// Dispatches a profiles-listbox event (event id in *a2):
//   13 -> confirm command handler
//   14 -> virtual slot 21 on the object (vtable offset 0x54)
//   otherwise -> returns (eventId - 14)
static int __fastcall CMenuProfiles_HandleListEvent(CMenuProfiles* self, void* /*edx*/, int* a2)
{
    int eventId = *a2;

    if (eventId == 13)
        return MenuProfiles_HandleEvent(self, nullptr, reinterpret_cast<int>(a2));

    int result = eventId - 14;

    if (eventId == 14)
    {
        void** vtbl = CSF_AT(void**, self, 0);
        return ((int(__fastcall*)(void*, void*, int*))vtbl[0x54 / 4])(self, nullptr, a2);
    }

    return result;
}

HOOK(0x53BF20, CMenuProfiles_HandleListEvent, "auto");
