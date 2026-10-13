#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __stdcall, NetGameDdl_FinishFieldProcessing, 0x0065E710, (int a1, int a2));

// Calls the object's vtable slot 7 (byte offset 0x1c) with the flag argument 1,
// then forwards to NetGameDdl_FinishFieldProcessing.
static void __stdcall NetProyectilDdl_ProcessFieldFlagAndFinish(int a1, int a2)
{
    void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    if (a2 != 0) {
        if (a2 == 1) {
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[7])(obj, nullptr, 1);
        }
        NetGameDdl_FinishFieldProcessing(a1, a2);
    } else {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[7])(obj, nullptr, 1);
        NetGameDdl_FinishFieldProcessing(a1, 0);
    }
}

HOOK(0x007475A0, NetProyectilDdl_ProcessFieldFlagAndFinish, "auto");
