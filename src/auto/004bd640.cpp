#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GameConfig_IsSceneObjectRemovable, 0x007558C0, (int objectType));

// Fills a 16-byte info record {vtable, x, y, sceneDataId}. x/y are the removable
// object's fields at +0x18C/+0x190 when the handle resolves to a removable object,
// otherwise -1. Returns the record.
static int* __fastcall S_004bd310_FillHandleInfo(S_004bd310* self, void* /*edx*/, int* out, int a2)
{
    // Scene data id: g_pCurrentScene->[+0xB0]->[+8]
    int sceneDataId = CSF_AT(int, CSF_AT(void*, g_pCurrentScene, 0xB0), 8);

    int resultX = -1;
    int resultY = -1;

    int ownerFieldValue = CSF_AT(int, reinterpret_cast<void*>(self->field_0000), 0x20);
    int requestedField4 = CSF_AT(int, reinterpret_cast<void*>(a2), 4);

    if (ownerFieldValue != requestedField4)
    {
        void* sceneObject = reinterpret_cast<void*>(CHandleTable_Resolve(g_pCurrentScene, nullptr, a2));
        if (sceneObject)
        {
            if (GameConfig_IsSceneObjectRemovable(CSF_AT(int, sceneObject, 0x50)) & 0xFF)
            {
                resultX = CSF_AT(int, sceneObject, 0x18C);
                resultY = CSF_AT(int, sceneObject, 0x190);
            }
        }
    }

    out[1] = resultX;
    out[2] = resultY;
    *reinterpret_cast<void***>(out) = CSF_VTBL(0x0082D448);
    out[3] = sceneDataId;
    return out;
}

HOOK(0x004BD640, S_004bd310_FillHandleInfo, "auto");
