#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable of CMenuScreenFader (0x0084004C). Bound by address.
CSF_GLOBAL(uint8_t, g_CMenuScreenFaderVtbl, 0x0084004C);

// Game heap free (0x691110, cdecl, one argument).
CSF_FUNC(void, __cdecl, Mem_Free, 0x691110, (void* block));

// Debug-heap free (0x690E60, cdecl, one argument, returns int).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x690E60, (void* block));

// Releases the node list at the embedded range (0x469760, cdecl, four arguments).
// Caller pushes: first node, &field_0038 (head), [this+0x40] (last), this (owner).
CSF_FUNC(int, __cdecl, DestroyElementRange, 0x469760,
         (void* first, void* head, void* last, void* owner));

// Releases the fader's owned resources and resets it to its default state.
// Original: __thiscall, ret 0.
static int* __fastcall CMenuScreenFader_ReleaseResources(CMenuScreenFader* self, void* /*edx*/)
{
    // Release the child object at +0x34 via its vtable slot 0 (deleting dtor, flag 1).
    void* overlayObject = reinterpret_cast<void*>(self->m_pOverlay);
    CSF_AT(uint32_t, self, 0x00) = reinterpret_cast<uint32_t>(&g_CMenuScreenFaderVtbl);
    if (overlayObject)
    {
        void** overlayVtbl = *reinterpret_cast<void***>(overlayObject);
        ((void(__fastcall*)(void*, void*, int))overlayVtbl[0])(overlayObject, nullptr, 1);
        self->m_pOverlay = 0;
    }

    if (self->field_0048)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0048));
        self->field_0048 = 0;
    }

    // Eax tracking: the original's final eax is the value returned to the caller.
    int* result = nullptr;
    if (self->field_003c)
    {
        DestroyElementRange(reinterpret_cast<void*>(self->field_003c),
                          reinterpret_cast<uint8_t*>(self) + 0x38,
                          reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x40)),
                          self);
        uint32_t remainingBlock = CSF_AT(uint32_t, self, 0x3C);
        if (remainingBlock)
            result = reinterpret_cast<int*>(Mem_FreeDebug(reinterpret_cast<void*>(remainingBlock)));
    }

    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;

    // Unsigned compare (jb): free the secondary buffer only when capacity >= 16.
    if (self->field_001c >= 0x10u)
    {
        uint32_t secondaryBuffer = self->field_0008;
        result = secondaryBuffer
                     ? reinterpret_cast<int*>(Mem_FreeDebug(reinterpret_cast<void*>(secondaryBuffer)))
                     : nullptr;
    }

    self->field_0018 = 0;
    self->field_001c = 0x0F;
    CSF_AT(uint8_t, self, 0x08) = 0;

    return result;
}

HOOK(0x0050C370, CMenuScreenFader_ReleaseResources, "auto");
