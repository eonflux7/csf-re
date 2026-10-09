#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_DegreesToRadians, 0x008313E8);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObject_CancelPendingAction, 0x0048FE90, (void* self, void* edx));

static int __fastcall ApplyFacingRecordToSceneObject(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, std::uint32_t* a4)
{
    // 12-byte descriptor passed to the sub-object's vtable slot 42.
    struct FacingDescriptor
    {
        std::int32_t byteSize;   // always 24
        float angleRadians;
        std::uint8_t flag;
    };

    void* recordPtr = reinterpret_cast<void*>(a4[0]);
    std::uint32_t paramBlockAddr = CSF_AT(std::uint32_t, recordPtr, 4);
    void* paramBlock = reinterpret_cast<void*>(paramBlockAddr);
    int returnValue = CSF_AT(int, paramBlock, 12);

    if (returnValue >= 0)
    {
        std::uint32_t positionBlockAddr = paramBlockAddr + 4;
        void* positionBlock = reinterpret_cast<void*>(positionBlockAddr);

        returnValue = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(positionBlockAddr));
        C_00833908* object = reinterpret_cast<C_00833908*>(returnValue);
        if (object != nullptr)
        {
            returnValue = (int)object->field_0034;
            if (returnValue != 0)
            {
                if (!object->vGetField016c())
                    return SceneObject_CancelPendingAction(object, nullptr);

                void* stateObject = (void*)object->vGetField016c();
                std::uint8_t stateRejects =
                    ((std::uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, stateObject, 0)[42])(stateObject, nullptr);
                if (stateRejects != 0)
                    return SceneObject_CancelPendingAction(object, nullptr);

                // Both parameter-block pointers are loaded from a4 before either is dereferenced.
                void* rotationParam = reinterpret_cast<void*>(a4[1]);
                void* flagParam = reinterpret_cast<void*>(a4[2]);

                void* rotationValue = reinterpret_cast<void*>(CSF_AT(std::uint32_t, rotationParam, 4));

                FacingDescriptor descriptor{};
                descriptor.byteSize = 24;
                descriptor.angleRadians = CSF_AT(float, rotationValue, 4) * g_DegreesToRadians;

                void* flagValue = reinterpret_cast<void*>(CSF_AT(std::uint32_t, flagParam, 4));
                descriptor.flag = CSF_AT(std::uint8_t, flagValue, 1);

                // The original reloads field_0034 here, after the record reads.
                C_008672e4* subObject = reinterpret_cast<C_008672e4*>(object->field_0034);
                ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, subObject, 0)[42])(subObject, nullptr, &descriptor);

                returnValue = CSF_AT(int, positionBlock, 4);
                CSF_AT(int, self, 0x60) = returnValue;
                CSF_AT(int, self, 0x64) = CSF_AT(int, positionBlock, 8);
                CSF_AT(std::uint8_t, self, 0x82) = 0;
            }
        }
    }
    return returnValue;
}

HOOK(0x0045EB80, ApplyFacingRecordToSceneObject, "auto");
