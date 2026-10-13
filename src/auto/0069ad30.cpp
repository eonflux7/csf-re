#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Allocates a fresh 3-slot reference node and returns its address in eax.
CSF_FUNC(uint32_t*, __cdecl, AllocateReferenceNode, 0x0069A7B0, ());

// Builds the self-referencing reference registry: a node whose three link
// slots all point back to the node itself, with the flag byte at +0x15 set.
// Returns the registry object (this).
static uint32_t* __fastcall SceneLoader_CreateReferenceRegistry(S_0069ad30* self, void* /*edx*/)
{
    uint32_t* node = AllocateReferenceNode();
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(node));

    CSF_AT(uint8_t, node, 0x15) = 1;

    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 4) = self->field_0004;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0) = self->field_0004;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 8) = self->field_0004;

    self->field_0008 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0069AD30, SceneLoader_CreateReferenceRegistry, "auto");
