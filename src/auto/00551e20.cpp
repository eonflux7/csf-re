#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResetSpawnEntries, 0x00550AE0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall C_00844694_ResetSpawnState(C_00844694* self, void* /*edx*/)
{
    self->field_1480 = 0;

    uint8_t* text = reinterpret_cast<uint8_t*>(self) + 0xC;
    for (int remaining = 50; remaining != 0; --remaining, text += 64)
    {
        text[32] = 0;
        TextInputSubobject_Init(text, nullptr, &g_byte_82E65C);
        CSF_AT(uint32_t, text, 0x28) = -1;
        CSF_AT(uint32_t, text, 0x2C) = -1;
        CSF_AT(uint32_t, text, -0x8) = -1;
        CSF_AT(uint32_t, text, -0x4) = -1;
        CSF_AT(uint32_t, text, 0x30) = -1;
        CSF_AT(uint32_t, text, 0x1C) = 0;
        CSF_AT(uint32_t, text, 0x24) = 0;
        text[54] = 0;
        text[53] = 0;
        text[52] = 0;
    }

    self->field_0c84 = 0;
    self->field_0c88 = 0;
    self->field_0c8c = 0;
    self->field_0c9d = 0;
    self->field_0ca4 = 0;
    self->field_0ca0 = -1;
    self->field_0c9c = 1;
    self->field_13c4 = 1157234688;

    int result = ResetSpawnEntries(reinterpret_cast<uint8_t*>(self) + 0xCA8, nullptr);

    self->field_13d4 = -1;
    self->field_13f8 = -1;
    self->field_13fc = -1;
    self->field_13e4 = -1;
    self->field_13e8 = -1;
    self->field_13ec = -1;
    self->field_13bc = 0;
    self->field_13b8 = 0;
    self->field_13c8 = 0;
    self->field_13c0 = 0;
    self->field_13d8 = 0;
    self->field_13f4 = 0;
    self->field_1401 = 0;
    self->field_1402 = 0;
    self->field_13d0 = 0;
    self->field_13cc = 0;
    self->field_141c = 0;
    self->field_1420 = 0;
    self->field_1424 = 0;
    self->field_1428 = 0;
    self->field_142c = 0;
    self->field_1430 = 0;
    self->field_1434 = 0;
    self->field_1440 = 0;
    self->field_1444 = 0;
    self->field_1448 = 0;
    self->field_144c = 0;
    self->field_1450 = 0;
    self->field_1454 = 0;
    self->field_1458 = 0;
    self->field_145c = 0;
    self->field_1460 = 0;
    self->field_1470 = 0;
    self->field_1474 = 0;
    self->field_147c = 0;
    self->field_1498 = 0;
    self->field_14a2 = 0;
    self->field_0c90 = 0;
    self->field_1484 = 0;
    self->field_14a4 = 0;
    self->field_148c = 0;
    self->field_14ac = 0;
    self->field_13dc = 1;
    self->field_1400 = 1;
    self->field_1488 = 305419896;
    self->field_1478 = 1065353216;
    self->field_14a1 = 1;
    self->field_14a3 = 1;

    return result;
}

HOOK(0x00551E20, C_00844694_ResetSpawnState, "auto");
