#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, AppendFourByteElement_50CDF0, 0x0050CDF0, (void* self, void* , std::uint32_t* source));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __fastcall, MenuControls_ApplyChildIndex, 0x00712D90, (void* self, void* edx, int index));

static int __fastcall CMenuManager_ReleaseMenus(CMenuManager* self, void* /*edx*/)
{
    void* container = reinterpret_cast<void*>(self->field_0014);
    void* control =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, container, 0)[0x54 / 4])(container, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, control, 0)[0x0C / 4])(control, nullptr, 0);

    container = reinterpret_cast<void*>(self->field_0014);
    control =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, container, 0)[0x54 / 4])(container, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, control, 0)[0x0C / 4])(control, nullptr, 1);

    self->vSlot88();
    self->vSlot84();

    if (self->field_0014)
    {
        alignas(8) std::uint8_t elementsBytes[0x10]{};
        S_0050c9f0& elements =
            *reinterpret_cast<S_0050c9f0*>(elementsBytes);
        alignas(8) std::uint8_t screenBytes[0x4];
        std::uint32_t& screen =
            *reinterpret_cast<std::uint32_t*>(screenBytes);

        screen = self->m_pScreenGame;
        std::memset(&elements.field_0004, 0, 12);
        AppendFourByteElement_50CDF0(&elements, nullptr, &screen);

        container = reinterpret_cast<void*>(self->field_0014);
        reinterpret_cast<void (__fastcall*)(void*, void*, S_0050c9f0*, int)>(
            CSF_AT(void**, container, 0)[0x64 / 4])(
                container, nullptr, &elements, 1);

        MenuControls_ApplyChildIndex(
            reinterpret_cast<void*>(self->m_pGuiRoot),
            nullptr,
            static_cast<int>(self->field_0014));

        container = reinterpret_cast<void*>(self->field_0014);
        if (container)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, container, 0)[0])(
                    container, nullptr, 1);
            self->field_0014 = 0;
        }

        if (elements.field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(elements.field_0004));
    }

    return self->vReleaseOwnedScreens();
}

HOOK(0x005101E0, CMenuManager_ReleaseMenus, "auto");
