#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, TCharTask_ReleaseCurrentTask, 0x004DB110, (TCharTask* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetStance, 0x00492720, (void* self, void* , int newStance));

// Releases the current task, resets the two task-slot values at +0x3BC/+0x3C0 to -1,
// sets the cover-attack stance from this->field_0078, asks the owner's controller (slot 20)
// for a flag byte, and finally ORs bit 2 into the state word at +0x18.
static int __fastcall TDarObjeto_ReleaseTaskAndSetStance(TDarObjeto* self, void* /*edx*/)
{
    TCharTask_ReleaseCurrentTask(self, nullptr);

    uint8_t* state = reinterpret_cast<uint8_t*>(self->field_0008);
    CSF_AT(int, state, 0x3BC + 4) = -1;
    CSF_AT(int, state, 0x3BC + 8) = -1;

    void* stanceOwner = CSF_AT(void*, state, 0x14);
    CoverAttack_SetStance(stanceOwner, nullptr, static_cast<int>(self->field_0078));

    void* controller = CSF_AT(void*, reinterpret_cast<uint8_t*>(self->field_0008), 0x14);
    controller = static_cast<uint8_t*>(controller) + 4;
    char flagByte = 0;
    ((void(__fastcall*)(void*, void*, char*))CSF_AT(void**, controller, 0)[80 / 4])(controller, nullptr, &flagByte);

    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    int result = CSF_AT(int, owner, 0x18) | 2;
    CSF_AT(int, owner, 0x18) = result;
    return result;
}

HOOK(0x4D5230, TDarObjeto_ReleaseTaskAndSetStance, "auto");
