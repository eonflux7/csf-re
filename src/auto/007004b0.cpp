#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00700250, __stdcall, ret 8 (void* out, int element)
CSF_FUNC(void*, __stdcall, sub_700250, 0x00700250, (void* a1, int arg4));

// Returns the product of the doubled x, y, z floats of a record (x at +0xC, y at +0x10, z at +0x14)
// taken from the element at index a2 of the array pointed to by this->field_0004->+0x10.
static double __fastcall C_008630e0_GetDoubledComponentProduct(C_008630e0* self, void* /*edx*/, int a2)
{
    // Stack object the original passes to sub_700250 (0x48 bytes at ebp-0x48).
    alignas(8) uint8_t recordBytes[0x48];

    void* owner = reinterpret_cast<void*>(self->field_0004);
    uint32_t arrayBase = CSF_AT(uint32_t, owner, 0x10);
    // Element stride is 12 bytes: index is 3*a2 dwords.
    uint32_t element = CSF_AT(uint32_t, reinterpret_cast<void*>(arrayBase), 12 * a2);

    void* found = sub_700250(recordBytes, static_cast<int>(element));

    float* components = reinterpret_cast<float*>(static_cast<uint8_t*>(found) + 0x0C);
    float x = components[0];
    float y = components[1];
    float z = components[2];

    float doubledZ = z + z;
    float doubledY = y + y;
    float doubledX = x + x;

    return static_cast<double>(doubledZ * doubledY * doubledX);
}

HOOK(0x7004B0, C_008630e0_GetDoubledComponentProduct, "auto");
