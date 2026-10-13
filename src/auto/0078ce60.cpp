#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address.
CSF_FUNC(uint32_t*, __fastcall, ResetBaseState, 0x0064A690, (void* self, void* edx));
CSF_FUNC(void**, __fastcall, AssignStringName, 0x0064A710, (void* self, void* edx, char* a2));

// Initializes a C_008310b8 object: resets the base, assigns the name string,
// then clears the trailing fields. Returns the object pointer as an int.
static int __fastcall C_008310b8_Initialize(C_008310b8* self, void* /*edx*/, const char* a2)
{
    ResetBaseState(self, nullptr);
    AssignStringName(self, nullptr, const_cast<char*>(a2));

    self->field_0004 = 0;
    self->field_0008 = 0;
    CSF_AT(uint8_t, self, 0x10) = 0;

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x0078CE60, C_008310b8_Initialize, "auto");
