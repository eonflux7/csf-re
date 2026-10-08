#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszMenu_P_EditG, 0x00899B04);

CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetSelectionState, 0x005449D0, (CMenuSaveGame* self, void* edx));
CSF_FUNC(void, __fastcall, CMenuSaveGame_HandleNameDialogResult, 0x00544EA0, (CMenuSaveGame* self, void* edx, void* saveEntry));
CSF_FUNC(int, __fastcall, CMenuSaveGame_HandleButtonEvent, 0x00545660, (CMenuSaveGame* self, void* edx, uint32_t* event));

// Dispatches a menu message. Event 13 goes to the button handler. Event 14 looks up the
// entry named by g_pszMenu_P_EditG in the object at +0x40 and, if the entry's id matches
// event[1], resets selection state, sets the root object's flag 1, and selects that entry.
static void __fastcall CMenuSaveGame_HandleMessage(CMenuSaveGame* self, void* /*edx*/, uint32_t* event)
{
    if (event[0] == 13) {
        CMenuSaveGame_HandleButtonEvent(self, nullptr, event);
    } else if (event[0] == 14) {
        void* editorObject = reinterpret_cast<void*>(self->field_0040);
        if (editorObject) {
            void** editorVtbl = CSF_AT(void**, editorObject, 0);
            void* saveEntry = ((void* (__fastcall*)(void*, void*, const char*))editorVtbl[0x5c / 4])(
                editorObject, nullptr, g_pszMenu_P_EditG);
            if (saveEntry) {
                void** entryVtbl = CSF_AT(void**, saveEntry, 0);
                int entryId = ((int (__fastcall*)(void*, void*))entryVtbl[0x20 / 4])(saveEntry, nullptr);
                if (static_cast<uint32_t>(entryId) == event[1]) {
                    CMenuSaveGame_ResetSelectionState(self, nullptr);
                    void** selfVtbl = CSF_AT(void**, self, 0);
                    void* rootObject = reinterpret_cast<void*>(
                        ((int (__fastcall*)(CMenuSaveGame*, void*))selfVtbl[0x20 / 4])(self, nullptr));
                    void** rootVtbl = CSF_AT(void**, rootObject, 0);
                    ((void (__fastcall*)(void*, void*, int))rootVtbl[0xc0 / 4])(rootObject, nullptr, 1);
                    CMenuSaveGame_HandleNameDialogResult(self, nullptr, saveEntry);
                }
            }
        }
    }
}

HOOK(0x00545C40, CMenuSaveGame_HandleMessage, "auto");
