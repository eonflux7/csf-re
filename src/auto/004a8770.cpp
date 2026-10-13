#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(char, __fastcall, FinalizeStartedMovement, 0x004A80E0, (void* self, void* edx, int arg0));
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovement_4DB140, 0x004DB140, (C_0082f9f8* self, void* edx, int movement));

// Queues a movement action on this task from a request record (handle, dwords at +4 and +8).
// Returns the result of the finalizing step, or the target-name query when the handle does not resolve.
static char __fastcall C_0082f9f8_QueueActionWithParam(C_0082f9f8* self, void* /*edx*/, int a2, int a3)
{
    if (!CHandleTable_Resolve(g_pCurrentScene, nullptr, a2)) {
        void* owner = reinterpret_cast<void*>(self->field_0008);
        C_00833908* target = CSF_AT(C_00833908*, owner, 0x118);
        return static_cast<char>(C_00833908_GetTargetName(target, nullptr));
    }

    CSF_AT(uint32_t, self, 0x104) = CSF_AT(uint32_t, reinterpret_cast<void*>(a2), 4);
    CSF_AT(uint32_t, self, 0x108) = CSF_AT(uint32_t, reinterpret_cast<void*>(a2), 8);

    int movementBlock = static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x28));
    int started = C_0082f9f8_StartMovement_4DB140(self, nullptr, movementBlock);
    return FinalizeStartedMovement(reinterpret_cast<void*>(started), nullptr, a3);
}

HOOK(0x4A8770, C_0082f9f8_QueueActionWithParam, "auto");
