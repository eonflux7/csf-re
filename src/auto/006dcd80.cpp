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

// Bound callees (addresses from the packet).
CSF_FUNC(int, __fastcall, SetupRenderTargetFromDesc, 0x006CA410, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

// Forwards to SetupRenderTargetFromDesc, resolves the global accessor, calls its
// slot +0x10 getter, then, when a2->field_0018 is nonzero, calls the returned
// object's slot +0x1C with either a pointer to a2+0x8 (when a2->field_001C < 0x10)
// or the value stored at a2+0x8.
static int __fastcall C_00861508_InitAndDispatchAccessorCall(C_00861508* self, void* /*edx*/, uint32_t* a2)
{
    SetupRenderTargetFromDesc(self, nullptr, (int)(uintptr_t)a2);

    int globalInt = GetGlobalInt();
    void* accessor = (void*)(uintptr_t)GlobalInt_ResolveAccessor((void*)(uintptr_t)globalInt, nullptr);

    int result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, accessor, 0)[4])(accessor, nullptr);

    if (CSF_AT(uint32_t, a2, 0x18)) {
        void* resultObj = (void*)(uintptr_t)result;
        if (CSF_AT(uint32_t, a2, 0x1C) < 0x10u) {
            return ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, resultObj, 0)[7])(
                resultObj, nullptr, (void*)&CSF_AT(uint32_t, a2, 0x8));
        }
        return ((int(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, resultObj, 0)[7])(
            resultObj, nullptr, CSF_AT(uint32_t, a2, 0x8));
    }
    return result;
}

HOOK(0x006DCD80, C_00861508_InitAndDispatchAccessorCall, "auto");
