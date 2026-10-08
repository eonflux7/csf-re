#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// 0x0054A090: helper invoked with the notification record. In the original the
// record pointer is both pushed on the stack (as the float argument a3) and
// kept in edi (the a2 register parameter); here it is forwarded on the stack.
CSF_FUNC(void, __fastcall, CMenuVideoOptions_HandleNotifyParam, 0x54A090,
         (void* self, void* edx, int* a2));

static int __fastcall CMenuVideoOptions_OnNotify(CMenuVideoOptions* self, void* /*edx*/, int* a2)
{
    // Only notification code 13 is handled; the original returns the caller's
    // eax unchanged otherwise.
    if (*a2 != 13)
        return 0;

    CMenuVideoOptions_HandleNotifyParam(self, nullptr, a2);

    // Forward the notification to the base handler: vtable slot 0x48 / 4 = 18.
    return ((int (__fastcall*)(void*, void*, int*))CSF_AT(void**, self, 0)[18])(self, nullptr, a2);
}

HOOK(0x54A280, CMenuVideoOptions_OnNotify, "auto");
