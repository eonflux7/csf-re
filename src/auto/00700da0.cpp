#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address.
CSF_FUNC(int, __fastcall, C_00867358_ClearFields, 0x006C3620, (C_00867358* self, void*));
CSF_FUNC(void*, __fastcall, TeamGfx_EnsureInnerObject, 0x006FF7E0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, TeamGfx_DispatchPlayerEntry, 0x00700630, (void* self, void* edx, int a2, int a3));

// Vtable referenced by the original (stored at the sub-object created by 0x6FF7E0).
[[maybe_unused]] static const void* const s_InnerObjectVtable = CSF_VTBL(0x006FF7E0);

// Rebuilds the team's player list: clears the inner object, then dispatches every
// 12-byte entry of the table to TeamGfx_DispatchPlayerEntry.
// Returns the entry count (low byte) on the full path, 0 on the early-out paths.
static char __fastcall S_006ff7e0_RefreshPlayerEntries(S_006ff7e0* self, void* /*edx*/)
{
    if (self->field_0004 == 0)
        return 0;
    if (self->field_0011 == 0)
        return 0;

    if (self->field_0024 == 0)
        TeamGfx_EnsureInnerObject(self, nullptr);
    C_00867358_ClearFields(reinterpret_cast<C_00867358*>(self->field_0024), nullptr);

    uint16_t entryCount = CSF_AT(uint16_t, reinterpret_cast<uint8_t*>(self->field_0004), 0xC);
    int index = 0;
    if (entryCount != 0)
    {
        uint32_t byteOffset = 0;
        do
        {
            uint8_t* header = reinterpret_cast<uint8_t*>(self->field_0004);
            uint8_t* table = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, header, 0x10));
            uint32_t entry = CSF_AT(uint32_t, table, byteOffset);
            TeamGfx_DispatchPlayerEntry(self, nullptr, static_cast<int>(entry), index);
            ++index;
            byteOffset += 12;
        } while (index < static_cast<int>(entryCount));
    }
    return static_cast<char>(entryCount);
}

HOOK(0x700DA0, S_006ff7e0_RefreshPlayerEntries, "auto");
