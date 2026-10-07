#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseResourceReference, 0x006923C0,
         (void* self, void* edx));

static char __fastcall CGuiBuilder_AssignCurrentResource(
    CGuiBuilder* self, void* /*edx*/, void* control)
{
    const uint8_t enabled = self->field_0008;
    if (!enabled)
        return static_cast<char>(enabled);

    void* provider = reinterpret_cast<void*>(self->field_0084);
    if (!provider)
        return 0;

    void* resource =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, provider, 0)[0x80 / 4])(provider, nullptr);
    if (!resource)
        return 0;

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, control, 0)[0x248 / 4])(control, nullptr, resource);

    return static_cast<char>(ReleaseResourceReference(
        static_cast<uint8_t*>(resource) + 4, nullptr));
}

HOOK(0x0071D850, CGuiBuilder_AssignCurrentResource, "auto");
