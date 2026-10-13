#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __stdcall ComputeLinkedTaskFlag(void* self, int value) {
    void* link = CSF_AT(void*, self, 12);
    if (!link) return 0;
    void* object = CSF_AT(void*, link, 16);
    using Query = int (__thiscall*)(void*);
    return reinterpret_cast<Query>((*reinterpret_cast<void***>(object))[7])(object) == value;
}
HOOK(0x004DB620, ComputeLinkedTaskFlag, "auto");
