#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static CSF_GLOBAL(void*, g_FontVtable, 0x00867444);

static CSF_FUNC(C_0085e65c*, __fastcall, ConstructFontBase, 0x00692390,
                (C_0085e65c* self, void* edx));
static CSF_FUNC(void*, __fastcall, CreateEmptyRecord, 0x00739A70,
                (void* tree, void* edx));
static CSF_FUNC(int, __fastcall, C_00867444_ClearAndReset, 0x00739EC0,
                (C_00867444* self, void* edx));

static int __fastcall C_00867444_Construct(
    C_00867444* self, void* /*edx*/, int owner)
{
    ConstructFontBase(reinterpret_cast<C_0085e65c*>(self), nullptr);
    CSF_AT(void**, self, 0) = &g_FontVtable;

    void* tree = reinterpret_cast<std::uint8_t*>(self) + 0x28;
    CSF_AT(std::uint32_t, self, 0x24) = 15;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint8_t, self, 0x10) = 0;

    void* sentinel = CreateEmptyRecord(tree, nullptr);
    CSF_AT(void*, self, 0x2C) = sentinel;
    CSF_AT(std::uint8_t, sentinel, 0x49) = 1;

    sentinel = CSF_AT(void*, self, 0x2C);
    CSF_AT(void*, sentinel, 0x04) = sentinel;
    sentinel = CSF_AT(void*, self, 0x2C);
    CSF_AT(void*, sentinel, 0) = sentinel;
    sentinel = CSF_AT(void*, self, 0x2C);
    CSF_AT(void*, sentinel, 0x08) = sentinel;

    CSF_AT(std::uint32_t, self, 0x30) = 0;
    CSF_AT(std::uint32_t, self, 0x34) = 0;
    CSF_AT(std::uint32_t, self, 0x38) = 0;
    CSF_AT(std::uint32_t, self, 0x3C) = 0;
    CSF_AT(std::uint32_t, self, 0x40) = 10;
    CSF_AT(std::uint32_t, self, 0x54) = 0;
    CSF_AT(std::uint32_t, self, 0x58) = 0;
    CSF_AT(int, self, 0x08) = owner;

    C_00867444_ClearAndReset(self, nullptr);
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x0073B450, C_00867444_Construct, "auto");
