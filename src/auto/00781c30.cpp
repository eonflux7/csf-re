// Constructor-style initializer for CLevelData (original 0x00781C30).
// Sets the vtable, zeroes the header fields, initializes the embedded sub-object at +0x54
// via sub_7770C0, then resets the mesh/dst/string-like fields at +0x1F58..+0x1FB8.
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_7770C0, 0x007770C0, (void* a1, int a2));

static CLevelData* __fastcall CLevelData_Construct(CLevelData* self, int edxArg)
{
    self->vtbl = CSF_VTBL(0x008723F8);

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 10;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;

    // Sub-object embedded at +0x54 (address only, not a pointer load). edx is forwarded unchanged.
    sub_7770C0(reinterpret_cast<uint8_t*>(self) + 0x54, edxArg);

    self->field_1f68 = 0;
    self->field_1f6c = 15;
    CSF_AT(uint8_t, self, 0x1F58) = 0;   // byte store in the original
    self->field_1f88 = 15;
    self->field_1f84 = 0;
    CSF_AT(uint8_t, self, 0x1F74) = 0;   // byte store in the original

    self->field_1fa0 = 0;
    self->field_1fa4 = 0;
    self->field_1fa8 = 0;
    self->field_1fac = 0;
    self->field_1fb0 = 0;
    self->m_navMesh = 0;
    self->m_dummyMesh = 0;
    self->m_areaMesh = 0;
    self->m_lightMesh = 0;
    self->m_sceneObjMesh = 0;
    self->field_1fb4 = 0;
    self->m_dstData = 0;

    return self;
}

HOOK(0x781C30, CLevelData_Construct, "auto");
