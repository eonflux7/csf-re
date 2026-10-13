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

CSF_GLOBAL(int, g_Value_8B0660, 0x008B0660);
CSF_GLOBAL(uint32_t, g_NetProfile_Instance, 0x008BB3F0);

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, sub_69D840, 0x0069D840, (void* self, void* edx));

// Releases the net profile singleton: calls its owned sub-object's
// virtual deleter, destroys two embedded sub-objects, frees the block.
static int NetProfile_ReleaseInstanceBody()
{
    int result = 0;
    if (g_NetProfile_Instance)
    {
        uint8_t* instance = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(g_NetProfile_Instance));

        uint32_t owned = CSF_AT(uint32_t, instance, 0x288);
        if (owned)
        {
            void* ownedObj = reinterpret_cast<void*>(static_cast<uintptr_t>(owned));
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, ownedObj, 0)[0])(ownedObj, nullptr, 1);
            CSF_AT(uint32_t, instance, 0x288) = 0;
        }

        sub_69D840(instance + 0x270, nullptr);
        sub_69D840(instance + 0x130, nullptr);

        result = Mem_Free(instance);
        g_NetProfile_Instance = 0;
    }
    return result;
}

static int __cdecl NetProfile_ReleaseState()
{
    if (g_Value_8B0660)
    {
        void* callbackObj = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8B0660));
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, callbackObj, 0)[0])(callbackObj, nullptr, 1);
        g_Value_8B0660 = 0;
    }
    return NetProfile_ReleaseInstanceBody();
}

HOOK(0x00519170, NetProfile_ReleaseState, "auto");
