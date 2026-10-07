#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall ResolveIndexedEntry(void* self, void* /*edx*/, int index)
{
    void* owner = CSF_AT(void*, self, 0x08);
    void* context =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[10])(
            owner, nullptr);

    void* resolver =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, context, 0)[26])(
            context, nullptr);

    void* entries = CSF_AT(void*, self, 0x0C);
    const std::uint32_t offset = static_cast<std::uint32_t>(index) * 0x2Cu;
    void** resolverVtable = CSF_AT(void**, resolver, 0);
    void* entry = reinterpret_cast<void*>(
        reinterpret_cast<std::uintptr_t>(entries) + offset);
    const std::uint32_t key = CSF_AT(std::uint32_t, entry, 0x08);

    return ((int(__fastcall*)(void*, void*, std::uint32_t))resolverVtable[27])(
        resolver, nullptr, key);
}

HOOK(0x00406E90, ResolveIndexedEntry, "auto");
