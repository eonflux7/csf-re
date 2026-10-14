#include <cstdint>
#include <cmath>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (uint32_t* matrix));
CSF_FUNC(int, __fastcall, ScaleMatrix, 0x006ADB60,
         (void* self, void* edx, uint32_t x, uint32_t y, uint32_t z));
CSF_FUNC(int, __fastcall, TranslateMatrixHandle, 0x006ADBB0,
         (void* self, void* edx, uint32_t x, uint32_t y, uint32_t z));
CSF_FUNC(int, __fastcall, RotateMatrixHandle, 0x006AE210,
         (void* self, void* edx, float radians));
CSF_FUNC(int, __fastcall, C_00865a10_InvokeWithScaledFourthComponent, 0x00728AF0,
         (C_00865a10* self, void* edx, void* texture));

CSF_GLOBAL(float, g_DegreesToRadians, 0x008656B0);
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_EdgeAxisEpsilon, 0x008656B4);

namespace {

struct MatrixHandle
{
    uint32_t* matrix;
    char owned;
};

uint32_t Bits(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, 4);
    return bits;
}

template <typename Fn>
Fn Slot(void* obj, int offset)
{
    return reinterpret_cast<Fn>(CSF_AT(void**, obj, 0)[offset / 4]);
}

void ResetToIdentity(uint32_t* m)
{
    m[10] = m[5] = m[0] = 0x3F800000;
    m[4] = m[2] = m[1] = 0;
    m[9] = m[8] = m[6] = 0;
    m[14] = m[13] = m[12] = 0;
    m[3] |= 0x20003;
}

}  // namespace

// Lays the edge texture (+0x54) along the segment start..end, tile by tile (+0x58 tile length, read back from
// the texture through slot 0x34): every whole tile is drawn (slot 8) with its colour alpha scaled by +0x44,
// then the last partial tile is stretched to fit and left as the texture's transform. Horizontal when the
// segment's y difference is within 1e-6, vertical otherwise; `angleDegrees` rotates around the segment start.
static char __fastcall C_00865a10_ApplyEdgeTextureTransform(C_00865a10* self, void* /*edx*/, float angleDegrees,
                                                            float* start, float* end)
{
    MatrixHandle handle;
    handle.matrix = RwMatrixCreate();
    handle.owned = 1;

    float* tile = &CSF_AT(float, self, 0x58);
    void* texture = CSF_AT(void*, self, 0x54);
    Slot<void(__fastcall*)(void*, void*, float*)>(texture, 0x34)(texture, nullptr, tile);
    float uv[4] = {0.0f, 0.0f, 1.0f, 1.0f};
    texture = CSF_AT(void*, self, 0x54);
    Slot<void(__fastcall*)(void*, void*, float*)>(texture, 0x4C)(texture, nullptr, uv);

    float pivot[2] = {start[0], start[1]};
    float position = *tile;
    const double halfTile = static_cast<double>(*tile) * g_HalfDurationScale;
    const bool horizontal = std::fabs(end[1] - start[1]) <= g_EdgeAxisEpsilon;
    const int axis = horizontal ? 0 : 1;

    pivot[axis] = static_cast<float>(halfTile + pivot[axis]);
    const float length = end[axis] - start[axis];
    texture = CSF_AT(void*, self, 0x54);
    Slot<void(__fastcall*)(void*, void*, float*)>(texture, 0x24)(texture, nullptr, pivot);

    if (position <= length)
    {
        const float radians = static_cast<float>(static_cast<double>(angleDegrees) * g_DegreesToRadians);
        do
        {
            ResetToIdentity(handle.matrix);
            TranslateMatrixHandle(&handle, nullptr, Bits(-pivot[0]), Bits(-pivot[1]), 0);
            RotateMatrixHandle(&handle, nullptr, radians);
            TranslateMatrixHandle(&handle, nullptr, Bits(pivot[0]), Bits(pivot[1]), 0);
            const uint32_t offset = Bits(position - *tile);
            if (horizontal)
                TranslateMatrixHandle(&handle, nullptr, offset, 0, 0);
            else
                TranslateMatrixHandle(&handle, nullptr, 0, offset, 0);

            texture = CSF_AT(void*, self, 0x54);
            Slot<void(__fastcall*)(void*, void*, MatrixHandle*)>(texture, 0x60)(texture, nullptr, &handle);
            texture = CSF_AT(void*, self, 0x54);
            const float* colour = Slot<const float*(__fastcall*)(void*, void*)>(texture, 0x6C)(texture, nullptr);
            float original[4] = {colour[0], colour[1], colour[2], colour[3]};
            float faded[4] = {colour[0], colour[1], colour[2], colour[3] * CSF_AT(float, self, 0x44)};
            Slot<void(__fastcall*)(void*, void*, float*)>(texture, 0x68)(texture, nullptr, faded);
            Slot<void(__fastcall*)(void*, void*)>(texture, 0x08)(texture, nullptr);
            Slot<void(__fastcall*)(void*, void*, float*)>(texture, 0x68)(texture, nullptr, original);
            position = position + *tile;
        } while (position <= length);
    }

    position = position - *tile;
    const double rest = static_cast<double>(end[axis]) - start[axis] - position;
    const float restFloat = static_cast<float>(rest);
    const float stretch = static_cast<float>(rest / *tile);
    texture = CSF_AT(void*, self, 0x54);
    const float* current = Slot<const float*(__fastcall*)(void*, void*)>(texture, 0x50)(texture, nullptr);
    float stretched[4] = {current[0], current[1], stretch, current[3]};
    texture = CSF_AT(void*, self, 0x54);
    Slot<void(__fastcall*)(void*, void*, float*)>(texture, 0x4C)(texture, nullptr, stretched);

    pivot[axis] = static_cast<float>(static_cast<double>(restFloat) * g_HalfDurationScale + start[axis]);
    texture = CSF_AT(void*, self, 0x54);
    Slot<void(__fastcall*)(void*, void*, float*)>(texture, 0x24)(texture, nullptr, pivot);

    ResetToIdentity(handle.matrix);
    TranslateMatrixHandle(&handle, nullptr, Bits(-pivot[0]), Bits(-pivot[1]), 0);
    RotateMatrixHandle(&handle, nullptr, static_cast<float>(static_cast<double>(angleDegrees) * g_DegreesToRadians));
    if (horizontal)
        ScaleMatrix(&handle, nullptr, Bits(stretch), 0x3F800000, 0);
    else
        ScaleMatrix(&handle, nullptr, 0x3F800000, Bits(stretch), 0);
    TranslateMatrixHandle(&handle, nullptr, Bits(pivot[0]), Bits(pivot[1]), 0);
    if (horizontal)
        TranslateMatrixHandle(&handle, nullptr, Bits(position), 0, 0);
    else
        TranslateMatrixHandle(&handle, nullptr, 0, Bits(position), 0);

    texture = CSF_AT(void*, self, 0x54);
    Slot<void(__fastcall*)(void*, void*, MatrixHandle*)>(texture, 0x60)(texture, nullptr, &handle);
    C_00865a10_InvokeWithScaledFourthComponent(self, nullptr, CSF_AT(void*, self, 0x54));

    const char owned = handle.owned;
    if (owned)
        return static_cast<char>(RwMatrixDestroy(handle.matrix));
    return owned;
}

HOOK(0x00729710, C_00865a10_ApplyEdgeTextureTransform, "auto");
