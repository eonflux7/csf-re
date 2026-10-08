#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void* (__fastcall* const ResolveInstanceFactory)(void*, void*) =
    reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_ADDR_FN(0x00686C10));

static int __fastcall C_008657e0_ReplaceInstance(
    C_008657e0* self, void* /*edx*/, int configuration, int value)
{
    void* source =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(self->vtbl[6])(
            self, nullptr);
    if (!source)
        return 0;

    void* factory = ResolveInstanceFactory(source, nullptr);
    if (!factory)
        return 0;

    void* previousInstance = reinterpret_cast<void*>(self->field_0030);
    if (previousInstance)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previousInstance, 0)[0])(
                previousInstance, nullptr, 1);
        self->field_0030 = 0;
    }

    void* instance =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, factory, 0)[17])(factory, nullptr);

    self->field_0030 = reinterpret_cast<std::uint32_t>(instance);

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, instance, 0)[11])(instance, nullptr, configuration);

    self->field_0034 = static_cast<std::uint32_t>(value);
    self->field_0038 = static_cast<std::uint32_t>(value);
    return value;
}

HOOK(0x007284A0, C_008657e0_ReplaceInstance, "auto");
