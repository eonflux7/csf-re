#include <cstdint>

#include "globals.h"
#include "hook.h"

// 0x536AB0  int __thiscall sub_536AB0(std::uint8_t *this /*ecx*/, char a2 /*stack*/)
// The callee pops 4 bytes (ret 4), i.e. the single forwarded stack argument.
CSF_FUNC(int, __fastcall, Menu_Sub536AB0, 0x536AB0, (uint8_t* self, void* /*edx*/, char a2));

// 0x0050D7F0 (12 bytes, module menu_system)
//
//   push ebp
//   mov  ebp, esp
//   mov  ecx, [ecx+0x18]   ; this = *(this + 0x18)
//   pop  ebp
//   jmp  0x536AB0          ; tail call, stack argument forwarded unchanged
//
// Takes its object in ecx plus one stack argument; retargets ecx onto the
// pointer stored at +0x18 and tail-jumps to 0x536AB0 with the same stack
// argument.  The tail-called function's `ret 4` performs the cleanup of that
// argument, so this function must also pop 4 bytes.
static int __fastcall Menus_ForwardCallToChildAt18(void* self, void* /*edx*/, char a2)
{
    uint8_t* child = CSF_AT(uint8_t*, self, 0x18);
    return Menu_Sub536AB0(child, nullptr, a2);
}

HOOK(0x50D7F0, Menus_ForwardCallToChildAt18, "auto");
