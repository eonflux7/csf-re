#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, AllocateGameMemory, 0x006910F0,
         (std::size_t size));
CSF_FUNC(void*, __fastcall, CGuiButton_Ctor, 0x00725390,
         (void* storage, void* edx, std::uint32_t arg1, void* arg2));
CSF_FUNC(void, __fastcall, CGuiBuilder_ReadButtonProps, 0x0071FCB0,
         (CGuiBuilder* self, void* edx, void* button, std::uint8_t* node));

// Allocates a button object, constructs it with the builder's owner/context and
// fills it from the current .fbs keyword node. Returns the new button (or null).
static void* __fastcall CGuiBuilder_CreateButton(
    CGuiBuilder* self, void* /*edx*/, std::uint8_t* node)
{
    void* storage = AllocateGameMemory(0x1F0);
    void* button = nullptr;

    if (storage)
    {
        button = CGuiButton_Ctor(
            storage,
            nullptr,
            self->field_0004,
            reinterpret_cast<std::uint8_t*>(self) + 0x0C);
    }

    CGuiBuilder_ReadButtonProps(self, nullptr, button, node);
    return button;
}

HOOK(0x00720090, CGuiBuilder_CreateButton, "auto");
