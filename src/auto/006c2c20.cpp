#include "hook.h"

static float* __cdecl GfxAnimCompress_ComposeKeyframes(
    float* output, float* first, float* second)
{
    output[5] = second[5] * first[5]
        - (second[3] * first[3] + second[2] * first[2]
           + first[4] * second[4]);

    output[2] = second[4] * first[3] - second[3] * first[4];
    output[3] = second[2] * first[4] - second[4] * first[2];
    output[4] = second[3] * first[2] - second[2] * first[3];

    output[2] = second[2] * first[5] + output[2];
    output[3] = second[3] * first[5] + output[3];
    output[4] = second[4] * first[5] + output[4];

    output[2] = second[5] * first[2] + output[2];
    output[3] = second[5] * first[3] + output[3];
    output[4] = second[5] * first[4] + output[4];

    output[6] = first[6] + second[6];
    output[7] = first[7] + second[7];
    output[8] = first[8] + second[8];

    return second;
}

HOOK(0x006C2C20, GfxAnimCompress_ComposeKeyframes, "auto");
