#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* minimum, float* maximum));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, CharActionFsm_ForwardToStateDispatch, 0x00482840, (void* self, void* edx, int a2));
CSF_FUNC(C_00833908*, __fastcall, CCharActionFsm_SetActionMode, 0x00484420, (CCharActionFsm* self, void* , char a2));
CSF_FUNC(C_00833908*, __fastcall, CCharActionFsm_ProcessCurrentItem, 0x004844C0, (CCharActionFsm* self, void*));
CSF_FUNC(C_00833908*, __fastcall, CCharActionFsm_ResolveActionTarget_488430, 0x00488430, (CCharActionFsm* self, void*));
CSF_FUNC(C_00833908*, __fastcall, CCharActionFsm_ResolveActionTarget_4884B0, 0x004884B0, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_IsSecondCheckTrue, 0x0048E140, (void* self, void* edx));
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, int payload));
CSF_FUNC(double, __fastcall, C_00833ee8_GetContextHeightOffset, 0x004954A0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, C_008342e8_IsContextActionAvailable, 0x00495730, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetContextTransform, 0x00495890, ());
CSF_FUNC(void, __fastcall, C_008342e8_PrepareContextOffset, 0x004963E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008342e8_UpdateContextAction, 0x00499F00, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, ObjectProvider_SetPendingRequest, 0x004F5EF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetMatrixEulerAngles, 0x006AE3A0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_AxisX, 0x008A7D6C);
CSF_GLOBAL(float, g_AxisZ, 0x008A7D84);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

// The matrix reference passed to virtual accessors includes an ownership byte.
struct ContextMatrixReference {
    std::uint32_t* matrix;
    std::uint8_t owned;
    std::uint8_t padding[3];
};

static void* C_008342e8_GetContextObject(C_008342e8* self)
{
    return ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0xA0 / 4])(self, nullptr);
}

