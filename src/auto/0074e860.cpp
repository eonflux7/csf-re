#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, NetGameDdl_FinishFieldProcessing, 0x0065E710, (int a1, int a2));

// Original: __stdcall (int a1, int a2), ret 8. ecx is forwarded as an input to the
// finisher, so it is taken as a hidden first argument here.
static int __fastcall NetControlDdl_SweepFlagsThenFinish(void* ecxArg, void* /*edx*/, int a1, int a2)
{
    void* obj = reinterpret_cast<void*>(a1);

    if (a2 == 0 || a2 == 1)
    {
        // Slot 0x1C / 4 = 7, called with 1, 2, 3 in order.
        // The vtable is re-read before each call, as the original does.
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[7])(obj, nullptr, 1);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[7])(obj, nullptr, 2);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[7])(obj, nullptr, 3);
    }

    NetGameDdl_FinishFieldProcessing(a1, a2);
    return 0;
}

HOOK(0x0074E860, NetControlDdl_SweepFlagsThenFinish, "auto");
