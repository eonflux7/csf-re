#include "globals.h"
#include "hook.h"
static float* __cdecl TransformDirections(float* out, float* in, unsigned count, float* m) {
    float* original=in;
    do {
        const double x=in[0], y=in[1], z=in[2];
        const double a=(double(m[0])*x + double(m[4])*y) + double(m[8])*z;
        const double b=(double(m[1])*x + double(m[5])*y) + double(m[9])*z;
        const double c=(x*double(m[2]) + y*double(m[6])) + z*double(m[10]);
        out[0]=float(a); out[1]=float(b); out[2]=float(c);
        in+=3; out+=3;
    } while (--count);
    return original;
}
HOOK(0x0082C07F, TransformDirections, "initfn");
