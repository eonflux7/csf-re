#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// CMenuGameOptions::sub_53B310 (0x0053B310)
// On the "confirm" key (13 / Enter) forwards the input to two virtual handlers:
//   vtable[0x5c] (slot 23) — first handler, return value discarded
//   vtable[0x48] (slot 18) — second handler, its return value is the result
// The vtable pointer is re-read from the object before each dispatch because the
// callee may change it.
static int __fastcall CMenuGameOptions_ActivateOnConfirmKey(CMenuGameOptions* self, void* /*edx*/, uint32_t* a2)
{
    if (*a2 == 13)
    {
        ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, self, 0)[0x5c / 4])(self, nullptr, a2);
        return ((int(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, self, 0)[0x48 / 4])(self, nullptr, a2);
    }
    return 0;
}

HOOK(0x53B310, CMenuGameOptions_ActivateOnConfirmKey, "auto");
