#include "globals.h"
#include "hook.h"
static float* __cdecl MultiplyMatricesAlternate(float* out, float* in, float* m) {
    float* original=in;
    for (unsigned row=0; row<4; ++row) {
        const double x=in[0], y=in[1], z=in[2];
        const double a=(double(m[0])*x+double(m[4])*y)+double(m[8])*z+(row==3 ? m[12]:0.0);
        const double b=(double(m[1])*x+double(m[5])*y)+double(m[9])*z+(row==3 ? m[13]:0.0);
        const double c=(z*double(m[10])+y*double(m[6]))+x*double(m[2])+(row==3 ? m[14]:0.0);
        out[0]=float(a); out[1]=float(b); out[2]=float(c);
        in+=4; out+=4;
    }
    return original;
}
HOOK(0x0082C2A0, MultiplyMatricesAlternate, "initfn");
