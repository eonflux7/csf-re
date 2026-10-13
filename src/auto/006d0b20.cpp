#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees (bound by address, signatures as given in the packet)
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(uint32_t*, __cdecl, GfxRenderTargets_QueryStateToken, 0x006D0730, ());

static unsigned int* __fastcall C_00860db0_Construct(C_00860db0* self, void* /*edx*/)
{
    // Base constructor first
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);

    // Install this class's vtable
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860DB0);

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x010) = 10;
    CSF_AT(uint32_t, self, 0x020) = 10;
    self->field_0014 = 0;
    CSF_AT(uint32_t, self, 0x018) = 0;
    CSF_AT(uint32_t, self, 0x01C) = 0;

    self->field_002c = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(GfxRenderTargets_QueryStateToken()));
    self->field_0030 = 0;

    // Grow the first array to capacity 20
    uint32_t firstCount = self->field_0008;
    if (firstCount < 0x14)
    {
        (void)CSF_VTBL(0x0041E5F0);  // S_0041e5f0 vtable reference at this call site
        S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(&self->field_0004), nullptr,
                                     static_cast<int>(20 - firstCount));
    }

    // Grow the second array to capacity 20
    uint32_t secondCount = CSF_AT(uint32_t, self, 0x018);
    if (secondCount < 0x14)
    {
        (void)CSF_VTBL(0x0041E5F0);  // S_0041e5f0 vtable reference at this call site
        S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(&self->field_0014), nullptr,
                                     static_cast<int>(20 - secondCount));
    }

    self->field_0024 = 0;
    return reinterpret_cast<unsigned int*>(self);
}

HOOK(0x006D0B20, C_00860db0_Construct, "auto");
