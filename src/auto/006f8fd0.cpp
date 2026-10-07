// Commandos: Strike Force (Pyro Studios) - hook reimplementation
// Target: 0x006F8FD0 - __thiscall setter storing the single byte argument
//         into C_008629a4::field_0009 and returning that same value (in al).

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// void __thiscall C_008629a4::SetField0009(this, char value)
//   mov al, [ebp+8]
//   mov [ecx+9], al
//   ret 4
// The store is a byte write to offset 0x0009; the result is returned in al
// exactly as supplied (the argument byte is used for both the store and the
// return value), so the incoming value is stored before being returned.
static char __fastcall C_008629a4_SetField0009(C_008629a4* self, void* /*edx*/, char value)
{
    self->field_0009 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x006F8FD0, C_008629a4_SetField0009, "auto");
