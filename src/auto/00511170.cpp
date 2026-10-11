#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CWeaponDefTable_ReleaseRecordBlock, 0x00511220, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CWeaponDefTable_InitRecordBlock, 0x005111C0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall CWeaponDefTable_ResetAndAllocateRecords(CWeaponDefTable* self, void* /*edx*/)
{
    if (self->m_pRecords) {
        void* oldBlock = reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->m_pRecords));
        CWeaponDefTable_ReleaseRecordBlock(oldBlock, nullptr);
        Mem_Free(oldBlock);
        self->m_pRecords = 0;
    }

    void* newBlock = Mem_Alloc(16);
    if (newBlock) {
        int result = CWeaponDefTable_InitRecordBlock(newBlock, nullptr, 0);
        self->m_pRecords = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(newBlock));
        return result;
    }

    self->m_pRecords = 0;
    return 0;
}

HOOK(0x00511170, CWeaponDefTable_ResetAndAllocateRecords, "auto");
