#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Forwards the invoke to the object stored at +0x48 by tail-jumping through its
// vtable slot 0x58 (slot 22). Returns 0 with `ret 4` when no object is set.
static hook::Tail __fastcall ForwardInvokeToComponent(
    void* self, const uint32_t* /*args*/)
{
    void* component = CSF_AT(void*, self, 0x48);
    if (component)
        return hook::tail_jump(component, (*reinterpret_cast<void***>(component))[0x58 / 4]);

    return hook::tail_ret(0, 4);  // ret 4 on the path without a target
}

HOOK_TAIL(0x0048E530, ForwardInvokeToComponent, "auto");
