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

// Original callees, bound by address with the signatures given in the packet.
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, GetFlagBit0At4, 0x006B8120, (void* self, void*));

// Render callbacks stored into the object at +0x4C.
// Their addresses are used only as values here.

// Vtable slot 0x90 of C_00860e68 is slot 36 (0x90 / 4).
static const int kSelectorVtableSlot = 0x90 / 4;

// Chooses which callback an RpLight-style object gets, based on the object's attachment.
// __thiscall (ecx = this, edx unused), callee pops 4 bytes.
static int __fastcall C_00860e68_SelectAttachmentCallback(C_00860e68* self, void* /*edx*/, int a2)
{
    int objectAttachment = GfxObjectProps_GetObjectAttachment(a2);

    // Virtual call through slot 0x90 with (this, attachment).
    int result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[kSelectorVtableSlot])(
        self, nullptr, objectAttachment);

    if ((uint8_t)result)
    {
        result = GetFlagBit0At4(reinterpret_cast<void*>(objectAttachment), nullptr);
        uint8_t* objectProps = reinterpret_cast<uint8_t*>(a2);
        if ((uint8_t)result)
        {
            CSF_AT(uint32_t, objectProps, 0x4C) =
                static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x006D1C70)));
        }
        else
        {
            CSF_AT(uint32_t, objectProps, 0x4C) =
                static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x006D1C90)));
        }
    }
    return result;
}

HOOK(0x006D2220, C_00860e68_SelectAttachmentCallback, "auto");
