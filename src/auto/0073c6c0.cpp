#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// ---------------------------------------------------------------------------
// Commandos: Strike Force (Pyro Studios, 2006)
//
// 0x0073C6C0   char __stdcall (float start, float end, float step)   ret 0xc
//
// The original is a 14-byte thunk: `mov ecx, 0x8BABC0` (address of the global
// float-helper object) followed by a tail jump to 0x0068F670. The routine at
// 0x0068F670 is flattened inline below. It validates the (start,end,step)
// triple and, when valid, performs one linear interpolation step through
// 0x0068F560; otherwise it reports the failure through 0x00646D30 and
// returns 0.
// ---------------------------------------------------------------------------

// 0x0082D4CC : float bound the step is compared against (0.0f).
CSF_GLOBAL(float, g_Value_0082D4CC, 0x82D4CC);

// 0x008BABC0 : global float-helper object whose address the original loads
// into ecx with `mov ecx, 0x8BABC0`.
CSF_GLOBAL(uint8_t, g_FloatHelper_008BABC0, 0x8BABC0);

// 0x00646D30 : __cdecl failure/diagnostics reporter (caller pops 16 bytes).
CSF_FUNC(void, __cdecl, ReportInterpolationFailure, 0x646D30,
         (int a, int b, int code, int c));

// 0x0068F560 : __stdcall interpolation helper, 4 stack args (callee pops 16).
CSF_FUNC(void, __stdcall, InterpolateSegment, 0x68F560,
         (float from, float t, float zero, float to));

static char __stdcall InterpolateRangeChecked(float start, float end, float step)
{
    // Original first instruction: `mov ecx, 0x8BABC0`. The helper object's
    // address is pushed only as a scratch slot that is immediately overwritten
    // by the `fstp` of the interpolation factor, so the value is dead, but the
    // reference to the global must be preserved: the volatile touch keeps it
    // alive in the generated code.
    (void)*(volatile uint8_t*)&g_FloatHelper_008BABC0;

    // First test (test ah,41h / jnp): the original advances only when the
    // compare is neither "below" nor "equal", i.e. step > bound, and it also
    // advances on an unordered compare (NaN). `!(step <= bound)` reproduces
    // both behaviours. Second test (test ah,05h / jp): advances while end is
    // not less than start, again treating unordered as advancing, hence
    // `!(end < start)`.
    if (!(step <= g_Value_0082D4CC) && !(end < start))
    {
        float factor = (end - start) / step;
        InterpolateSegment(start, factor, 0.0f, end);
        return 1;
    }

    ReportInterpolationFailure(0, 0, -536674299, 0);  // 0xE0030005
    return 0;
}

HOOK(0x73C6C0, InterpolateRangeChecked, "auto");
