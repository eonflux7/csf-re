#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// __thiscall, ecx = this, no stack arguments (ret 0).
CSF_FUNC(int, __fastcall, CMenuMessageBox_MarkCurrentButtonFlag, 0x528E70, (CMenuMessageBox* self, void* edx));

// Called with a pointer whose first dword is a command/button id.
// If that id is 13 the menu message box command handler is invoked (result forwarded),
// otherwise the command pointer itself is returned in eax.
static int __fastcall CMenuMessageBox_DispatchCommand(CMenuMessageBox* self, void* /*edx*/, uint32_t* command)
{
    if (*command == 13)
        return CMenuMessageBox_MarkCurrentButtonFlag(self, nullptr);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(command));
}

HOOK(0x528F20, CMenuMessageBox_DispatchCommand, "auto");
