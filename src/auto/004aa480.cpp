#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Fetches the owner pointer at +0x2C, then the sub-object at owner+0x44, and asks
// its vtable slot 47 (+0xBC) for the status of channel 6. True unless it returns -1.
static bool __fastcall AiDistracted_CheckActorEligibility(void* self, void* /*edx*/)
{
    void* owner = CSF_AT(void*, self, 0x2C);
    void* controller = CSF_AT(void*, owner, 0x44);
    int status = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, controller, 0)[0xBC / 4])(controller, nullptr, 6);
    return status != -1;
}

HOOK(0x4AA480, AiDistracted_CheckActorEligibility, "auto");
