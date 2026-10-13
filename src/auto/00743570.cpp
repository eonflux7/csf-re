#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x4E16C0: __fastcall-bound player event handler on C_0083e248 (ecx = this, edx unused, 8 stack ints, pops 32).
CSF_FUNC(int, __fastcall, C_0083e248_ApplyPlayerEvent, 0x004E16C0,
         (C_0083e248* self, void* /*edx*/, int value1, int value2, int value3, int value4,
          int playerId, int value6, int value7, int value8));

// 0x743570: forwards a player event (8 stack ints, ret 32) to the handler object stored at +0xD4.
// The original tail-jumps to C_0083e248_ApplyPlayerEvent with the stack arguments unchanged.
// If no handler is set, it does nothing and returns (eax left undefined in the original; 0 here).
static int __fastcall S_00743570_ForwardPlayerEvent(S_00743570* self, void* /*edx*/,
                                                    int a2, int a3, int a4, int a5,
                                                    int a6, int a7, int a8, int a9)
{
    C_0083e248* handler = reinterpret_cast<C_0083e248*>(self->field_00d4);
    if (handler != nullptr)
        return C_0083e248_ApplyPlayerEvent(handler, nullptr, a2, a3, a4, a5, a6, a7, a8, a9);
    return 0;
}

HOOK(0x743570, S_00743570_ForwardPlayerEvent, "auto");
