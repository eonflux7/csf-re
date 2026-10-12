#include "globals.h"
#include "hook.h"

// Original callee: __thiscall-style with one stack argument (ret 4).
CSF_FUNC(int, __fastcall, SaveGame_NameCheckResult, 0x00696AF0, (void* self, void* edx, char* a2));

// Keeps the original's vtable reference to 0x00696AF0 (no side effects).
static void SaveGame_TouchVtableRef()
{
    (void)CSF_VTBL(0x00696AF0);
}

// Returns TRUE when the callee's result is nonzero, FALSE otherwise
// (the neg/sbb/neg sequence normalizes eax to 0 or 1).
static int __fastcall SaveGame_ValidateName(void* self, void* /*edx*/, char* a2)
{
    const int checkResult = SaveGame_NameCheckResult(self, nullptr, a2);
    return checkResult != 0 ? 1 : 0;
}

HOOK(0x00696BB0, SaveGame_ValidateName, "auto");
