#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, RunSceneSelectionTransition, 0x00457C60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_IsSecondCheckTrue, 0x0048E140, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __stdcall, MenuMissionInfo_SetGlobalIndex, 0x00525DD0, (int a1));

// Resets the transition slots of a scene selection object, copies the source
// positions from the incoming record (a3) and, when a2 passes the check,
// copies its coordinates. Then starts the transition.
static char __fastcall C_00833b88_TransitionObjects(C_00833b88* self, void* /*edx*/, C_00833b88* a2, int a3)
{
    CSF_AT(uint32_t, self, 0x08) = static_cast<uint32_t>(-1);
    self->field_000c = static_cast<uint32_t>(-1);
    self->field_0014 = static_cast<uint32_t>(-1);
    CSF_AT(uint32_t, self, 0x18) = static_cast<uint32_t>(-1);
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;

    char result = static_cast<char>(a3);
    if (a3 != 0) {
        void* a3Ptr = reinterpret_cast<void*>(static_cast<uintptr_t>(a3));
        self->field_0014 = CSF_AT(uint32_t, a3Ptr, 0x20);
        CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, a3Ptr, 0x24);

        if (a2 != nullptr) {
            if (static_cast<uint8_t>(SceneObject_IsSecondCheckTrue(a2, nullptr)) != 0) {
                CSF_AT(uint32_t, self, 0x08) = a2->field_0020;
                self->field_000c = a2->field_0024;
            }
        }

        Get_008b0678();
        MenuMissionInfo_SetGlobalIndex(99);
        self->field_001c = 1;
        result = RunSceneSelectionTransition(self, nullptr);
        self->field_0020 = 0;
    }
    return result;
}

HOOK(0x458040, C_00833b88_TransitionObjects, "auto");
