#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Vehicle_GetIndexedChild, 0x004084C0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Vehicle_GetIndexedChildValue, 0x004084E0,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(void*, __fastcall, Vehicle_FindOrCreateChild, 0x004087D0,
         (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, Vehicle_SetValuePairAndClearTail, 0x0040A600,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, Vehicle_SetChildParameter, 0x0040B590,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10,
         (std::uint32_t* matrix));

CSF_GLOBAL(float, g_VehiclePositionScale, 0x0082E420);

static char __fastcall C_0082dd78_ConfigureChildAndPosition(
    C_0082dd78* self, void* /*edx*/)
{
    void* child = Vehicle_FindOrCreateChild(
        reinterpret_cast<void*>(self->field_0030), nullptr, 27);

    float parameters[4] = {-60.0f, 1000.0f, 0.0f, -1.0f};

    // This callee's float-typed argument carries the pointer's raw bits.
    void* parameterAddress = parameters;
    float parameterArgument;
    std::memcpy(&parameterArgument, &parameterAddress, sizeof(parameterArgument));

    Vehicle_SetChildParameter(child, nullptr, 5, parameterArgument);
    Vehicle_SetValuePairAndClearTail(
        parameters, nullptr, 1114636288, 1116471296);
    Vehicle_SetChildParameter(child, nullptr, 5, parameterArgument);

    float childPosition[3] = {};
    Vehicle_GetIndexedChildValue(
        reinterpret_cast<void*>(self->field_0030), nullptr,
        reinterpret_cast<int>(childPosition), 25);

    void* transformChild = reinterpret_cast<void*>(
        Vehicle_GetIndexedChild(
            reinterpret_cast<void*>(self->field_0030), nullptr, 26));

    struct MatrixReference
    {
        std::uint32_t* matrix;
        char owned;
        char padding[3];
    };
    MatrixReference matrixReference = {};

    void* reference = reinterpret_cast<
        void* (__fastcall*)(void*, void*, MatrixReference*)>(
            CSF_AT(void**, transformChild, 0)[0x3C / 4])(
                transformChild, nullptr, &matrixReference);

    float* matrix = CSF_AT(float*, reference, 0);
    float position[3];
    position[2] = matrix[10] * g_VehiclePositionScale;
    position[1] = matrix[9] * g_VehiclePositionScale;
    position[0] = matrix[8] * g_VehiclePositionScale;

    reinterpret_cast<void (__fastcall*)(C_0082dd78*, void*, float*, float*)>(
        self->vtbl[0x54 / 4])(self, nullptr, childPosition, position);

    char result = matrixReference.owned;
    if (result)
        result = static_cast<char>(RwMatrixDestroy(matrixReference.matrix));

    CSF_AT(std::uint32_t, self, 0x8C) = 3;
    return result;
}

HOOK(0x0041D180, C_0082dd78_ConfigureChildAndPosition, "auto");
