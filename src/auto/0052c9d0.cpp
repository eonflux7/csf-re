#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_MessageBoxCollapsedScale, 0x0082DFCC);
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

// Vtable slot signatures used by this function (__thiscall callees written as __fastcall).
using PtrArgVoidMethod = void(__fastcall*)(void* self, void* edx, int32_t* arg);
using RectFlagVoidMethod = void(__fastcall*)(void* self, void* edx, int32_t* rect, int32_t flag);
using PtrArgIntMethod = int(__fastcall*)(void* self, void* edx, int32_t* arg);

// Equivalent of the MSVC __ftol2 helper: truncate toward zero, keep the low 32 bits of the int64.
static int32_t TruncateToInt32(double value)
{
    return static_cast<int32_t>(static_cast<int64_t>(value));
}

static int32_t AddWrapping32(int32_t a, int32_t b)
{
    return static_cast<int32_t>(static_cast<uint32_t>(a) + static_cast<uint32_t>(b));
}

// Shrinks the container described by fields 0x140..0x14C into a centered "collapsed" box
// (scaled by g_MessageBoxCollapsedScale), stores it at 0x130..0x13C, and pushes the
// resulting rectangle to the child objects. Returns the result of the final vtable call.
static int __fastcall MenuProgressInput_ApplyCollapsedBounds(void* self, void* /*edx*/)
{
    const int32_t containerWidth = CSF_AT(int32_t, self, 0x148);
    CSF_AT(int32_t, self, 0x12C) = 0;

    const int32_t collapsedWidth = TruncateToInt32(
        static_cast<double>(g_MessageBoxCollapsedScale) * static_cast<double>(containerWidth));
    CSF_AT(int32_t, self, 0x138) = collapsedWidth;

    const int32_t containerHeight = CSF_AT(int32_t, self, 0x14C);
    const float containerHeightF = static_cast<float>(containerHeight);
    const int32_t collapsedHeight = TruncateToInt32(
        static_cast<double>(containerHeight) * static_cast<double>(g_MessageBoxCollapsedScale));
    CSF_AT(int32_t, self, 0x13C) = collapsedHeight;

    const double halfScale = static_cast<double>(g_HalfDurationScale);

    const int32_t containerX = CSF_AT(int32_t, self, 0x140);
    const int32_t collapsedX = TruncateToInt32(
        static_cast<double>(containerX)
        + static_cast<double>(containerWidth) * halfScale
        - static_cast<double>(collapsedWidth) * halfScale);
    CSF_AT(int32_t, self, 0x130) = collapsedX;

    const int32_t containerY = CSF_AT(int32_t, self, 0x144);
    const int32_t collapsedY = TruncateToInt32(
        static_cast<double>(containerY)
        + static_cast<double>(containerHeightF) * halfScale
        - static_cast<double>(collapsedHeight) * halfScale);
    CSF_AT(int32_t, self, 0x134) = collapsedY;

    // Child at +0x34: slot 7 receives the top-left point, slot 9 receives the size.
    int32_t* topLeftPtr = reinterpret_cast<int32_t*>(static_cast<uint8_t*>(self) + 0x130);
    reinterpret_cast<PtrArgVoidMethod>(CSF_AT(void**, CSF_AT(void*, self, 0x34), 0)[7])(
        CSF_AT(void*, self, 0x34), nullptr, topLeftPtr);

    int32_t* sizePtr = reinterpret_cast<int32_t*>(static_cast<uint8_t*>(self) + 0x138);
    reinterpret_cast<PtrArgVoidMethod>(CSF_AT(void**, CSF_AT(void*, self, 0x34), 0)[9])(
        CSF_AT(void*, self, 0x34), nullptr, sizePtr);

    // Rectangle {left, top, right, bottom} built from the (possibly updated) position and size.
    const int32_t boundsX = CSF_AT(int32_t, self, 0x130);
    const int32_t boundsY = CSF_AT(int32_t, self, 0x134);
    const int32_t boundsWidth = CSF_AT(int32_t, self, 0x138);
    const int32_t boundsHeight = CSF_AT(int32_t, self, 0x13C);

    int32_t boundsRect[4];
    boundsRect[0] = boundsX;
    boundsRect[1] = boundsY;
    boundsRect[2] = AddWrapping32(boundsWidth, boundsX);
    boundsRect[3] = AddWrapping32(boundsHeight, boundsY);

    // Child at +0x1C: slot 130 takes the rectangle and a zero flag.
    void* rectTarget = CSF_AT(void*, self, 0x1C);
    reinterpret_cast<RectFlagVoidMethod>(CSF_AT(void**, rectTarget, 0)[130])(
        rectTarget, nullptr, boundsRect, 0);

    // Child at +0x38: slot 11 receives an all-zero rectangle; its result is the function result.
    int32_t zeroRect[4] = {0, 0, 0, 0};
    void* resultTarget = CSF_AT(void*, self, 0x38);
    const int result = reinterpret_cast<PtrArgIntMethod>(CSF_AT(void**, resultTarget, 0)[11])(
        resultTarget, nullptr, zeroRect);

    CSF_AT(int32_t, self, 0x20) = 0;
    return result;
}

HOOK(0x0052C9D0, MenuProgressInput_ApplyCollapsedBounds, "auto");
