#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00863588*, __fastcall, C_00863588_Construct, 0x0070A420, (C_00863588* self, void* , C_0085ec18* owner));
CSF_FUNC(C_00863600*, __fastcall, C_00863600_InitWithArg, 0x0070AA70, (C_00863600* self, void* , int a2));
CSF_FUNC(C_00863678*, __fastcall, C_00863678_Construct, 0x0070B020, (C_00863678* self, void* , C_0085ec18* ref));

// Picks a team-player implementation according to the referenced object's kind
// (0 = plain 60-byte player via C_00863588_Construct,
//  1 = 64-byte player via C_00863600_InitWithArg,
//  2 = 48-byte player via C_00863678_Construct)
// and constructs it. Any other kind returns nullptr.
static uint32_t* __cdecl GfxTeam_CreatePlayerForReference(C_0085ec18* a2)
{
    if (!a2)
        return nullptr;

    // Virtual GetReferencedObject: vtable slot 0x10 / 4 = 4.
    int referenceKind = ((int(__fastcall*)(void*, void*))CSF_AT(void**, a2, 0)[4])(a2, nullptr);

    if (referenceKind == 0) {
        void* memory = Mem_Alloc(60);
        if (memory) {
            return reinterpret_cast<uint32_t*>(
                C_00863588_Construct(static_cast<C_00863588*>(memory), nullptr, a2));
        }
    } else if (referenceKind == 1) {
        void* memory = Mem_Alloc(64);
        if (memory) {
            return reinterpret_cast<uint32_t*>(
                C_00863600_InitWithArg(static_cast<C_00863600*>(memory), nullptr,
                                       reinterpret_cast<int>(a2)));
        }
    } else if (referenceKind == 2) {
        void* memory = Mem_Alloc(48);
        if (memory) {
            return reinterpret_cast<uint32_t*>(
                C_00863678_Construct(static_cast<C_00863678*>(memory), nullptr, a2));
        }
    }

    return nullptr;
}

HOOK(0x006FF490, GfxTeam_CreatePlayerForReference, "auto");
