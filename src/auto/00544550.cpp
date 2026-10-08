#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszMenu_P_Edit, 0x00899B00);
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// The disassembly pushes four cdecl arguments and cleans them with add esp,0x10.
CSF_FUNC(int, __cdecl, MenuSaveGame_ClearSlotRange, 0x00544340,
         (int* a1, int* a2, void* a3, void* a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130,
         (void* self, void* edx, uint8_t* flagByte));

static int __fastcall CMenuProfilesPC_OpenProfileNameEdit(CMenuProfilesPC* self, void* /*edx*/)
{
    // Look up the P_Edit screen on the menu manager at +0x2C (vtable slot 0x5C).
    void* menuManager = CSF_AT(void*, self, 0x2C);
    void* editScreen =
        ((void* (__fastcall*)(void*, void*, const char*))(CSF_AT(void**, menuManager, 0)[23]))(
            menuManager, nullptr, g_pszMenu_P_Edit);

    // Back-pointer to this object.
    CSF_AT(uint32_t, self, 0x44) = reinterpret_cast<uint32_t>(self);

    // Release the previously saved slot range, if any.
    uint32_t firstSlot = CSF_AT(uint32_t, self, 0x4C);
    if (firstSlot != 0) {
        MenuSaveGame_ClearSlotRange(
            reinterpret_cast<int*>(firstSlot),
            reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x50)),
            reinterpret_cast<uint8_t*>(self) + 0x48,
            editScreen);
        if (CSF_AT(uint32_t, self, 0x4C) != 0)
            Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x4C)));
    }

    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(uint32_t, self, 0x54) = 0;

    // Initialise the embedded sub-object at +0x58 with the global flag byte.
    TextInputSubobject_Init(
        reinterpret_cast<uint8_t*>(self) + 0x58, nullptr, &g_byte_82E65C);

    CSF_AT(uint8_t, self, 0x74) = 1;
    CSF_AT(uint32_t, editScreen, 0x54) =
        reinterpret_cast<uint32_t>(reinterpret_cast<uint8_t*>(self) + 0x44);

    // Enable the view manager at +0x24 (vtable slot 0xA8).
    void* viewManager = CSF_AT(void*, self, 0x24);
    ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, viewManager, 0)[42]))(
        viewManager, nullptr, 1);

    // Activate the P_Edit screen through the menu manager (vtable slot 0x58).
    void* menuManagerAgain = CSF_AT(void*, self, 0x2C);
    int result =
        ((int(__fastcall*)(void*, void*, const char*))(CSF_AT(void**, menuManagerAgain, 0)[22]))(
            menuManagerAgain, nullptr, g_pszMenu_P_Edit);

    CSF_AT(int32_t, self, 0x38) = -1;
    return result;
}

HOOK(0x00544550, CMenuProfilesPC_OpenProfileNameEdit, "auto");
