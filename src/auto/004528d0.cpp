#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, CScene_RemoveObjectFromList, 0x0044FC80,
         (int a1, uint32_t* a2));
CSF_FUNC(void, __fastcall, CScene_RemoveObjectFromGroup, 0x00450EE0,
         (void* self, void* edx, int a2));

static int __fastcall CScene_RemoveObjectFromCategories(
    C_008312b0* self, void* /*edx*/, int handleAddress, int categoryMask)
{
    const void* handle = reinterpret_cast<const void*>(handleAddress);
    const int index = CSF_AT(int, handle, 0x8);
    void* object = nullptr;

    if (index >= 0 && index < CSF_AT(int, self, 0x18))
    {
        object = CSF_AT(void**, self, 0x10)[index];
        if (object && CSF_AT(int, object, 0x20) != CSF_AT(int, handle, 0x4))
            object = nullptr;
    }

    const int objectAddress = reinterpret_cast<int>(object);
    auto* sceneBytes = reinterpret_cast<uint8_t*>(self);

    if (categoryMask & 1)
        CScene_RemoveObjectFromList(
            objectAddress, reinterpret_cast<uint32_t*>(sceneBytes + 0x20));
    if (categoryMask & 2)
        CScene_RemoveObjectFromList(
            objectAddress, reinterpret_cast<uint32_t*>(sceneBytes + 0x30));
    if (categoryMask & 0x10)
        CScene_RemoveObjectFromList(
            objectAddress, reinterpret_cast<uint32_t*>(sceneBytes + 0x40));
    if (categoryMask & 4)
        CScene_RemoveObjectFromGroup(self, nullptr, objectAddress);
    if (categoryMask & 8)
        CScene_RemoveObjectFromList(
            objectAddress, reinterpret_cast<uint32_t*>(sceneBytes + 0x98));
    if (categoryMask & 0x20)
        CScene_RemoveObjectFromList(
            objectAddress, reinterpret_cast<uint32_t*>(sceneBytes + 0x50));

    const int remainingFlags = ~categoryMask & CSF_AT(int, object, 0x94);
    CSF_AT(int, object, 0x94) = remainingFlags;
    return remainingFlags;
}

HOOK(0x004528D0, CScene_RemoveObjectFromCategories, "auto");
