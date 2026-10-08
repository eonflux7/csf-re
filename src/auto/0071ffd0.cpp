#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, AllocateGameMemory, 0x006910F0, (std::size_t size));
CSF_FUNC(std::uint32_t*, __fastcall, ConstructChoiceNumButton, 0x0072F7C0,
         (std::uint32_t* storage, void* edx, std::uint32_t parent, void* context));
CSF_FUNC(void, __fastcall, CGuiBuilder_ReadButtonProps, 0x0071FCB0,
         (CGuiBuilder* self, void* edx, std::uint32_t* button, std::uint8_t* node));

static std::uint32_t* __fastcall CGuiBuilder_CreateChoiceNumButton(
    CGuiBuilder* self, void* /*edx*/, std::uint8_t* node)
{
    void* storage = AllocateGameMemory(0x208);
    std::uint32_t* button = nullptr;

    if (storage)
    {
        button = ConstructChoiceNumButton(
            static_cast<std::uint32_t*>(storage),
            nullptr,
            self->field_0004,
            reinterpret_cast<std::uint8_t*>(self) + 0x0C);
    }

    CGuiBuilder_ReadButtonProps(self, nullptr, button, node);
    return button;
}

HOOK(0x0071FFD0, CGuiBuilder_CreateChoiceNumButton, "auto");
