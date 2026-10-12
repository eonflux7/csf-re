#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 1/512: scale from the bounding-box extent to a 0..511 cell index.
CSF_GLOBAL(float, g_PointToCellScale, 0x0083E10C);

// Returns 1 and writes the two cell coordinates when (x, y) lies inside the box
// [field_000c..field_0010] x [field_0014..field_0018]; otherwise returns 0 and writes nothing.
static char __fastcall S_004ddf20_ConvertPointToCell(S_004ddf20* self, void* /*edx*/,
                                                     float x, float y,
                                                     uint32_t* outCellX, uint32_t* outCellY)
{
    if (x < self->field_000c
        || x > self->field_0010
        || y < self->field_0014
        || y > self->field_0018)
    {
        return 0;
    }

    float cellXValue = (self->field_0010 - self->field_000c - (self->field_0010 - x)) * g_PointToCellScale;
    *outCellX = static_cast<uint32_t>(static_cast<int64_t>(cellXValue));

    float cellYValue = (self->field_0018 - self->field_0014 - (self->field_0018 - y)) * g_PointToCellScale;
    *outCellY = static_cast<uint32_t>(static_cast<int64_t>(cellYValue));

    return 1;
}

HOOK(0x004DDF80, S_004ddf20_ConvertPointToCell, "auto");
