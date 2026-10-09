#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, sub_49AB10, 0x0049AB10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SetMenuContextState, 0x0049A0E0, (void* self, void* edx, int a4, char a5));
CSF_FUNC(int, __fastcall, sub_4528D0, 0x004528D0, (void* self, void* edx, int a2, int a3));

static void __fastcall CScene_DetachSceneEntry(CScene* self, void* /*edx*/, C_00833ee8* a2)
{
    C_00833ee8* entry = nullptr;
    if (a2) {
        entry = a2;
    } else {
        uint32_t currentIndex = CSF_AT(uint32_t, self, 0x70);
        if (currentIndex != 0xFFFFFFFFu && currentIndex < CSF_AT(uint32_t, self, 0x68)) {
            entry = CSF_AT(C_00833ee8**, self, 0x60)[currentIndex];
        }
    }

    if (!entry) {
        return;
    }

    sub_49AB10(entry, nullptr);
    if (!a2) {
        SetMenuContextState(entry, nullptr, 1, 1);
    }

    int entryNameAddress = static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(entry) + 0x1c));
    int entryAuxValue = CSF_AT(int, entry, 0x94);
    sub_4528D0(self, nullptr, entryNameAddress, entryAuxValue);

    uint32_t* slotOwnerTable = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x10));
    slotOwnerTable[CSF_AT(uint32_t, entry, 0x24)] = 0;

    if (!a2) {
        CSF_AT(uint32_t, self, 0x70) = 0xFFFFFFFFu;
    }

    ((void(__fastcall*)(void*, void*))CSF_AT(void**, entry, 0)[0x80 / 4])(entry, nullptr);
}

HOOK(0x004530D0, CScene_DetachSceneEntry, "auto");
