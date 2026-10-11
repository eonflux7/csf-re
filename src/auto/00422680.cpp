#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t*, __fastcall, AiTask_Finish, 0x00421CA0, (void* self, void* edx));

// Forwards a 3-dword event record to the owner's virtual handler (slot 20),
// then finishes this task state.
static uint32_t* __fastcall AiTask_ForwardEventToOwnerThenFinish(C_008342e8* self, void* /*edx*/, const uint32_t* eventRecord)
{
    uint32_t eventData[3];
    eventData[0] = eventRecord[0];
    eventData[1] = eventRecord[1];
    eventData[2] = eventRecord[2];

    void* owner = CSF_AT(void*, self, 0x14);
    void* ownerTarget = CSF_AT(void*, owner, 0x4C);
    void** targetVtbl = CSF_AT(void**, ownerTarget, 0);

    ((void (__fastcall*)(void*, void*, uint32_t*))targetVtbl[80 / 4])(ownerTarget, nullptr, eventData);

    return AiTask_Finish(self, nullptr);
}

HOOK(0x00422680, AiTask_ForwardEventToOwnerThenFinish, "auto");
