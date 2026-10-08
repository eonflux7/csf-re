#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: int __thiscall sub_518890(std::uint32_t *this); called with ecx = this + 0x34
CSF_FUNC(int, __fastcall, CMenuAccountEdit_ResetInnerObject, 0x518890, (void* self, void* edx));

// Performs the field resets and the inner call. Returns the vtable pointer (low dword, eax)
// and the account-edit this pointer (high dword, edx) so the thunk can restore ecx and
// tail-jump through the vtable without touching the stack.
static uint64_t __fastcall CMenuAccountEdit_ResetFieldsPrepare(CMenuAccountEdit* self, void* /*edx*/)
{
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0031 = 0;

    CMenuAccountEdit_ResetInnerObject(reinterpret_cast<uint8_t*>(self) + 0x34, nullptr);

    // Original: eax = [esi] (vtable pointer), read after the inner call.
    uint32_t vtable = *reinterpret_cast<uint32_t*>(self);
    self->field_0280 = 0;

    uint32_t selfBits = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));
    return (static_cast<uint64_t>(selfBits) << 32) | static_cast<uint64_t>(vtable);
}

// Function pointer taken in C++ so the naked thunk loads it without referencing a decorated name.
static uint64_t (__fastcall* s_resetFieldsPrepareFn)(CMenuAccountEdit*, void*) =
    &CMenuAccountEdit_ResetFieldsPrepare;

// Naked thunk: ecx = this on entry. Calls the helper (ecx is its first argument),
// moves this back into ecx from edx, then tail-jumps through vtable slot 0x40.
// No push/pop: the caller's stack arguments are left untouched.
static __declspec(naked) void CMenuAccountEdit_ResetAndForward()
{
    __asm {
        mov eax, dword ptr [s_resetFieldsPrepareFn]
        call eax
        mov ecx, edx
        jmp dword ptr [eax + 0x40]
    }
}

HOOK(0x0055BFC0, CMenuAccountEdit_ResetAndForward, "auto");
