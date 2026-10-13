#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwIm3DTransform, 0x0063CED0, (int a1, unsigned int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwIm3DEnd, 0x0063CFA0, ());
CSF_FUNC(int, __cdecl, RwIm3DRenderPrimitive, 0x0063D130, (int a1));
CSF_FUNC(int, __fastcall, GfxMatrix_SetFourRowsAndUpdate, 0x006ADA10, (S_006ada10* self, void* , uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

using SweepScaleArray = float[8];
using SweepAxisArray = std::uint8_t[8];

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ScoreboardRefreshInterval, 0x0082DAF0);
CSF_GLOBAL(float, g_SweepEpsilon, 0x008631A4);
CSF_GLOBAL(SweepScaleArray, g_ForwardSweepScales, 0x008A815C);
CSF_GLOBAL(SweepScaleArray, g_ReverseSweepScales, 0x008A817C);
CSF_GLOBAL(SweepAxisArray, g_SweepAxes, 0x008A819C);

CSF_GLOBAL(float, g_CenterX, 0x008BBE68);
CSF_GLOBAL(float, g_CenterY, 0x008BBE6C);
CSF_GLOBAL(std::int32_t, g_CenterZBits, 0x008BBE70);
CSF_GLOBAL(float, g_CenterU, 0x008BBE84);
CSF_GLOBAL(float, g_CenterV, 0x008BBE88);
CSF_GLOBAL(float, g_FirstX, 0x008BBE8C);
CSF_GLOBAL(float, g_FirstY, 0x008BBE90);
CSF_GLOBAL(std::int32_t, g_FirstZBits, 0x008BBE94);
CSF_GLOBAL(float, g_FirstU, 0x008BBEA8);
CSF_GLOBAL(float, g_FirstV, 0x008BBEAC);
CSF_GLOBAL(float, g_SecondX, 0x008BBEB0);
CSF_GLOBAL(float, g_SecondY, 0x008BBEB4);
CSF_GLOBAL(std::int32_t, g_SecondZBits, 0x008BBEB8);
CSF_GLOBAL(float, g_SecondU, 0x008BBECC);
CSF_GLOBAL(float, g_SecondV, 0x008BBED0);

static char __fastcall S_00705300_RenderRadialSweep(
    S_00705300* self, void* /*edx*/, float amount, int forward)
{
    alignas(8) std::uint8_t rightBytes[0xC];
    alignas(8) std::uint8_t forwardBytes[0xC];
    alignas(8) std::uint8_t stateBytes[0x20];

    auto& cameraMatrix = CSF_AT(std::uint32_t*, stateBytes, 0x0);
    auto& cameraMatrixOwned = CSF_AT(std::uint8_t, stateBytes, 0x4);
    auto& sweepMatrix = *reinterpret_cast<S_006ada10*>(stateBytes + 0x8);
    auto& sweepMatrixOwned = CSF_AT(std::uint8_t, stateBytes, 0xC);
    auto& firstV = CSF_AT(float, stateBytes, 0x10);
    auto& secondV = CSF_AT(float, stateBytes, 0x14);
    auto& firstU = CSF_AT(float, stateBytes, 0x18);
    auto& secondU = CSF_AT(float, stateBytes, 0x1C);

    firstU = 0.5f;
    float remaining = amount * g_ScoreboardRefreshInterval;
    firstV = 0.0f;
    secondU = 0.5f;
    secondV = 0.0f;

    void* globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* cameraProvider = CSF_AT(void*, globals, 0xF0);
    void* camera =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, cameraProvider, 0)[0x10 / 4])(
                cameraProvider, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, camera, 0)[0x8C / 4])(
            camera, nullptr, stateBytes);

    sweepMatrix.field_0000 =
        reinterpret_cast<decltype(sweepMatrix.field_0000)>(RwMatrixCreate());
    sweepMatrixOwned = 1;

    CSF_AT(float, forwardBytes, 0x0) = -CSF_AT(float, cameraMatrix, 0x20);
    CSF_AT(float, forwardBytes, 0x4) = -CSF_AT(float, cameraMatrix, 0x24);
    CSF_AT(float, forwardBytes, 0x8) = -CSF_AT(float, cameraMatrix, 0x28);
    CSF_AT(float, rightBytes, 0x0) = -CSF_AT(float, cameraMatrix, 0x0);
    CSF_AT(float, rightBytes, 0x4) = -CSF_AT(float, cameraMatrix, 0x4);
    CSF_AT(float, rightBytes, 0x8) = -CSF_AT(float, cameraMatrix, 0x8);

    // Listed as a vtable reference in the packet, but used as a direct callee.
    (void)CSF_VTBL(0x006ADA10);
    GfxMatrix_SetFourRowsAndUpdate(
        &sweepMatrix, nullptr,
        reinterpret_cast<std::uint32_t*>(rightBytes),
        reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint8_t*>(cameraMatrix) + 0x10),
        reinterpret_cast<std::uint32_t*>(forwardBytes),
        reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x4));

    for (int segment = 0; remaining > g_SweepEpsilon; ++segment)
    {
        if (segment >= 8)
            break;

        float step = g_HalfDurationScale;
        if (remaining > g_HalfDurationScale)
        {
            remaining = remaining - g_HalfDurationScale;
        }
        else
        {
            step = remaining;
            remaining = 0.0f;
        }

        const std::uint8_t axis = g_SweepAxes[segment];
        if (forward)
        {
            const float delta = step * g_ForwardSweepScales[segment];
            if (axis)
                secondU = delta + firstU;
            else
                secondV = delta + firstV;
        }
        else
        {
            const float delta = step * g_ReverseSweepScales[segment];
            if (axis)
                firstU = delta + secondU;
            else
                firstV = delta + secondV;
        }

        float vSpan = self->field_0020 - self->field_0018;
        g_CenterU = (self->field_001c - self->field_0014) *
                    g_HalfDurationScale + self->field_0014;
        g_CenterV = vSpan * g_HalfDurationScale + self->field_0018;
        g_CenterZBits = -1110651699;
        g_CenterX = g_fZero;
        g_CenterY = g_fZero;

        vSpan = self->field_0020 - self->field_0018;
        g_FirstU = (self->field_001c - self->field_0014) *
                   firstU + self->field_0014;
        g_FirstV = vSpan * firstV + self->field_0018;
        float width = self->field_0024 * self->field_002c;
        float height = self->field_002c * self->field_0028;
        g_FirstZBits = -1110651699;
        g_FirstX = firstU * width - width * g_HalfDurationScale;
        g_FirstY = g_HalfDurationScale * height - height * firstV;

        vSpan = self->field_0020 - self->field_0018;
        g_SecondU = (self->field_001c - self->field_0014) *
                    secondU + self->field_0014;
        g_SecondV = vSpan * secondV + self->field_0018;
        width = self->field_0024 * self->field_002c;
        height = self->field_002c * self->field_0028;
        g_SecondZBits = -1110651699;
        g_SecondX = secondU * width - width * g_HalfDurationScale;
        g_SecondY = g_HalfDurationScale * height - height * secondV;

        if (RwIm3DTransform(
                reinterpret_cast<int>(&g_CenterX), 3,
                reinterpret_cast<int>(
                    reinterpret_cast<std::uint32_t*>(sweepMatrix.field_0000)),
                25))
        {
            RwIm3DRenderPrimitive(3);
            RwIm3DEnd();
        }

        if (forward)
        {
            firstU = secondU;
            firstV = secondV;
        }
        else
        {
            secondU = firstU;
            secondV = firstV;
        }
    }

    if (sweepMatrixOwned)
        RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(sweepMatrix.field_0000));

    if (cameraMatrixOwned)
        return static_cast<char>(RwMatrixDestroy(cameraMatrix));
    return static_cast<char>(cameraMatrixOwned);
}

HOOK(0x00705300, S_00705300_RenderRadialSweep, "auto");