static char __fastcall C_008342e8_SetMenuContextState(
    C_008342e8* self, void* /*edx*/, std::uint32_t stateBits, char updateAction)
{
    const char enabled = CSF_AT(char, self, 0x23C);
    if (!enabled)
        return enabled;

    auto* actionFsm = reinterpret_cast<CCharActionFsm*>(self->field_0044);
    CSF_AT(std::uint8_t, self, 0x404) = 0;

    float position[3] = {};
    float angles[3] = {};
    ContextMatrixReference matrixRef = {};
    ContextMatrixReference resetMatrixRef = {};
    bool dispatchAction = false;

    if (static_cast<std::uint8_t>(stateBits))
    {
        CSF_AT(float, self, 0x230) = g_ZeroVectorX;
        CSF_AT(float, self, 0x234) = g_ZeroVectorY;
        CSF_AT(float, self, 0x238) = g_ZeroVectorZ;
        C_008342e8_PrepareContextOffset(self, nullptr);
        CCharActionFsm_SetActionMode(actionFsm, nullptr, 0);
        CCharActionFsm_ResolveActionTarget_488430(actionFsm, nullptr);

        if (!CSF_AT(std::uint8_t, self, 0x228))
        {
            if (CSF_AT(std::uint32_t, self, 0x2E0) & 0x10000)
            {
                const float offset =
                    static_cast<float>(C_00833ee8_GetContextHeightOffset(self, nullptr));
                if (CSF_AT(std::uint8_t, self, 0x3D0))
                {
                    const int settings = GetGlobalValue_8BC1E0();
                    const float height = CSF_AT(float, settings, 0x994);
                    CSF_AT(float, self, 0x234) =
                        CSF_AT(float, self->field_002c, 0xE8) -
                        CSF_AT(float, self->field_002c, 0xE4) - height + offset;
                    float minimum = -height;
                    float maximum = 0.0f;
                    ClampFloatToBounds(&CSF_AT(float, self, 0x234),
                                       &minimum, &maximum);
                }
                else
                {
                    const int settings = GetGlobalValue_8BC1E0();
                    const float height = CSF_AT(float, settings, 0x990);
                    CSF_AT(float, self, 0x234) =
                        CSF_AT(float, self->field_002c, 0xE8) -
                        CSF_AT(float, self->field_002c, 0xE4) - height + offset;
                }
            }

            void* context = C_008342e8_GetContextObject(self);
            void* contextTransform = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, context, 0)[0x18 / 4])(context, nullptr);
            resetMatrixRef.matrix = RwMatrixCreate();
            resetMatrixRef.owned = 1;
            ((void(__fastcall*)(void*, void*, ContextMatrixReference*))
                CSF_AT(void**, contextTransform, 0)[0x94 / 4])(
                    contextTransform, nullptr, &resetMatrixRef);
            if (resetMatrixRef.owned)
                RwMatrixDestroy(resetMatrixRef.matrix);

            void* object = reinterpret_cast<void*>(self->field_016c);
            void* objectTransform = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
            context = C_008342e8_GetContextObject(self);
            contextTransform = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, context, 0)[0x18 / 4])(context, nullptr);
            float* sourcePosition = ((float*(__fastcall*)(void*, void*))
                CSF_AT(void**, objectTransform, 0)[0x10 / 4])(
                    objectTransform, nullptr);
            angles[2] = CSF_AT(float, self, 0x238) + sourcePosition[2];
            angles[1] = CSF_AT(float, self, 0x234) + sourcePosition[1];
            angles[0] = CSF_AT(float, self, 0x230) + sourcePosition[0];
            ((void(__fastcall*)(void*, void*, float*))
                CSF_AT(void**, contextTransform, 0)[0x14 / 4])(
                    contextTransform, nullptr, angles);

            object = reinterpret_cast<void*>(self->field_016c);
            objectTransform = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
            context = C_008342e8_GetContextObject(self);
            contextTransform = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, context, 0)[0x18 / 4])(context, nullptr);
            auto setHeading = reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
                CSF_AT(void**, contextTransform, 0)[0x34 / 4]);
            const float heading = static_cast<float>(
                ((double(__fastcall*)(void*, void*))
                    CSF_AT(void**, objectTransform, 0)[0x28 / 4])(
                        objectTransform, nullptr));
            setHeading(contextTransform, nullptr, heading);
        }

        void* object = reinterpret_cast<void*>(self->field_016c);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0x34 / 4])(object, nullptr, 0);
        object = reinterpret_cast<void*>(self->field_016c);
        void* objectData = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, object, 0)[0x70 / 4])(object, nullptr);
        float* sourcePosition = ((float*(__fastcall*)(void*, void*))
            CSF_AT(void**, objectData, 0)[0x30 / 4])(objectData, nullptr);
        position[0] = sourcePosition[0];
        position[1] = sourcePosition[1];
        position[2] = sourcePosition[2];

        void* context = C_008342e8_GetContextObject(self);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, context, 0)[0x34 / 4])(context, nullptr, 1);
        context = C_008342e8_GetContextObject(self);
        void* contextData = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, context, 0)[0x70 / 4])(context, nullptr);
        ((void(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, contextData, 0)[0x2C / 4])(
                contextData, nullptr, position);

        CSF_AT(std::uint8_t, self, 0x228) = static_cast<std::uint8_t>(stateBits);
        CCharActionFsm_ProcessCurrentItem(actionFsm, nullptr);
        CCharActionFsm_ResolveActionTarget_4884B0(actionFsm, nullptr);
        self->vSlot6C(1);

        if ((CSF_AT(std::uint32_t, self, 0x2E0) & 0x2000) ||
            CSF_AT(std::uint32_t, self, 0x398))
        {
            CharActionFsm_ForwardToStateDispatch(actionFsm, nullptr, 0);
        }

        if (CSF_AT(std::uint32_t, self, 0x2E0) & 0x2000)
        {
            void* sceneObject = reinterpret_cast<void*>(CHandleTable_Resolve(
                g_pCurrentScene, nullptr,
                reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x1B4)));
            if (sceneObject && CSF_AT(int, sceneObject, 0x50) == 19)
            {
                auto* payloadRef =
                    ((ContextMatrixReference*(__fastcall*)(
                        void*, void*, ContextMatrixReference*))
                        CSF_AT(void**, self, 0)[0xAC / 4])(
                            self, nullptr, &matrixRef);
                const int payload = reinterpret_cast<int>(payloadRef->matrix);
                DispatchSceneObjectWithPayload(self, nullptr, payload);

                void* sceneData = ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, sceneObject, 0)[0x28 / 4])(
                        sceneObject, nullptr);
                const int index = ((int(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, sceneData, 0)[0x88 / 4])(
                        sceneData, nullptr, 203);
                if (index != -1)
                {
                    sceneData = ((void*(__fastcall*)(void*, void*))
                        CSF_AT(void**, sceneObject, 0)[0x28 / 4])(
                            sceneObject, nullptr);
                    void* attachment = ((void*(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, sceneData, 0)[0x80 / 4])(
                            sceneData, nullptr, index);
                    auto* rotationRef =
                        ((ContextMatrixReference*(__fastcall*)(
                            void*, void*, ContextMatrixReference*))
                            CSF_AT(void**, attachment, 0)[0x40 / 4])(
                                attachment, nullptr, &matrixRef);
                    const float yaw =
                        GetMatrixEulerAngles(rotationRef->matrix, nullptr, angles)[1];
                    if (matrixRef.owned)
                        RwMatrixDestroy(matrixRef.matrix);

                    const float xOffset = static_cast<float>(
                        -(std::cos(static_cast<double>(yaw)) *
                          CSF_AT(float, self, 0x1B0)));
                    ((void(__fastcall*)(void*, void*, float*, float))
                        CSF_AT(void**, attachment, 0)[0x48 / 4])(
                            attachment, nullptr, &g_AxisX, xOffset);
                    const float zOffset = static_cast<float>(
                        std::sin(static_cast<double>(yaw)) *
                        CSF_AT(float, self, 0x1B0));
                    ((void(__fastcall*)(void*, void*, float*, float))
                        CSF_AT(void**, attachment, 0)[0x48 / 4])(
                            attachment, nullptr, &g_AxisZ, zOffset);
                    CSF_AT(std::uint32_t, self, 0x1B0) = 0;
                }
            }
        }

        void* component = reinterpret_cast<void*>(self->field_0030);
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, component, 0)[0x9C / 4])(component, nullptr);

        if (updateAction)
        {
            const bool available =
                C_008342e8_IsContextActionAvailable(self, nullptr);
            C_008342e8_UpdateContextAction(self, nullptr, available);
            if (!SceneObject_IsSecondCheckTrue(self, nullptr))
            {
                if (SceneObject_PassesFirstCheck(self, nullptr))
                {
                    const int slot = self->vGetField0080();
                    self->vSlot8C(slot, 0, -1, 0);
                }
                else if (SceneObj_IsInactive(self, nullptr))
                {
                    void* listener = reinterpret_cast<void*>(self->field_0048);
                    auto notify = reinterpret_cast<void(__fastcall*)(
                        void*, void*, int)>(
                            CSF_AT(void**, listener, 0)[0x58 / 4]);
                    const int slot = self->vGetField0080();
                    notify(listener, nullptr, slot);
                }
            }
            else if (!C_008342e8_IsContextActionAvailable(self, nullptr))
            {
                ((void(__fastcall*)(void*, void*))
                    CSF_AT(void**, actionFsm, 0)[0x8C / 4])(
                        actionFsm, nullptr);
            }
            else
            {
                dispatchAction = true;
            }
        }
    }
    else
    {
        self->vSlot70(1065353216);
        CCharActionFsm_SetActionMode(actionFsm, nullptr, 0);
        CCharActionFsm_ResolveActionTarget_488430(actionFsm, nullptr);

        if (CSF_AT(std::uint8_t, self, 0x228))
        {
            void* object = reinterpret_cast<void*>(self->field_016c);
            void* objectTransform = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
            matrixRef.matrix = RwMatrixCreate();
            matrixRef.owned = 1;
            ((void(__fastcall*)(void*, void*, ContextMatrixReference*))
                CSF_AT(void**, objectTransform, 0)[0x94 / 4])(
                    objectTransform, nullptr, &matrixRef);
            if (matrixRef.owned)
                RwMatrixDestroy(matrixRef.matrix);

            void* context = C_008342e8_GetContextObject(self);
            void* contextTransform = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, context, 0)[0x18 / 4])(context, nullptr);
            object = reinterpret_cast<void*>(self->field_016c);
            objectTransform = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
            float* sourcePosition = ((float*(__fastcall*)(void*, void*))
                CSF_AT(void**, contextTransform, 0)[0x10 / 4])(
                    contextTransform, nullptr);
            position[2] = sourcePosition[2] - CSF_AT(float, self, 0x238);
            position[1] = sourcePosition[1] - CSF_AT(float, self, 0x234);
            position[0] = sourcePosition[0] - CSF_AT(float, self, 0x230);
            ((void(__fastcall*)(void*, void*, float*))
                CSF_AT(void**, objectTransform, 0)[0x14 / 4])(
                    objectTransform, nullptr, position);

            context = C_008342e8_GetContextObject(self);
            contextTransform = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, context, 0)[0x18 / 4])(context, nullptr);
            object = reinterpret_cast<void*>(self->field_016c);
            objectTransform = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
            stateBits = reinterpret_cast<std::uint32_t>(objectTransform);
            auto setHeading = reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
                CSF_AT(void**, objectTransform, 0)[0x34 / 4]);
            const float heading = static_cast<float>(
                ((double(__fastcall*)(void*, void*))
                    CSF_AT(void**, contextTransform, 0)[0x28 / 4])(
                        contextTransform, nullptr));
            setHeading(objectTransform, nullptr, heading);
        }

        void* context = C_008342e8_GetContextObject(self);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, context, 0)[0x34 / 4])(context, nullptr, 0);
        void* object = reinterpret_cast<void*>(self->field_016c);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0x34 / 4])(object, nullptr, 1);
        CSF_AT(std::uint8_t, self, 0x228) = 0;
        CCharActionFsm_ProcessCurrentItem(actionFsm, nullptr);
        CCharActionFsm_ResolveActionTarget_4884B0(actionFsm, nullptr);

        if (CSF_AT(std::uint32_t, self, 0x398))
            stateBits &= ~std::uint32_t(0xFF);
        else
        {
            stateBits = (stateBits & ~std::uint32_t(0xFF)) | 1;
            if (CSF_AT(std::uint32_t, self, 0x2E0) & 0x2000)
                stateBits &= ~std::uint32_t(0xFF);
        }
        self->vSlot6C(static_cast<int>(stateBits));

        const int collection = GetGlobalValue_4FBDE0();
        const int first = GetFirstElement(reinterpret_cast<void*>(collection), nullptr);
        void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(first), nullptr);
        ObjectProvider_SetPendingRequest(
            provider, nullptr, reinterpret_cast<int>(&self->field_001c));

        if ((CSF_AT(std::uint32_t, self, 0x2E0) & 0x2000) &&
            CSF_AT(std::uint8_t, self, 0x3A5))
        {
            void* sceneObject = reinterpret_cast<void*>(CHandleTable_Resolve(
                g_pCurrentScene, nullptr,
                reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x1B4)));
            if (sceneObject)
            {
                void* sceneData = ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, sceneObject, 0)[0x28 / 4])(
                        sceneObject, nullptr);
                const int index = ((int(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, sceneData, 0)[0x98 / 4])(
                        sceneData, nullptr, 3000);
                if (index != -1)
                {
                    sceneData = ((void*(__fastcall*)(void*, void*))
                        CSF_AT(void**, sceneObject, 0)[0x28 / 4])(
                            sceneObject, nullptr);
                    void* attachment = ((void*(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, sceneData, 0)[0x8C / 4])(
                            sceneData, nullptr, index);
                    void* targetTransform =
                        reinterpret_cast<void*>(GetContextTransform());
                    void* attachmentTransform =
                        ((void*(__fastcall*)(void*, void*))
                            CSF_AT(void**, attachment, 0)[0x18 / 4])(
                                attachment, nullptr);
                    auto setPosition =
                        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
                            CSF_AT(void**, targetTransform, 0)[0x14 / 4]);
                    float* sourcePosition = ((float*(__fastcall*)(void*, void*))
                        CSF_AT(void**, attachmentTransform, 0)[0x10 / 4])(
                            attachmentTransform, nullptr);
                    setPosition(targetTransform, nullptr, sourcePosition);

                    attachmentTransform = ((void*(__fastcall*)(void*, void*))
                        CSF_AT(void**, attachment, 0)[0x18 / 4])(
                            attachment, nullptr);
                    auto* attachmentMatrix =
                        ((ContextMatrixReference*(__fastcall*)(
                            void*, void*, ContextMatrixReference*))
                            CSF_AT(void**, attachmentTransform, 0)[0x8C / 4])(
                                attachmentTransform, nullptr, &matrixRef);
                    ((void(__fastcall*)(void*, void*, void*))
                        CSF_AT(void**, targetTransform, 0)[0x58 / 4])(
                            targetTransform, nullptr,
                            reinterpret_cast<std::uint8_t*>(attachmentMatrix->matrix) + 0x20);
                    if (matrixRef.owned)
                        RwMatrixDestroy(matrixRef.matrix);

                    auto* targetMatrix =
                        ((ContextMatrixReference*(__fastcall*)(
                            void*, void*, ContextMatrixReference*))
                            CSF_AT(void**, targetTransform, 0)[0x8C / 4])(
                                targetTransform, nullptr, &matrixRef);
                    auto* savedVector =
                        reinterpret_cast<std::uint8_t*>(targetMatrix->matrix) + 0x20;
                    CSF_AT(std::uint32_t, self, 0x3A8) =
                        CSF_AT(std::uint32_t, savedVector, 0);
                    CSF_AT(std::uint32_t, self, 0x3AC) =
                        CSF_AT(std::uint32_t, savedVector, 4);
                    CSF_AT(std::uint32_t, self, 0x3B0) =
                        CSF_AT(std::uint32_t, savedVector, 8);
                    if (matrixRef.owned)
                        RwMatrixDestroy(matrixRef.matrix);
                }
            }
        }
        else
        {
            auto* currentAction = reinterpret_cast<C_008319c0*>(
                reinterpret_cast<CCharActionFsm*>(self->field_0044)->field_009c);
            if ((!currentAction ||
                 C_008319c0_GetSubObjectField74(currentAction, nullptr) != 27) &&
                CSF_AT(std::int8_t, self, 0x2E0) >= 0)
            {
                self->vSlot94();
            }
            else
            {
                dispatchAction = true;
            }
        }
    }

    if (dispatchAction && self->vGetField0080() != -1)
    {
        const float actionValue = static_cast<float>(actionFsm->vSlot114());
        int actionBits;
        std::memcpy(&actionBits, &actionValue, sizeof(actionBits));
        const int slot = self->vGetField0080();
        self->vSlot8C(slot, 0, -1, actionBits);
    }

    self->field_0178 = 0;
    self->field_017c = 0;
    self->field_0180 = 0;
    return 0;
}

HOOK(0x0049A0E0, C_008342e8_SetMenuContextState, "auto");
