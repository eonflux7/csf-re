#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

// int GetGlobalState() as bound by the packet: __cdecl, no arguments, pops 0 bytes.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// int __fastcall Menus_ForwardCallToChildAt18(void* self, void* edx, char a2)
CSF_FUNC(int, __fastcall, Menus_ForwardCallToChildAt18, 0x0050D7F0, (void* self, void* /*edx*/, char a2));

// 0x0042DF80 (23 bytes, ret 4): fetch the global state and forward it to the child at offset 0x18.
// The caller's argument is pushed for GetGlobalState but that callee takes no arguments and
// ignores it; the original's epilogue discards the leftover stack slot, so it is not used here.
static int __stdcall MenuManager_SetGameplayControllerState(int a1)
{
    (void)a1;
    int globalState = GetGlobalState_50D190();
    return Menus_ForwardCallToChildAt18(reinterpret_cast<void*>(static_cast<std::uintptr_t>(globalState)),
                                        nullptr, 0);
}

HOOK(0x42DF80, MenuManager_SetGameplayControllerState, "auto");
