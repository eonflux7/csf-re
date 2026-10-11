#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, GetControlEntryByIndex, 0x0049ED10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_008342e8_GetControlFlagsAddress, 0x0049ED30, (void* self, void* edx, int a2));

// Turns an address-sized integer returned by a callee into a byte pointer.
static uint8_t* IntToBytePtr(int value) {
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

static bool __fastcall C_0083b668_CanControlEntryBeUsed(C_0083b668* self, void* /*edx*/, int controlIndex) {
    void* controlTable = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014));
    bool result = false;

    uint8_t* entry = IntToBytePtr(GetControlEntryByIndex(controlTable, nullptr, controlIndex));
    if (CSF_AT(int, entry, 0x8) >= 0) {
        void* scene = g_pCurrentScene;
        int entryHandle = GetControlEntryByIndex(controlTable, nullptr, controlIndex);
        uint8_t* resolved = IntToBytePtr(CHandleTable_Resolve(scene, nullptr, entryHandle));
        if (resolved != nullptr) {
            if (CSF_AT(int, resolved, 0x50) != 0) {
                uint8_t* flags = IntToBytePtr(C_008342e8_GetControlFlagsAddress(controlTable, nullptr, controlIndex));
                if ((*flags & 2) == 0) {
                    return true;
                }
            }
        }
    }
    return result;
}

HOOK(0x004D01F0, C_0083b668_CanControlEntryBeUsed, "auto");
