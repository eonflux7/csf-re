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

CSF_FUNC(void**, __fastcall, LoadState_Construct, 0x00694C80, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CProfileManager_PrepareLoadState, 0x00515E80, (void* self, void* edx, void* a1, int a2));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(char, __fastcall, MenuExtras_ParseLoadedFile, 0x006944A0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ResetEmbeddedMemberAt8, 0x00712D10, (void** self, void* edx));

// Vtable/address reference of MenuExtras_InitializeFile's target as recorded by the packet.
static void* const s_MenuExtrasFileVtbl = CSF_VTBL(0x00692B10);

// Virtual slot 0x84 of CProfileManager (index 33): (this, a2, LoadState*) -> bool (al)
using ProfileSlot84Fn = unsigned char(__fastcall*)(void* self, void* edx, int a2, void* loadState);

static bool __fastcall CProfileManager_LoadFromSlot(CProfileManager* self, void* /*edx*/, int a2, int a3)
{
    // Frame objects owned by this function: room 0x918 and 0x3C bytes.
    alignas(8) uint8_t loadStateBytes[0x918];
    alignas(8) uint8_t fileBytes[0x3C];
    void* loadState = loadStateBytes;
    void* fileObj = fileBytes;

    LoadState_Construct(loadState, nullptr);
    MenuExtras_InitializeFile(fileObj, nullptr);
    CProfileManager_PrepareLoadState(self, nullptr, loadState, a3);

    bool ok = ((ProfileSlot84Fn)CSF_AT(void**, self, 0)[0x84 / 4])(self, nullptr, a2, loadState) != 0
        && MenuExtras_LoadFile(fileObj, nullptr, reinterpret_cast<const char*>(static_cast<uintptr_t>(a2))) != 0
        && MenuExtras_ParseLoadedFile(fileObj, nullptr, static_cast<unsigned int>(a2)) != 0;

    MenuExtras_CloseFile(fileObj, nullptr);
    ResetEmbeddedMemberAt8(static_cast<void**>(loadState), nullptr);
    return ok;
}

HOOK(0x514810, CProfileManager_LoadFromSlot, "auto");
