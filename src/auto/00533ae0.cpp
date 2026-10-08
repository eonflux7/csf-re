#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the current global integer (cdecl, no args).
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

// Resolves an object from a global id (thiscall; ecx = this, edx unused).
CSF_FUNC(int, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

static int __cdecl MenuMpPause_ApplyDisplayRectToCurrentObject()
{
    int objectId = GetGlobalInt();
    int resolved = MenuControls_ResolveObject(reinterpret_cast<void*>(static_cast<uintptr_t>(objectId)), nullptr);

    // Vtable slot 18 (+0x48): returns the target object pointer.
    void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(resolved));
    int targetValue = ((int(__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[18])(target, nullptr);
    int result = targetValue;

    if (result != 0)
    {
        // Local float block: {255, 255, 255, 50} as in the original bit patterns
        // (0x437F0000, 0x437F0000, 0x437F0000, 0x42480000).
        float rect[4];
        rect[3] = 50.0f;
        rect[0] = 255.0f;
        rect[1] = 255.0f;
        rect[2] = 255.0f;

        void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
        // Vtable slot 11 (+0x2C): thiscall taking a pointer to the float block.
        ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, obj, 0)[11])(obj, nullptr, rect);
        return result;
    }
    return result;
}

HOOK(0x00533AE0, MenuMpPause_ApplyDisplayRectToCurrentObject, "auto");
