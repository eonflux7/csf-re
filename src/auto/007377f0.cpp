#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, RwV3dTransformPoints, 0x00623040,
         (float* out, const float* points, int count, void* matrix));

// Transforms the quad's four corners by the matrix at +0x84 and writes each vertex's screen x/y (stride 0x1C
// from +0x14) minus the owner's pixel offset (slot 0x20 of the object at +0xC, re-read for every coordinate).
// Corners from (x, y) = (+0x34, +0x38), size (+0x4C, +0x50); +0x8C mirrors the quad horizontally.
static std::uint32_t __fastcall C_00867260_CommitQuad(void* self, void* /*edx*/)
{
    const float x = CSF_AT(float, self, 0x34);
    const float y = CSF_AT(float, self, 0x38);
    const float right = x + CSF_AT(float, self, 0x4C);
    const float bottom = y + CSF_AT(float, self, 0x50);
    const float corners[2][4][2] = {
        {{x, y}, {x, bottom}, {right, y}, {right, bottom}},
        {{right, y}, {x, y}, {right, bottom}, {x, bottom}},
    };
    const auto& quad = corners[CSF_AT(std::uint8_t, self, 0x8C) ? 1 : 0];

    for (int i = 0; i < 4; ++i)
    {
        float point[3] = {quad[i][0], quad[i][1], 0.0f};
        float out[3];
        RwV3dTransformPoints(out, point, 1, CSF_AT(void*, self, 0x84));
        for (int axis = 0; axis < 2; ++axis)
        {
            void* owner = CSF_AT(void*, self, 0x0C);
            const float offset =
                reinterpret_cast<float(__fastcall*)(void*, void*)>(CSF_AT(void**, owner, 0)[0x20 / 4])(owner, nullptr);
            CSF_AT(float, CSF_AT(std::uint8_t*, self, 0x14) + i * 0x1C, axis * 4) = out[axis] - offset;
        }
    }
    return CSF_AT(std::uint32_t, self, 0x14);
}

HOOK(0x007377F0, C_00867260_CommitQuad, "auto");
