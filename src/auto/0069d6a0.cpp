#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_0085ea2c constructor (__thiscall, ret 0). Installs the vtable, zeroes
// field_0004, sets field_0008 to 1, and returns the object pointer.
static uint32_t* __fastcall C_0085ea2c_ConstructFileStream(C_0085ea2c* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x0085EA2C);
    self->field_0004 = 0;
    self->field_0008 = 1;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0069D6A0, C_0085ea2c_ConstructFileStream, "auto");
