#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, AllocateGameMemory, 0x006910F0,
         (std::size_t size));
CSF_FUNC(std::uint32_t*, __fastcall, ConstructGuiPanel, 0x0072C580,
         (std::uint32_t* storage, void* edx, int parent, void* context));
CSF_FUNC(void, __fastcall, ConfigureGuiPanel, 0x0071F790,
         (CGuiBuilder* builder, void* edx, std::uint32_t* panel,
          std::uint8_t* node));

static std::uint32_t* __fastcall CGuiBuilder_CreatePanel(
    CGuiBuilder* self, void* /*edx*/, std::uint8_t* node)
{
    void* storage = AllocateGameMemory(0x18C);
    std::uint32_t* panel = nullptr;

    if (storage)
    {
        panel = ConstructGuiPanel(
            static_cast<std::uint32_t*>(storage),
            nullptr,
            static_cast<int>(self->field_0004),
            &self->field_000c);
    }

    ConfigureGuiPanel(self, nullptr, panel, node);
    return panel;
}

HOOK(0x0071F850, CGuiBuilder_CreatePanel, "auto");
