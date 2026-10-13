#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, TextDatabase_TestHeight, 0x00770470, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, TextDatabase_TestHorizontalPosition, 0x00770760, (void* self, void* edx, float a2, float a3));

static char __stdcall TextDatabase_TestObjectPosition(S_00770760* region, int handle)
{
    // Retain the vtable binding identified by the packet; no vtable write occurs.
    (void)CSF_VTBL(0x00770760);

    auto* object = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    if (!object || (object->field_0170 & 1) == 0)
        return 0;

    void* position = GetObjectPositionVector(object, nullptr);
    const float z = CSF_AT(float, position, 0x8);
    const float y = CSF_AT(float, position, 0x4);
    const float x = CSF_AT(float, position, 0x0);

    void* relatedObject = C_0082dd78_DispatchReturnedObject(
        reinterpret_cast<void*>(object->field_004c), nullptr);
    void* dimensions =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, relatedObject, 0x0)[0x30 / 4])(
                relatedObject, nullptr);
    const float height = CSF_AT(float, dimensions, 0x4);

    if (static_cast<uint8_t>(TextDatabase_TestHeight(region, nullptr, y)))
        return TextDatabase_TestHorizontalPosition(region, nullptr, x, z);

    if (static_cast<uint8_t>(
            TextDatabase_TestHeight(region, nullptr, y + height)))
        return TextDatabase_TestHorizontalPosition(region, nullptr, x, z);

    if (static_cast<uint8_t>(TextDatabase_TestHeight(
            region, nullptr, height * g_HalfDurationScale + y)))
        return TextDatabase_TestHorizontalPosition(region, nullptr, x, z);

    return 0;
}

HOOK(0x004EF080, TextDatabase_TestObjectPosition, "auto");
