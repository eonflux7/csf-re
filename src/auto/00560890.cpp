#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees of CMenuMpSessions_RebuildPartidasList, bound by address.

// Returns the global session table object that GetSessionEntryByIndex is called on (cdecl, no args).
CSF_FUNC(void*, __cdecl, GetGlobalDword_8B0680, 0x0054E520, ());

// Returns the session entry at `index`, or null past the end (__thiscall, one stack arg).
CSF_FUNC(uint8_t*, __fastcall, GetSessionEntryByIndex, 0x0054E610, (void* self, void* edx, int index));

// Copies `byteCount` bytes from `source` to `destination` (cdecl, three stack args).
CSF_FUNC(uint32_t, __cdecl, CopyEntryBytes, 0x00695590, (const uint8_t* source, uint8_t* destination, int byteCount));

// Clears the listbox items and sends its event (__thiscall, no stack args).
CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearStorageAndSendEvent, 0x00715660, (void* self, void* edx));

// Declared int so the eax left by the callee is returned unchanged, as the original does.
CSF_FUNC(int, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x00716A60, (void* self, void* edx, int itemIndex));

// Listbox vtable slots (byte offset / 4).
static constexpr int kListBoxResetVtblSlot = 0x298 / 4;   // vtbl[166]
static constexpr int kListBoxAddRowVtblSlot = 0x294 / 4;  // vtbl[165]

// Maps the filter mode stored in field_0018 to the visibility bit an entry must have.
static int MenuMpSessions_FilterModeToEntryFlags(uint32_t filterMode)
{
    if (filterMode == 0)
        return 1;
    if (filterMode == 1)
        return 2;
    if (filterMode == 3)
        return 8;
    return 0;
}

static int __fastcall CMenuMpSessions_RebuildPartidasList(CMenuMpSessions* self, void* /*edx*/)
{
    // Clear the listbox through its vtable before refilling it.
    {
        void* listBox = reinterpret_cast<void*>(self->field_0034);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, listBox, 0)[kListBoxResetVtblSlot])(listBox, nullptr);
    }

    int entryIndex = 0;
    int rowCount = 0;
    self->field_01f4 = 0;

    uint8_t entryText[512];
    uint8_t* entry = nullptr;
    do
    {
        void* sessionTable = GetGlobalDword_8B0680();
        entry = GetSessionEntryByIndex(sessionTable, nullptr, entryIndex);

        // field_0018 is re-read on every iteration, as in the original.
        const int requiredFlags = MenuMpSessions_FilterModeToEntryFlags(self->field_0018);
        if (entry != nullptr && (CSF_AT(int8_t, entry, 0x300) & requiredFlags) != 0)
        {
            CopyEntryBytes(entry + 0x100, entryText, 0x100);

            // Add the row through the listbox vtable (slot 0x294).
            void* listBox = reinterpret_cast<void*>(self->field_0034);
            reinterpret_cast<void(__fastcall*)(void*, void*, int, uint32_t, uint8_t*, int)>(
                CSF_AT(void**, listBox, 0)[kListBoxAddRowVtblSlot])(
                listBox, nullptr, rowCount, 0u, entryText, entryIndex);

            self->field_01f4 = self->field_01f4 + 1;
            ++rowCount;
        }
        ++entryIndex;
    } while (entry != nullptr);

    CGuiListBoxBase_ClearStorageAndSendEvent(reinterpret_cast<void*>(self->field_0034), nullptr);
    return CGuiListBoxBase_AddSelectedItem(reinterpret_cast<void*>(self->field_0034), nullptr,
                                           reinterpret_cast<int>(entry));
}

HOOK(0x00560890, CMenuMpSessions_RebuildPartidasList, "auto");
