#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static bool __fastcall S_004aa4d0_IsWithinMovementThreshold(
    S_004aa4d0* self, void* /*edx*/, float threshold)
{
    auto* context = reinterpret_cast<C_008342e8*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (!context)
        return false;

    void* link = reinterpret_cast<uint8_t*>(self) + 8;
    int linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    auto* owner = linkedValue
        ? reinterpret_cast<uint8_t*>(static_cast<uint32_t>(linkedValue)) - 4
        : nullptr;

    void* object = CSF_AT(void*, owner, 0x2C);
    void* queryObject = CSF_AT(void*, object, 0x3C);
    if (!((bool(__fastcall*)(void*, void*, uint32_t*, int))
              CSF_AT(void**, queryObject, 0)[0x9C / 4])(
            queryObject, nullptr, &context->field_001c, 14))
        return false;

    linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    owner = linkedValue
        ? reinterpret_cast<uint8_t*>(static_cast<uint32_t>(linkedValue)) - 4
        : nullptr;

    float* objectPosition =
        GetObjectPositionVector(CSF_AT(void*, owner, 0x2C), nullptr);
    float* contextPosition = GetObjectPositionVector(context, nullptr);
    GetLinkedObjectValue_4DB5C0(link, nullptr);

    float dx = CSF_AT(float, contextPosition, 0) -
               CSF_AT(float, objectPosition, 0);
    float dy = CSF_AT(float, contextPosition, 4) -
               CSF_AT(float, objectPosition, 4);
    float dz = CSF_AT(float, contextPosition, 8) -
               CSF_AT(float, objectPosition, 8);

    return threshold * threshold > dx * dx + dz * dz + dy * dy;
}

HOOK(0x004AA4D0, S_004aa4d0_IsWithinMovementThreshold, "auto");
