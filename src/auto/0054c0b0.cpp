#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008442a0_ClearContents, 0x0054B450,
         (void* self, void* edx));
CSF_FUNC(int*, __fastcall, TypeRegistry_EraseNodes, 0x0054BC60,
         (uint32_t* a1, int a2, int* a3, int a4, int a5));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_008442a0_Destroy(C_008442a0* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008442A0);
    C_008442a0_ClearContents(self, nullptr);

    alignas(8) uint8_t iteratorBytes[0x4];
    const uint32_t sentinel = self->field_009c;
    const int firstNode = CSF_AT(int, reinterpret_cast<void*>(sentinel), 0);
    TypeRegistry_EraseNodes(
        reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x98),
        0, reinterpret_cast<int*>(iteratorBytes), firstNode,
        static_cast<int>(sentinel));

    if (self->field_009c)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_009c));
    self->field_009c = 0;
    CSF_AT(uint32_t, self, 0xA0) = 0;

    int result = static_cast<int>(self->field_008c);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_008c));
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_0094 = 0;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00844200);
    return result;
}

HOOK(0x0054C0B0, C_008442a0_Destroy, "auto");
