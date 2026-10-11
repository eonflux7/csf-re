#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void, __fastcall, RefreshState, 0x0048FA00, (void* self, void* edx));

static void __fastcall C_00833908_ForwardValueAndRefresh(
    C_00833908* self, void* /*edx*/, int value)
{
    void* related = reinterpret_cast<void*>(self->vGetField016c());
    if (related)
    {
        void* recipient =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, related, 0)[0x50 / 4])(related, nullptr);
        if (recipient)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, recipient, 0)[4 / 4])(recipient, nullptr, value);
        }
    }
    RefreshState(self, nullptr);
}

HOOK(0x0048FE50, C_00833908_ForwardValueAndRefresh, "auto");
