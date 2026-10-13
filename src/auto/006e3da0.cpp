#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Vtable of C_00861a78 (stored at +0x0 of the object).
static const std::uintptr_t kShapeWrapperVtableAddr = CSF_ADDR_VT(0x00861A78);

static C_00861a78* __fastcall C_00861a78_ConstructShapeWrapper(C_00861a78* self, void* /*edx*/, int a2)
{
    C_00860178_Construct(self, nullptr);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861A78);
    self->field_0008 = reinterpret_cast<std::uint32_t>(RwMatrixCreate());
    self->field_000c = 1;
    self->field_0004 = static_cast<std::uint32_t>(a2);
    return self;
}

HOOK(0x006E3DA0, C_00861a78_ConstructShapeWrapper, "auto");
