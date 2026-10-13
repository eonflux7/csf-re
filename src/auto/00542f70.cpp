#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Cheats_RefreshSelection, 0x0042AF90, (int group, int context, CGuiListBoxBase* listBox));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __stdcall, MenuControls_IsIdNotInTwoSlots, 0x00542BF0, (int id));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x00716A60, (CGuiListBoxBase* self, void* , int index));

static uint32_t __fastcall C_0085ec18_StoreControlBindingParameters(
    C_0085ec18* self, void* /*edx*/, int id, int eventData)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    if (CSF_AT(int, self, 0x2D8) == 1)
    {
        if (id == 1 ||
            static_cast<uint8_t>(MenuControls_IsIdNotInTwoSlots(id)) == 0)
        {
            if (id != 1)
            {
                void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
                GameBddLoader_DispatchMenuRequest(
                    globalState, nullptr, 1263218739, 1, 16, 0);
            }

            Cheats_RefreshSelection(
                CSF_AT(int, self, 0x2C4),
                reinterpret_cast<int>(bytes + 0x44),
                CSF_AT(CGuiListBoxBase*, self, 0x28));

            // Preserve the callee's machine return, despite its void binding.
            using AddSelectedItemWithResult =
                uint32_t(__fastcall*)(CGuiListBoxBase*, void*, int);
            uint32_t result =
                reinterpret_cast<AddSelectedItemWithResult>(
                    CGuiListBoxBase_AddSelectedItem)(
                        CSF_AT(CGuiListBoxBase*, self, 0x28),
                        nullptr,
                        CSF_AT(int, self, 0x2D4));

            CSF_AT(int, self, 0x2D8) = -1;
            return result;
        }

        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        void* input = CSF_AT(void*, globalState, 0x10);

        alignas(8) uint8_t pressedKeysBytes[0x10];
        CSF_AT(uint32_t, pressedKeysBytes, 0x0) = 0;
        CSF_AT(uint32_t, pressedKeysBytes, 0x4) = 0;
        CSF_AT(int, pressedKeysBytes, 0x8) = 0;
        CSF_AT(int, pressedKeysBytes, 0xC) = 10;

        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, input, 0)[0x3C / 4])(
                input, nullptr, pressedKeysBytes);

        int keyCount = CSF_AT(int, pressedKeysBytes, 0x8);
        void* keys = CSF_AT(void*, pressedKeysBytes, 0x0);

        CSF_AT(int, self, 0x2C8) = 0;

        int binding = id;
        if (keyCount == 2 &&
            CSF_AT(int, keys, 0x0) == 29 &&
            CSF_AT(int, keys, 0x4) == 184)
        {
            binding = 184;
        }

        CSF_AT(int, self, 0x2CC) = binding;
        CSF_AT(int, self, 0x2D8) = 2;

        if (keys)
            return static_cast<uint32_t>(CoreMemory_Free(keys));

        return static_cast<uint32_t>(binding);
    }

    if (id == 1)
    {
        void* menu = bytes - 4;
        return ((uint32_t(__fastcall*)(void*, void*))
            CSF_AT(void**, menu, 0)[0x4C / 4])(menu, nullptr);
    }

    if (id == 50)
    {
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(eventData), 0x38) != 0)
        {
            void* referencedObject = GetReferencedObject(self, nullptr);
            return ((uint32_t(__fastcall*)(void*, void*))
                CSF_AT(void**, referencedObject, 0)[0x1C / 4])(
                    referencedObject, nullptr);
        }
        return 0;
    }

    return static_cast<uint32_t>(id) - 50;
}

HOOK(0x00542F70, C_0085ec18_StoreControlBindingParameters, "auto");
