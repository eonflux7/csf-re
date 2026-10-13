#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Runs an inner file-system operation with the manager's state flag set to 3
// while the call is in progress and to 2 afterwards. The inner object is
// re-read after the call because the callee may replace it.
static int __cdecl RtFSManager_RunWithStateSwitch(void* self, int arg1, int arg2) {
    void* inner = CSF_AT(void*, self, 0x40);
    CSF_AT(int, self, 0x20) = 3;
    int result = ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, inner, 0)[3])(inner, nullptr, arg1, arg2);

    inner = CSF_AT(void*, self, 0x40);
    CSF_AT(int, self, 0x20) = 2;
    CSF_AT(int, self, 0x08) = ((int(__fastcall*)(void*, void*))CSF_AT(void**, inner, 0)[4])(inner, nullptr);
    CSF_AT(int, self, 0x0C) = 0;
    return result;
}

HOOK(0x0069FF00, RtFSManager_RunWithStateSwitch, "auto");
