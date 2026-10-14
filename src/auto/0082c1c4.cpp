#include "globals.h"
#include "hook.h"
// x87 accumulates the third component in reverse order in this variant.
static void __cdecl MultiplyMatrices(float* out, float* in, float* m) {
    for (unsigned row=0; row<4; ++row) {
        const double x=in[0], y=in[1], z=in[2];
        const double a=row==3 ? ((x*double(m[0])+m[12])+y*double(m[4]))+z*double(m[8])
                              : (x*double(m[0])+y*double(m[4]))+z*double(m[8]);
        const double b=row==3 ? ((x*double(m[1])+m[13])+y*double(m[5]))+z*double(m[9])
                              : (x*double(m[1])+y*double(m[5]))+z*double(m[9]);
        const double cx=row==3 ? x*double(m[2])+m[14] : x*double(m[2]);
        const double c=(z*double(m[10])+y*double(m[6]))+cx;
        out[0]=float(a); out[1]=float(b); out[2]=float(c);
        in+=4; out+=4;
    }
}
HOOK(0x0082C1C4, MultiplyMatrices, "initfn");
