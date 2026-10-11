#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int AppMain_DestroyOwnedMenu(uint32_t& menuField, int previousResult)
{
    auto* menu = reinterpret_cast<C_008407a8*>(menuField);
    if (!menu)
        return previousResult;

    const int result =
        reinterpret_cast<int(__fastcall*)(C_008407a8*, void*, char)>(
            CSF_AT(void**, menu, 0)[0])(menu, nullptr, 1);
    menuField = 0;
    return result;
}

static int __fastcall C_00840754_ReleaseOwnedMenus(
    C_00840754* self, void* /*edx*/)
{
    int result = 0;
    result = AppMain_DestroyOwnedMenu(self->field_006c, result);
    result = AppMain_DestroyOwnedMenu(self->field_0064, result);
    result = AppMain_DestroyOwnedMenu(self->field_0068, result);
    result = AppMain_DestroyOwnedMenu(self->field_0070, result);
    result = AppMain_DestroyOwnedMenu(self->field_0008, result);
    result = AppMain_DestroyOwnedMenu(self->field_0014, result);
    result = AppMain_DestroyOwnedMenu(self->field_0010, result);
    result = AppMain_DestroyOwnedMenu(self->field_000c, result);
    result = AppMain_DestroyOwnedMenu(self->field_0018, result);
    result = AppMain_DestroyOwnedMenu(self->field_001c, result);
    result = AppMain_DestroyOwnedMenu(self->field_0020, result);
    result = AppMain_DestroyOwnedMenu(self->field_003c, result);
    result = AppMain_DestroyOwnedMenu(self->field_0024, result);
    result = AppMain_DestroyOwnedMenu(self->field_0044, result);
    result = AppMain_DestroyOwnedMenu(self->field_004c, result);
    result = AppMain_DestroyOwnedMenu(self->field_0048, result);
    result = AppMain_DestroyOwnedMenu(self->field_0050, result);
    result = AppMain_DestroyOwnedMenu(self->field_0030, result);
    result = AppMain_DestroyOwnedMenu(self->field_0034, result);
    result = AppMain_DestroyOwnedMenu(self->field_0074, result);
    return result;
}

HOOK(0x00509CC0, C_00840754_ReleaseOwnedMenus, "auto");
