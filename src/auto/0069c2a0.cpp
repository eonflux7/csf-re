#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));
CSF_FUNC(uint8_t, __fastcall, sub_69C220, 0x0069C220, (void* self, void* edx, unsigned int a2, int a3, int a4, void* Block, int a6, int a7, int a8, int a9, unsigned int a10));

static uint32_t* __fastcall C_0085e7d8_Construct(C_0085e7d8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0085E7D8);
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_002c = 7;
    self->field_0028 = 0;
    CSF_AT(uint16_t, self, 0x18) = 0;

    // Frame of the original: sub esp, 0x20. v5 lives at frame+4.
    alignas(8) uint8_t frame[0x20];
    uint8_t* v5 = frame + 4;

    CSF_AT(uint32_t, frame, 0) = 0;
    CSF_AT(uint32_t, v5, 0x14) = 0;
    CSF_AT(uint32_t, v5, 0x18) = 0xF;
    CSF_AT(uint8_t, v5, 4) = 0;

    AssignStringRange_429B60(v5, nullptr, reinterpret_cast<char*>(CSF_ADDR_DATA(0x0082E65C)), 0);

    uint8_t* padSubobject = reinterpret_cast<uint8_t*>(self) + 4;
    sub_69C220(padSubobject, nullptr, 0xA, 0,
               CSF_AT(int, v5, 0),
               CSF_AT(void*, v5, 4),
               CSF_AT(int, v5, 8),
               0,
               0,
               static_cast<int>(CSF_AT(uint32_t, v5, 0x14)),
               CSF_AT(unsigned int, v5, 0x18));

    uint8_t* textInputSubobject = reinterpret_cast<uint8_t*>(self) + 0x14;
    TextInputSubobject_Init(textInputSubobject, nullptr,
                            reinterpret_cast<uint8_t*>(CSF_ADDR_DATA(0x0082E65C)));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0069C2A0, C_0085e7d8_Construct, "auto");
