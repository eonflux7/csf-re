#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, UpdateAttachedObjectTransform, 0x004133A0, (void* self, void*));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(void*, __fastcall, GetContextTransform, 0x00495890, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Called when a scene object has been updated: refreshes the attached object's transform when the
// updated object is the one our attachment follows, and, when it is the object we are linked to by
// handle, recomputes the cached aiming direction (the "at" axis of the context transform).
static void __fastcall C_008342e8_OnSceneObjectUpdated(C_008342e8* self, void* /*edx*/, void* updatedObject)
{
    void* attachment = reinterpret_cast<void*>(self->field_004c);
    void* attachmentOwner =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, attachment, 0)[0x4C / 4])(attachment, nullptr);
    if (attachmentOwner != nullptr && updatedObject == attachmentOwner)
    {
        UpdateAttachedObjectTransform(reinterpret_cast<void*>(self->field_004c), nullptr);
    }

    if ((CSF_AT(uint32_t, self, 0x2E0) & 0x2000) == 0)
        return;
    if (CSF_AT(uint8_t, self, 0x228) != 0)
        return;
    if (CSF_AT(uint8_t, self, 0x3A5) == 0)
        return;

    void* linkedObject = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x1B4)));
    if (linkedObject == nullptr || linkedObject != updatedObject)
        return;

    void* model = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, linkedObject, 0)[0x28 / 4])(linkedObject, nullptr);
    int nodeIndex = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, model, 0)[0x98 / 4])(model, nullptr, 3000);
    if (nodeIndex == -1)
        return;

    model = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, linkedObject, 0)[0x28 / 4])(linkedObject, nullptr);
    void* node = ((void*(__fastcall*)(void*, void*, int))CSF_AT(void**, model, 0)[0x8C / 4])(model, nullptr, nodeIndex);

    void* transform = GetContextTransform(self, nullptr);

    void* nodeFrame = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, node, 0)[0x18 / 4])(node, nullptr);
    void** transformVtbl = CSF_AT(void**, transform, 0);
    int nodeMatrix = ((int(__fastcall*)(void*, void*))CSF_AT(void**, nodeFrame, 0)[0x10 / 4])(nodeFrame, nullptr);
    ((void(__fastcall*)(void*, void*, int))transformVtbl[0x14 / 4])(transform, nullptr, nodeMatrix);

    ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, transform, 0)[0x30 / 4])(
        transform, nullptr, CSF_AT(uint32_t, self, 0x1B0));

    transformVtbl = CSF_AT(void**, transform, 0);
    float baseAngle;
    float linkedScalar =
        static_cast<float>(C_008342e8_ComputeResolvedObjectScalar(reinterpret_cast<C_008342e8*>(linkedObject), nullptr));
    std::memcpy(&baseAngle, &self->field_01ac, sizeof(float));
    float angle = linkedScalar + baseAngle;
    ((void(__fastcall*)(void*, void*, float))transformVtbl[0x34 / 4])(transform, nullptr, angle);

    // Small on-stack holder filled by the callee: { matrix pointer, owns-matrix flag }.
    alignas(8) uint8_t matrixHolderBytes[8] = {};
    void* holderResult = ((void*(__fastcall*)(void*, void*, void*))CSF_AT(void**, transform, 0)[0x8C / 4])(
        transform, nullptr, matrixHolderBytes);

    uint8_t* atVector = CSF_AT(uint8_t*, holderResult, 0) + 0x20;
    CSF_AT(uint32_t, self, 0x3A8) = CSF_AT(uint32_t, atVector, 0);
    CSF_AT(uint32_t, self, 0x3AC) = CSF_AT(uint32_t, atVector, 4);
    bool ownsMatrix = CSF_AT(uint8_t, matrixHolderBytes, 4) != 0;
    CSF_AT(uint32_t, self, 0x3B0) = CSF_AT(uint32_t, atVector, 8);

    if (ownsMatrix)
    {
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixHolderBytes, 0));
    }
}

HOOK(0x004959A0, C_008342e8_OnSceneObjectUpdated, "auto");
