#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

static void* __fastcall SceneObject_GetTransform(
    void* self, void* /*edx*/, void* output)
{
    std::uint32_t* temporaryMatrix = RwMatrixCreate();

    void* scene = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x28 / 4])(self, nullptr);

    int index = ((int (__fastcall*)(void*, void*, int))
        CSF_AT(void**, scene, 0)[0x98 / 4])(scene, nullptr, 2200);

    void* transformOwner;
    if (index == -1)
    {
        transformOwner = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, scene, 0)[0x18 / 4])(scene, nullptr);
    }
    else
    {
        void* child = ((void* (__fastcall*)(void*, void*, int))
            CSF_AT(void**, scene, 0)[0x8C / 4])(scene, nullptr, index);

        transformOwner = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, child, 0)[0x18 / 4])(child, nullptr);
    }

    alignas(8) std::uint8_t transformBytes[0x8];
    void* transform = ((void* (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, transformOwner, 0)[0x8C / 4])(
            transformOwner, nullptr, transformBytes);

    std::memcpy(temporaryMatrix,
                CSF_AT(std::uint32_t*, transform, 0), 0x40);

    if (CSF_AT(std::uint8_t, transformBytes, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, transformBytes, 0));

    std::uint32_t* outputMatrix = RwMatrixCreate();
    CSF_AT(std::uint32_t*, output, 0) = outputMatrix;
    std::memcpy(outputMatrix, temporaryMatrix, 0x40);
    CSF_AT(std::uint8_t, output, 4) = 1;
    RwMatrixDestroy(temporaryMatrix);

    return output;
}

HOOK(0x00493890, SceneObject_GetTransform, "auto");
