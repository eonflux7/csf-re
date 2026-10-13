#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(char, __fastcall, S_00405180_TraceGroundWithState, 0x004053A0, (S_00405180* self, void* , float* a2, float* a3, int* a4));
CSF_FUNC(void*, __fastcall, SpatialQuery_InitializeQueryInfo, 0x0040E070, (void* self, void*));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GfxMatrix_InitializeQuaternion, 0x006AE6D0, (uint32_t* self, void*));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionAxisAngle, 0x006AE970, (float* self, void* , S_00526d40* axis, float angle));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static char __fastcall C_008317c8_UpdateLinkedProjectilePosition(
    C_008317c8* self, void* /*edx*/)
{
    if (static_cast<std::int32_t>(self->field_006c) < 0)
        return 0;

    auto* projectile = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x64)));
    if (!projectile)
        return 0;

    self->vSlotE4();

    void* model = (void*)projectile->vGetField016c();
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, model, 0)[0x3C / 4])(model, nullptr, 1);

    auto* attachedEffect = reinterpret_cast<C_008672e4*>(projectile->field_0034);
    if (attachedEffect)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, attachedEffect, 0)[0xA4 / 4])(
                attachedEffect, nullptr, 1);
    }

    alignas(8) std::uint8_t queryBytes[0x64];
    alignas(8) std::uint8_t directionBytes[0xC];
    // The line query, quaternion, and returned vector share this stack region.
    alignas(8) std::uint8_t auxiliaryBytes[0x20];
    alignas(8) std::uint8_t originBytes[0xC];
    alignas(8) std::uint8_t savedMatrixBytes[0x8];
    alignas(8) std::uint8_t destinationBytes[0xC];
    alignas(8) std::uint8_t temporaryMatrixBytes[0xC];
    alignas(8) std::uint8_t stateBytes[0x8];

    auto* query = reinterpret_cast<float*>(queryBytes);
    auto* direction = reinterpret_cast<float*>(directionBytes);
    auto* origin = reinterpret_cast<float*>(originBytes);
    auto* destination = reinterpret_cast<float*>(destinationBytes);
    auto* line = reinterpret_cast<float*>(auxiliaryBytes);
    auto* quaternion = reinterpret_cast<float*>(auxiliaryBytes + 0x8);
    auto* vectorResult = reinterpret_cast<std::uint32_t*>(auxiliaryBytes + 0xC);
    auto* state = reinterpret_cast<int*>(stateBytes);

    destination[0] = 0.0f;
    destination[1] = 0.0f;
    destination[2] = 0.0f;

    void* owner = reinterpret_cast<void*>(self->field_0004);
    *reinterpret_cast<std::uint32_t**>(savedMatrixBytes) = RwMatrixCreate();
    savedMatrixBytes[4] = 1;

    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, owner, 0)[0x34 / 4])(owner, nullptr, origin);

    owner = reinterpret_cast<void*>(self->field_0004);
    if (!CSF_AT(std::uint32_t, owner, 0x50) &&
        !CSF_AT(std::uint8_t, owner, 0x228))
    {
        void* ownerModel = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
        void* transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, ownerModel, 0)[0x18 / 4])(ownerModel, nullptr);
        void* matrixReference =
            reinterpret_cast<void*(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, transform, 0)[0x8C / 4])(
                    transform, nullptr, temporaryMatrixBytes);
        std::memcpy(
            *reinterpret_cast<std::uint32_t**>(savedMatrixBytes),
            CSF_AT(void*, matrixReference, 0), 0x40);
        if (temporaryMatrixBytes[4])
            RwMatrixDestroy(
                *reinterpret_cast<std::uint32_t**>(temporaryMatrixBytes));

        int globalValue = GetGlobalValue_4FBDE0();
        int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
        void* provider = GetCachedObjectProvider(
            reinterpret_cast<void*>(firstElement), nullptr);
        void* referencedObject = GetReferencedObject(provider, nullptr);
        void* sourceTransform =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, referencedObject, 0)[0x10 / 4])(
                    referencedObject, nullptr);

        owner = reinterpret_cast<void*>(self->field_0004);
        ownerModel = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
        transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, ownerModel, 0)[0x18 / 4])(ownerModel, nullptr);

        void** destinationVtable = CSF_AT(void**, transform, 0);
        matrixReference =
            reinterpret_cast<void*(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, sourceTransform, 0)[0x8C / 4])(
                    sourceTransform, nullptr, temporaryMatrixBytes);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            destinationVtable[0x94 / 4])(transform, nullptr, matrixReference);

        if (temporaryMatrixBytes[4])
            RwMatrixDestroy(
                *reinterpret_cast<std::uint32_t**>(temporaryMatrixBytes));
    }

    switch (CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(self->field_0040), 0x74))
    {
    case 6:
    case 7:
    case 0xA:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x18:
    case 0x19:
    {
        owner = reinterpret_cast<void*>(self->field_0004);
        void* ownerModel = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
        void* attachmentProvider =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, ownerModel, 0)[0x68 / 4])(ownerModel, nullptr);
        int attachmentId = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, attachmentProvider, 0)[0x68 / 4])(
                attachmentProvider, nullptr,
                CSF_AT(std::uint32_t,
                       reinterpret_cast<void*>(self->field_0040), 0x150) != 1
                    ? 40 : 50);
        void* attachment =
            reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, attachmentProvider, 0)[0x6C / 4])(
                    attachmentProvider, nullptr, attachmentId);
        if (attachment)
        {
            float* position =
                reinterpret_cast<float*(__fastcall*)(void*, void*, std::uint32_t*)>(
                    CSF_AT(void**, attachment, 0)[0x24 / 4])(
                        attachment, nullptr, vectorResult);
            destination[0] = position[0];
            destination[1] = position[1];
            destination[2] = position[2];
        }
        break;
    }
    case 0x13:
    {
        void* source = reinterpret_cast<void*>(self->vResolveEntryLookupValue());
        void* sourceTransform =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, source, 0)[0x18 / 4])(source, nullptr);
        model = (void*)projectile->vGetField016c();
        void* transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, model, 0)[0x18 / 4])(model, nullptr);
        void** destinationVtable = CSF_AT(void**, transform, 0);
        void* matrixReference =
            reinterpret_cast<void*(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, sourceTransform, 0)[0x8C / 4])(
                    sourceTransform, nullptr, temporaryMatrixBytes);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            destinationVtable[0x94 / 4])(transform, nullptr, matrixReference);
        if (temporaryMatrixBytes[4])
            RwMatrixDestroy(
                *reinterpret_cast<std::uint32_t**>(temporaryMatrixBytes));

        float* position = GetObjectPositionVector(projectile, nullptr);
        destination[0] = position[0];
        destination[1] = position[1];
        destination[2] = position[2];
        break;
    }
    default:
    {
        void* source = reinterpret_cast<void*>(self->vSlotD8());
        if (source)
        {
            void* transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, source, 0)[0x18 / 4])(source, nullptr);
            float* position = reinterpret_cast<float*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);
            destination[0] = position[0];
            destination[1] = position[1];
            destination[2] = position[2];
        }
        break;
    }
    }

    model = (void*)projectile->vGetField016c();
    void* transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, model, 0)[0x18 / 4])(model, nullptr);
    void** projectileVtable = CSF_AT(void**, projectile, 0);
    int transformValue = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, transform, 0)[0x48 / 4])(transform, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
        projectileVtable[0x4C / 4])(
            projectile, nullptr, reinterpret_cast<int>(origin), transformValue);

    void* dispatchedObject = C_0082dd78_DispatchReturnedObject(
        reinterpret_cast<void*>(projectile->field_004c), nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, dispatchedObject, 0)[0x28 / 4])(dispatchedObject, nullptr);

    direction[0] = destination[0] - origin[0];
    direction[1] = destination[1] - origin[1];
    direction[2] = destination[2] - origin[2];
    SpatialQuery_InitializeQueryInfo(query, nullptr);
    state[0] = 2;
    char obstructed = S_00405180_TraceGroundWithState(
        reinterpret_cast<S_00405180*>(
            reinterpret_cast<C_0082dd78*>(projectile->field_004c)->field_0008),
        nullptr, direction, query, state);

    if (!obstructed)
    {
        int globalValue = GetGlobalInt();
        void* sceneController = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(reinterpret_cast<void*>(globalValue), nullptr));
        void* collisionWorld =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, sceneController, 0)[0x34 / 4])(
                    sceneController, nullptr);
        line[0] = origin[0];
        line[2] = origin[2];
        line[4] = destination[1];
        line[1] = origin[1];
        line[3] = destination[0];
        line[5] = destination[2];
        state[0] = 2;
        if (reinterpret_cast<int(__fastcall*)(void*, void*, int, float*, int*, int)>(
                CSF_AT(void**, collisionWorld, 0)[0x34 / 4])(
                    collisionWorld, nullptr, 0, line, state, 8))
        {
            obstructed = 1;
        }
        else
        {
            model = (void*)projectile->vGetField016c();
            transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, model, 0)[0x18 / 4])(model, nullptr);
            projectileVtable = CSF_AT(void**, projectile, 0);
            transformValue = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, transform, 0)[0x48 / 4])(transform, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
                projectileVtable[0x4C / 4])(
                    projectile, nullptr, reinterpret_cast<int>(destination),
                    transformValue);
        }
    }

    if (CSF_AT(std::uint32_t,
               reinterpret_cast<void*>(self->field_0040), 0x74) == 10)
    {
        model = (void*)projectile->vGetField016c();
        int attachmentId = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, model, 0)[0x98 / 4])(model, nullptr, 2200);
        if (attachmentId != -1)
        {
            transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, model, 0)[0x18 / 4])(model, nullptr);
            owner = reinterpret_cast<void*>(self->field_0004);
            void* ownerModel = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
            void* ownerTransform =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, ownerModel, 0)[0x18 / 4])(ownerModel, nullptr);
            void** transformVtable = CSF_AT(void**, transform, 0);
            float scale = static_cast<float>(
                reinterpret_cast<double(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, ownerTransform, 0)[0x28 / 4])(
                        ownerTransform, nullptr));
            reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
                transformVtable[0x34 / 4])(transform, nullptr, scale);

            void* attachment =
                reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, model, 0)[0x8C / 4])(
                        model, nullptr, attachmentId);
            void* attachmentTransform =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, attachment, 0)[0x18 / 4])(attachment, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, attachmentTransform, 0)[0x8C / 4])(
                    attachmentTransform, nullptr, temporaryMatrixBytes);
            GfxMatrix_InitializeQuaternion(
                reinterpret_cast<std::uint32_t*>(quaternion), nullptr);
            GfxMatrix_SetQuaternionAxisAngle(
                quaternion, nullptr,
                *reinterpret_cast<S_00526d40**>(temporaryMatrixBytes), 1.5707964f);
            reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, transform, 0)[0x60 / 4])(transform, nullptr, quaternion);
            if (temporaryMatrixBytes[4])
                RwMatrixDestroy(
                    *reinterpret_cast<std::uint32_t**>(temporaryMatrixBytes));
        }
    }

    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, projectile, 0)[0x6C / 4])(projectile, nullptr, 0);
    owner = reinterpret_cast<void*>(self->field_0004);
    if (!CSF_AT(std::uint32_t, owner, 0x50) &&
        !CSF_AT(std::uint8_t, owner, 0x228))
    {
        void* ownerModel = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
        void* ownerTransform =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, ownerModel, 0)[0x18 / 4])(ownerModel, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, ownerTransform, 0)[0x94 / 4])(
                ownerTransform, nullptr, savedMatrixBytes);
    }
    if (savedMatrixBytes[4])
        RwMatrixDestroy(*reinterpret_cast<std::uint32_t**>(savedMatrixBytes));
    return obstructed;
}

HOOK(0x00478C90, C_008317c8_UpdateLinkedProjectilePosition, "auto");
