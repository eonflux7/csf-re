#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable of S_00518780, referenced by the original's call site at 0x518854.
static const void* const s_S00518780Vtbl = CSF_VTBL(0x00518780);

CSF_FUNC(int, __fastcall, sub_518780, 0x00518780, (void* self, void* edx, int a2));

// __thiscall, no stack args: forwards the address of this->field_0004 as the single argument.
static int __fastcall S_00518780_CallWithFieldFourAddress(S_00518780* self, void* /*edx*/)
{
    int fieldFourAddress = static_cast<int>(reinterpret_cast<std::uintptr_t>(&self->field_0004));
    return sub_518780(self, nullptr, fieldFourAddress);
}

HOOK(0x00518850, S_00518780_CallWithFieldFourAddress, "auto");
