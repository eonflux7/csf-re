#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseForwardedObject, 0x006923C0,
         (void* self, void* edx));

static int __fastcall C_008655e8_ForwardAndReleaseObject(
    C_008655e8* self, void* /*edx*/, int selector)
{
    void* object =
        ((void* (__fastcall*)(C_008655e8*, void*, int))self->vtbl[19])(
            self, nullptr, selector);

    if (!object)
        return 0;

    void* recipient =
        ((void* (__fastcall*)(C_008655e8*, void*))self->vtbl[22])(
            self, nullptr);

    if (recipient)
    {
        ((void (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, recipient, 0)[9])(
                recipient, nullptr, object);
    }

    ReleaseForwardedObject(object, nullptr);
    return static_cast<int>(reinterpret_cast<std::intptr_t>(recipient));
}

HOOK(0x00726940, C_008655e8_ForwardAndReleaseObject, "auto");
