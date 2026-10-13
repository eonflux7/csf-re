#include <cstdint>
#include "hook.h"

// 0x706170: __thiscall, 4 stack args, ret 0x10.
// Zeroes a six-dword record, copies the first three dwords from the source
// record a5, then stores the three scalar arguments in slots 3..5.
static uint32_t* __fastcall InitSixDwordRecordFromSource(uint32_t* self, void* /*edx*/,
                                                         uint32_t a2, uint32_t a3,
                                                         uint32_t a4, uint32_t* a5)
{
    self[0] = 0;
    self[1] = 0;
    self[2] = 0;
    self[3] = 0;
    self[4] = 0;
    self[5] = 0;

    self[0] = a5[0];
    self[1] = a5[1];
    self[2] = a5[2];
    self[3] = a2;
    self[4] = a3;
    self[5] = a4;
    return self;
}

HOOK(0x706170, InitSixDwordRecordFromSource, "auto");
