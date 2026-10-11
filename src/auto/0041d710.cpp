#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Vehicle_InitBase, 0x00418A50, (void* self, void* edx, char a2));
CSF_FUNC(void*, __fastcall, Vehicle_GrowTable54, 0x004189D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Vehicle_GrowTable10, 0x0040CDB0, (void* self, void* edx, int a2));

static int* __fastcall C_0082e428_Construct(C_0082e428* self, void* /*edx*/, char a2)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    Vehicle_InitBase(self, nullptr, a2);
    self->vtbl = CSF_VTBL(0x0082E428);

    self->field_0094 = 0.0f;
    self->field_0098 = 0.0f;
    self->field_009c = 0.0f;

    // first table at +0x54 (24-byte entries)
    void* table54 = base + 0x54;
    if (CSF_AT(uint32_t, base, 0x5C) >= CSF_AT(uint32_t, base, 0x58))
        Vehicle_GrowTable54(table54, nullptr, (int)CSF_AT(uint32_t, base, 0x60));
    uint32_t count54 = CSF_AT(uint32_t, base, 0x5C);
    uint8_t* data54 = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, base, 0x54));
    CSF_AT(uint32_t, base, 0x5C) = count54 + 1;
    uint8_t* e1 = data54 + 24 * count54;
    CSF_AT(int, e1, 0x00) = 20;
    CSF_AT(int, e1, 0x04) = 22;
    CSF_AT(int, e1, 0x08) = 21;
    CSF_AT(int, e1, 0x0C) = 23;
    CSF_AT(float, e1, 0x10) = 40.0f;
    CSF_AT(float, e1, 0x14) = 10.0f;

    // second table at +0x10 (24-byte entries)
    void* table10 = base + 0x10;
    uint32_t cap10 = CSF_AT(uint32_t, base, 0x14);
    if (cap10 < 1)
        Vehicle_GrowTable10(table10, nullptr, (int)(1 - cap10));

    int local_val = 0x44C;
    uint32_t local_pad;                                  // only the low 16 bits are written
    *reinterpret_cast<uint16_t*>(&local_pad) = 0;
    if (CSF_AT(uint32_t, base, 0x18) >= CSF_AT(uint32_t, base, 0x14))
        Vehicle_GrowTable10(table10, nullptr, (int)CSF_AT(uint32_t, base, 0x1C));
    uint32_t count10 = CSF_AT(uint32_t, base, 0x18);
    uint8_t* data10 = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, base, 0x10));
    CSF_AT(uint32_t, base, 0x18) = count10 + 1;
    uint8_t* e2 = data10 + 24 * count10;
    CSF_AT(int, e2, 0x00) = 0;
    CSF_AT(int, e2, 0x04) = 0x44C;
    CSF_AT(int, e2, 0x08) = local_val;
    CSF_AT(uint32_t, e2, 0x0C) = local_pad;
    CSF_AT(int, e2, 0x10) = 0x1D;
    CSF_AT(int, e2, 0x14) = 0x1E;

    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_00a0 = 0.0f;

    return reinterpret_cast<int*>(self);
}

HOOK(0x0041D710, C_0082e428_Construct, "auto");
