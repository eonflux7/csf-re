#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, FrameEntry_Update, 0x00467E30, (void* self, void* edx, float a3));
CSF_FUNC(unsigned int, __fastcall, FrameController_Prepare, 0x00469630, (void* self, void* edx));

// Prepares the controller, then updates every non-null entry of its pointer list.
// Entry count is re-read from the object after each update, so a callee that
// changes the list size is honored.
static unsigned int __fastcall S_0045d610_UpdateFrameController(S_0045d610* self, void* /*edx*/, float a2)
{
    unsigned int result;
    unsigned int i;

    FrameController_Prepare(self, nullptr);
    result = self->field_0030;
    for (i = 0; i < result; ++i)
    {
        uint32_t entry = reinterpret_cast<uint32_t*>(self->field_0028)[i];
        if (entry)
            FrameEntry_Update(reinterpret_cast<void*>(entry), nullptr, a2);
        result = self->field_0030;
    }
    return result;
}

HOOK(0x00469800, S_0045d610_UpdateFrameController, "auto");
