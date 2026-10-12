#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(int, __fastcall, GfxMatrix_SetFourRowsAndUpdate, 0x006ADA10, (S_006ada10* self, void* , uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));

CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);

[[maybe_unused]] static const auto g_MatrixPacketVtable = CSF_VTBL(0x006ADA10);

static char __fastcall C_00862748_SetPositionAndDirection(
    C_00862748* self, void* /*edx*/,
    std::uint32_t* position, float* direction)
{
    alignas(8) std::uint8_t rightBytes[0xC];
    alignas(8) std::uint8_t forwardBytes[0xC];
    alignas(8) std::uint8_t matrixBytes[0x8];

    float* right = reinterpret_cast<float*>(rightBytes);
    float* forward = reinterpret_cast<float*>(forwardBytes);
    S_006ada10& matrix = *reinterpret_cast<S_006ada10*>(matrixBytes);

    const float directionZ = direction[1];
    forward[0] = direction[0];
    forward[1] = 0.0f;
    forward[2] = directionZ;
    NormalizeVector3(reinterpret_cast<C_00833b88*>(forwardBytes), nullptr);

    const float rightX = g_AxisY_Y * forward[2] - g_AxisY_Z * forward[1];
    const float rightY = g_AxisY_Z * forward[0] - g_AxisY * forward[2];
    const float rightZ = g_AxisY * forward[1] - g_AxisY_Y * forward[0];
    right[0] = rightX;
    right[2] = rightZ;
    right[1] = rightY;

    matrix.field_0000 = reinterpret_cast<std::uint32_t>(RwMatrixCreate());
    CSF_AT(std::uint8_t, matrixBytes, 0x4) = 1;

    GfxMatrix_SetFourRowsAndUpdate(
        &matrix, nullptr,
        reinterpret_cast<std::uint32_t*>(rightBytes),
        reinterpret_cast<std::uint32_t*>(&g_AxisY),
        reinterpret_cast<std::uint32_t*>(forwardBytes),
        position);

    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, self, 0)[0x40 / 4])(
            self, nullptr, reinterpret_cast<int*>(&matrix));

    const char ownsMatrix = static_cast<char>(
        CSF_AT(std::uint8_t, matrixBytes, 0x4));
    if (ownsMatrix)
        return static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(matrix.field_0000)));
    return ownsMatrix;
}

HOOK(0x006F3E60, C_00862748_SetPositionAndDirection, "auto");
