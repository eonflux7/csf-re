#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __stdcall, SceneLoader_ConstructArrayElements, 0x004011C0, (void* first, int elementSize, int count, int (__thiscall* constructor)(void*)));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

static std::uint32_t* __cdecl C_00861988_AllocateElementArray(
    int elementType, int count, std::uint32_t* elementSizeOut)
{
    using Constructor = int (__thiscall*)(void*);
    int elementSize;
    Constructor constructor;

    switch (elementType)
    {
    case 0:
        elementSize = 136;
        constructor = reinterpret_cast<Constructor>(CSF_ADDR_FN(0x006F4CD0));
        break;
    case 1:
        elementSize = 152;
        constructor = reinterpret_cast<Constructor>(CSF_ADDR_FN(0x006DCA90));
        break;
    case 2:
        elementSize = 84;
        constructor = reinterpret_cast<Constructor>(CSF_ADDR_FN(0x006F2D90));
        break;
    case 3:
        elementSize = 84;
        constructor = reinterpret_cast<Constructor>(CSF_ADDR_FN(0x006F2AE0));
        break;
    case 4:
        elementSize = 144;
        constructor = reinterpret_cast<Constructor>(CSF_ADDR_FN(0x006F1A00));
        break;
    case 5:
        elementSize = 60;
        constructor = reinterpret_cast<Constructor>(CSF_ADDR_FN(0x006F0CB0));
        break;
    default:
        return nullptr;
    }

    auto* allocation = reinterpret_cast<std::uint32_t*>(
        CoreMemory_AllocUntagged(
            static_cast<std::size_t>(elementSize) *
            static_cast<std::size_t>(count) + 4));

    std::uint32_t* elements = nullptr;
    if (allocation)
    {
        *allocation = static_cast<std::uint32_t>(count);
        elements = allocation + 1;
        SceneLoader_ConstructArrayElements(elements, elementSize, count, constructor);
    }

    *elementSizeOut = static_cast<std::uint32_t>(elementSize);
    return elements;
}

HOOK(0x006C9CE0, C_00861988_AllocateElementArray, "auto");
