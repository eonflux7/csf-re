#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, ResetStringRecordRange, 0x0072EF70,
         (int begin, int end, void* container, void* owner));
CSF_FUNC(int, __cdecl, MenuExtras_ResetStringRange, 0x00546D60,
         (int begin, int end, void* container, void* owner));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static void** const g_MenuExtrasVtable =
    CSF_VTBL(0x00843CB8);
static void** const g_MenuExtrasSecondaryVtable =
    reinterpret_cast<void**>(0x00843C68);
static void** const g_MenuScreenIntermediateSecondaryVtable =
    reinterpret_cast<void**>(CSF_ADDR_DATA(0x00830488));
static void** const g_MenuScreenSecondaryVtable =
    CSF_VTBL(0x00830438);
static void** const g_MenuScreenVtable =
    CSF_VTBL(0x0083042C);

static int __fastcall CMenuExtras_DestroyContents(
    CMenuExtras* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(self->field_0018);

    CSF_AT(void** volatile, self, 0) = g_MenuExtrasVtable;
    static_cast<volatile std::uint32_t&>(self->field_0004) =
        reinterpret_cast<std::uint32_t>(g_MenuExtrasSecondaryVtable);

    if (child)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, child, 0)[0])(child, nullptr, 1);
        self->field_0018 = 0;
    }

    std::uint32_t stringBegin = self->field_0050;
    if (stringBegin)
    {
        ResetStringRecordRange(
            static_cast<int>(stringBegin),
            static_cast<int>(self->field_0054),
            reinterpret_cast<std::uint8_t*>(self) + 0x4C,
            self);

        stringBegin = self->field_0050;
        if (stringBegin)
            Mem_FreeDebug(reinterpret_cast<void*>(stringBegin));
    }

    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;

    int result = static_cast<int>(self->field_0038);
    if (result)
    {
        MenuExtras_ResetStringRange(
            result,
            static_cast<int>(self->field_003c),
            reinterpret_cast<std::uint8_t*>(self) + 0x34,
            self);

        result = static_cast<int>(self->field_0038);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }

    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;

    static_cast<volatile std::uint32_t&>(self->field_0004) =
        reinterpret_cast<std::uint32_t>(
            g_MenuScreenIntermediateSecondaryVtable);
    static_cast<volatile std::uint32_t&>(self->field_0004) =
        reinterpret_cast<std::uint32_t>(g_MenuScreenSecondaryVtable);
    CSF_AT(void** volatile, self, 0) = g_MenuScreenVtable;

    return result;
}

HOOK(0x00547720, CMenuExtras_DestroyContents, "auto");
