#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4084C0, 0x004084C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4084E0, 0x004084E0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void*, __fastcall, sub_4087D0, 0x004087D0, (void* self, void* edx, int a3));
CSF_FUNC(float*, __fastcall, sub_40B550, 0x0040B550, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(int, __fastcall, sub_41D640, 0x0041D640, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (void* self, void*));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (void* self, void*, float** a2));

static char __fastcall C_0082e428_SetTargetPosition(C_0082e428* self, void* /*edx*/, float* pos)
{
    float angleParams[4];
    std::uint32_t dirVec[3];
    float rel[3];
    float origin[3];
    std::uint32_t local[3];
    std::uint8_t matHolder[0x10];  // not zeroed: the original's [ebp-0x10] is written only by the callee at slot 0x3C

    self->field_0094 = pos[0];
    CSF_AT(float, self, 0x98) = pos[1];
    CSF_AT(float, self, 0x9C) = pos[2];

    sub_4084E0(reinterpret_cast<void*>(self->field_0030), nullptr, (int)origin, 25);

    int entry = sub_4084C0(reinterpret_cast<void*>(self->field_0030), nullptr, 25);
    {
        void* obj = reinterpret_cast<void*>(entry);
        typedef void(__fastcall * VFn)(void*, void*, void*);
        ((VFn)CSF_AT(void**, obj, 0)[0x3C / 4])(obj, nullptr, matHolder);
    }

    rel[0] = pos[0] - origin[0];
    rel[1] = pos[1] - origin[1];
    rel[2] = pos[2] - origin[2];

    CSF_AT(std::uint32_t*, matHolder, 8) = RwMatrixCreate();
    CSF_AT(std::uint8_t, matHolder, 0xC) = 1;

    Matrix_InvertWithStoredTarget(matHolder, nullptr, reinterpret_cast<float**>(matHolder + 8));

    local[0] = 0;
    local[1] = 0;
    local[2] = 0;
    RwV3dTransformVectors((int)local, (int)rel, 1, CSF_AT(int, matHolder, 8));

    dirVec[0] = local[0];
    dirVec[1] = local[1];
    dirVec[2] = local[2];

    float angle = (float)S_00526d40_GetVectorAngleRadians(dirVec, nullptr);

    void* child = sub_4087D0(reinterpret_cast<void*>(self->field_0030), nullptr, 25);
    angleParams[0] = angle;
    angleParams[1] = 0.2f;
    angleParams[2] = 0.0f;
    angleParams[3] = -1.0f;
    sub_40B550(child, nullptr, 1, angleParams);

    sub_41D640(self, nullptr, 1);
    self->field_008c = 1;

    if (CSF_AT(std::uint8_t, matHolder, 0xC)) {
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matHolder, 8));
    }
    if (CSF_AT(std::uint8_t, matHolder, 4)) {
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matHolder, 0));
    }
    return 1;
}

HOOK(0x0041D830, C_0082e428_SetTargetPosition, "auto");
