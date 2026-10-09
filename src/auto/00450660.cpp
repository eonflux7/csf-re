#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_GetByte023C, 0x00495490, (char* self, void*));
CSF_FUNC(int, __fastcall, SceneObject_IsSecondCheckTrue, 0x0048E140, (void* self, void* edx));

// Scans the right-neighbour id list of a GUI control, skipping `excludedObject`.
// Returns true if any other entry passes C_GetByte023C and then SceneObject_IsSecondCheckTrue.
static char __fastcall CGuiControl_AnyRightOtherSceneTrue(CGuiControl* self, void* /*edx*/, int excludedObject)
{
    char found = 0;
    uint32_t index = 0;

    if (CSF_AT(uint32_t, self, 0x68) == 0)
        return 0;

    do {
        void** elements = CSF_AT(void**, self, 0x60);
        C_00833b88* entry = reinterpret_cast<C_00833b88*>(elements[index]);

        if (entry != reinterpret_cast<C_00833b88*>(excludedObject)
            && C_GetByte023C(reinterpret_cast<char*>(entry), nullptr))
        {
            if ((char)SceneObject_IsSecondCheckTrue(entry, nullptr))
                found = 1;
        }
        ++index;
    } while (index < CSF_AT(uint32_t, self, 0x68));

    return found;
}

HOOK(0x00450660, CGuiControl_AnyRightOtherSceneTrue, "auto");
