#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0085ff6c_ConstructGatheringBase, 0x00798DF0, (C_0085ff4c* self, void* edx));

static uint32_t* __fastcall C_0085ff6c_Construct(C_0085ff6c* self, void* /*edx*/)
{
    C_0085ff6c_ConstructGatheringBase(reinterpret_cast<C_0085ff4c*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x0085FF6C);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006A74C0, C_0085ff6c_Construct, "auto");
