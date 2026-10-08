#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable and data addresses referenced by the original destructor body.
CSF_GLOBAL(uint8_t, off_841978, 0x00841978);
CSF_GLOBAL(uint8_t, off_841928, 0x00841928);
CSF_GLOBAL(uint8_t, off_830488, 0x00830488);
CSF_GLOBAL(uint8_t, off_830438, 0x00830438);
CSF_GLOBAL(uint8_t, g_VtblCMenuScreenBase, 0x0083042C);

// Container-release helper (cdecl, caller pops 0x10 bytes). Original pushes four arguments.
CSF_FUNC(int, __cdecl, MenuMpServerInfo_ReleaseSceneList, 0x00469760,
         (uint32_t firstEntry, uint32_t secondEntry, void* ownerSubobject, void* owner));

// Game heap free used by the original (cdecl, caller pops 4 bytes).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Body of the CMenuSceneSelect destructor: resets vtables, releases the owned object at +0x14,
// the scene list at +0x3C..+0x44 and the string storage at +0x1C, then restores the CMenuScreen vtable.
// Returns the last eax value the original leaves behind.
// Vtable pointers are stored as immediates, exactly as the original's `mov dword ptr [..], imm32` does.
static int* __fastcall CMenuSceneSelect_Destroy(CMenuSceneSelect* self, void* /*edx*/) {
    // Original reads the owned object pointer first, before writing the vtable slots.
    void* ownedObject = reinterpret_cast<void*>(self->field_0014);

    CSF_AT(uint32_t, self, 0x00) = 0x00841978u;
    self->field_0004 = 0x00841928u;

    if (ownedObject != nullptr) {
        // Slot 0 of the owned object's vtable is called with flag 1 (__thiscall, callee pops 4).
        auto destroyOwned = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedObject, 0)[0]);
        destroyOwned(ownedObject, nullptr, 1);
        self->field_0014 = 0;
    }

    // Scene list release. Head is read once for the test and the first call's argument.
    uint32_t sceneHead = self->field_003c;
    int* result = reinterpret_cast<int*>(sceneHead);
    if (sceneHead != 0) {
        MenuMpServerInfo_ReleaseSceneList(
            sceneHead,
            CSF_AT(uint32_t, self, 0x40),
            reinterpret_cast<uint8_t*>(self) + 0x38,
            self);

        // The original reloads the head after the callee returns and frees that value.
        uint32_t reloadedHead = self->field_003c;
        result = reinterpret_cast<int*>(reloadedHead);
        if (reloadedHead != 0) {
            result = reinterpret_cast<int*>(Mem_FreeDebug(reinterpret_cast<void*>(reloadedHead)));
        }
    }

    self->field_003c = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;

    // Unsigned compare against 0x10 (jb): free the string heap buffer only when capacity >= 16.
    if (self->field_0030 >= 0x10u) {
        uint32_t stringHeapBuffer = self->field_001c;
        result = reinterpret_cast<int*>(stringHeapBuffer);
        if (stringHeapBuffer != 0) {
            result = reinterpret_cast<int*>(Mem_FreeDebug(reinterpret_cast<void*>(stringHeapBuffer)));
        }
    }

    self->field_002c = 0;
    self->field_0030 = 0xF;
    // Byte-width store (mov byte ptr [esi+0x1C], bl): only the low byte of field_001c is cleared.
    CSF_AT(uint8_t, self, 0x1C) = 0;

    self->field_0004 = 0x00830488u;
    self->field_0004 = 0x00830438u;
    CSF_AT(uint32_t, self, 0x00) = 0x0083042Cu;

    return result;
}

HOOK(0x0052FE20, CMenuSceneSelect_Destroy, "auto");
