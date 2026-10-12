#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x0048E460 SceneLoader_ConfigureFocusEntry (__fastcall, ret 4)
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));

// Original: 0x0041E0C0 C_0082dd78_DispatchReturnedObject (forwarder, no stack args)
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));

// Copies the 0x64-byte payload into this object, derives a focus position from
// the payload's offset/position vectors, hands it to the scene loader, then
// dispatches to the returned object's slot 10.
static int __fastcall C_0082e090_ApplyPositionAndDispatch(C_0082e090* self, void* /*edx*/, float* a2)
{
    self->field_0074 = 1;
    std::memcpy(&self->field_0010, a2, 0x64);

    float focusPosition[3];
    focusPosition[0] = a2[11] + a2[2];
    focusPosition[1] = a2[12] + a2[3];
    focusPosition[2] = a2[13] + a2[4];

    C_00834050* sceneEntry = reinterpret_cast<C_00834050*>(self->field_0004);
    SceneLoader_ConfigureFocusEntry(sceneEntry, nullptr, reinterpret_cast<int*>(focusPosition));

    void* dispatched = C_0082dd78_DispatchReturnedObject(self, nullptr);
    return ((int(__fastcall*)(void*, void*))CSF_AT(void**, dispatched, 0)[10])(dispatched, nullptr);
}

HOOK(0x004161B0, C_0082e090_ApplyPositionAndDispatch, "auto");
