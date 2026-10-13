#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectPropertyWord, 0x006B8800, (int objectIndex));
CSF_FUNC(int, __fastcall, GfxLights_UpdateTypeMembership, 0x006D2030, (void* self, void* , int object, int oldType, int newType));

// Sets the light's type code and, when it changes, updates type membership and the
// property word of the underlying RpLight. Returns the previous type (or the property
// word pointer when the update path runs).
static int* __fastcall CLightObject_SetLightTypeAndUpdateMembership(CLightObject* self, void* /*edx*/, int newType)
{
    int currentType = self->vSlot1C();
    int* result = reinterpret_cast<int*>(currentType);

    if (currentType != newType)
    {
        GfxLights_UpdateTypeMembership(CSF_AT(void*, self, 0x8), nullptr, reinterpret_cast<int>(self), currentType, newType);

        result = reinterpret_cast<int*>(GfxObjectProps_GetObjectPropertyWord(static_cast<int>(self->m_pLight)));
        if (result)
        {
            switch (newType)
            {
            case 0:
                *result = 2;
                break;
            case 1:
                *result = 0;
                break;
            case 2:
                *result = 1;
                break;
            case 3:
                *result = 3;
                break;
            default:
                *result = newType;
                break;
            }
        }
    }
    return result;
}

HOOK(0x006FCF50, CLightObject_SetLightTypeAndUpdateMembership, "auto");
