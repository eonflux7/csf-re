#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base constructor chain: C_0083dbb0 (bound as in the packet).
CSF_FUNC(int, __fastcall, C_0083dbb0_Construct, 0x004D99F0, (C_0083dbb0* self, void* /*edx*/, uint32_t* owner));

// Initializes the embedded TDarObjeto sub-object at +0x340 (__thiscall, no stack args).
CSF_FUNC(uint32_t*, __fastcall, TDarObjeto_InitEmbedded, 0x004D4DA0, (void* self, void* /*edx*/));

// Constructs a C_0083c198 (TCharTask-derived AI task that gives an object): sets vtables,
// initializes the embedded TDarObjeto and resets the task state fields, then returns this.
static uint32_t* __fastcall C_0083c198_Construct(C_0083c198* self, void* /*edx*/, int a2)
{
    C_0083dbb0_Construct(self, nullptr, reinterpret_cast<uint32_t*>(a2));

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083C198);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083C17C));

    TDarObjeto_InitEmbedded(reinterpret_cast<uint8_t*>(self) + 0x340, nullptr);

    CSF_AT(void*, self, 0x3BC) = CSF_VTBL(0x0082D448);

    self->field_03c0 = static_cast<uint32_t>(-1);
    self->field_03c4 = static_cast<uint32_t>(-1);
    self->field_03c8 = 0;
    self->field_03cc = 0;
    self->field_03d0 = 0;
    self->field_03d8 = 0;
    self->field_03dc = 0;
    self->field_03e0 = 0;
    self->field_0039 = 0;
    self->field_03e4 = 0;
    self->field_03d4 = 0;
    self->field_03e8 = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004D50C0, C_0083c198_Construct, "auto");
