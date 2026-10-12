#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __stdcall, sub_4483F0, 0x004483F0, (int a2));
CSF_FUNC(int, __fastcall, sub_448890, 0x00448890, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, sub_44A640, 0x0044A640, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_44A690, 0x0044A690, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall S_00448830_GetPositionCategory(
    S_00448830* self, void* /*edx*/, int handle)
{
    if (!sub_4483F0(handle))
        return 0;

    void* object = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    if (!object)
        return 0;

    if ((CSF_AT(std::uint32_t, object, 0x170) & 0x2000) == 0)
        return 0;

    int position = CSF_AT(int, object, 0x1F4);
    if (position == -1)
        return 0;

    sub_448890(self, nullptr, position);
    sub_44A640(self, nullptr, handle);
    int category = sub_44A690(self, nullptr, handle);

    if (category == 1)
        return 4;
    if (category == 2)
        return 3;
    return 0;
}

HOOK(0x0044A950, S_00448830_GetPositionCategory, "auto");
