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

// Game heap allocator (cdecl, one stack argument: the size).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Initializer for C_008300c0 (__thiscall, returns int).
CSF_FUNC(int, __fastcall, C_008300c0_Init, 0x00429360, (C_008300c0* self, void*));

// Caches a lazily created C_008300c0 object at this+0x1c, then tail-jumps
// into its vtable slot 15 (offset 0x3c) with the caller's stack args intact.
static hook::Tail __fastcall GetOrCreateSubobjectAndForward(void* self, const uint32_t* args)
{
    int result = CSF_AT(int, self, 0x1c);

    if (result == 0)
    {
        void* allocated = Mem_Alloc(0xe9c);
        if (allocated)
            result = C_008300c0_Init(reinterpret_cast<C_008300c0*>(allocated), nullptr);
        else
            result = 0;

        CSF_AT(int, self, 0x1c) = result;

        if (result != 0)
        {
            void* obj = reinterpret_cast<void*>(result);
            return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x3c / 4]);
        }
    }

    return hook::tail_ret(result);
}

HOOK_TAIL(0x506C10, GetOrCreateSubobjectAndForward, "app_main");
