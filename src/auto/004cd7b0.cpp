#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

[[maybe_unused]] static const auto kHandleAccessorVtable = CSF_VTBL(0x00492B20);

static int __fastcall C_0083ab58_UpdateSearchTarget(
    C_0083ab58* self, void* /*edx*/, int /*argument*/)
{
    alignas(8) uint8_t handleBytes[0x10];

    void* owner = reinterpret_cast<void*>(self->field_0014);
    int result = CSF_AT(int, owner, 0x38);
    CSF_AT(C_0083ab58*, handleBytes, 0x0C) = self;
    if (result == 0)
        return result;

    void* sourceHandle = reinterpret_cast<void*>(result);
    CSF_AT(void**, handleBytes, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, handleBytes, 0x04) =
        CSF_AT(uint32_t, sourceHandle, 0x24);
    result = CSF_AT(int, sourceHandle, 0x28);
    CSF_AT(int, handleBytes, 0x08) = result;
    if (result < 0)
        return result;

    result = CHandleTable_Resolve(
        g_pCurrentScene, nullptr, reinterpret_cast<int>(handleBytes));
    if (result == 0)
        return result;

    void* target = reinterpret_cast<void*>(result);
    result = SceneObj_IsInactive(target, nullptr);
    if (static_cast<uint8_t>(result) != 0)
        return result;
    if (CSF_AT(int, target, 0x50) != 1)
        return result;

    owner = reinterpret_cast<void*>(self->field_0014);
    result = IsField1BCNonNegative(owner, nullptr);
    if (static_cast<uint8_t>(result) != 0)
    {
        void* scene = g_pCurrentScene;
        char* relatedHandle = S_00492b20_GetPtr0x1B4(owner, nullptr);
        result = CHandleTable_Resolve(
            scene, nullptr, reinterpret_cast<int>(relatedHandle));
        if (result == 0)
            return result;

        void* relatedObject = reinterpret_cast<void*>(result);
        if (CSF_AT(int, relatedObject, 0x50) != 19)
            return result;

        void* innerObject = CSF_AT(void*, relatedObject, 0x34);
        result =
            ((int(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, innerObject, 0x00)[0xDC / 4])(
                innerObject, nullptr,
                reinterpret_cast<uint8_t*>(target) + 0x1C);
        if (static_cast<uint8_t>(result) == 0)
            return result;

        self = CSF_AT(C_0083ab58*, handleBytes, 0x0C);
    }

    self->field_0048 = CSF_AT(uint32_t, handleBytes, 0x04);
    self->field_004c = CSF_AT(uint32_t, handleBytes, 0x08);
    self->field_0040 = 2;
    return result;
}

HOOK(0x004CD7B0, C_0083ab58_UpdateSearchTarget, "auto");
