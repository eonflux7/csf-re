#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(uint32_t*, __fastcall, C_008342e8_SetHandle, 0x00494DC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_CarryOffsetDistance, 0x00833D98); // 72.88

namespace {
struct MatrixRef            // returned by value through vtbl+0x8C
{
    uint32_t* matrix;
    uint8_t   owned;
    uint8_t   pad[3];
};

struct HandleValue          // C_0082d448: { vtbl, index, serial }
{
    void** vtbl;
    int    index;
    int    serial;
};
}

// Keeps the attached object (handle at +0x2D4) placed in front of this entity,
// or clears the handle when the target is gone / no longer valid.
static int __fastcall C_00833ee8_UpdateAttachedObjectPlacement(C_00833ee8* self, void* /*edx*/)
{
    int handleIndex = (int)self->field_02dc;
    if (handleIndex < 0)
        return handleIndex;

    void* attached = (void*)CHandleTable_Resolve(g_pCurrentScene, nullptr, (int)&self->field_02d4);
    if (attached)
    {
        void* attachedInner = CSF_AT(void*, attached, 0x38);
        if (((int(__fastcall*)(void*, void*))CSF_AT(void**, attachedInner, 0)[0xA0 / 4])(attachedInner, nullptr) > 0)
        {
            void* ownVisual = (void*)self->vGetField016c();
            void* ownFrame = (void*)((int(__fastcall*)(void*, void*))CSF_AT(void**, ownVisual, 0)[0x18 / 4])(ownVisual, nullptr);

            MatrixRef matrixRef;
            matrixRef.matrix = nullptr;
            matrixRef.owned = 0;
            MatrixRef* resultRef = ((MatrixRef*(__fastcall*)(void*, void*, MatrixRef*))CSF_AT(void**, ownFrame, 0)[0x8C / 4])(ownFrame, nullptr, &matrixRef);

            // direction[0..2] = forward axis; direction[3] shares the stack slot
            // the original uses for the scaled X offset.
            const float* forwardAxis = (const float*)((uint8_t*)resultRef->matrix + 0x20);
            float direction[4];
            direction[0] = forwardAxis[0];
            direction[1] = forwardAxis[1];
            direction[2] = forwardAxis[2];

            if (matrixRef.owned)
                RwMatrixDestroy(matrixRef.matrix);

            direction[3] = direction[0] * g_CarryOffsetDistance;
            float offsetY = direction[1] * g_CarryOffsetDistance;
            float offsetZ = direction[2] * g_CarryOffsetDistance;

            const float* ownPosition = (const float*)GetObjectPositionVector(self, nullptr);
            float targetPosition[3];
            targetPosition[0] = direction[3] + ownPosition[0];
            targetPosition[1] = offsetY + ownPosition[1];
            targetPosition[2] = offsetZ + ownPosition[2];

            SceneLoader_ConfigureFocusEntry(attached, nullptr, (int*)targetPosition);

            void* attachedVisual = (void*)((int(__fastcall*)(void*, void*))CSF_AT(void**, attached, 0)[0x28 / 4])(attached, nullptr);
            void* attachedFrame = (void*)((int(__fastcall*)(void*, void*))CSF_AT(void**, attachedVisual, 0)[0x18 / 4])(attachedVisual, nullptr);
            return ((int(__fastcall*)(void*, void*, float*))CSF_AT(void**, attachedFrame, 0)[0x58 / 4])(attachedFrame, nullptr, direction);
        }
    }

    HandleValue invalidHandle;
    invalidHandle.vtbl = (void**)CSF_VTBL(0x0082D448);
    invalidHandle.index = -1;
    invalidHandle.serial = -1;
    return (int)C_008342e8_SetHandle(self, nullptr, (int)&invalidHandle);
}

HOOK(0x004974C0, C_00833ee8_UpdateAttachedObjectPlacement, "auto");
