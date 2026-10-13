#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_LandingThresholdScale, 0x0082E050);
CSF_GLOBAL(float, g_flt_0082E25C, 0x0082E25C);

static void __fastcall C_008655e8_ClampVertexCoordinates(
    C_008655e8* self, void* /*edx*/, float* vertices, int vertexCount)
{
    for (int i = 0; i < vertexCount; ++i, vertices += 7)
    {
        if (vertices[0] < g_LandingThresholdScale)
        {
            vertices[0] = -1000.0f;
        }
        else
        {
            const float maximumX =
                static_cast<float>(static_cast<int32_t>(self->field_0010))
                + g_flt_0082E25C;
            if (vertices[0] > maximumX)
                vertices[0] = maximumX;
        }

        if (vertices[1] < g_LandingThresholdScale)
        {
            vertices[1] = -1000.0f;
        }
        else
        {
            const float maximumY =
                static_cast<float>(static_cast<int32_t>(self->field_0014))
                + g_flt_0082E25C;
            if (vertices[1] > maximumY)
                vertices[1] = maximumY;
        }
    }
}

HOOK(0x007264C0, C_008655e8_ClampVertexCoordinates, "auto");
