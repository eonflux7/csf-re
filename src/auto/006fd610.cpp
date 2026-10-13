#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, Light_GetWorldMatrixPointer, 0x006FD330, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00860e68_AddLightIfNotCurrent, 0x006D2000, (C_00860e68* self, void* , C_0085e7b8* a2));

static void __fastcall CLightObject_ResetDirtyAndRegisterLight(CLightObject* self, void* /*edx*/, int a2)
{
    // call dword ptr [eax+0x14] where eax = the value of field_0010:
    // the target is the pointer stored at (field_0010 + 0x14), not a vtable slot.
    void* embeddedObj = reinterpret_cast<void*>(self->field_0010);
    using MethodFn = void(__fastcall*)(void* self, void* edx, int a2);
    MethodFn target = reinterpret_cast<MethodFn>(CSF_AT(void*, embeddedObj, 0x14));
    target(reinterpret_cast<void*>(&self->field_0010), nullptr, a2);

    CSF_AT(uint8_t, self, 0x34) = 0;

    Light_GetWorldMatrixPointer(self, nullptr);

    C_00860e68_AddLightIfNotCurrent(
        reinterpret_cast<C_00860e68*>(self->field_0008),
        nullptr,
        reinterpret_cast<C_0085e7b8*>(self));
}

HOOK(0x006FD610, CLightObject_ResetDirtyAndRegisterLight, "auto");
