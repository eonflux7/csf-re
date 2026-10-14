#include "globals.h"
#include "hook.h"
static float* __cdecl TransformPoint(float* out, float* in, float* m) {
    const double x=in[0], y=in[1], z=in[2];
    const double a=(double(m[0])*x + double(m[4])*y) + double(m[8])*z + m[12];
    const double b=(double(m[1])*x + double(m[5])*y) + double(m[9])*z + m[13];
    const double c=(x*double(m[2]) + y*double(m[6])) + z*double(m[10]) + m[14];
    out[0]=float(a); out[1]=float(b); out[2]=float(c);
    return in;
}
HOOK(0x0082C0EF, TransformPoint, "initfn");
