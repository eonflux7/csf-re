#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address.
CSF_FUNC(void, __cdecl, Vector_DestroyRange, 0x00469760,
         (void* firstElement, void* endElement, void* rangeHeader, void* ownerObject));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Secondary-base vtable blobs (not registered class vtables), bound by address.
CSF_GLOBAL(uint32_t, g_VtblCMenuScreenSecondaryA, 0x00830488);
CSF_GLOBAL(uint32_t, g_VtblCMenuLanguageSecondary, 0x00841C80);

static int* __fastcall CMenuLanguage_Destroy(CMenuLanguage* self, void* /*edx*/) {
    // Owned object at +0x30 is read before the vtables are set.
    uint32_t ownedObjectWord = self->field_0030;

    // CMenuLanguage vtable (0x841CD0) and its secondary-base vtable (0x841C80).
    CSF_AT(void*, self, 0) = reinterpret_cast<void*>(0x841CD0);
    self->field_0004 = reinterpret_cast<uint32_t>(&g_VtblCMenuLanguageSecondary);

    if (ownedObjectWord != 0) {
        void* ownedObject = reinterpret_cast<void*>(ownedObjectWord);
        // Virtual slot 0 with flag 1 (__thiscall, callee pops its one stack argument).
        ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, ownedObject, 0)[0]))(ownedObject, nullptr, 1);
        self->field_0030 = 0;
    }

    // Free the heap buffer only when the string is in heap mode (capacity >= 16, unsigned).
    if (self->field_0054 >= 0x10u) {
        uint32_t heapBufferWord = self->field_0040;
        if (heapBufferWord != 0) {
            Mem_FreeDebug(reinterpret_cast<void*>(heapBufferWord));
        }
    }

    // Reset the string to empty small-buffer mode.
    self->field_0054 = 0x0F;
    self->field_0050 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0040) = 0;  // single byte: first char of the inline buffer

    // Auxiliary block at +0x24.
    uint32_t auxBlockWord = self->field_0024;
    if (auxBlockWord != 0) {
        Mem_FreeDebug(reinterpret_cast<void*>(auxBlockWord));
    }
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;

    // Range at +0x14/+0x18/+0x1C, with +0x10 as the sub-object address passed to the helper.
    uint8_t* rangeHeader = reinterpret_cast<uint8_t*>(self) + 0x10;
    uint32_t beginWord = CSF_AT(uint32_t, self, 0x14);
    // Value left in eax at the final return.
    int* eaxAtReturn = reinterpret_cast<int*>(beginWord);
    if (beginWord != 0) {
        uint32_t endWord = CSF_AT(uint32_t, self, 0x18);
        Vector_DestroyRange(reinterpret_cast<void*>(beginWord),
                            reinterpret_cast<void*>(endWord),
                            rangeHeader,
                            self);
        // Re-read after the call, as the original does.
        eaxAtReturn = reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x14));
        if (eaxAtReturn != nullptr) {
            eaxAtReturn = reinterpret_cast<int*>(Mem_FreeDebug(reinterpret_cast<void*>(eaxAtReturn)));
        }
    }

    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;

    // Base-class vtable restoration, in the original's store order.
    // The first +4 store is volatile so it is not removed as a dead store.
    *reinterpret_cast<volatile uint32_t*>(reinterpret_cast<uint8_t*>(self) + 4) =
        reinterpret_cast<uint32_t>(&g_VtblCMenuScreenSecondaryA);
    self->field_0004 = 0x830438;
    CSF_AT(void*, self, 0) = reinterpret_cast<void*>(0x83042C);  // g_VtblCMenuScreenBase

    return eaxAtReturn;
}

HOOK(0x005343E0, CMenuLanguage_Destroy, "auto");
