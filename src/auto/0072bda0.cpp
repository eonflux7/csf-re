#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));

static void __fastcall CGuiPanel_RemoveChildControl(
    CGuiPanel* self, void* /*edx*/, CGuiControl* child)
{
    if (child == nullptr || self->field_0148 != 0)
        return;

    const std::uint32_t beginAddress = self->m_childrenBegin;
    const std::uint32_t childCount = beginAddress != 0
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->m_childrenEnd - beginAddress) >> 2)
        : 0;

    if (childCount == 0)
        return;

    auto* children = reinterpret_cast<std::uint32_t*>(self->m_childrenBegin);
    const auto childAddress = reinterpret_cast<std::uint32_t>(child);
    std::uint32_t index = 0;

    while (children[index] != childAddress)
    {
        ++index;
        if (index >= childCount)
            return;
    }

    const std::uint32_t byteOffset = index * 4;
    self->field_0148 = 1;

    void* removedChild = reinterpret_cast<void*>(children[index]);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, removedChild, 0)[0x108 / 4])(
            removedChild, nullptr, 0);

    const std::uint32_t destinationAddress = self->m_childrenBegin + byteOffset;
    self->field_0148 = 0;

    const std::uint32_t sourceAddress = destinationAddress + 4;
    const std::uint32_t remainingCount = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(self->m_childrenEnd - sourceAddress) >> 2);

    _memcpy(
        reinterpret_cast<void*>(destinationAddress),
        reinterpret_cast<const void*>(sourceAddress),
        remainingCount << 2);

    self->m_childrenEnd -= 4;

    const std::uint32_t currentChildAddress = self->field_014c;
    if (currentChildAddress == childAddress)
    {
        void* currentChild = reinterpret_cast<void*>(currentChildAddress);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, currentChild, 0)[0xB8 / 4])(
                currentChild, nullptr, 0);
        self->field_014c = 0;
    }
}

HOOK(0x0072BDA0, CGuiPanel_RemoveChildControl, "auto");
