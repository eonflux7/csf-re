#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Fetches the embedded object's matrix (vtable slot 36), blends the a2/a3-scaled
// rows into a 3-vector offset, passes it to vtable slot 8, and destroys the
// matrix when the ownership flag returned by the getter is set.
static char __fastcall CCamera_ApplyBlendedMatrixOffset(CCamera* self, void* /*edx*/, float a2, float a3)
{
    // Embedded sub-object at +0x10; its first dword is its vtable pointer.
    void* embedded = static_cast<void*>(&self->field_0010);

    // Out-buffer for the getter: pointer at offset 0, ownership flag byte at offset 4.
    alignas(8) uint8_t matrixResultBytes[8];

    using GetMatrixFn = void(__fastcall*)(void*, void*, float**);
    GetMatrixFn getMatrix = reinterpret_cast<GetMatrixFn>(reinterpret_cast<void**>(self->field_0010)[36]);
    getMatrix(embedded, nullptr, reinterpret_cast<float**>(matrixResultBytes));

    float* blendMatrixRows = CSF_AT(float*, matrixResultBytes, 0);

    float offset[4];
    offset[0] = a2 * blendMatrixRows[0] + a3 * blendMatrixRows[4];
    offset[1] = a2 * blendMatrixRows[1] + a3 * blendMatrixRows[5];
    offset[2] = a2 * blendMatrixRows[2] + a3 * blendMatrixRows[6];

    using SetOffsetFn = void(__fastcall*)(void*, void*, float*);
    SetOffsetFn setOffset = reinterpret_cast<SetOffsetFn>(reinterpret_cast<void**>(self->field_0010)[8]);
    setOffset(embedded, nullptr, offset);

    if (CSF_AT(uint8_t, matrixResultBytes, 4) != 0) {
        return static_cast<char>(RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(blendMatrixRows)));
    }
    return 0;
}

HOOK(0x006D2CF0, CCamera_ApplyBlendedMatrixOffset, "auto");
