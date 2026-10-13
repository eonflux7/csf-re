#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RtQuatConvertFromMatrix, 0x007D9D40, (float* a1, float* a2));

static int __fastcall C_008633a0_UpdateQuatFromHierarchyMatrix(C_008633a0* self, void* /*edx*/, int* a2)
{
    // Convert the matrix at *a2 into the quaternion stored at field_0010.
    RtQuatConvertFromMatrix(reinterpret_cast<float*>(self->field_0010),
                            reinterpret_cast<float*>(*a2));

    // Copy the 3 dwords at matrix+0x30 (translation) into the buffer at field_0014.
    uint32_t* dst = reinterpret_cast<uint32_t*>(self->field_0014);
    const uint32_t* src = reinterpret_cast<const uint32_t*>(static_cast<uintptr_t>(*a2) + 0x30);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];

    // Virtual call: slot 0x8C / 4 = 35 on the object at field_0008, one argument field_000c.
    void* target = reinterpret_cast<void*>(self->field_0008);
    int result = ((int(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, target, 0)[35])(
        target, nullptr, self->field_000c);

    self->field_0020 = 1;
    return result;
}

HOOK(0x007077E0, C_008633a0_UpdateQuatFromHierarchyMatrix, "auto");
