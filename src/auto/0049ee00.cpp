#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082dd78_StoreByteAndSetFlag, 0x0040FD20, (C_0082dd78* self, void* , char a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint32_t* __fastcall C_008342e8_CopyFocusEntryPosition(
    C_008342e8* self, void* /*edx*/, int entryIndex, int handle)
{
    const uint32_t entryOffset = 28 * static_cast<uint32_t>(entryIndex);
    auto* entry = reinterpret_cast<uint8_t*>(self->field_0198) + entryOffset;
    auto* source = reinterpret_cast<void*>(handle);

    CSF_AT(uint32_t, entry, 0x04) = CSF_AT(uint32_t, source, 0x04);
    CSF_AT(uint32_t, entry, 0x08) = CSF_AT(uint32_t, source, 0x08);

    auto* control = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));

    if (CSF_AT(uint32_t, control, 0x50))
    {
        entry = reinterpret_cast<uint8_t*>(self->field_0198) + entryOffset;
        if ((CSF_AT(uint8_t, entry, 0x18) & 0x10) == 0)
        {
            void** controlVtable = CSF_AT(void**, control, 0);

            // The observed callees each consume one argument; the supplied
            // wrappers do not describe these slot signatures correctly.
            const int position = ((int(__fastcall*)(C_008342e8*, void*, int))
                CSF_AT(void**, self, 0)[0x28 / 4])(
                    self, nullptr, CSF_AT(int, entry, 0x10));

            ((void(__fastcall*)(void*, void*, int))
                controlVtable[0x74 / 4])(control, nullptr, position);

            entry = reinterpret_cast<uint8_t*>(self->field_0198) + entryOffset;
            CSF_AT(uint32_t, entry, 0x18) |= 8u;
        }
    }

    entry = reinterpret_cast<uint8_t*>(self->field_0198) + entryOffset;
    if ((CSF_AT(uint8_t, entry, 0x18) & 0x10) != 0)
    {
        C_0082dd78_StoreByteAndSetFlag(
            reinterpret_cast<C_0082dd78*>(CSF_AT(uint32_t, control, 0x4C)),
            nullptr, 1);
    }

    auto* listener = reinterpret_cast<void*>(self->field_0034);
    if (listener)
    {
        ((void(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, listener, 0)[0xE0 / 4])(
                listener, nullptr, handle, entryIndex);
    }

    entry = reinterpret_cast<uint8_t*>(self->field_0198) + entryOffset;
    uint32_t* result = &CSF_AT(uint32_t, entry, 0x18);
    *result &= 0xFFFFFFFD;

    entry = reinterpret_cast<uint8_t*>(self->field_0198) + entryOffset;
    if ((CSF_AT(uint8_t, entry, 0x18) & 4) != 0)
        self->field_0170 &= ~0x200u;

    return result;
}

HOOK(0x0049EE00, C_008342e8_CopyFocusEntryPosition, "auto");
