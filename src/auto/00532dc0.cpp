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

// Game heap free (cdecl, one argument). Bound by address; this is what the original calls.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Debug-variant free used for the secondary buffer (cdecl, one argument).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Vtable of the base CMenuScreen (data symbol at 0x0083042C).
CSF_GLOBAL(uint8_t, g_VtblCMenuScreenBase, 0x0083042C);

// Vtable and secondary vtable values written by the original (immediate data).
static constexpr uint32_t kVtblCMenuEula      = 0x00841B78u;
static constexpr uint32_t kVtblCMenuEulaSub   = 0x00841BC8u;
static constexpr uint32_t kVtblCMenuScreenA   = 0x00830488u;
static constexpr uint32_t kVtblCMenuScreenB   = 0x00830438u;

// Releases the owned sub-object, the two buffers, and resets the object to the
// CMenuScreen base state. Mirrors the original sequence exactly.
static int __fastcall CMenuEula_ReleaseResources(CMenuEula* self, void* /*edx*/)
{
    // Load the owned helper object pointer first (original reads it before writing vtables).
    uint32_t helperPtr = self->field_001c;

    // Install CMenuEula vtables.
    CSF_AT(uint32_t, self, 0x0) = kVtblCMenuEula;
    CSF_AT(uint32_t, self, 0x4) = kVtblCMenuEulaSub;

    if (helperPtr != 0)
    {
        void* helper = reinterpret_cast<void*>(helperPtr);
        // Virtual slot 0 (scalar deleting dtor), __thiscall with argument 1.
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, helper, 0)[0])(helper, nullptr, 1);
        self->field_001c = 0;
    }

    int result = static_cast<int>(self->field_0158);
    if (result != 0)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0158));
        self->field_0158 = 0;
    }

    // Unsigned compare (jb): buffer-size field >= 8 means a heap-allocated buffer is in use.
    if (self->field_0054 >= 8u)
    {
        uint32_t buffer = self->field_0040;
        if (buffer != 0)
        {
            result = Mem_FreeDebug(reinterpret_cast<void*>(buffer));
        }
    }

    self->field_0050 = 0;
    self->field_0054 = 7;
    CSF_AT(uint16_t, self, 0x40) = 0;

    CSF_AT(uint32_t, self, 0x4) = kVtblCMenuScreenA;
    CSF_AT(uint32_t, self, 0x4) = kVtblCMenuScreenB;
    CSF_AT(uint32_t, self, 0x0) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&g_VtblCMenuScreenBase));

    return result;
}

HOOK(0x00532DC0, CMenuEula_ReleaseResources, "auto");
