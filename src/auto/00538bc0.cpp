#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00538A50 — CMenuLoading helper taking a float value in the stack
// (ecx = this, one float stack argument, `ret 4`).
CSF_FUNC(int, __fastcall, CMenuLoading_SetProgress, 0x00538A50, (CMenuLoading * self, void* /*edx*/, float value));

// 0x00538BC0 — calls the same float setter twice with 0.0f and returns the
// result of the second invocation.
static int __fastcall CMenuLoading_ResetProgress(CMenuLoading* self, void* /*edx*/)
{
    CMenuLoading_SetProgress(self, nullptr, 0.0f);
    return CMenuLoading_SetProgress(self, nullptr, 0.0f);
}

HOOK(0x00538BC0, CMenuLoading_ResetProgress, "auto");
