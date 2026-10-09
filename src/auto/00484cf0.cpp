#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// sub_4844C0: thiscall, no stack args, ret 0.
CSF_FUNC(void*, __fastcall, sub_4844C0, 0x004844C0, (void* self, void* edx));
// sub_498F50: thiscall, two stack args, ret 8.
CSF_FUNC(int, __fastcall, sub_498F50, 0x00498F50, (void* self, void* edx, int a2, char a3));
// sub_49AF10: thiscall, one stack arg, ret 4.
CSF_FUNC(void, __fastcall, sub_49AF10, 0x0049AF10, (void* self, void* edx, char a3));
// ObjectProvider_SetUpdateMode: thiscall, one stack arg, ret 4.
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));
// GetGlobalValue_4FBDE0: cdecl, no args, ret 0.
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
// GetFirstElement: thiscall, no stack args.
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
// GetCachedObjectProvider: thiscall, no stack args.
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Resets the weapon FSM base state, clears the owner's flag, runs the
// 0x1000 owner call, then disables update mode on the first cached object provider.
static int __fastcall CCharActionFsm_ResetAndDisableObjectUpdates(CCharActionFsm* self, void* /*edx*/)
{
    sub_4844C0(self, nullptr);

    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008));
    sub_49AF10(owner, nullptr, 0);

    owner = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008));
    sub_498F50(owner, nullptr, 4096, 0);

    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);
    GetCachedObjectProvider(reinterpret_cast<void*>(static_cast<uintptr_t>(firstElement)), nullptr);

    return ObjectProvider_SetUpdateMode(
        reinterpret_cast<void*>(static_cast<uintptr_t>(firstElement)), nullptr, 0);
}

HOOK(0x00484CF0, CCharActionFsm_ResetAndDisableObjectUpdates, "auto");
