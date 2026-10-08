#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetMenuContextValue, 0x00518980, ());

static int __fastcall MenuScenarioSelect_AlignChildToReferencedPosition(
    void* self, void* /*edx*/)
{
    using GetObject = void* (__fastcall*)(void*, void*);
    using GetPosition = std::uint32_t* (__fastcall*)(
        void*, void*, std::uint32_t*);
    using SetPosition = int (__fastcall*)(
        void*, void*, std::uint32_t*, int);

    void* reference = static_cast<std::uint8_t*>(self) + 4;
    void* object = GetReferencedObject(reference, nullptr);
    void* positionObject =
        reinterpret_cast<GetObject>(CSF_AT(void**, object, 0)[0x30 / 4])(
            object, nullptr);
    if (!positionObject)
        return 0;

    GetMenuContextValue();

    void* verticalObject = GetReferencedObject(reference, nullptr);
    void* horizontalObject = GetReferencedObject(reference, nullptr);

    void* verticalPosition =
        reinterpret_cast<GetObject>(
            CSF_AT(void**, verticalObject, 0)[0x30 / 4])(
                verticalObject, nullptr);
    std::uint32_t y = CSF_AT(std::uint32_t, verticalPosition, 0x18);

    void* horizontalPosition =
        reinterpret_cast<GetObject>(
            CSF_AT(void**, horizontalObject, 0)[0x30 / 4])(
                horizontalObject, nullptr);
    std::uint32_t x = CSF_AT(std::uint32_t, horizontalPosition, 0x14);

    std::uint32_t positionBuffer[2];
    void* child = CSF_AT(void*, self, 0x18);
    std::uint32_t* currentPosition =
        reinterpret_cast<GetPosition>(
            CSF_AT(void**, child, 0)[0x160 / 4])(
                child, nullptr, positionBuffer);

    y -= currentPosition[1];
    x -= currentPosition[0];

    child = CSF_AT(void*, self, 0x18);
    std::uint32_t displacement[2] = {x, y};
    return reinterpret_cast<SetPosition>(
        CSF_AT(void**, child, 0)[0x14C / 4])(
            child, nullptr, displacement, 1);
}

HOOK(0x0052D850, MenuScenarioSelect_AlignChildToReferencedPosition, "auto");
