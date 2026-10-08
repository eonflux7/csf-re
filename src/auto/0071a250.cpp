#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(std::uint32_t, __fastcall, AppendListenerEntry, 0x0071A100,
         (void* self, void* edx, std::uint32_t* listener));

static void __fastcall CGuiControl_AddListener(
    CGuiControl* self, void* /*edx*/, std::uint32_t listener)
{
    if (listener == 0)
        return;

    const std::uint32_t begin = self->m_listenersBegin;
    std::uint32_t count = 0;
    if (begin != 0)
    {
        const std::uint32_t byteDistance =
            CSF_AT(std::uint32_t, self, 0x104) - begin;
        count = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(byteDistance) >> 2);
    }

    if (count != 0)
    {
        const auto* entry =
            reinterpret_cast<const std::uint32_t*>(self->m_listenersBegin);
        for (std::uint32_t index = 0; index < count; ++index, ++entry)
        {
            if (*entry == listener)
                return;
        }
    }

    AppendListenerEntry(
        reinterpret_cast<std::uint8_t*>(self) + 0xFC, nullptr, &listener);
}

HOOK(0x0071A250, CGuiControl_AddListener, "auto");
