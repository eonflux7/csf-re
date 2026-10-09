#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __stdcall, CScene_AddObjectToList, 0x00450F50,
         (int a1, void* a2));

static int __fastcall CScene_AddObjectFlags(
    CScene* self, void* /*edx*/, int handleAddress, int flags)
{
    const void* handle = reinterpret_cast<const void*>(handleAddress);
    const int index = CSF_AT(int, handle, 0x08);
    void* object = nullptr;

    if (index >= 0 && index < CSF_AT(int, self, 0x18))
    {
        object = CSF_AT(void**, self, 0x10)[index];
        if (object && CSF_AT(int, object, 0x20) != CSF_AT(int, handle, 0x04))
            object = nullptr;
    }

    if ((flags & 1) != 0)
    {
        void* component =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, component, 0)[0x34 / 4])(component, nullptr, 1);
        CScene_AddObjectToList(
            reinterpret_cast<int>(object),
            reinterpret_cast<std::uint8_t*>(self) + 0x20);
    }

    if ((flags & 2) != 0)
    {
        void* component =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, component, 0)[0x34 / 4])(component, nullptr, 0);
        CScene_AddObjectToList(
            reinterpret_cast<int>(object),
            reinterpret_cast<std::uint8_t*>(self) + 0x30);
    }

    if ((flags & 0x10) != 0)
        CScene_AddObjectToList(
            reinterpret_cast<int>(object),
            reinterpret_cast<std::uint8_t*>(self) + 0x40);

    if ((flags & 4) != 0)
        CScene_AddObjectToList(
            reinterpret_cast<int>(object),
            reinterpret_cast<std::uint8_t*>(self) + 0x60);

    if ((flags & 8) != 0)
        CScene_AddObjectToList(
            reinterpret_cast<int>(object),
            reinterpret_cast<std::uint8_t*>(self) + 0x98);

    if ((flags & 0x20) != 0)
        CScene_AddObjectToList(
            reinterpret_cast<int>(object),
            reinterpret_cast<std::uint8_t*>(self) + 0x50);

    const int result = flags | CSF_AT(int, object, 0x94);
    CSF_AT(int, object, 0x94) = result;
    return result;
}

HOOK(0x00452800, CScene_AddObjectFlags, "auto");
