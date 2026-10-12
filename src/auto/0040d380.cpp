#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(void, __fastcall, C_008342e8_OnSceneObjectUpdated, 0x004959A0, (C_008342e8* self, void* edx, void* updatedObject));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static unsigned int __fastcall C_0082dc88_RefreshSceneObjectFocus(C_0082dc88* self, void* /*edx*/)
{
    // Virtual slot 0x28 (vtable index 10) on the owner object, no arguments.
    void* ownerObject = reinterpret_cast<void*>(self->field_0004);
    ((void (__fastcall*)(void*, void*))CSF_AT(void**, ownerObject, 0)[10])(ownerObject, nullptr);

    // Read the owner's position vector and offset it into a focus point.
    int positionVector = GetObjectPositionVector(reinterpret_cast<void*>(self->field_0004), nullptr);
    const float* position = reinterpret_cast<const float*>(positionVector);

    alignas(4) float focus[3];
    focus[0] = position[0] + *reinterpret_cast<float*>(&self->field_0020);
    focus[1] = position[1];
    focus[2] = position[2] + *reinterpret_cast<float*>(&self->field_0024);

    SceneLoader_ConfigureFocusEntry(reinterpret_cast<void*>(self->field_0004), nullptr,
                                    reinterpret_cast<int*>(focus));

    // Notify every object in the current scene's list. Count and array are reloaded each pass.
    void* scene = g_pCurrentScene;
    unsigned int result = CSF_AT(uint32_t, scene, 0x68);
    for (unsigned int i = 0; i < result; ++i) {
        uint32_t* sceneObjects = CSF_AT(uint32_t*, scene, 0x60);
        C_008342e8_OnSceneObjectUpdated(reinterpret_cast<C_008342e8*>(sceneObjects[i]), nullptr,
                                        reinterpret_cast<void*>(self->field_0004));
        result = CSF_AT(uint32_t, scene, 0x68);
    }
    return result;
}

HOOK(0x0040D380, C_0082dc88_RefreshSceneObjectFocus, "auto");
