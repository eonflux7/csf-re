#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(char, __fastcall, FinishStartedMovement_4A7DC0, 0x004A7DC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovement_4DB140, 0x004DB140, (C_0082f9f8* self, void*, int movement));

// Queues a movement action for the given request record. If the current scene
// cannot resolve the request handle, the target name of the owner's owner is returned
// instead; otherwise the request's two payload dwords are stored on the object,
// a movement is started, and the started movement is finalized.
static char __fastcall C_0082f9f8_QueueAction(C_0082f9f8* self, void* /*edx*/, int a2)
{
    if (!CHandleTable_Resolve(g_pCurrentScene, nullptr, a2))
    {
        uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
        C_00833908* target = reinterpret_cast<C_00833908*>(CSF_AT(uint32_t, owner, 0x118));
        return static_cast<char>(C_00833908_GetTargetName(target, nullptr));
    }

    void* request = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
    CSF_AT(uint32_t, self, 0x104) = CSF_AT(uint32_t, request, 4);
    CSF_AT(uint32_t, self, 0x108) = CSF_AT(uint32_t, request, 8);

    int movementArg = static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x28));
    int started = C_0082f9f8_StartMovement_4DB140(self, nullptr, movementArg);
    void* startedPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(started));
    return FinishStartedMovement_4A7DC0(startedPtr, nullptr);
}

HOOK(0x004A8710, C_0082f9f8_QueueAction, "auto");
