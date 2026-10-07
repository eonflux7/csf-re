#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, CopyMemory, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));

static void __fastcall CGuiControl_RemoveListener(
    CGuiControl* self, void* /*edx*/, std::uint32_t listener)
{
    if (listener == 0)
        return;

    const std::uint32_t initialBegin = self->m_listenersBegin;
    const std::uint32_t listenerCount = initialBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  self->m_listenersEnd - initialBegin) >> 2)
        : 0;

    if (listenerCount == 0)
        return;

    const std::uint32_t begin = self->m_listenersBegin;
    std::uint32_t entryAddress = begin;
    std::uint32_t index = 0;

    while (*reinterpret_cast<const std::uint32_t*>(entryAddress) != listener)
    {
        ++index;
        entryAddress += 4;
        if (index >= listenerCount)
            return;
    }

    const std::uint32_t end = self->m_listenersEnd;
    const std::uint32_t destinationAddress = begin + index * 4;
    const std::uint32_t sourceAddress = destinationAddress + 4;
    const std::uint32_t byteCount =
        static_cast<std::uint32_t>(
            static_cast<std::int32_t>(end - sourceAddress) >> 2) << 2;

    CopyMemory(reinterpret_cast<void*>(destinationAddress),
               reinterpret_cast<const void*>(sourceAddress),
               byteCount);

    self->m_listenersEnd -= 4;
}

HOOK(0x00719C90, CGuiControl_RemoveListener, "auto");
