#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void*));
static uintptr_t __fastcall AiCore_DestroyTrailingTask(void* self, void*) {
    uintptr_t result = reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410));
    CSF_AT(void*, self, 0x4c) = CSF_VTBL(0x0082D410);
    CSF_AT(void*, self, 0x34) = CSF_VTBL(0x0082D410);
    void* child = CSF_AT(void*, self, 0x18);
    if (child) {
        if (CSF_AT(uint32_t, child, -4)) {
            using Delete = uintptr_t (__thiscall*)(void*, int);
            result = reinterpret_cast<Delete>(*reinterpret_cast<void***>(child)[0])(child, 3);
        } else result = CoreMemory_Free(static_cast<char*>(child)-4);
        CSF_AT(void*, self, 0x18) = nullptr;
    }
    char* array = CSF_AT(char*, self, 8);
    if (array) {
        int count = CSF_AT(int, array, -4);
        for (int i = count; i > 0; --i) {
            CSF_AT(void*, array, (i-1)*52+36) = CSF_VTBL(0x0082D410);
            CSF_AT(void*, array, (i-1)*52+12) = CSF_VTBL(0x0082D410);
        }
        result = CoreMemory_Free(array-4);
        CSF_AT(void*, self, 8) = nullptr;
    }
    return result;
}
HOOK(0x004B9660, AiCore_DestroyTrailingTask, "auto");
