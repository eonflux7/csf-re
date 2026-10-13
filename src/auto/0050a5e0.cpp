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

// Callees (bound by address, signatures copied from the packet)
CSF_FUNC(void, __fastcall, LoadState_Construct, 0x00694C80, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_50BD10, 0x0050BD10, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(char, __fastcall, MenuExtras_ParseLoadedFile, 0x006944A0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ResetEmbeddedMemberAt8, 0x00712D10, (void** self, void* edx));

// Reads the options file at a2 through the object's vtable slot 13 (+0x34),
// then loads and parses it through the MenuExtras helpers.
static bool __fastcall COptionsFile_ReadOptionsFile(COptionsFile* self, void* /*edx*/, int a2)
{
    // Stack-owned objects; sized to the room the original frame gives them.
    alignas(8) uint8_t loadStateBytes[0x918];   // [ebp-0x954]
    alignas(8) uint8_t menuFileBytes[0x3C];     // [ebp-0x3C]

    // Vtable of the MenuExtras file object (stored by its constructor path).
    (void)CSF_VTBL(0x00692B10);

    LoadState_Construct(loadStateBytes, nullptr);
    MenuExtras_InitializeFile(menuFileBytes, nullptr);
    sub_50BD10(self, nullptr, loadStateBytes);

    // Virtual slot +0x34 (index 13) of COptionsFile.
    auto writeBufferAndVerify = reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
        CSF_AT(void**, self, 0)[13]);
    bool loadOk = writeBufferAndVerify(self, nullptr, a2, loadStateBytes)
        && MenuExtras_LoadFile(menuFileBytes, nullptr, reinterpret_cast<const char*>(static_cast<intptr_t>(a2)))
        && MenuExtras_ParseLoadedFile(menuFileBytes, nullptr, static_cast<unsigned int>(a2));

    MenuExtras_CloseFile(menuFileBytes, nullptr);
    ResetEmbeddedMemberAt8(reinterpret_cast<void**>(loadStateBytes), nullptr);
    return loadOk;
}

HOOK(0x0050A5E0, COptionsFile_ReadOptionsFile, "auto");
