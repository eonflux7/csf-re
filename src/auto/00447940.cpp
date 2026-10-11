#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint64_t, __fastcall, UpdateControllerTiming, 0x00445B40,
         (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, UpdateControllerState, 0x00445C30,
         (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, UpdateControllerMotion, 0x00445D70,
         (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, UpdateControllerActions, 0x004471D0,
         (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, UpdateControllerInput, 0x00447800,
         (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, UpdateLinkedController, 0x0044E180,
         (void* self, void* edx, float a2));

static int __fastcall S_00445b40_UpdatePrimaryController(
    S_00445b40* self, void* /*edx*/, float elapsed)
{
    // Listed by the packet as a vtable reference.
    (void)CSF_VTBL(0x00445B40);

    self->field_0008 = elapsed + self->field_0008;
    UpdateControllerTiming(self, nullptr, elapsed);
    UpdateControllerInput(self, nullptr, elapsed);
    UpdateControllerState(self, nullptr, elapsed);
    UpdateControllerMotion(self, nullptr, elapsed);
    UpdateControllerActions(self, nullptr, elapsed);
    // eax on return is the last call's (in-game diff: ours returned the previous call's result)
    return static_cast<int>(UpdateLinkedController(
        reinterpret_cast<void*>(self->field_0114), nullptr, elapsed));
}

HOOK(0x00447940, S_00445b40_UpdatePrimaryController, "auto");

