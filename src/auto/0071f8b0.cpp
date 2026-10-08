#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, AllocateGameMemory, 0x006910F0,
         (std::size_t size));
CSF_FUNC(std::uint32_t*, __fastcall, ConstructRadioGroup, 0x0072E790,
         (std::uint32_t* storage, void* edx, int parent, void* context));
CSF_FUNC(void, __fastcall, ConfigureRadioGroup, 0x0071F790,
         (CGuiBuilder* builder, void* edx, std::uint32_t* group,
          std::uint8_t* node));

static std::uint32_t* __fastcall CGuiBuilder_CreateRadioGroup(
    CGuiBuilder* self, void* /*edx*/, std::uint8_t* node)
{
    void* storage = AllocateGameMemory(0x18C);
    std::uint32_t* group = nullptr;

    if (storage)
    {
        group = ConstructRadioGroup(
            static_cast<std::uint32_t*>(storage),
            nullptr,
            static_cast<int>(self->field_0004),
            reinterpret_cast<std::uint8_t*>(self) + 0x0C);
    }

    ConfigureRadioGroup(self, nullptr, group, node);
    return group;
}

HOOK(0x0071F8B0, CGuiBuilder_CreateRadioGroup, "auto");
