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

// Bound callee: CHandleTable::Resolve, __thiscall, ret 4.
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Stack-built argument block passed by pointer to the target object's slot-17 method.
// Layout matches the original frame: 0x24 bytes.
struct SceneCommandArgs {
    void*    vtbl;      // +0x00  C_0082d448 vtable
    int32_t  f04;       // +0x04  -1
    int32_t  f08;       // +0x08  -1
    uint8_t  f0C;       // +0x0C  0
    uint8_t  pad0D[3];
    int32_t  f10;       // +0x10  0
    int32_t  f14;       // +0x14  0
    int32_t  f18;       // +0x18  0
    float    f1C;       // +0x1C  -1.0f
    int32_t  f20;       // +0x20  7 or 8 (from target flag bit 1)
};
static_assert(sizeof(SceneCommandArgs) == 0x24);

static int __stdcall SceneDispatchHandleCommand(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    // a3 points to a pointer-to-object; object+4 holds a sub-record whose +12 is a signed value.
    uintptr_t objPtr = static_cast<uintptr_t>(*reinterpret_cast<int*>(a3));
    uintptr_t v3 = static_cast<uintptr_t>(*CSF_AT(int*, reinterpret_cast<void*>(objPtr), 4));
    int v4 = *CSF_AT(int*, reinterpret_cast<void*>(v3), 12);
    int result = static_cast<int>(v3 + 4);

    if (v4 >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result != 0)
        {
            uintptr_t entry = static_cast<uintptr_t>(result);
            if (*CSF_AT(int*, reinterpret_cast<void*>(entry), 0x50) == 26)
            {
                void* target = reinterpret_cast<void*>(
                    static_cast<uintptr_t>(*CSF_AT(int*, reinterpret_cast<void*>(entry), 0x34)));

                SceneCommandArgs args;
                std::memset(&args, 0, sizeof(args));
                args.vtbl = CSF_VTBL(0x0082D448);
                args.f04 = -1;
                args.f08 = -1;
                args.f0C = 0;
                args.f10 = 0;
                args.f14 = 0;
                args.f18 = 0;
                args.f1C = -1.0f;

                uint32_t targetFlags = *CSF_AT(uint32_t*, target, 0x44);
                args.f20 = ((targetFlags & 2u) != 0u) ? 8 : 7;

                // Virtual slot 0x44 / 4 = 17, __thiscall: target->vtbl[17](target, &args)
                return ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, target, 0)[17])(
                    target, nullptr, &args);
            }
        }
    }
    return result;
}

HOOK(0x00463DF0, SceneDispatchHandleCommand, "auto");
