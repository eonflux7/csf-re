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

// Vtable of S_004054a0, referenced by the original (the call target 0x4054A0 is the
// class's ctor/method entry). Bound by address; no runtime store is performed here.
static const void* const kS004054A0Vtable = CSF_VTBL(0x004054A0);

CSF_FUNC(int, __fastcall, sub_4054A0, 0x004054A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4065B0, 0x004065B0, (void* self, void* edx, unsigned int a2));

static char __fastcall S_004054A0_EnsureActivated(S_004054a0* self, void* /*edx*/)
{
    char result = static_cast<char>(self->field_0039);
    if (!result)
    {
        self->field_0038 = 0;

        void* pChildObject = reinterpret_cast<void*>(sub_4054A0(self, nullptr, 0x201));
        if (pChildObject)
        {
            // vtbl[10]: returns a sub-object; vtbl[6] on it, with no args.
            void* pSubObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, pChildObject, 0)[10])(pChildObject, nullptr);
            ((void (__fastcall*)(void*, void*))CSF_AT(void**, pSubObject, 0)[6])(pSubObject, nullptr);

            // vtbl[10] again, then vtbl[1] with a single argument 0.
            void* pSubObjectAgain = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, pChildObject, 0)[10])(pChildObject, nullptr);
            ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, pSubObjectAgain, 0)[1])(pSubObjectAgain, nullptr, 0);
        }

        void* pOwnerObject = reinterpret_cast<void*>(self->field_0040);
        if (pOwnerObject)
        {
            ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, pOwnerObject, 0)[11])(pOwnerObject, nullptr, 1);
        }

        result = static_cast<char>(sub_4065B0(self, nullptr, 1u));
        self->field_0039 = 1;
    }
    return result;
}

HOOK(0x00406720, S_004054A0_EnsureActivated, "auto");
