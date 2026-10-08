#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetMenuContextValue, 0x00518980, ());

// Moves the list-box child so that its current position lines up with the
// position of the object referenced through the sub-object at self + 0x04.
static int __fastcall CMenuSceneSelect_AlignChildToReferencedPosition(
    CMenuSceneSelect* self, void* /*edx*/)
{
    using GetPositionObject = void* (__fastcall*)(void*, void*);
    using GetPosition = std::uint32_t* (__fastcall*)(
        void*, void*, std::uint32_t*);
    using SetPosition = int (__fastcall*)(
        void*, void*, std::uint32_t*, int);

    void* reference = reinterpret_cast<std::uint8_t*>(self) + 4;
    void* object = GetReferencedObject(reference, nullptr);
    void* positionObject =
        reinterpret_cast<GetPositionObject>(
            CSF_AT(void**, object, 0)[0x30 / 4])(object, nullptr);
    if (!positionObject)
        return 0;

    GetMenuContextValue();

    void* verticalObject = GetReferencedObject(reference, nullptr);
    void* horizontalObject = GetReferencedObject(reference, nullptr);

    void* verticalPosition =
        reinterpret_cast<GetPositionObject>(
            CSF_AT(void**, verticalObject, 0)[0x30 / 4])(verticalObject, nullptr);
    std::uint32_t y = CSF_AT(std::uint32_t, verticalPosition, 0x18);

    void* horizontalPosition =
        reinterpret_cast<GetPositionObject>(
            CSF_AT(void**, horizontalObject, 0)[0x30 / 4])(horizontalObject, nullptr);
    std::uint32_t x = CSF_AT(std::uint32_t, horizontalPosition, 0x14);

    std::uint32_t positionBuffer[2];
    void* child = reinterpret_cast<void*>(self->field_0014);
    std::uint32_t* currentPosition =
        reinterpret_cast<GetPosition>(
            CSF_AT(void**, child, 0)[0x160 / 4])(child, nullptr, positionBuffer);

    y -= currentPosition[1];
    x -= currentPosition[0];

    child = reinterpret_cast<void*>(self->field_0014);
    std::uint32_t displacement[2] = {x, y};
    return reinterpret_cast<SetPosition>(
        CSF_AT(void**, child, 0)[0x14C / 4])(child, nullptr, displacement, 1);
}

HOOK(0x0052FBD0, CMenuSceneSelect_AlignChildToReferencedPosition, "auto");
