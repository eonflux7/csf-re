#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, HudStringShow_DestroyEntryRange, 0x004F23A0,
         (int a1, int a2));
CSF_FUNC(int, __fastcall, HudStringShow_AssignEntry, 0x004F2AD0,
         (void* self, void* edx, int a2));

static int __fastcall S_004f1f00_RemoveExpiredEntries(
    S_004f1f00* self, void* /*edx*/)
{
    int count = 0;
    if (self->field_0008)
        count = static_cast<int32_t>(self->field_000c - self->field_0008) / 124;

    int result = count - 1;
    if (result < 0)
        return result;

    int offset = 124 * result;
    do
    {
        void* entry = reinterpret_cast<void*>(self->field_0008 + offset);
        if (CSF_AT(float, entry, 0x3C) >= CSF_AT(float, entry, 0x40))
        {
            void* resource = CSF_AT(void*, entry, 0x78);
            if (resource)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, resource, 0)[0])(resource, nullptr, 1);
                CSF_AT(uint32_t, entry, 0x78) = 0;
            }

            uint32_t source = self->field_0008 + offset + 124;
            const uint32_t end = self->field_000c;
            while (source != end)
            {
                HudStringShow_AssignEntry(
                    reinterpret_cast<void*>(source - 124), nullptr,
                    static_cast<int32_t>(source));
                source += 124;
            }

            const uint32_t oldEnd = self->field_000c;
            HudStringShow_DestroyEntryRange(
                static_cast<int32_t>(oldEnd - 124),
                static_cast<int32_t>(oldEnd));
            self->field_000c -= 124;
        }

        offset -= 124;
        result = --count;
    } while (count != 0);

    return result;
}

HOOK(0x004F3290, S_004f1f00_RemoveExpiredEntries, "auto");
