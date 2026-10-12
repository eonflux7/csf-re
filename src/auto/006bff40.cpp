#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Helpers for the original's callees, bound by address.
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* edx));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwFrameRemoveChild, 0x00623750, (int a1));

// Removes the RwFrame child held in an embedded state object, when the
// virtual predicate in slot 0x78 reports true and the child is still attached.
static void __fastcall C_00860700_RemoveChildFrameIfReady(C_00860700* self, void* /*edx*/, int a2)
{
    // Stack-resident embedded state object (room 0x20 bytes in the frame).
    alignas(8) uint8_t embeddedBytes[0x20];
    C_00860630& v4 = *reinterpret_cast<C_00860630*>(embeddedBytes);

    C_00860178_Construct(reinterpret_cast<C_00860178*>(&v4), nullptr);

    // Vptr at offset 0 of the embedded object (the derived class does not expose __vftable as a member).
    CSF_AT(void*, embeddedBytes, 0x00) = reinterpret_cast<void*>(CSF_VTBL(0x00860700));
    v4.field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008606F8)));
    v4.field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008606DC)));
    memset(&v4.field_0014, 0, 12);
    v4.field_000c = 0;
    CSF_AT(uint8_t, embeddedBytes, 0x10) = 0;

    // Virtual predicate: slot 0x78, arguments (a2, &v4) as pushed by the original.
    if (self->vSlot78(a2, reinterpret_cast<int>(&v4)) && CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(v4.field_000c), 4) != 0)
    {
        RwFrameRemoveChild(static_cast<int>(v4.field_000c));
    }

    GfxObject_DestroyEmbeddedState(&v4, nullptr);
}

HOOK(0x006BFF40, C_00860700_RemoveChildFrameIfReady, "auto");
