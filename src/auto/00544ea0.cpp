#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830,
         (void* destination, void* edx, const void* source,
          uint32_t position, uint32_t count));
CSF_FUNC(void*, __fastcall, SaveGame_AssignString, 0x004F25F0,
         (void* destination, void* edx, const void* source));
CSF_FUNC(int, __fastcall, ResetInnerBuffer, 0x00501170,
         (void* record, void* edx));
CSF_FUNC(uint32_t, __fastcall, SaveGame_DestroyRecord, 0x00500A80,
         (void* record, void* edx));
CSF_FUNC(int, __fastcall, SaveGame_ValidateName, 0x00696BB0,
         (void* name, void* edx, const uint8_t* separator));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720,
         (void* destination, const char* key, int option));
CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90,
         (void* string, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(uint32_t, __fastcall, SaveGame_ShowMessage, 0x0050F860,
         (void* state, void* edx, const void* text, int kind, int option));
CSF_FUNC(int*, __cdecl, MenuSaveGame_ClearListContainer, 0x00541650,
         (int* container));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
         (void* storage));

// These bindings cannot express the original additional EBX argument.
// Both call sites require EBX == 0; see metadata.
CSF_FUNC(void, __fastcall, CMenuSaveGame_RefreshSaveGameList, 0x00544900,
         (CMenuSaveGame* self, void* edx));
CSF_FUNC(void, __cdecl, MenuMpStats_ForwardToLayout541230, 0x00541570,
         (void* container, uint32_t value));

CSF_GLOBAL(uint8_t, g_SaveGameNameSeparator, 0x0082E65C);

static void __fastcall CMenuSaveGame_HandleNameDialogResult(
    CMenuSaveGame* self, void* /*edx*/, void* result)
{
    void* layout = reinterpret_cast<void*>(self->field_0040);
    reinterpret_cast<void (__fastcall*)(void*, void*, CMenuSaveGame*, int)>(
        CSF_AT(void**, layout, 0)[0x7C / 4])(
            layout, nullptr, self, 1);

    const uint8_t accepted = CSF_AT(uint8_t, result, 0x30);

    alignas(4) uint32_t name[7];
    name[6] = 7;
    name[5] = 0;
    CSF_AT(uint16_t, name, 4) = 0;
    AssignStringRange_488830(
        name, nullptr, static_cast<uint8_t*>(result) + 0x34,
        0, UINT32_MAX);

    const uint32_t operation = self->field_0050;
    if ((operation == 2 || operation == 4) &&
        accepted && self->field_0058 != 0)
    {
        if (static_cast<uint8_t>(
                SaveGame_ValidateName(name, nullptr, &g_SaveGameNameSeparator)))
        {
            if (operation == 2)
            {
                uint32_t recordAddress =
                    self->field_0054 * 200u + self->field_0024;
                void* saveManager =
                    reinterpret_cast<void*>(self->field_0058);
                reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, saveManager, 0)[0x34 / 4])(
                        saveManager, nullptr,
                        reinterpret_cast<void*>(recordAddress + 0x50u));

                recordAddress =
                    self->field_0054 * 200u + self->field_0024;
                SaveGame_AssignString(
                    reinterpret_cast<void*>(recordAddress), nullptr, name);

                recordAddress =
                    self->field_0054 * 200u + self->field_0024;
                saveManager = reinterpret_cast<void*>(self->field_0058);
                reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, saveManager, 0)[0x0C / 4])(
                        saveManager, nullptr,
                        reinterpret_cast<void*>(recordAddress + 0x1Cu));

                recordAddress =
                    self->field_0054 * 200u + self->field_0024;
                saveManager = reinterpret_cast<void*>(self->field_0058);
                reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, saveManager, 0)[0x20 / 4])(
                        saveManager, nullptr,
                        reinterpret_cast<void*>(recordAddress));

                void** menuVtable = CSF_AT(void**, self, 0);
                self->field_0050 = UINT32_MAX;
                void* dialog =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        menuVtable[0x20 / 4])(self, nullptr);
                reinterpret_cast<void (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, dialog, 0)[0x16C / 4])(dialog, nullptr);

                CMenuSaveGame_RefreshSaveGameList(self, nullptr);
            }
            else
            {
                alignas(4) uint8_t record[200];
                ResetInnerBuffer(record, nullptr);

                void* saveManager =
                    reinterpret_cast<void*>(self->field_0058);
                reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, saveManager, 0)[0x0C / 4])(
                        saveManager, nullptr, record + 0x1C);

                AssignStringRange_488830(
                    record, nullptr, name, 0, UINT32_MAX);

                saveManager = reinterpret_cast<void*>(self->field_0058);
                reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, saveManager, 0)[0x20 / 4])(
                        saveManager, nullptr, record);

                void** menuVtable = CSF_AT(void**, self, 0);
                self->field_0050 = UINT32_MAX;
                void* dialog =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        menuVtable[0x20 / 4])(self, nullptr);
                reinterpret_cast<void (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, dialog, 0)[0x16C / 4])(dialog, nullptr);

                CMenuSaveGame_RefreshSaveGameList(self, nullptr);
                SaveGame_DestroyRecord(record, nullptr);
            }
        }
        else
        {
            alignas(4) uint32_t message[7];
            CSF_AT(uint16_t, message, 4) = 0;
            message[5] = 0;
            message[6] = 7;
            self->field_0050 = 5;

            InitStringFromLiteral(
                message, CSF_STR(0x00843B24, "GEN7"), 1);

            const void* text =
                reinterpret_cast<const void*>(message[1]);
            if (message[6] < 8)
                text = reinterpret_cast<uint8_t*>(message) + 4;

            void* state = GetGlobalState_50D190();
            SaveGame_ShowMessage(state, nullptr, text, 16, 0);
            SaveGame_DestroyLocalizedString(message, nullptr);
        }
    }

    if (!accepted)
    {
        void* dialog =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0x20 / 4])(self, nullptr);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, dialog, 0)[0x16C / 4])(dialog, nullptr);

        void* container = reinterpret_cast<uint8_t*>(self) + 0x20;
        MenuSaveGame_ClearListContainer(static_cast<int*>(container));
        MenuMpStats_ForwardToLayout541230(container, self->field_001c);
        self->field_0050 = UINT32_MAX;
    }

    if (name[6] >= 8)
    {
        void* storage = reinterpret_cast<void*>(name[1]);
        if (storage)
            Mem_FreeDebug(storage);
    }
}

HOOK(0x00544EA0, CMenuSaveGame_HandleNameDialogResult, "auto");
