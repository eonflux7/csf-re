#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CollisionShapes_GetElementCount, 0x006CDF00, (void* self, void*));
CSF_FUNC(int, __fastcall, GfxCollisionShapes_GetRecordValue, 0x006CDF90, (void* self, void* , int index));
CSF_FUNC(uint8_t, __fastcall, ClumpLoader_RefreshSceneCapacityIfEnabled, 0x006FE840, (void* self, void*));

static int __fastcall Gfx2d_CollectWorldObjectReferences(void* self, void* /*edx*/)
{
    // The packet annotates this call target as a vtable reference;
    // the disassembly contains no corresponding vtable write.
    (void)CSF_VTBL(0x0041E5F0);

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(global, nullptr));
    void* collection = reinterpret_cast<void*>(
        SceneLoader_GetWorldObjectCollection(accessor, nullptr));

    for (uint32_t index = 0;
         index < static_cast<uint32_t>(
             CollisionShapes_GetElementCount(collection, nullptr));
         ++index)
    {
        auto* record = reinterpret_cast<C_0085ec18*>(
            GfxCollisionShapes_GetRecordValue(
                collection, nullptr, static_cast<int>(index)));

        if (record &&
            record->vGetReferencedObject() == reinterpret_cast<void*>(1))
        {
            void* entry = Mem_Alloc(20);
            if (entry)
            {
                CSF_AT(uint32_t, entry, 0x04) = 0;
                CSF_AT(uint32_t, entry, 0x08) = 0;
                CSF_AT(uint32_t, entry, 0x0C) = 0;
                CSF_AT(uint32_t, entry, 0x10) = 10;
            }

            void* referencedObject = GetReferencedObject(record, nullptr);
            CSF_AT(void*, entry, 0x00) = referencedObject;

            if (CSF_AT(uint32_t, self, 0x6C) >=
                CSF_AT(uint32_t, self, 0x68))
            {
                S_0041e5f0_GrowArrayCapacity(
                    reinterpret_cast<S_0041e5f0*>(
                        static_cast<uint8_t*>(self) + 0x64),
                    nullptr,
                    CSF_AT(int, self, 0x70));
            }

            uint32_t count = CSF_AT(uint32_t, self, 0x6C);
            void** destination =
                CSF_AT(void**, self, 0x64) + count;
            CSF_AT(uint32_t, self, 0x6C) = count + 1;
            *destination = entry;
        }
    }

    return ClumpLoader_RefreshSceneCapacityIfEnabled(
        static_cast<uint8_t*>(self) + 0x74, nullptr);
}

HOOK(0x006D6C00, Gfx2d_CollectWorldObjectReferences, "auto");
