#include <cstdint>
#include "globals.h"
#include "types/auto_classes.h"
#include "hook.h"

// 0x6B1570 (ret 0):
//   eax = this->field_008C;                  // dword at +0x8C (35 dwords)
//   if (!eax) return 0;                      // returns the null pointer value
//   ecx = eax; eax = *ecx;                   // child's vtable
//   jmp [eax + 0x0C];                        // tail-call child virtual slot 3, forwarding
//                                            // the caller's frame and returning its result
static hook::Tail __fastcall ForwardCallToChildVirtualSlot3(void* self, const uint32_t* args)
{
    (void)args;
    void* child = CSF_AT(void*, self, 0x8C);
    if (!child) {
        return hook::tail_ret(0);
    }
    return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0x0C / 4]);
}

HOOK_TAIL(0x6B1570, ForwardCallToChildVirtualSlot3, "auto");
