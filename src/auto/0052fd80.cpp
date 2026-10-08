#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// 0x0052FC60 - __thiscall(CMenuSceneSelect* this, int a2), ret 4 - performs the actual command work.
CSF_FUNC(int, __fastcall, CMenuSceneSelect_DispatchCommand, 0x52FC60, (CMenuSceneSelect* self, void* /*edx*/, int a2));

// 0x0052FD80 - __thiscall(CMenuSceneSelect* this, uint32_t* a2), ret 4
// Inspects the first dword of the incoming command; only command id 13 is dispatched to the handler,
// otherwise the command pointer itself is returned unchanged (as an int).
static int __fastcall CMenuSceneSelect_HandleCommand(CMenuSceneSelect* self, void* /*edx*/, uint32_t* a2)
{
    if (*a2 == 13)
        return CMenuSceneSelect_DispatchCommand(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(a2)));
    return static_cast<int>(reinterpret_cast<uintptr_t>(a2));
}

HOOK(0x52FD80, CMenuSceneSelect_HandleCommand, "auto");
