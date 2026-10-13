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

// Game heap free (__cdecl, caller pops the argument).
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Returns the object referenced by this (__thiscall, ecx = this).
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

// Removes a child index from the menu controls (__thiscall, one stack arg, ret 4).
CSF_FUNC(void, __fastcall, MenuControls_ApplyChildIndex, 0x00712D90, (void* self, void* edx, int index));

// Releases each 16-byte entry (owned block at +0xC, owned object at +0x8 whose slot 0
// is a deleting destructor called with 1), then releases the object at +0x10 the same way.
static int __fastcall C_00840cf8_ReleaseEntriesAndDetach(C_00840cf8* self, void* /*edx*/)
{
    uint32_t entryOffset = 0;
    for (uint32_t i = 0; i < self->field_001c; ++i)
    {
        // The entry table is reloaded on every iteration, as the original does.
        uint32_t entriesBase = self->field_0014;
        uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entriesBase)) + entryOffset;

        uint32_t ownedBlock = CSF_AT(uint32_t, entry, 0xC);
        if (ownedBlock != 0)
        {
            CoreMemory_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(ownedBlock)));
            CSF_AT(uint32_t, entry, 0xC) = 0;
        }

        uint32_t ownedObjectValue = CSF_AT(uint32_t, entry, 0x8);
        if (ownedObjectValue != 0)
        {
            void* ownedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(ownedObjectValue));
            // Slot 0 of the owned object's vtable, called with argument 1.
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
            CSF_AT(uint32_t, entry, 0x8) = 0;
        }

        entryOffset += 16;
    }

    void* referencedObject = GetReferencedObject(self, nullptr);
    MenuControls_ApplyChildIndex(referencedObject, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(self)));

    int result = 0;
    uint32_t ownedMember = self->field_0010;
    if (ownedMember != 0)
    {
        void* member = reinterpret_cast<void*>(static_cast<uintptr_t>(ownedMember));
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, member, 0)[0])(member, nullptr, 1);
        self->field_0010 = 0;
    }
    return result;
}

HOOK(0x519480, C_00840cf8_ReleaseEntriesAndDetach, "auto");
