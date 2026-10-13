#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, menuState, 0x008B0674);
CSF_GLOBAL(void*, auxiliaryState, 0x008BBF2C);
static uintptr_t __cdecl MenuSubsystem_ReleaseState() {
    uintptr_t result = reinterpret_cast<uintptr_t>(menuState);
    if (menuState) {
        using Delete = uintptr_t (__thiscall*)(void*, int);
        void* object = static_cast<char*>(menuState)+4;
        result = reinterpret_cast<Delete>((*reinterpret_cast<void***>(object))[0])(object, 1);
        menuState = nullptr;
        if (auxiliaryState) {
            result = reinterpret_cast<Delete>((*reinterpret_cast<void***>(auxiliaryState))[0])(auxiliaryState, 1);
            auxiliaryState = nullptr;
        }
    }
    return result;
}
HOOK(0x00521BC0, MenuSubsystem_ReleaseState, "auto");
