#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowEntryBuffer, 0x00406EC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, HandleTargetUnavailable, 0x0048FE90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char __fastcall C_0082d77c_ResolveEntriesAndActivate(
    C_0082d77c* self, void* /*edx*/, void* target)
{
    self->field_0008 = reinterpret_cast<uint32_t>(target);

    void* context =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, target, 0)[0xA0 / 4])(target, nullptr);
    void* resolver =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, context, 0)[0x68 / 4])(context, nullptr);

    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x24);
    const uint32_t capacity = self->field_0010;
    if (entryCount > capacity)
    {
        GrowEntryBuffer(
            reinterpret_cast<uint8_t*>(self) + 0x0C,
            nullptr,
            static_cast<int>(entryCount - capacity));
    }
    CSF_AT(uint32_t, self, 0x14) = entryCount;

    uint32_t entryOffset = 0;
    for (uint32_t index = 0; index < entryCount; ++index)
    {
        auto* entry =
            reinterpret_cast<uint8_t*>(self->field_000c) + entryOffset;
        const uint32_t identifier = CSF_AT(
            uint32_t, reinterpret_cast<void*>(self->field_001c),
            8 * index + 4);

        const uint32_t resolved =
            ((uint32_t (__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, resolver, 0)[0x68 / 4])(
                    resolver, nullptr, identifier);
        CSF_AT(uint32_t, entry, 8) = resolved;
        entryOffset += 44;
    }

    void* pendingEntries = reinterpret_cast<void*>(self->field_001c);
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    if (pendingEntries)
    {
        Mem_Free(pendingEntries);
        self->field_001c = 0;
    }

    resolver =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, context, 0)[0x68 / 4])(context, nullptr);
    const uint32_t activationEntry =
        ((uint32_t (__fastcall*)(void*, void*, int))
            CSF_AT(void**, resolver, 0)[0x68 / 4])(resolver, nullptr, 20);
    const int available =
        ((int (__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, resolver, 0)[0x6C / 4])(
                resolver, nullptr, activationEntry);

    if (available)
    {
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, resolver, 0)[0x10 / 4])(resolver, nullptr, 20);
    }
    else
    {
        HandleTargetUnavailable(
            reinterpret_cast<void*>(self->field_0008), nullptr);
    }
    return 1;
}

HOOK(0x00407370, C_0082d77c_ResolveEntriesAndActivate, "auto");
