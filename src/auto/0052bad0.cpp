#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_Zero, 0x0082D4CC);

CSF_FUNC(void, __cdecl, Nullsub_506AC0,
         0x00506AC0, (int value));
CSF_FUNC(void*, __fastcall, MenuMessageBox_ResetEmbeddedState,
         0x005017E0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwRasterDestroy,
         0x00627570, (void* raster));

static int __fastcall CMenuMessageBox_Close(
    CMenuMessageBox* self, void* /*edx*/)
{
    if (self->field_08b4 == g_Zero && self->field_007c == 0)
        Nullsub_506AC0(1);

    void* owner = reinterpret_cast<void*>(self->field_0890);
    if (owner)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, CMenuMessageBox*, int)>(
            CSF_AT(void**, owner, 0)[0x60 / 4])(
                owner, nullptr, self, 1);
    }

    MenuMessageBox_ResetEmbeddedState(
        reinterpret_cast<std::uint8_t*>(self) + 0x8A0, nullptr);

    void* control = reinterpret_cast<void*>(self->field_0894);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, control, 0)[0x158 / 4])(
            control, nullptr, 0);

    int result = static_cast<int>(self->field_09b0);
    if (result != 0)
    {
        result = RwRasterDestroy(
            reinterpret_cast<void*>(self->field_09b0));
        self->field_09b0 = 0;
    }
    return result;
}

HOOK(0x0052BAD0, CMenuMessageBox_Close, "auto");
