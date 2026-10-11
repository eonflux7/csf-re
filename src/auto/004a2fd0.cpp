#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

// Forwards a2 to the inner object's slot 2 (at +0x1FC4 of GetValue8B0554()),
// then calls this->vtbl[0x7C/4] with (-1, result, a3, a4, a5). If that returns -1,
// releases the sub-object at this->field_0008 (+4) via its vtable slot 3 and returns -1.
static int __fastcall C_008344f0_ForwardAndReleaseOnFailure(C_008344f0* self, void* /*edx*/,
                                                            int a2, int a3, int a4, int a5) {
    void* gameObj = reinterpret_cast<void*>(GetValue8B0554());
    void* inner = CSF_AT(void*, gameObj, 0x1FC4);

    int resolvedValue =
        ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, inner, 0)[2])(inner, nullptr, a2);

    int result =
        ((int(__fastcall*)(void*, void*, int, int, int, int, int))self->vtbl[0x7C / 4])(
            self, nullptr, -1, resolvedValue, a3, a4, a5);

    if (result == -1) {
        C_0082d410* owner = reinterpret_cast<C_0082d410*>(self->field_0008);
        void* sub = &owner->field_0004;
        void** subVtbl = reinterpret_cast<void**>(owner->field_0004);
        ((void(__fastcall*)(void*, void*))subVtbl[0x0C / 4])(sub, nullptr);
        return -1;
    }
    return result;
}

HOOK(0x4A2FD0, C_008344f0_ForwardAndReleaseOnFailure, "auto");
