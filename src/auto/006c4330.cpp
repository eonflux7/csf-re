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

CSF_FUNC(int, __cdecl, RwIm3DTransform, 0x0063CED0, (int a1, unsigned int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwIm3DEnd, 0x0063CFA0, ());
CSF_FUNC(int, __cdecl, RwIm3DRenderIndexedPrimitive, 0x0063CFD0, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwIm3DRenderPrimitive, 0x0063D130, (int a1));

// Flushes the particle's Im3D batch: transforms the vertex data, draws it
// (indexed or plain primitive depending on the index count), then ends the batch.
static int __fastcall ParticleInner_Update(void* self, void* /*edx*/)
{
    int vertexCount = CSF_AT(int, self, 0x0C);
    int result = vertexCount;

    if (vertexCount > 0 && CSF_AT(uint8_t, self, 0x20) != 0)
    {
        int transformFlags = CSF_AT(uint8_t, self, 0x2C) != 0 ? 0x19 : 0;
        result = RwIm3DTransform(
            CSF_AT(int, self, 0x04),
            vertexCount,
            CSF_AT(int, self, 0x24),
            transformFlags);

        if (result != 0)
        {
            int indexCount = CSF_AT(int, self, 0x18);
            if (indexCount <= 0)
            {
                RwIm3DRenderPrimitive(CSF_AT(int, self, 0x1C));
            }
            else
            {
                RwIm3DRenderIndexedPrimitive(
                    CSF_AT(int, self, 0x1C),
                    CSF_AT(int, self, 0x10),
                    indexCount);
            }
            return RwIm3DEnd();
        }
    }
    return result;
}

HOOK(0x006C4330, ParticleInner_Update, "auto");
