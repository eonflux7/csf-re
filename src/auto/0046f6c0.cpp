#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_46F130, 0x0046F130, (void* self, void* edx, int16_t* a2));
CSF_FUNC(int, __fastcall, sub_46F1A0, 0x0046F1A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_46F210, 0x0046F210, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, sub_4714E0, 0x004714E0, (void* Block));

// Listed by the packet as a vtable, although the disassembly calls it directly.
[[maybe_unused]] static const auto kListedResolverVtable = CSF_VTBL(0x0046F130);

static int __fastcall S_0046f130_ResolveEntryReference(
    S_0046f130* self, void* /*edx*/, unsigned int index)
{
    const uint32_t entryOffset = 4 * index;
    auto* entry = CSF_AT(
        S_0069b1c0*, reinterpret_cast<void*>(self->field_0004), entryOffset);

    int resolvedIndex;
    if (CSF_AT(uint16_t, entry, 2) == 0xFFFF)
    {
        void* value = reinterpret_cast<void*>(entry->field_0004);
        if (CSF_AT(int32_t, value, 8) == -1)
        {
            resolvedIndex = sub_46F210(
                self, nullptr, reinterpret_cast<int>(entry));
        }
        else
        {
            resolvedIndex = sub_46F1A0(
                self, nullptr, reinterpret_cast<int>(entry));
        }
    }
    else
    {
        resolvedIndex = sub_46F130(
            self, nullptr, reinterpret_cast<int16_t*>(entry));
    }

    if (resolvedIndex == -1)
        return resolvedIndex;

    entry = CSF_AT(
        S_0069b1c0*, reinterpret_cast<void*>(self->field_0004), entryOffset);
    if (entry)
    {
        auto* value = reinterpret_cast<S_0069b1c0*>(entry->field_0004);
        if (value)
        {
            ((void (__fastcall*)(S_0069b1c0*, void*, uint32_t))
                CSF_AT(void**, value, 0)[0])(value, nullptr, 0);
            sub_4714E0(value);
        }
        entry->field_0004 = 0;
        sub_4714E0(entry);
    }

    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), entryOffset) = 0;

    void* entries = reinterpret_cast<void*>(self->field_0004);
    CSF_AT(uint32_t, entries, entryOffset) =
        CSF_AT(uint32_t, entries, 4 * static_cast<uint32_t>(resolvedIndex));

    uint32_t& pendingBits = CSF_AT(
        uint32_t, reinterpret_cast<void*>(self->field_0018), 4 * (index >> 5));
    const uint32_t remainingBits = pendingBits & ~(1u << (index & 0x1F));
    pendingBits = remainingBits;
    return static_cast<int>(remainingBits);
}

HOOK(0x0046F6C0, S_0046f130_ResolveEntryReference, "auto");
