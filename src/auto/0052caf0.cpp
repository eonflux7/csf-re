#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Float constants of the original, bound by address (exact values preserved).
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);  // progress threshold (0.1)
CSF_GLOBAL(float, g_flt_0082D7C0, 0x0082D7C0);  // distance scale (10.0)
CSF_GLOBAL(float, g_flt_00831430, 0x00831430);  // overshoot factor (-2.0)
CSF_GLOBAL(float, g_flt_00841270, 0x00841270);  // velocity scale (1270.0)

// 32-bit wrapping add/sub, matching the original's plain x86 add/sub.
static int32_t AddWrap32(int32_t a, int32_t b)
{
    return static_cast<int32_t>(static_cast<uint32_t>(a) + static_cast<uint32_t>(b));
}

static int32_t SubWrap32(int32_t a, int32_t b)
{
    return static_cast<int32_t>(static_cast<uint32_t>(a) - static_cast<uint32_t>(b));
}

// Replaces the MSVC __ftol2 helper: truncate toward zero to int64, keep the low 32 bits.
static int32_t TruncToLow32(double value)
{
    return static_cast<int32_t>(static_cast<int64_t>(value));
}

// Advances a smooth tween of a menu progress/input element. While the progress value at +0x12C
// is below the threshold, it computes a clamped target rectangle from the offsets to the
// current position, pushes it to the child at +0x34 (slots 7 and 9), applies the rectangle to
// the receiver at +0x1C (slot 130, flag 0), applies a scale vector to the receiver at +0x38
// (slot 11), and adds the time step to the progress. Otherwise it sets the finished flag at +0x20.
static void __fastcall MenuProgressInput_AdvanceTweenStep(void* self, void* /*edx*/, float timeStep)
{
    uint8_t* base = static_cast<uint8_t*>(self);

    if (CSF_AT(float, base, 0x12C) >= g_flt_0082DC84) {
        CSF_AT(int32_t, base, 0x20) = 1;
        return;
    }

    const float speed = CSF_AT(float, base, 0x12C);
    const double speedD = static_cast<double>(speed);
    const double distanceScale = static_cast<double>(g_flt_0082D7C0);

    const int32_t currentX = CSF_AT(int32_t, base, 0x130);
    const int32_t currentY = CSF_AT(int32_t, base, 0x134);
    const int32_t deltaX = SubWrap32(currentX, CSF_AT(int32_t, base, 0x140));
    const int32_t deltaY = SubWrap32(currentY, CSF_AT(int32_t, base, 0x144));

    // X offset is stored as a float (single rounding); Y offset stays double.
    const float scaledDeltaXf =
        static_cast<float>(static_cast<double>(deltaX) * speedD * distanceScale);
    const double scaledDeltaY =
        static_cast<double>(deltaY) * speedD * distanceScale;

    int32_t targetX = SubWrap32(currentX, TruncToLow32(scaledDeltaXf));
    int32_t targetY = SubWrap32(currentY, TruncToLow32(scaledDeltaY));
    int32_t sizeW = SubWrap32(CSF_AT(int32_t, base, 0x138),
        TruncToLow32(static_cast<double>(scaledDeltaXf) * static_cast<double>(g_flt_00831430)));
    int32_t sizeH = SubWrap32(CSF_AT(int32_t, base, 0x13C),
        TruncToLow32(scaledDeltaY * static_cast<double>(g_flt_00831430)));

    // Clamp against the bounds stored in the object (signed comparisons).
    if (targetX <= CSF_AT(int32_t, base, 0x140))
        targetX = CSF_AT(int32_t, base, 0x140);
    if (targetY <= CSF_AT(int32_t, base, 0x144))
        targetY = CSF_AT(int32_t, base, 0x144);
    if (sizeW >= CSF_AT(int32_t, base, 0x148))
        sizeW = CSF_AT(int32_t, base, 0x148);
    if (sizeH >= CSF_AT(int32_t, base, 0x14C))
        sizeH = CSF_AT(int32_t, base, 0x14C);

    // Push the position and size to the child at +0x34 (vtable slots 7 and 9).
    void* scrollChild = CSF_AT(void*, base, 0x34);
    ((void(__fastcall*)(void*, void*, int32_t*))CSF_AT(void**, scrollChild, 0)[7])(
        scrollChild, nullptr, &targetX);
    ((void(__fastcall*)(void*, void*, int32_t*))CSF_AT(void**, scrollChild, 0)[9])(
        scrollChild, nullptr, &sizeW);

    // Re-read after the calls, since they receive pointers to targetX and sizeW.
    int32_t rect[4];
    rect[0] = targetX;
    rect[1] = targetY;
    rect[2] = AddWrap32(targetX, sizeW);
    rect[3] = AddWrap32(targetY, sizeH);

    void* rectReceiver = CSF_AT(void*, base, 0x1C);
    ((void(__fastcall*)(void*, void*, int32_t*, int32_t))CSF_AT(void**, rectReceiver, 0)[130])(
        rectReceiver, nullptr, rect, 0);

    // Scale vector: (0, 0, 0, progress * 1270).
    float velocity[4];
    velocity[0] = 0.0f;
    velocity[1] = 0.0f;
    velocity[2] = 0.0f;
    velocity[3] = CSF_AT(float, base, 0x12C) * g_flt_00841270;

    void* scaleReceiver = CSF_AT(void*, base, 0x38);
    ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, scaleReceiver, 0)[11])(
        scaleReceiver, nullptr, velocity);

    CSF_AT(float, base, 0x12C) = timeStep + CSF_AT(float, base, 0x12C);
}

HOOK(0x0052CAF0, MenuProgressInput_AdvanceTweenStep, "auto");
