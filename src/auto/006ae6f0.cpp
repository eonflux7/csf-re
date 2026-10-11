#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, sub_6ADA10, 0x006ADA10, (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

// The packet labels the direct row-setter call as a vtable reference.
// Preserve its binding without introducing a vtable store.
[[maybe_unused]] static const auto g_RowSetterPacketVtable = CSF_VTBL(0x006ADA10);

static int __fastcall GfxMatrix_CreateQuaternionMatrix(
    float* self, void* /*edx*/, int outputAddress)
{
    const float xx = self[0] * self[0];
    const float yy = self[1] * self[1];
    const float zz = self[2] * self[2];
    const float ww = self[3] * self[3];
    const float xy = self[1] * self[0];
    const float yz = self[2] * self[1];
    const float wy = self[3] * self[1];
    const float xz = self[2] * self[0];
    const float wz = self[3] * self[2];
    const float wx = self[3] * self[0];

    alignas(S_006ada10) std::uint8_t temporaryStorage[0x30];
    auto* temporary = reinterpret_cast<S_006ada10*>(temporaryStorage);
    temporary->field_0000 =
        reinterpret_cast<std::uint32_t>(RwMatrixCreate());
    CSF_AT(std::uint8_t, temporary, 0x4) = 1;

    float forward[3] = {
        (xz + wy) + (xz + wy),
        (yz - wx) + (yz - wx),
        ww - (yy + xx) + zz
    };
    float up[3] = {
        (xy - wz) + (xy - wz),
        yy - xx - zz + ww,
        (wx + yz) + (wx + yz)
    };
    float right[3] = {
        xx - yy - zz + ww,
        (wz + xy) + (wz + xy),
        (xz - wy) + (xz - wy)
    };

    sub_6ADA10(
        temporary, nullptr,
        reinterpret_cast<std::uint32_t*>(right),
        reinterpret_cast<std::uint32_t*>(up),
        reinterpret_cast<std::uint32_t*>(forward),
        reinterpret_cast<std::uint32_t*>(&g_ZeroVectorX));

    std::uint32_t* matrix = RwMatrixCreate();
    const void* source =
        reinterpret_cast<const void*>(temporary->field_0000);
    auto* output = reinterpret_cast<S_006ada10*>(outputAddress);
    output->field_0000 = reinterpret_cast<std::uint32_t>(matrix);
    std::memcpy(matrix, source, 0x40);

    const bool ownsTemporary = CSF_AT(std::uint8_t, temporary, 0x4) != 0;
    CSF_AT(std::uint8_t, output, 0x4) = 1;
    if (ownsTemporary) {
        RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(temporary->field_0000));
    }
    return outputAddress;
}

HOOK(0x006AE6F0, GfxMatrix_CreateQuaternionMatrix, "auto");
