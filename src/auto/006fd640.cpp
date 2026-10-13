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

CSF_FUNC(void, __fastcall, C_00860e68_AddLightIfNotCurrent, 0x006D2000, (C_00860e68* self, void* , C_0085e7b8* a2));
CSF_FUNC(char, __fastcall, C_00860e68_AppendLightAndAttach, 0x006D20D0, (C_00860e68* self, void* , C_0085e7b8* a2));
CSF_FUNC(uint32_t*, __fastcall, Light_GetWorldMatrixPointer, 0x006FD330, (void* self, void* edx));

// Original: __thiscall, ret 8. Calls a2's slot 33 with (v4, a3), where v4 = this->slot 9 result.
static int __fastcall CLightObject_ProcessArgWithZeroTail(CLightObject* self, void* /*edx*/, void* a2, int a3)
{
    // v4 = this->vSlot24()  (vtable slot 0x24 / 4 = 9)
    void* v4 = reinterpret_cast<void*>(
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[9])(self, nullptr));

    // v4->vtbl[136 / 4]()  -> slot 34, no arguments
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, v4, 0)[34])(v4, nullptr);

    // a2->vtbl[132 / 4](v4, a3)  -> slot 33
    ((void(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, a2, 0)[33])(a2, nullptr, v4, a3);

    CSF_AT(uint8_t, self, 0x34) = 0;

    Light_GetWorldMatrixPointer(self, nullptr);

    C_00860e68_AddLightIfNotCurrent(
        CSF_AT(C_00860e68*, self, 0x08),
        nullptr,
        reinterpret_cast<C_0085e7b8*>(self));

    char appended = C_00860e68_AppendLightAndAttach(
        CSF_AT(C_00860e68*, self, 0x08),
        nullptr,
        reinterpret_cast<C_0085e7b8*>(self));
    return static_cast<int>(appended);
}

HOOK(0x006FD640, CLightObject_ProcessArgWithZeroTail, "auto");
