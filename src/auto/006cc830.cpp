#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, S_006fdfe0_FindWorldFromClump, 0x0058DF40, (int a1));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, C_008656d0_ProcessObject, 0x006C66F0, (C_008656d0* self, void* , C_0085e7b8* object));

static const auto g_ArrayCapacityVtable = CSF_VTBL(0x0041E5F0);

static int __fastcall S_006cc4f0_ProcessSceneObjects(
    S_006cc4f0* self, void* /*edx*/, uint32_t* objects)
{
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(objects));
    const uint32_t objectCount = objects[2];

    for (uint32_t index = 0; index < objectCount; ++index)
    {
        auto* object = reinterpret_cast<C_0085e7b8**>(objects[0])[index];
        if (!object)
            continue;

        result = ((bool(__fastcall*)(void*, void*))object->vtbl[0x38 / 4])(
            object, nullptr);
        if (!result)
            continue;

        void* provider = GetCachedObjectProvider(object, nullptr);
        if (provider &&
            S_006fdfe0_FindWorldFromClump(
                static_cast<int>(reinterpret_cast<uintptr_t>(provider))))
        {
            auto* processor = reinterpret_cast<C_008656d0*>(
                GetGlobalValue_6C5760());
            if (C_008656d0_ProcessObject(processor, nullptr, object))
            {
                if (self->field_00d0 >= CSF_AT(uint32_t, self, 0xCC))
                {
                    S_0041e5f0_GrowArrayCapacity(
                        reinterpret_cast<S_0041e5f0*>(
                            reinterpret_cast<uint8_t*>(self) + 0xC8),
                        nullptr,
                        CSF_AT(int, self, 0xD4));
                }

                const uint32_t count = self->field_00d0;
                auto* destination =
                    reinterpret_cast<C_0085e7b8**>(
                        CSF_AT(uint32_t, self, 0xC8)) + count;
                self->field_00d0 = count + 1;
                *destination = object;
            }
        }

        result = static_cast<int>(reinterpret_cast<uintptr_t>(self));
        ++self->field_00e8;
    }

    return result;
}

HOOK(0x006CC830, S_006cc4f0_ProcessSceneObjects, "auto");
