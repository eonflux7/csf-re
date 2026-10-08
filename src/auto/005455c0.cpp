#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address.
CSF_FUNC(void*, __fastcall, MenuSaveGame_ClearStorage, 0x005444D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MenuSaveGame_ReleaseSlotBuffer, 0x005129A0, (uint32_t slotBuffer, uint32_t slotCount, void* slotOwnerSubobject, void* owner));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Every store the original performs is kept as an observable write, including
// vtable pointer stores that are overwritten later in the same body.
static void WriteDwordAt(void* base, uint32_t offset, uint32_t value)
{
    *reinterpret_cast<volatile uint32_t*>(static_cast<uint8_t*>(base) + offset) = value;
}

static int __fastcall CMenuSaveGame_Destruct(CMenuSaveGame* self, void* /*edx*/)
{
    // Owned virtual object at +0x44: slot 0 is the deleting destructor, arg 1 = delete flag.
    const uint32_t ownedObjectValue = self->field_0044;

    WriteDwordAt(self, 0x00, CSF_ADDR_VT(0x843B30)); // CMenuSaveGame primary vtable
    WriteDwordAt(self, 0x04, CSF_ADDR_VT(0x843B70)); // CMenuSaveGame secondary vtable

    if (ownedObjectValue != 0) {
        void* ownedObject = reinterpret_cast<void*>(ownedObjectValue);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        WriteDwordAt(self, 0x44, 0);
    }

    MenuSaveGame_ClearStorage(reinterpret_cast<uint8_t*>(self) + 0x5C, nullptr);

    const uint32_t ownedBuffer = self->field_0034;
    if (ownedBuffer != 0) {
        Mem_FreeDebug(reinterpret_cast<void*>(ownedBuffer));
    }
    WriteDwordAt(self, 0x34, 0);
    WriteDwordAt(self, 0x38, 0);
    WriteDwordAt(self, 0x3C, 0);

    // Sub-object at +0x20 is passed by address to the release helper; +0x24/+0x28/+0x2C are undeclared.
    uint8_t* slotOwnerSubobject = reinterpret_cast<uint8_t*>(self) + 0x20;
    int result = 0;
    const uint32_t slotBuffer = CSF_AT(uint32_t, self, 0x24);
    if (slotBuffer != 0) {
        MenuSaveGame_ReleaseSlotBuffer(slotBuffer, CSF_AT(uint32_t, self, 0x28), slotOwnerSubobject, self);
        const uint32_t slotBufferAfterRelease = CSF_AT(uint32_t, self, 0x24);
        if (slotBufferAfterRelease != 0) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(slotBufferAfterRelease));
        }
    }
    WriteDwordAt(self, 0x24, 0);
    WriteDwordAt(self, 0x28, 0);
    WriteDwordAt(self, 0x2C, 0);

    WriteDwordAt(self, 0x04, CSF_ADDR_DATA(0x830488)); // intermediate base secondary vtable
    WriteDwordAt(self, 0x04, CSF_ADDR_VT(0x830438)); // CMenuScreen secondary base vtable
    WriteDwordAt(self, 0x00, CSF_ADDR_VT(0x83042C)); // CMenuScreen base vtable

    return result;
}

HOOK(0x005455C0, CMenuSaveGame_Destruct, "auto");
