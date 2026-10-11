#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(char, __fastcall, Vehicle_SceneObjectUpdate, 0x004959A0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_RadToDeg, 0x0082E1D8);

typedef void* (__fastcall* VFnGetObj)(void* self, void* edx);
typedef int* (__fastcall* VFnGetMatrix)(void* self, void* edx, std::uint32_t* out);
typedef void (__fastcall* VFnSetFloat)(void* self, void* edx, float v);

static unsigned int __fastcall C_0082e1e8_UpdateFocus(C_0082e1e8* self, void* /*edx*/)
{
    void* v2;
    {
        void* target = reinterpret_cast<void*>(self->field_0004);
        v2 = ((VFnGetObj)CSF_AT(void**, target, 0)[0x28 / 4])(target, nullptr);
    }

    // field_0004 is reloaded after the virtual call
    int* posVec = reinterpret_cast<int*>(
        GetObjectPositionVector(reinterpret_cast<void*>(self->field_0004), nullptr));
    float focus[3];
    float posX = *reinterpret_cast<float*>(&posVec[0]);
    focus[0] = posX + CSF_AT(float, self, 0x20);
    focus[1] = *reinterpret_cast<float*>(&posVec[1]);
    float posZ = *reinterpret_cast<float*>(&posVec[2]);
    focus[2] = posZ + CSF_AT(float, self, 0x24);

    void* obj = ((VFnGetObj)CSF_AT(void**, v2, 0)[0x18 / 4])(v2, nullptr);

    float fx = CSF_AT(float, self, 0x20);
    float fz = CSF_AT(float, self, 0x24);
    std::uint32_t matrixBuf[3];
    float dist = (float)std::sqrt(fx * fx + fz * fz);
    CSF_AT(float, matrixBuf, 8) = dist;

    int* res = ((VFnGetMatrix)CSF_AT(void**, obj, 0)[0x8C / 4])(obj, nullptr, matrixBuf);
    focus[1] = CSF_AT(float, matrixBuf, 8) * CSF_AT(float, reinterpret_cast<void*>(*res), 0x24) + focus[1];

    if (CSF_AT(std::uint8_t, matrixBuf, 4)) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixBuf[0]));
    }

    SceneLoader_ConfigureFocusEntry(reinterpret_cast<void*>(self->field_0004), nullptr,
                                    reinterpret_cast<int*>(focus));

    void* obj2 = ((VFnGetObj)CSF_AT(void**, v2, 0)[0x18 / 4])(v2, nullptr);
    float angle = CSF_AT(float, self, 0x28) * g_RadToDeg;
    ((VFnSetFloat)CSF_AT(void**, obj2, 0)[0x40 / 4])(obj2, nullptr, angle);

    std::uint8_t* scene = reinterpret_cast<std::uint8_t*>(g_pCurrentScene);
    unsigned int result = CSF_AT(std::uint32_t, scene, 0x68);
    for (unsigned int i = 0; i < result; ++i) {
        void* element = CSF_AT(void**, scene, 0x60)[i];
        Vehicle_SceneObjectUpdate(element, nullptr, reinterpret_cast<void*>(self->field_0004));
        result = CSF_AT(std::uint32_t, scene, 0x68);
    }
    return result;
}

HOOK(0x00418E50, C_0082e1e8_UpdateFocus, "auto");
