#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Sets up the vtable pointer and the byte at +0x04, returning the object (constructor/
// initializer for C_00840e68). No stack arguments; __thiscall ret pops nothing.
static C_00840e68* __fastcall C_00840e68_Construct(C_00840e68* self, void* /*edx*/)
{
    self->vtbl = reinterpret_cast<void**>(0x00840E68);
    self->field_0004 = 0;
    return self;
}

HOOK(0x0051C0C0, C_00840e68_Construct, "auto");
