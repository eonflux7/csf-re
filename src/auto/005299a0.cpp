#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(void, __fastcall, CGuiStatic_CopyStateToTarget, 0x00718230,
         (void* self, void* edx, void* target));

static void __fastcall MenuMessageBox_LayoutOptionControl(
    void* self, void* /*edx*/, std::uint32_t index,
    std::uint32_t horizontalOffset, std::uint32_t finalComponent)
{
    if (index > 2)
        return;
    if (!CSF_AT(void*, self, 0x5C))
        return;

    void* control = CSF_AT(void*, self, 0x60 + index * 4);
    if (!control)
        return;

    void* child =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, control, 0)[68])(
            control, nullptr);

    std::uint32_t size[2];
    if (child)
    {
        size[0] =
            ((std::uint32_t(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[41])(child, nullptr);
        size[1] =
            ((std::uint32_t(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[40])(child, nullptr);
    }
    else
    {
        size[0] = 0;
        size[1] = 0;
    }

    control = CSF_AT(void*, self, 0x60 + index * 4);
    ((void(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, control, 0)[40])(control, nullptr, size);

    void* reference = CSF_AT(void*, self, 0x5C);
    void* referencePosition =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, reference, 0)[39])(
            reference, nullptr);

    std::uint32_t position[2];
    position[0] = CSF_AT(std::uint32_t, referencePosition, 0)
                + horizontalOffset;
    position[1] = CSF_AT(std::uint32_t, referencePosition, 4);

    control = CSF_AT(void*, self, 0x60 + index * 4);
    ((void(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, control, 0)[38])(control, nullptr, position);

    CGuiStatic_CopyStateToTarget(
        CSF_AT(void*, self, 0x60 + index * 4), nullptr, child);

    if (child)
    {
        void* source =
            ((void*(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[25])(
                child, nullptr);
        std::uint32_t rectangle[4];
        rectangle[0] = CSF_AT(std::uint32_t, source, 0);
        rectangle[1] = CSF_AT(std::uint32_t, source, 4);
        rectangle[2] = CSF_AT(std::uint32_t, source, 8);
        rectangle[3] = CSF_AT(volatile std::uint32_t, source, 0xC);

        void** vtable = CSF_AT(void**, child, 0);
        rectangle[3] = finalComponent;
        ((void(__fastcall*)(void*, void*, std::uint32_t*))
            vtable[24])(child, nullptr, rectangle);
    }
}

HOOK(0x005299A0, MenuMessageBox_LayoutOptionControl, "auto");
