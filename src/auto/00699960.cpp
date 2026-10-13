#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static char* __fastcall C_0085e720_ResetStringState(C_0085e720* self, void* /*edx*/)
{
    // Vtable of C_0085e720
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0085E720);

    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    self->field_0034 = 0;

    // Embedded string-holder sub-object at +0x20
    uint8_t* strObj = reinterpret_cast<uint8_t*>(self) + 0x20;
    CSF_AT(uint32_t, strObj, 0x18) = 15;   // v2[2].__vftable = 15 (sub-object slot at +0x38)
    CSF_AT(uint8_t, strObj, 0x04) = 0;     // LOBYTE(v2->field_0004) = 0

    self->field_0054 = 15;
    self->field_0050 = 0;
    CSF_AT(uint8_t, self, 0x40) = 0;

    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;

    AssignStringRange_429B60(strObj, nullptr,
                             reinterpret_cast<char*>(&g_byte_82E65C), 0);

    self->field_0058 = 0;

    if (self->field_0008 != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));

    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;

    return reinterpret_cast<char*>(self);
}

HOOK(0x699960, C_0085e720_ResetStringState, "auto");
