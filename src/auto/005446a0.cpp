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

// Vtable addresses referenced by the original: primary vtable (0x00843A90) and
// the secondary-base vtable (0x00843A40) stored at this+4.
CSF_GLOBAL(uint8_t, g_VtableCMenuProfilesPC, 0x00843A90);
CSF_GLOBAL(uint8_t, g_VtableCMenuProfilesPCSecondaryBase, 0x00843A40);

// Callees bound by address.
CSF_FUNC(int*, __fastcall, MenuSaveGame_ClearStorage, 0x005444D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuProfiles_Destruct, 0x0053BC50, (void* self, void* edx));
CSF_FUNC(void, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting-destructor style teardown of CMenuProfilesPC. Re-establishes both
// vtable pointers, clears the storage embedded at +0x44, runs the CMenuProfiles
// base teardown, and frees the object when bit 0 of the flag argument is set.
// Returns the object pointer.
static void* __fastcall CMenuProfilesPC_DestroyAndMaybeFree(CMenuProfilesPC* self, void* /*edx*/, char deleteFlag)
{
    // Primary vtable at +0x00: the original stores the immediate 0x843A90 (mov [esi], imm32).
    // The literal address is used directly so the emitted store carries the same constant.
    CSF_AT(void*, self, 0x000) = reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x00843A90u));

    // Secondary-base vtable at +0x04: the original stores the immediate 0x843A40 (mov [esi+4], imm32).
    CSF_AT(void*, self, 0x004) = reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x00843A40u));

    // Clear the storage embedded at +0x44 (address of sub-object, not a pointer load).
    MenuSaveGame_ClearStorage(reinterpret_cast<uint8_t*>(self) + 0x44, nullptr);

    // Base-class teardown.
    CMenuProfiles_Destruct(self, nullptr);

    // Free the memory block only when the deleting flag is set.
    if ((deleteFlag & 1) != 0) {
        Mem_Free(self);
    }

    return self;
}

HOOK(0x005446A0, CMenuProfilesPC_DestroyAndMaybeFree, "auto");
