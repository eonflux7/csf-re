#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0048E6F0: __thiscall, no stack arguments (ret 0).
// Calls virtual slot 10 (0x28) on `self`, then calls virtual slot 15 (0x3C)
// with (result, 0) and returns its int result.
static int __fastcall GetRelatedObjectAndClearFlag(void* self, void* /*edx*/)
{
    void* related = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x28 / 4])(self, nullptr);

    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, related, 0)[0x3C / 4])(related, nullptr, 0);
}

HOOK(0x48E6F0, GetRelatedObjectAndClearFlag, "auto");
