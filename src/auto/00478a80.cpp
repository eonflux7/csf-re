#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, WeaponFirearms_UpdateBase, 0x00475C30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_008317c8_UpdateLinkedObjects(
    C_008317c8* self, void* /*edx*/, int updateValue)
{
    WeaponFirearms_UpdateBase(self, nullptr, updateValue);

    void* linkedObject = reinterpret_cast<void*>(self->field_0070);
    if (linkedObject)
    {
        int entry = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, linkedObject, 0)[0xA8 / 4])(linkedObject, nullptr);
        if (entry)
        {
            void* object = reinterpret_cast<void*>(entry);
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr, updateValue);
        }
    }

    int result = static_cast<int32_t>(self->field_006c);
    if (result >= 0)
    {
        result = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x64));
        if (result)
        {
            void* resolvedObject = reinterpret_cast<void*>(result);
            result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, resolvedObject, 0)[0x28 / 4])(resolvedObject, nullptr);
            if (result)
            {
                void* object = reinterpret_cast<void*>(result);
                return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr, updateValue);
            }
        }
    }
    return result;
}

HOOK(0x00478A80, C_008317c8_UpdateLinkedObjects, "auto");
