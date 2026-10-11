#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint32_t LookupResourceById(void* source, int resourceId)
{
    if (!source)
        return 0;

    void* resources =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, source, 0)[0x28 / 4])(source, nullptr);
    if (!resources)
        return 0;

    int index =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, resources, 0)[0x88 / 4])(
                resources, nullptr, resourceId);
    if (index == -1)
        return 0;

    return reinterpret_cast<uint32_t (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, resources, 0)[0x80 / 4])(
            resources, nullptr, index);
}

static void __fastcall C_0082d58c_CacheResources(
    C_0082d58c* self, void* /*edx*/)
{
    void* source = reinterpret_cast<void*>(self->field_0008);
    uint32_t resource = LookupResourceById(source, 510);

    source = reinterpret_cast<void*>(self->field_0008);
    self->field_0028 = resource;
    resource = LookupResourceById(source, 520);

    source = reinterpret_cast<void*>(self->field_0008);
    self->field_002c = resource;
    resource = LookupResourceById(source, 511);

    source = reinterpret_cast<void*>(self->field_0008);
    self->field_0030 = resource;
    self->field_0034 = LookupResourceById(source, 521);
}

HOOK(0x004054F0, C_0082d58c_CacheResources, "auto");
