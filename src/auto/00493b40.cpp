#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, TaskState_SelectSpeedAndSetInput, 0x0041FEC0, (void* self, void* , float magnitude));
CSF_FUNC(void, __fastcall, TaskState_SelectSpeedAndSetAxis, 0x00420000, (void* self, void* , float magnitude));
CSF_FUNC(void, __fastcall, ForwardFloatToNestedChild, 0x00425590, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, ForwardFloatIfStateTwo, 0x004255A0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Routes a two-axis (planar) movement input either to the character's own task state
// or, when the character is attached to another entity (type 7), to that entity's controller.
static void __fastcall S_00492b20_ApplyPlanarInput(S_00492b20* self, void* /*edx*/, float axisInput, float forwardInput)
{
    const uint32_t stateFlags = self->field_02e0;

    if ((stateFlags & 0x89000) != 0)
        return;

    if ((stateFlags & 0x10001) != 0)
    {
        void* taskState = reinterpret_cast<void*>(self->field_0030);

        if (axisInput != g_fZero)
            TaskState_SelectSpeedAndSetAxis(taskState, nullptr, axisInput);

        if (forwardInput != g_fZero)
            TaskState_SelectSpeedAndSetInput(taskState, nullptr, forwardInput);
    }
    else if ((stateFlags & 0x2000) != 0)
    {
        char* attachedHandle = S_00492b20_GetPtr0x1B4(self, nullptr);
        int attachedEntity = CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(attachedHandle));

        if (attachedEntity != 0 && CSF_AT(int, attachedEntity, 0x50) == 7)
        {
            void* attachedController = CSF_AT(void*, attachedEntity, 0x30);
            ForwardFloatToNestedChild(attachedController, nullptr, axisInput);
            ForwardFloatIfStateTwo(attachedController, nullptr, forwardInput);
        }
    }
}

HOOK(0x00493B40, S_00492b20_ApplyPlanarInput, "auto");
