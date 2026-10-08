#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t* GetElementPosition(void* element)
{
    return reinterpret_cast<uint32_t*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, element, 0)[0x9C / 4])(element, nullptr);
}

static uint32_t* GetElementExtent(void* element)
{
    return reinterpret_cast<uint32_t*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, element, 0)[0xA4 / 4])(element, nullptr);
}

static int __fastcall MenuMessageBox_UpdateChildClipping(
    void* self, void* /*edx*/, int index)
{
    if (!CSF_AT(void*, self, 0x5C))
        return 0;

    const uint32_t childOffset = 0x60u + static_cast<uint32_t>(index) * 4u;
    if (!CSF_AT(void*, self, childOffset))
        return 0;

    uint32_t* parentPosition =
        GetElementPosition(CSF_AT(void*, self, 0x5C));
    uint32_t* parentExtent =
        GetElementExtent(CSF_AT(void*, self, 0x5C));
    uint32_t* childPosition =
        GetElementPosition(CSF_AT(void*, self, childOffset));
    uint32_t* childExtent =
        GetElementExtent(CSF_AT(void*, self, childOffset));

    const int32_t childStart = static_cast<int32_t>(*childPosition);
    const uint32_t parentStart = *parentPosition;

    bool outside = childStart < static_cast<int32_t>(parentStart);
    if (!outside)
    {
        const uint32_t parentSize = *parentExtent;
        const uint32_t childSize = *childExtent;
        const uint32_t lastStart = parentSize - childSize + parentStart;
        outside = childStart > static_cast<int32_t>(lastStart);
    }

    // The in-range path clears AL only, retaining the pointer's upper bytes.
    const uint32_t hiddenArgument = outside
        ? 1u
        : (static_cast<uint32_t>(
               reinterpret_cast<uintptr_t>(childExtent)) & 0xFFFFFF00u);

    void* child = CSF_AT(void*, self, childOffset);
    return reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, child, 0)[0xA8 / 4])(
            child, nullptr, hiddenArgument);
}

HOOK(0x00529910, MenuMessageBox_UpdateChildClipping, "auto");
