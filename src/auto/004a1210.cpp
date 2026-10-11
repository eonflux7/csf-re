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

// Data at 0x8343F8 / 0x8343FC: small positive / negative tolerance constants.
CSF_GLOBAL(float, g_PositiveDistanceTolerance, 0x8343F8);
CSF_GLOBAL(float, g_NegativeDistanceTolerance, 0x8343FC);

// Returns TRUE when either field_000c or field_0010 lies outside the
// [-tolerance, +tolerance] band, i.e. the timer value is non-zero.
static int __fastcall S_004a1210_IsToggleTimerActive(S_004a1210* self, void* /*edx*/)
{
    if (self->field_000c > g_PositiveDistanceTolerance)
        return 1;
    if (self->field_000c < g_NegativeDistanceTolerance)
        return 1;
    if (self->field_0010 > g_PositiveDistanceTolerance)
        return 1;
    if (self->field_0010 < g_NegativeDistanceTolerance)
        return 1;
    return 0;
}

HOOK(0x004A1210, S_004a1210_IsToggleTimerActive, "auto");
