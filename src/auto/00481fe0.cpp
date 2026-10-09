#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, CharActionFsm_CreateEntry, 0x00475A40,
         (int a1, int a2, int a3));
CSF_FUNC(void, __fastcall, CharActionFsm_InitializeEntry, 0x00476170,
         (void* self, void* edx, void* a2));
CSF_FUNC(unsigned int, __fastcall, CharActionFsm_AppendEntry, 0x00481C90,
         (void* self, void* edx, uint32_t* a3));

static int __fastcall CCharWeaponFsm_GetOrCreateEntryAndDispatch(
    CCharWeaponFsm* self, void* /*edx*/, int entryKey, int command)
{
    const int index = self->vSlotC0(entryKey);
    if (index != -1)
    {
        void* entry = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t*>(self->field_0018)[index]);

        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, entry, 0)[0x5C / 4])(
                entry, nullptr, command);
    }

    uint32_t* entry = CharActionFsm_CreateEntry(
        2, entryKey, static_cast<int>(self->field_0008));

    uint32_t entryAddress = reinterpret_cast<uint32_t>(entry);
    CharActionFsm_InitializeEntry(entry, nullptr, nullptr);
    CharActionFsm_AppendEntry(
        reinterpret_cast<uint8_t*>(self) + 0x14, nullptr, &entryAddress);

    if (self->field_0024 == static_cast<uint32_t>(-1))
    {
        const uint32_t begin = self->field_0018;
        int count = 0;
        if (begin)
        {
            count = static_cast<int32_t>(
                CSF_AT(uint32_t, self, 0x1C) - begin) >> 2;
        }
        self->vSlotF8(count - 1);
    }

    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, entry, 0)[0x5C / 4])(
            entry, nullptr, command);
}

HOOK(0x00481FE0, CCharWeaponFsm_GetOrCreateEntryAndDispatch, "auto");
