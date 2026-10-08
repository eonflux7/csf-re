#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0,
         (std::size_t size));
CSF_FUNC(std::uint32_t*, __fastcall, CGuiRadioButton_Ctor, 0x0072FBB0,
         (std::uint32_t* storage, void* edx, int parent, void* context));
CSF_FUNC(void, __fastcall, CGuiBuilder_ReadButtonProps, 0x0071FCB0,
         (CGuiBuilder* builder, void* edx, std::uint32_t* button,
          std::uint8_t* node));

static std::uint32_t* __fastcall CGuiBuilder_CreateRadioButton(
    CGuiBuilder* self, void* /*edx*/, std::uint8_t* node)
{
    void* storage = Mem_Alloc(0x1F0);
    std::uint32_t* button = nullptr;

    if (storage)
    {
        button = CGuiRadioButton_Ctor(
            static_cast<std::uint32_t*>(storage),
            nullptr,
            static_cast<int>(self->field_0004),
            reinterpret_cast<std::uint8_t*>(self) + 0x0C);
    }

    CGuiBuilder_ReadButtonProps(self, nullptr, button, node);
    return button;
}

HOOK(0x00720030, CGuiBuilder_CreateRadioButton, "auto");
