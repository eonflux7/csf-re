#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0085ec18_ReleaseConsoleResources, 0x004F96E0, (C_0085ec18* self, void* edx));

CSF_GLOBAL(int, g_Value8B055C, 0x008B055C);

static void* ConsoleHandleToPointer(int handle) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(handle)));
}

static int __cdecl MenuControls_RefreshCursor() {
    int result = 0;

    if (g_Value8B055C) {
        C_0085ec18_ReleaseConsoleResources(
            reinterpret_cast<C_0085ec18*>(ConsoleHandleToPointer(g_Value8B055C)), nullptr);

        if (g_Value8B055C) {
            void* console = ConsoleHandleToPointer(g_Value8B055C);
            // Slot 0 of the vtable is the scalar deleting destructor (call [eax]), pushed arg = 1.
            using ScalarDeletingDtorFn = int(__fastcall*)(void* self, void* edx, int flags);
            result = reinterpret_cast<ScalarDeletingDtorFn>(CSF_AT(void**, console, 0)[0])(console, nullptr, 1);
            g_Value8B055C = 0;
        }
    }

    return result;
}

HOOK(0x004F9A20, MenuControls_RefreshCursor, "auto");
