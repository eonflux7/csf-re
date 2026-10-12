#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuManager_SetInnerState, 0x0050E930, (int* self, void* /*edx*/, char a2));

// Resets the instancing scene/result state of S_00536500, then forwards the
// caller's stack arguments to the object's vtable slot 0xB8.
static hook::Tail __fastcall S_00536500_ResetInstancingState(void* self, const uint32_t* /*args*/)
{
    // Slot 0xB0 on this (vtable pointer is the first dword of the object).
    reinterpret_cast<void (__fastcall*)(void*, void*)>(CSF_AT(void**, self, 0)[0xB0 / 4])(self, nullptr);

    // Child object at +0x138: if present, call slot 0xA8 with 1, then slot 0x58.
    void* child = reinterpret_cast<void*>(reinterpret_cast<S_00536500*>(self)->field_0138);
    if (child) {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, child, 0)[0xA8 / 4])(child, nullptr, 1);
        // The original reloads ecx from [esi+0x138] after the first call.
        child = reinterpret_cast<void*>(reinterpret_cast<S_00536500*>(self)->field_0138);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(CSF_AT(void**, child, 0)[0x58 / 4])(child, nullptr);
    }

    reinterpret_cast<S_00536500*>(self)->field_0141 = 0;
    reinterpret_cast<S_00536500*>(self)->field_0134 = 0;

    int globalState = GetGlobalState_50D190();
    MenuManager_SetInnerState(reinterpret_cast<int*>(globalState), nullptr, 0);

    // Indirect tail jump: ecx = this, target = vtable[0xB8 / 4] read after the calls above.
    void* obj = self;
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0xB8 / 4]);
}

HOOK_TAIL(0x536500, S_00536500_ResetInstancingState, "interface");
