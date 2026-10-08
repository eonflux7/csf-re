// Commandos: Strike Force (Pyro Studios) - CMenuMpOnline
// Reimplementation of sub_55F8F0 (0x0055F8F0, 33 bytes).
//
// Original behaviour:
//   globalState[0xCC] = -1;          // invalidate a flag in the global state block
//   sub_55F720(this);                // refresh this online-menu object
//   jmp [(*this)[0x54/4]]            // tail-dispatch to virtual slot 21
//
// The final indirect jump forwards registers and stack untouched, so it is
// reproduced as a naked thunk (never a call).

#include <cstdint>
#include <cstddef>

#include "types/auto_classes.h"
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

// ---------------------------------------------------------------------------
// Functions referenced by the original (bound by address)
// ---------------------------------------------------------------------------

// 0x0050D190 - returns the address of the global game-state block (as int).
CSF_FUNC(int, __cdecl, GetGlobalState, 0x50D190, ());

// 0x0055F720 - refresh this menu object's contents; __thiscall, no stack args.
CSF_FUNC(int, __fastcall, CMenuMpOnline_RefreshContents, 0x55F720, (void* self, void* edx));

// ---------------------------------------------------------------------------
// Tail dispatch to virtual slot 21 (byte offset 0x54).
//
// Original, right before the jump:
//   mov eax, dword ptr [esi]     ; eax = *this (vtable)
//   mov ecx, esi                 ; ecx = this
//   pop esi
//   jmp dword ptr [eax + 0x54]
//
// ecx already holds `this` on entry to the thunk, so only the vtable load is
// needed; the jump itself is preserved as a jump.
// ---------------------------------------------------------------------------
static __declspec(naked) int __fastcall CMenuMpOnline_DispatchVirtual21(void* /*self*/, void* /*edx*/)
{
    __asm
    {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 54h]
    }
}

// ---------------------------------------------------------------------------
// 0x0055F8F0 - CMenuMpOnline::ResetStateAndRedraw
// ---------------------------------------------------------------------------
static int __fastcall CMenuMpOnline_ResetStateAndRedraw(void** self, void* /*edx*/)
{
    // *(std::uint32_t *)(GetGlobalState() + 0xCC) = -1;
    CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState())), 0xCC) = -1;

    CMenuMpOnline_RefreshContents(self, nullptr);

    return CMenuMpOnline_DispatchVirtual21(self, nullptr);
}

HOOK(0x55F8F0, CMenuMpOnline_ResetStateAndRedraw, "auto");
