#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Module: hud_status_icons. Constructor-style initializer that resets the
// icon status object, its embedded sub-object and two heap-owned buffers,
// then binds the source record passed in by the caller.

CSF_FUNC(int, __fastcall, C_008313dc_ClearSubobjectContainer, 0x00458E40, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_008313dc_BindSourceRecord, 0x0045A560, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t, __cdecl, MpSpawnPoints_RegisterObject, 0x00475850, (int a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static char* __fastcall C_008313dc_Construct(C_008313dc* self, void* /*edx*/, void* a2)
{
    // Embedded sub-object vtable slot at +0x04 (stored twice, same as original).
    CSF_AT(void*, self, 0x04) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082D428));
    self->vtbl = CSF_VTBL(0x008313DC);
    CSF_AT(void*, self, 0x04) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x008313C0));

    // Sub-object lives at self+0x08; its fields at +0x0C, +0x10, +0x14.
    uint8_t* subObject = reinterpret_cast<uint8_t*>(self) + 0x08;
    CSF_AT(uint32_t, subObject, 0x04) = 0;   // +0x0C
    CSF_AT(uint32_t, subObject, 0x08) = 0;   // +0x10
    CSF_AT(uint32_t, subObject, 0x0C) = 0;   // +0x14

    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;

    C_008313dc_ClearSubobjectContainer(subObject, nullptr);

    if (self->field_001c)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_001c));
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;

    if (self->field_002c)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_002c));
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;

    C_008313dc_BindSourceRecord(self, nullptr, a2);
    MpSpawnPoints_RegisterObject(reinterpret_cast<int>(self));

    return reinterpret_cast<char*>(self);
}

HOOK(0x0045A7D0, C_008313dc_Construct, "auto");
