// Auto-generated reimplementation for 0x006B3780 (C_00860288 method).
// Packs four float channels (scaled by g_ColorChannelScale and truncated toward zero) into one 32-bit value,
// fetches an entry from the object at field_0068 through vtable slot 0x1C, and stores the packed value into
// that entry via C_008670f0_SetEntry.

#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008670f0_SetEntry, 0x006C4B00, (C_008670f0* self, void* /*edx*/, int key, int value));

CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

// fld dword channel; fmul dword g_ColorChannelScale; __ftol2: the product of two 24-bit significands is exact in
// double, and the conversion truncates toward zero into a 64-bit result (edx:eax in the original).
static int64_t ColorChannelToInt64(float channel)
{
    return static_cast<int64_t>(static_cast<double>(channel) * static_cast<double>(g_ColorChannelScale));
}

static int __fastcall C_00860288_PackColorIntoEntry(C_00860288* self, void* /*edx*/, float* a2)
{
    // Conversion order mirrors the original: a2[3], a2[0], a2[1], a2[2]. Each low dword is shifted in; the high
    // dword of the last conversion (a2[2]) is what the original leaves in edx for the vtable call.
    uint32_t packed = static_cast<uint32_t>(ColorChannelToInt64(a2[3]));
    packed = (packed << 8) | static_cast<uint32_t>(ColorChannelToInt64(a2[0]));
    packed = (packed << 8) | static_cast<uint32_t>(ColorChannelToInt64(a2[1]));
    const int64_t lastChannel = ColorChannelToInt64(a2[2]);
    packed = (packed << 8) | static_cast<uint32_t>(lastChannel);
    const uint32_t edxHigh = static_cast<uint32_t>(static_cast<uint64_t>(lastChannel) >> 32);

    // Re-read the field after the conversions, as the original does.
    void* colorObject = reinterpret_cast<void*>(self->field_0068);
    void** vtbl = CSF_AT(void**, colorObject, 0);

    // call [vtbl + 0x1C] with ecx = object, edx = edxHigh, stack = (6, packed), then mov ecx, eax; call SetEntry.
    // The original's pushes are the same two words SetEntry reads, so the stack goes through call_regs, which
    // removes them again after the vtable call. Registers the original leaves set are passed through too.
    hook::Regs r{};
    r.eax = reinterpret_cast<uint32_t>(vtbl);
    r.ecx = reinterpret_cast<uint32_t>(colorObject);
    r.edx = edxHigh;
    r.ebx = reinterpret_cast<uint32_t>(self);
    r.esi = packed;
    r.edi = reinterpret_cast<uint32_t>(a2);
    uint32_t stack[2] = {6u, packed};
    uint32_t entryValue = hook::call_regs(reinterpret_cast<uintptr_t>(vtbl[7]), &r, stack, 2);

    C_008670f0* entry = reinterpret_cast<C_008670f0*>(entryValue);
    return C_008670f0_SetEntry(entry, nullptr, 6, static_cast<int>(packed));
}

HOOK(0x006B3780, C_00860288_PackColorIntoEntry, "auto");
