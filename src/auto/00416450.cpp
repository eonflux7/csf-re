#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_HandleFocusRequest, 0x0049D220, (void* self, void* edx, int a2, int a3));

// Copies the scene-space position offset from the source record into a focus vector,
// clears the per-object flag, forwards the focus vector to the scene loader, then
// notifies the object returned by the dispatcher and runs the focus request handler.
static char __fastcall C_0082e090_ApplyFocusOffsetAndDispatch(C_0082e090* self, void* /*edx*/,
                                                              int /*a2*/, S_0049cfa0* a3, int /*a4*/)
{
    float focusVector[3];

    void* sceneObject = reinterpret_cast<void*>(self->field_0004);
    self->field_0077 = 0;

    focusVector[0] = CSF_AT(float, a3, 0x2C) + CSF_AT(float, a3, 0x08);
    focusVector[1] = CSF_AT(float, a3, 0x0C) + CSF_AT(float, a3, 0x30);
    focusVector[2] = CSF_AT(float, a3, 0x10) + CSF_AT(float, a3, 0x34);

    SceneLoader_ConfigureFocusEntry(sceneObject, nullptr, reinterpret_cast<int*>(focusVector));

    void* dispatched = C_0082dd78_DispatchReturnedObject(self, nullptr);
    using NotifyFn = void (__fastcall*)(void*, void*);
    reinterpret_cast<NotifyFn>(CSF_AT(void**, dispatched, 0)[0x28 / 4])(dispatched, nullptr);

    char result = SceneLoader_HandleFocusRequest(reinterpret_cast<void*>(self->field_0004), nullptr,
                                                 reinterpret_cast<int>(a3), 0);
    return result;
}

HOOK(0x00416450, C_0082e090_ApplyFocusOffsetAndDispatch, "auto");
