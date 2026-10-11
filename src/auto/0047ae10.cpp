#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, Task_GetTargetNameBuffer, 0x004D1510, (void* self, void* edx));

// Returns the target name buffer of the task object referenced by this->field_0014.
// The original tail-jumps into Task_GetTargetNameBuffer with ecx = the task pointer
// stored at (field_0014 + 0x34); its result is returned unchanged.
static char* __fastcall AiCore_GetTaskTargetNameBuffer(S_0047ae10* self, void* /*edx*/) {
    uint8_t* taskOwner = reinterpret_cast<uint8_t*>(self->field_0014);
    C_008672e4* taskObject = CSF_AT(C_008672e4*, taskOwner, 0x34);
    return Task_GetTargetNameBuffer(taskObject, nullptr);
}

HOOK(0x0047AE10, AiCore_GetTaskTargetNameBuffer, "auto");
