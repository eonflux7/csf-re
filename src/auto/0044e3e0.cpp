#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

// Refreshes the state block's dirty flag from the owner's slot 32 query, serializes the
// object reference at this+0x1C when dirty, then asks the owner's slot 31 whether to finish.
static int __fastcall C_00831230_UpdateAndSerializeRef(C_00831230* self, void* /*edx*/, void* state) {
    // Owner object stored at state+4.
    void* owner = CSF_AT(void*, state, 4);

    // v3 = !*state && !owner->vtbl[32](owner, 0, state)
    // The owner method takes two stack arguments (0, state) after ecx = owner.
    bool v3 = false;
    if (CSF_AT(uint8_t, state, 0) == 0) {
        uint8_t ownerResult = ((uint8_t(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, owner, 0)[32])(owner, nullptr, 0, state);
        v3 = (ownerResult == 0);
    }

    // Preserve the previous value at state+0x1C across the update.
    uint32_t savedField = CSF_AT(uint32_t, state, 0x1C);
    CSF_AT(uint8_t, state, 0) = static_cast<uint8_t>(v3);
    CSF_AT(uint32_t, state, 0x1C) = 0;

    if (!v3) {
        CSF_AT(uint8_t, state, 0x15) = 1;
        CSF_AT(uint8_t, state, 0x16) = static_cast<uint8_t>(v3);
        Archive_SerializeObjectRef(static_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x1C),
                                   reinterpret_cast<bool*>(state));
    }

    CSF_AT(uint32_t, state, 0x1C) = savedField;

    // Only when the state byte is still zero, ask the owner (re-read) whether to finish.
    if (CSF_AT(uint8_t, state, 0) == 0) {
        void* ownerAgain = CSF_AT(void*, state, 4);
        uint8_t finished = ((uint8_t(__fastcall*)(void*, void*, void*))CSF_AT(void**, ownerAgain, 0)[31])(ownerAgain, nullptr, state);
        if (finished != 0) {
            CSF_AT(uint8_t, state, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x0044E3E0, C_00831230_UpdateAndSerializeRef, "auto");
