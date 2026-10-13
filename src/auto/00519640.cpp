#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00840cf8_ClearDialog, 0x00519480, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0085ec18_AddDialogItem, 0x005195A0, (void* self, void* edx, int a2, int a3, int a4, char* a5));
CSF_FUNC(uint32_t*, __fastcall, C_00840d64_ConstructSearchItem, 0x005198C0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuSystem_AssignDialogPanel, 0x00712E80, (void* self, void* edx, int panel));

static char __fastcall C_0085ec18_InitializeSearchDialog(
    C_0085ec18* self, void* /*edx*/, uint32_t owner)
{
    if (CSF_AT(uint8_t, self, 0xC))
    {
        C_00840cf8_ClearDialog(self, nullptr);
        CSF_AT(uint8_t, self, 0xC) = 0;
    }

    CSF_AT(uint8_t, self, 0xD) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;

    void* controls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* font = reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, controls, 0)[0x50 / 4])(
            controls, nullptr, CSF_STR(0x008343B0, "menus\\Ingame_01.fnt"));
    CSF_AT(uint32_t, self, 0x10) = reinterpret_cast<uint32_t>(font);

    if (font)
    {
        void* display = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        uint32_t* dimensions =
            reinterpret_cast<uint32_t*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);

        uint32_t horizontalPosition = *dimensions - 10;
        void* storedFont = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));

        alignas(8) uint8_t positionBytes[0x8];
        auto* position = reinterpret_cast<uint32_t*>(positionBytes);
        position[0] = horizontalPosition;
        position[1] = 15;

        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, storedFont, 0)[0x58 / 4])(
                storedFont, nullptr, position);
    }

    self->field_0004 = owner;
    void* referencedObject = GetReferencedObject(self, nullptr);
    MenuSystem_AssignDialogPanel(
        referencedObject, nullptr, reinterpret_cast<int>(self));

    void* storage = reinterpret_cast<void*>(CoreMemory_AllocUntagged(40));
    if (storage)
    {
        uint32_t* searchItem = C_00840d64_ConstructSearchItem(storage, nullptr);
        if (searchItem)
        {
            C_0085ec18_AddDialogItem(
                self, nullptr, 0, 48, reinterpret_cast<int>(searchItem),
                const_cast<char*>(CSF_STR(0x00840D48, "(B)uscador")));
        }
    }

    CSF_AT(uint8_t, self, 0xC) = 1;
    return 1;
}

HOOK(0x00519640, C_0085ec18_InitializeSearchDialog, "auto");
