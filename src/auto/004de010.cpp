#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: writes cell row/column through the two out pointers and returns non-zero when the point lies in the grid.
CSF_FUNC(char, __fastcall, ProjectPointToGridCell, 0x004DDF80,
         (void* self, void* edx, float a2, float a3, uint32_t* a4, uint32_t* a5));

static int __fastcall GridPointToSlotOffset(S_004ddf20* self, void* /*edx*/, int a2)
{
    // a2 is a pointer to a 2D point: x at +0, y at +8 (as floats).
    float* point = reinterpret_cast<float*>(static_cast<uintptr_t>(a2));
    float pointX = point[0];
    float pointY = *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(point) + 8);

    uint32_t column = 0;                 // [ebp-4]
    // The callee also writes back through &a2 (the parameter slot), so the row is read from a2 afterwards.
    const char inside = ProjectPointToGridCell(self, nullptr, pointX, pointY,
                                               &column,
                                               reinterpret_cast<uint32_t*>(&a2));
    if (inside)
    {
        uint32_t row = static_cast<uint32_t>(a2);
        uint32_t cellIndex = column + row * self->field_0004;
        return static_cast<int>(self->field_001c + (cellIndex << 4));
    }
    return 0;
}

HOOK(0x004DE010, GridPointToSlotOffset, "auto");
