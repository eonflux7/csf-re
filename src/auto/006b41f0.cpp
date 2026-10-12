#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x006237C0, (int a1, int (__cdecl* a2)(int, int), int a3));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetEntryWord, 0x006B87E0, (int objectIndex));
CSF_FUNC(int, __cdecl, GfxObject_IsContainerFrame, 0x006C03C0, (int a1));
CSF_FUNC(char, __fastcall, C_00861dd8_InitializeFromFrame, 0x006E7DA0, (void* self, void* edx, int a2, char* a3, int a4));
CSF_FUNC(uint32_t*, __fastcall, C_00861dd8_Construct, 0x006E7E60, (void* self, void* edx));

static int __cdecl GfxObject_CollectFrameObjects(int frame, S_0041e5f0* objects)
{
    if (GfxObject_IsContainerFrame(frame))
    {
        RwFrameForAllChildren(
            frame,
            reinterpret_cast<int (__cdecl*)(int, int)>(CSF_ADDR_FN(0x006B41F0)),
            reinterpret_cast<int>(objects));
        return frame;
    }

    auto* entry = reinterpret_cast<uint32_t*>(GfxObjectProps_GetEntryWord(frame));
    if (!entry)
        return frame;

    void* nameStorage = reinterpret_cast<void*>(entry[0]);
    if (!nameStorage || !CSF_AT(uint32_t, nameStorage, 0x14))
        return frame;

    void* allocation = Mem_Alloc(60);
    uint32_t* object = allocation
        ? C_00861dd8_Construct(allocation, nullptr)
        : nullptr;

    int property = static_cast<int>(entry[1]);
    nameStorage = reinterpret_cast<void*>(entry[0]);
    char* name;
    if (CSF_AT(uint32_t, nameStorage, 0x18) < 0x10u)
        name = reinterpret_cast<char*>(nameStorage) + 4;
    else
        name = CSF_AT(char*, nameStorage, 0x04);

    if (C_00861dd8_InitializeFromFrame(object, nullptr, frame, name, property))
    {
        if (static_cast<uint32_t>(objects->field_0008) >=
            static_cast<uint32_t>(objects->field_0004))
        {
            // Retain the binding identified as a vtable reference by the packet.
            (void)CSF_VTBL(0x0041E5F0);
            S_0041e5f0_GrowArrayCapacity(objects, nullptr, objects->field_000c);
        }

        uint32_t count = objects->field_0008;
        auto* slot = reinterpret_cast<uint32_t*>(objects->field_0000) + count;
        objects->field_0008 = count + 1;
        *slot = reinterpret_cast<uint32_t>(object);
    }
    else if (object)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0x04 / 4])(object, nullptr, 1);
    }

    return frame;
}

HOOK(0x006B41F0, GfxObject_CollectFrameObjects, "auto");
