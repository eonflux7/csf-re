#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_008316d8_Fire, 0x00476D50, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_008317c8_DispatchFire(
    C_008317c8* self, void* /*edx*/,
    int a2, int a3, int a4, int a5,
    int a6, int a7, int a8, int a9)
{
    const int input[6] = {a5, a6, a7, a2, a3, a4};
    float firingVector[6];
    std::memcpy(firingVector, input, sizeof(firingVector));
    NormalizeVector3(firingVector, nullptr);

    bool useContextFlag;
    switch (CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x74))
    {
    case 6:
    case 0x10:
    case 0x11:
        useContextFlag = true;
        break;

    case 7:
    case 0xA:
    case 0x12:
    case 0x13:
    case 0x18:
    case 0x19:
        useContextFlag = false;
        break;

    case 8:
        return self->vSlotEC(firingVector, 0, 0);

    default:
        return C_008316d8_Fire(
            self, nullptr, a2, a3, a4, a5, a6, a7, a8, a9);
    }

    int result = static_cast<int32_t>(self->field_006c);
    if (result < 0)
        return result;

    result = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        static_cast<int>(reinterpret_cast<intptr_t>(
            reinterpret_cast<uint8_t*>(self) + 0x64)));
    if (!result)
        return result;

    if (self->vResolveEntryLookupValue())
    {
        void* entry = reinterpret_cast<void*>(self->vResolveEntryLookupValue());
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, entry, 0)[0x3C / 4])(entry, nullptr, 0);
    }

    if (useContextFlag)
    {
        return self->vSlotEC(
            firingVector, 1,
            static_cast<char>(CSF_AT(
                uint8_t, reinterpret_cast<void*>(a9), 0x31)));
    }

    return self->vSlotEC(firingVector, 1, 0);
}

HOOK(0x004786E0, C_008317c8_DispatchFire, "auto");
