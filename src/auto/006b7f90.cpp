#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base constructor (C_00860178), __thiscall: ecx = this, edx unused.
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* /*edx*/));

// Allocates and links the world root node (__cdecl, no args). Returns the node pointer.
CSF_FUNC(uint32_t*, __cdecl, CreateWorldRootNode, 0x006B6E90, ());

static int __fastcall C_00860388_Initialize(C_00860388* self, void* /*edx*/)
{
    C_00860178_Construct(self, nullptr);
    self->vtbl = CSF_VTBL(0x00860388);

    uint32_t* rootNode = CreateWorldRootNode();
    self->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(rootNode));

    // Root node: flag byte at +0x15, and self-linked words at +0, +4, +8.
    CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x15) = 1;
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 4) = self->field_0008;
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 0) = self->field_0008;
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 8) = self->field_0008;

    CSF_AT(uint32_t, self, 0x0C) = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 10;
    self->field_0010 = 0;
    self->field_0024 = 0;
    self->field_0025 = 0;
    self->field_0027 = 0;
    self->field_0028 = 0;
    self->field_0026 = 0;

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x006B7F90, C_00860388_Initialize, "auto");
