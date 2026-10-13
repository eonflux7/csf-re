#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00860a30_SetFlagAndOrBits, 0x006C9FB0, (C_00860a30* self, void* edx));

// Original: 0x006EFD40. Sets the base-class flags, then tail-jumps to the code
// address stored at [field_0024 + 0x20]. ecx for the target is &field_0024.
static hook::Tail __fastcall C_00862240_ApplyFlagsAndTailDispatch(void* self, const uint32_t* args)
{
    (void)args;
    C_00862240* thisObj = static_cast<C_00862240*>(self);

    C_00860a30_SetFlagAndOrBits(thisObj, nullptr);

    // eax = this->field_0024 (object pointer). jmp [eax + 0x20] reads the target
    // dword directly at that address; it is not a vtable lookup.
    void* subObject = reinterpret_cast<void*>(static_cast<uintptr_t>(thisObj->field_0024));
    void* target = CSF_AT(void*, subObject, 0x20);

    // ecx = &this->field_0024
    void* obj = reinterpret_cast<void*>(&thisObj->field_0024);
    return hook::tail_jump(obj, target);
}

HOOK_TAIL(0x006EFD40, C_00862240_ApplyFlagsAndTailDispatch, "auto");
