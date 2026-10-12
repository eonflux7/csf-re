#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee bindings (one name and signature per address).
CSF_FUNC(void*, __fastcall, WaterEntry_PrepareFromDescriptor, 0x006FE280, (void* self, void* edx, void* a2));

// Prepares the descriptor-based entry embedded at self+12, then dispatches
// through the object's vtable slot 3 (byte offset 0x0C) with the embedded entry as argument.
static int __fastcall GfxClumpLoader_PrepareDescriptorAndRead(void* self, void* /*edx*/, C_0082d410* descriptor)
{
    void* embeddedEntry = static_cast<uint8_t*>(self) + 0x0C;

    WaterEntry_PrepareFromDescriptor(embeddedEntry, nullptr, descriptor);

    return ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, self, 0)[0x0C / 4])(self, nullptr, embeddedEntry);
}

HOOK(0x006FE250, GfxClumpLoader_PrepareDescriptorAndRead, "auto");
