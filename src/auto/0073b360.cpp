#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static CSF_FUNC(int, __fastcall, C_00867444_ResetState, 0x00739F10,
                (C_00867444* self, void* edx));
static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110,
                (void* buffer));
static CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
                (void* storage));
static CSF_FUNC(int*, __fastcall, EraseTreeRange, 0x0073A500,
                (void* tree, void* edx, int* result, void* first, void* last));
static CSF_FUNC(void, __fastcall, C_0085e65c_InitVtable, 0x006923A0,
                (C_0085e65c* self, void* edx));

static void __fastcall C_00867444_Destroy(
    C_00867444* self, void* /*edx*/)
{
    void* owner = CSF_AT(void*, self, 0x08);
    CSF_AT(void**, self, 0) = CSF_VTBL(0x00867444);
    reinterpret_cast<void(__fastcall*)(void*, void*, C_00867444*)>(
        CSF_AT(void**, owner, 0)[0x90 / 4])(owner, nullptr, self);

    C_00867444_ResetState(self, nullptr);

    void* buffer = CSF_AT(void*, self, 0x34);
    if (buffer)
    {
        Mem_Free(buffer);
        CSF_AT(std::uint32_t, self, 0x34) = 0;
    }

    void* tree = reinterpret_cast<std::uint8_t*>(self) + 0x28;
    void* sentinel = CSF_AT(void*, self, 0x2C);
    void* first = CSF_AT(void*, sentinel, 0);
    int eraseResult;
    EraseTreeRange(tree, nullptr, &eraseResult, first, sentinel);

    void* treeStorage = CSF_AT(void*, self, 0x2C);
    if (treeStorage)
        Mem_FreeDebug(treeStorage);

    CSF_AT(std::uint32_t, self, 0x2C) = 0;
    CSF_AT(std::uint32_t, self, 0x30) = 0;

    if (CSF_AT(std::uint32_t, self, 0x24) >= 16u)
    {
        void* stringStorage = CSF_AT(void*, self, 0x10);
        if (stringStorage)
            Mem_FreeDebug(stringStorage);
    }

    CSF_AT(std::uint32_t, self, 0x24) = 15;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint8_t, self, 0x10) = 0;

    CSF_AT(void**, self, 0) = CSF_VTBL(0x008673FC);
    C_0085e65c_InitVtable(reinterpret_cast<C_0085e65c*>(self), nullptr);
}

HOOK(0x0073B360, C_00867444_Destroy, "auto");
