#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, Firearms_ConstructBase, 0x00477CF0,
         (void* self, void* edx, int a2, int a3));

static uint32_t* __fastcall C_008319c0_Construct(
    C_008319c0* self, void* /*edx*/, int a2, int a3)
{
    Firearms_ConstructBase(self, nullptr, a2, a3);
    self->vtbl = CSF_VTBL(0x008319C0);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0047A6B0, C_008319c0_Construct, "auto");
