#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original function at 0x00530630 (size 35 bytes)
// Prototype: int __thiscall sub_530630(int *this)
// It calls GetGlobalValue_521BB0 (0x521BB0), then calls Menu_5225D0 on the result,
// then if the member pointer at offset 0x1E8 is non-null, it tail-jumps
// to a virtual method at vtable slot 0x50/4 = 20 of that object.
// Otherwise it returns 0 (eax is the null member pointer).

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x521BB0, ());
CSF_FUNC(int, __fastcall, Menu_5225D0, 0x5225D0, (void* self, void* edx));

static hook::Tail __fastcall CMenuMpServerInfo_InvokeMemberAction(void* self, const uint32_t* args) {
    (void)args;
    void* global = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    Menu_5225D0(global, nullptr);

    void* member = CSF_AT(void*, self, 0x1e8);
    if (member == nullptr) {
        return hook::tail_ret(0);
    }
    return hook::tail_jump(member, (*reinterpret_cast<void***>(member))[0x50 / 4]);
}

HOOK_TAIL(0x530630, CMenuMpServerInfo_InvokeMemberAction, "auto");
