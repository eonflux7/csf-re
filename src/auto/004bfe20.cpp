#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __thiscall callee: ecx = this, one stack arg (ret 4)
CSF_FUNC(int, __fastcall, sub_4B1310, 0x004B1310, (void* self, void* edx, uint32_t* a2));
// __thiscall callee: ecx = this, four stack args (ret 16)
CSF_FUNC(int, __fastcall, sub_4BC870, 0x004BC870, (void* self, void* edx, int a2, int a3, int a4, int a5));

// Clears a three-dword scratch buffer, asks sub_4B1310 to fill it from the
// sub-object at +0xCC, then forwards it with the constant 4 to sub_4BC870 on
// the sub-object at +0x6C. Always returns 0.
static char __fastcall S_004bc930_ReadAndFlushActionBuffer(S_004bc930* self, void* /*edx*/, int a2)
{
    (void)a2;

    uint32_t scratchValues[3];
    scratchValues[0] = 0;
    scratchValues[1] = 0;
    scratchValues[2] = 0;

    void* sensorSubObject = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0xCC);
    sub_4B1310(sensorSubObject, nullptr, scratchValues);

    void* outputSubObject = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x6C);
    sub_4BC870(outputSubObject, nullptr,
               4,
               static_cast<int>(scratchValues[0]),
               static_cast<int>(scratchValues[1]),
               static_cast<int>(scratchValues[2]));

    return 0;
}

HOOK(0x004BFE20, S_004bc930_ReadAndFlushActionBuffer, "auto");
