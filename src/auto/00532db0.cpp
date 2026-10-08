#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// 0x00533010 -- real __thiscall handler; pops 4 stack bytes (its char argument).
CSF_FUNC(void*, __fastcall, CMenuEula_Destroy, 0x533010,
         (void* self, void* edx, char a2));

// 0x00532DB0 (8 bytes) -- compiler-generated adjustor thunk:
//   sub ecx, 4
//   jmp 0x533010
//
// It is entered with ecx pointing at a sub-object embedded 4 bytes into the
// CMenuEula object. The thunk recovers the full object pointer (ecx - 4) and
// forwards the single char stack argument unchanged to the real __thiscall
// handler at 0x533010. Because the entry point is a bare tail jump, it uses
// the same calling convention as the callee (this in ecx, char on the stack,
// callee pops the 4-byte argument slot).
static void* __fastcall MenuEula_AdjustThisAndDispatch(uint8_t* self,
                                                       void* /*edx*/,
                                                       char a2)
{
    return CMenuEula_Destroy(self - 4, nullptr, a2);
}

HOOK(0x532DB0, MenuEula_AdjustThisAndDispatch, "auto");
