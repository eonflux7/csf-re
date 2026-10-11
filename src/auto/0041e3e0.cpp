#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GuiControl_GetPathPointRecord, 0x004A0F30, (void* self, void* edx, int a2));

// Forwards to GuiControl_GetPathPointRecord with the embedded sub-object at +4
// of the pointer stored at this+0x14. edx is forwarded unchanged, as the original jmp does.
static int __fastcall AiTaskStates_GetPathPointRecord(void* self, void* edx, int a2)
{
    void* owner = CSF_AT(void*, self, 0x14);
    void* subObject = (uint8_t*)owner + 4;
    return GuiControl_GetPathPointRecord(subObject, edx, a2);
}

HOOK(0x41E3E0, AiTaskStates_GetPathPointRecord, "auto");
