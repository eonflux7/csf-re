#include "globals.h"
#include "hook.h"

// Writes into `out` the anchor point of an alert icon: x = this.x + this.z-ish pair
// taken from fields 0x0C/0x10 (added to 0x34/0x38), y likewise. Returns `out`.
static float* __fastcall ComputeAlertIconAnchorPosition(float* self, void* /*edx*/, float* out)
{
    float sumY = CSF_AT(float, self, 0x38) + CSF_AT(float, self, 0x10);
    out[0] = CSF_AT(float, self, 0x34) + CSF_AT(float, self, 0x0C);
    out[1] = sumY;
    return out;
}

HOOK(0x4A1520, ComputeAlertIconAnchorPosition, "auto");
