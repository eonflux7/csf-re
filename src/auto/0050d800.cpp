// Commandos: Strike Force — module menu_system (Menus/P_Common.fbs)
//
// Target 0x0050D800 (8 bytes) is a this-adjusting tail thunk:
//     mov ecx, dword ptr [ecx+0x18]   ; ecx = this->field_0018
//     jmp 0x536AF0                    ; jump into the middle of an existing routine
//
// 0x536AF0 is not a function boundary (the gate rejects binding it by address),
// so its body is reproduced inline here. That chunk:
//     push esi / mov esi, ecx / call GetGlobalState_50D190 (0x50D190)
//     if (globalState[0xD0] != 0)  -> tail-call 0x542840(inner->field_014C)
//     else                         -> return inner->byte_0x273

#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// int __cdecl sub_50D190() : pointer to the shared global game state block
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x50D190, (void));

// char __thiscall sub_542840(std::uint8_t *this) : state flag query on the sub-object
CSF_FUNC(char, __fastcall, Menus_QueryObjectStateFlag, 0x542840, (uint8_t* self, void* /*edx*/));

// char __thiscall sub_50D800(void* this)
static char __fastcall Menus_GetInnerObjectFlag(void* self, void* /*edx*/)
{
    // mov ecx, dword ptr [ecx+0x18]
    void* inner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x18));

    // --- inlined body of the 0x536AF0 chunk (not a bindable function start) ---

    // call 0x50D190 ; mov cl, byte ptr [eax+0xd0] ; test cl, cl ; je
    if (*reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalState_50D190()) + 0xD0))
    {
        // mov ecx, dword ptr [esi+0x14c] ; jmp 0x542840
        return Menus_QueryObjectStateFlag(
            reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, inner, 0x14C))),
            nullptr);
    }

    // mov al, byte ptr [esi+0x273]
    return static_cast<char>(CSF_AT(uint8_t, inner, 0x273));
}

HOOK(0x50D800, Menus_GetInnerObjectFlag, "auto");
