#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());

// Stores the value in the object's +0x0C slot, then forwards it to the child
// object at +0x1C of the global value (vtable slot 6) if that child exists.
// Returns the global value when there is no child, otherwise the child's result.
static int __fastcall OptionsFile_StoreValueAndNotifyChildSlot6(void* self, void* /*edx*/, int value)
{
    CSF_AT(int, self, 0x0C) = value;

    int result = GetGlobalValue_8B056C();
    void* globalObj = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
    void* child = CSF_AT(void*, globalObj, 0x1C);

    if (child)
    {
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[6])(
            child, nullptr, CSF_AT(int, self, 0x0C));
    }
    return result;
}

HOOK(0x0050A540, OptionsFile_StoreValueAndNotifyChildSlot6, "auto");
