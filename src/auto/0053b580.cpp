#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// 0x0053B580 (27 bytes, ret 4)
// If the stored button/id (field_0028) is non-zero and matches the id at a2+4,
// forward to the virtual handler at vtable slot 0x58; otherwise return the stored id.
static int __fastcall CMenuGameOptions_ActivateButtonIfMatching(
    CMenuGameOptions* self, void* /*edx*/, int* a2)
{
    int result = self->field_0028;
    if (result)
    {
        if (result == CSF_AT(int, a2, 0x4))
        {
            void** vtbl = CSF_AT(void**, self, 0x0);
            return ((int(__fastcall*)(CMenuGameOptions*, void*))vtbl[0x58 / 4])(self, nullptr);
        }
    }
    return result;
}

HOOK(0x0053B580, CMenuGameOptions_ActivateButtonIfMatching, "auto");
