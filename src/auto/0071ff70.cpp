#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0,
         (std::size_t size));
CSF_FUNC(void*, __fastcall, CGuiChoiceButton_Construct, 0x0072F4E0,
         (void* storage, void* edx, int parent, void* context));
CSF_FUNC(void, __fastcall, CGuiBuilder_ReadButtonProps, 0x0071FCB0,
         (CGuiBuilder* self, void* edx, void* control, std::uint8_t* node));

static void* __fastcall CGuiBuilder_CreateChoiceButton(
    CGuiBuilder* self, void* /*edx*/, std::uint8_t* node)
{
    void* storage = Mem_Alloc(0x208);
    void* control = nullptr;

    if (storage)
    {
        control = CGuiChoiceButton_Construct(
            storage,
            nullptr,
            static_cast<int>(self->field_0004),
            reinterpret_cast<std::uint8_t*>(self) + 0x0C);
    }

    CGuiBuilder_ReadButtonProps(self, nullptr, control, node);
    return control;
}

HOOK(0x0071FF70, CGuiBuilder_CreateChoiceButton, "auto");
