#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees bound by address, using the signatures from the packet.
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(char, __fastcall, SceneEntity_Activate, 0x004E51B0, (uint8_t* self, void*, char a2));
CSF_FUNC(char, __fastcall, SetSecondaryEntityActive, 0x004F0C30, (void* self, void* edx, char a2));

// Returns the linked object's base (linked value - 4), or null when no linked object exists.
// The original then reads the owner pointer at base+0x2C, which for a null base reads address 0x2C.
static uint8_t* FindLinkedObjectBase(TAiState* a1)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    if (linkedValue == 0)
        return nullptr;
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue - 4));
}

// TEstatico: prepares the static AI state, activates the linked scene entities,
// then clears bit 3 of the owner's flags word at +0x1F0 and returns its address.
static unsigned int* __stdcall TEstatico_ActivateStaticEntities(TAiState* a1)
{
    PrepareStaticAiState(a1);

    // Scene entity activation: this = *(owner + 0x3C), argument 1.
    uint8_t* base = FindLinkedObjectBase(a1);
    uint8_t* owner = CSF_AT(uint8_t*, base, 0x2C);
    SceneEntity_Activate(CSF_AT(uint8_t*, owner, 0x3C), nullptr, 1);

    // Secondary entity activation: this = *(owner + 0x40), argument 1.
    base = FindLinkedObjectBase(a1);
    owner = CSF_AT(uint8_t*, base, 0x2C);
    SetSecondaryEntityActive(CSF_AT(void*, owner, 0x40), nullptr, 1);

    // Clear bit 3 of the flags word at owner + 0x1F0 and return its address.
    base = FindLinkedObjectBase(a1);
    owner = CSF_AT(uint8_t*, base, 0x2C);
    CSF_AT(uint32_t, owner, 0x1F0) &= ~8u;
    return reinterpret_cast<unsigned int*>(owner + 0x1F0);
}

HOOK(0x004B0170, TEstatico_ActivateStaticEntities, "auto");
