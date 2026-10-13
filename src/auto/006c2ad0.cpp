#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, _rtCompressedKeyFrameCompressFloat, 0x0061F5E0, (float a1));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __cdecl GfxAnimCompress_MakeRelativeRotation(
    uint16_t* keyframe, uint16_t* referenceKeyframe)
{
    alignas(8) uint8_t rotationBytes[0x10];
    alignas(8) uint8_t referenceBytes[0x10];
    float* rotation = reinterpret_cast<float*>(rotationBytes);
    float* reference = reinterpret_cast<float*>(referenceBytes);

    hook::Regs regs{};
    regs.eax = reinterpret_cast<uint32_t>(keyframe);
    regs.esi = reinterpret_cast<uint32_t>(rotationBytes);
    hook::call_regs(CSF_ADDR_FN(0x006C24B0), &regs, nullptr, 0);

    regs = {};
    regs.eax = reinterpret_cast<uint32_t>(referenceKeyframe);
    regs.esi = reinterpret_cast<uint32_t>(referenceBytes);
    hook::call_regs(CSF_ADDR_FN(0x006C24B0), &regs, nullptr, 0);

    const double referenceX = reference[0];
    const double referenceY = reference[1];
    const double referenceZ = reference[2];
    const double referenceW = reference[3];
    const double normSquared =
        referenceY * referenceY + referenceX * referenceX +
        referenceZ * referenceZ + referenceW * referenceW;

    float inverseX = 0.0f;
    float inverseY = 0.0f;
    double inverseZ = 0.0;
    float inverseW = 0.0f;

    if (normSquared > g_fZero)
    {
        const double reciprocal = g_TransitionOne / normSquared;
        inverseW = static_cast<float>(referenceW * reciprocal);
        inverseX = static_cast<float>(referenceX * -reciprocal);
        inverseY = static_cast<float>(referenceY * -reciprocal);
        inverseZ = referenceZ * -reciprocal;
    }

    const double rotationX = rotation[0];
    const double rotationY = rotation[1];
    const double rotationZ = rotation[2];
    const double rotationW = rotation[3];

    const float resultW = static_cast<float>(
        rotationW * inverseW -
        (rotationY * inverseY + rotationX * inverseX +
         rotationZ * inverseZ));

    const float crossZ = static_cast<float>(
        rotationY * inverseX - rotationX * inverseY);
    const float baseX = static_cast<float>(
        rotationX * inverseW +
        (rotationZ * inverseY - rotationY * inverseZ));

    const float resultX = static_cast<float>(
        rotationW * inverseX + baseX);
    const float resultY = static_cast<float>(
        rotationW * inverseY +
        (rotationY * inverseW +
         (rotationX * inverseZ - rotationZ * inverseX)));
    const float resultZ = static_cast<float>(
        rotationW * inverseZ + (rotationZ * inverseW + crossZ));

    keyframe[4] = static_cast<uint16_t>(
        _rtCompressedKeyFrameCompressFloat(resultX));
    keyframe[5] = static_cast<uint16_t>(
        _rtCompressedKeyFrameCompressFloat(resultY));
    keyframe[6] = static_cast<uint16_t>(
        _rtCompressedKeyFrameCompressFloat(resultZ));

    const int result = _rtCompressedKeyFrameCompressFloat(resultW);
    keyframe[7] = static_cast<uint16_t>(result);
    return result;
}

HOOK(0x006C2AD0, GfxAnimCompress_MakeRelativeRotation, "auto");
