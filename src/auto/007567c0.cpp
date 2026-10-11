#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns the dword at index `index` of the table whose pointer is stored at +0x08 of the object.
static int __fastcall SceneLoader_GetResourceById(void* self, void* /*edx*/, int index) {
    int* table = CSF_AT(int*, self, 0x08);
    return table[index];
}

HOOK(0x7567C0, SceneLoader_GetResourceById, "auto");
