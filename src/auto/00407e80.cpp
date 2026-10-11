#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Resolves a child object through this->field_0008, then asks the resolved
// object for an index for a2, and returns the value that index maps to.
static int __fastcall C_0082d5e0_ResolveSubtreeValue(C_0082d5e0* self, void* /*edx*/, int a2)
{
    void* child = reinterpret_cast<void*>(self->field_0008);

    // First hop: child->vtbl[0x28 / 4](child), no arguments.
    void* resolved = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, child, 0)[0x28 / 4])(child, nullptr);

    // Second hop: resolved->vtbl[0x68 / 4](resolved), no arguments.
    void* target = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, resolved, 0)[0x68 / 4])(resolved, nullptr);
    if (!target)
        return 0;

    // Map a2 to an index on the target object.
    int index = ((int (__fastcall*)(void*, void*, int))
        CSF_AT(void**, target, 0)[0x68 / 4])(target, nullptr, a2);
    if (index < 0)
        return 0;

    // Return the value the target reports for that index.
    return ((int (__fastcall*)(void*, void*, int))
        CSF_AT(void**, target, 0)[0x6c / 4])(target, nullptr, index);
}

HOOK(0x00407E80, C_0082d5e0_ResolveSubtreeValue, "auto");
