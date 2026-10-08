#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable addresses referenced by this function, bound as immediate constants.
static const uintptr_t kVtblCMenuTextInputEdit = CSF_ADDR_VT(0x00841450);
static const uintptr_t kVtblCMenuScreenBaseEdit = CSF_ADDR_DATA(0x00841400);
static const uintptr_t kVtblCMenuTextInputIntermediate = CSF_ADDR_DATA(0x00830488);
static const uintptr_t kVtblCMenuTextInputFinal = CSF_ADDR_VT(0x00830438);
static const uintptr_t kVtblCMenuScreenRoot = CSF_ADDR_VT(0x0083042C);

// Callees bound by address.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x690E60, (void* block));

// Releases the owned sub-object at +0x14 and the text buffer at +0x38, then resets
// the text input to its idle state and restores the base-class vtables.
// Every store is volatile so each vtable write and field reset is emitted in the
// same order as the original.
static int __fastcall CMenuTextInput_TearDown(CMenuTextInput* self, void* /*edx*/) {
    int result = 0;

    uint8_t* selfBytes = reinterpret_cast<uint8_t*>(self);

    // Owned sub-object pointer is read before the vtables are rewritten.
    uint32_t ownedObjectAddr = self->field_0014;

    // [self+0x00] = CMenuTextInput vtable, [self+0x04] = CMenuScreen base vtable.
    *reinterpret_cast<volatile uint32_t*>(selfBytes + 0x00) =
        static_cast<uint32_t>(kVtblCMenuTextInputEdit);
    *reinterpret_cast<volatile uint32_t*>(selfBytes + 0x04) =
        static_cast<uint32_t>(kVtblCMenuScreenBaseEdit);

    if (ownedObjectAddr != 0) {
        // Virtual slot 0 (deleting destructor) called as thiscall with flag 1.
        void** ownedObjectVtbl = *reinterpret_cast<void***>(static_cast<uintptr_t>(ownedObjectAddr));
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(ownedObjectVtbl[0])(
            reinterpret_cast<void*>(static_cast<uintptr_t>(ownedObjectAddr)), nullptr, 1);
        self->field_0014 = 0;
    }

    // Free the heap text buffer only when the capacity is at least 8 (unsigned compare).
    if (self->field_004c >= 8u) {
        uint32_t textBufferAddr = self->field_0038;
        if (textBufferAddr != 0) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(textBufferAddr)));
        }
    }

    self->field_004c = 7;
    self->field_0048 = 0;
    *reinterpret_cast<volatile uint16_t*>(selfBytes + 0x38) = 0;

    // Transient vtable stores at +0x04, in the original order (intermediate, then final).
    *reinterpret_cast<volatile uint32_t*>(selfBytes + 0x04) =
        static_cast<uint32_t>(kVtblCMenuTextInputIntermediate);
    *reinterpret_cast<volatile uint32_t*>(selfBytes + 0x04) =
        static_cast<uint32_t>(kVtblCMenuTextInputFinal);
    *reinterpret_cast<volatile uint32_t*>(selfBytes + 0x00) =
        static_cast<uint32_t>(kVtblCMenuScreenRoot);

    return result;
}

HOOK(0x0052C2E0, CMenuTextInput_TearDown, "auto");
