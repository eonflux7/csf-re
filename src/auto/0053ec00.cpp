#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* block));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x690E60, (void* block));

// Secondary-base tables that the gate accepted as plain data bindings.
CSF_GLOBAL(uint8_t, g_VtblCMenuMissionInfoSecondary, 0x00842DB8);
CSF_GLOBAL(uint8_t, g_VtblCMenuScreenIntermediate, 0x00830488);

static int __fastcall CMenuMissionInfo_Destruct(CMenuMissionInfo* self, void* /*edx*/)
{
    int result = 0;

    // The child object pointer is loaded before the vtables are rewritten.
    void* childObject = reinterpret_cast<void*>(self->field_001c);

    // CMenuMissionInfo vtable 0x842E08 and its secondary-base table.
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_ADDR_VT(0x00842E08));
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&g_VtblCMenuMissionInfoSecondary));

    if (childObject != nullptr)
    {
        // Slot 0 of the child's vtable is its scalar deleting destructor, called with arg 1.
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, childObject, 0)[0])(childObject, nullptr, 1);

        self->field_001c = 0;

        const uint32_t ownedBuffer = self->field_0094;
        self->field_0098 = 0;
        self->field_009c = 0;
        if (ownedBuffer != 0)
        {
            Mem_Free(reinterpret_cast<void*>(ownedBuffer));
            self->field_0094 = 0;
        }
    }

    const uint32_t pendingBuffer = self->field_0094;
    if (pendingBuffer != 0)
    {
        Mem_Free(reinterpret_cast<void*>(pendingBuffer));
        self->field_0094 = 0;
    }

    const uint32_t listStorage = self->field_0068;
    if (listStorage != 0)
    {
        result = Mem_FreeDebug(reinterpret_cast<void*>(listStorage));
    }
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;

    // Wide string at +0x34: heap buffer only when the capacity is 8 or more.
    if (self->field_0048 >= 8u)
    {
        const uint32_t textBuffer = self->field_0034;
        result = 0;
        if (textBuffer != 0)
        {
            result = Mem_FreeDebug(reinterpret_cast<void*>(textBuffer));
        }
    }

    self->field_0044 = 0;
    self->field_0048 = 7;
    CSF_AT(uint16_t, self, 0x34) = 0;

    // Base-class destructor chain: +0x04 is stored twice, the first store is overwritten.
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&g_VtblCMenuScreenIntermediate));
    self->field_0004 = CSF_ADDR_VT(0x00830438);                                  // CMenuScreen secondary vtable 0x830438
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_ADDR_VT(0x0083042C)); // g_VtblCMenuScreenBase 0x83042C

    return result;
}

HOOK(0x0053EC00, CMenuMissionInfo_Destruct, "auto");
