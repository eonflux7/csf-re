#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall DynamicObj_ResetSceneService(void* self, void* /*edx*/)
{
    uint32_t groupIndex = 0;
    uint32_t groupOffset = 0;

    for (;;)
    {
        uint8_t* groups = CSF_AT(uint8_t*, self, 0x4);
        if (!groups)
            break;

        const uint32_t byteCount =
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(
                CSF_AT(uint8_t*, self, 0x8))) -
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(groups));
        const uint32_t groupCount =
            static_cast<uint32_t>(static_cast<int32_t>(byteCount) >> 4);
        if (groupIndex >= groupCount)
            break;

        uint32_t objectIndex = 0;
        if (CSF_AT(uint32_t, groups + groupOffset, 0x8))
        {
            do
            {
                groups = CSF_AT(uint8_t*, self, 0x4);
                void* objects = CSF_AT(void*, groups + groupOffset, 0);
                if (CSF_AT(void*, objects, objectIndex * 4))
                {
                    void* object = CSF_AT(void*, objects, objectIndex * 4);
                    if (object)
                    {
                        ((void(__fastcall*)(void*, void*, int))
                            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
                    }

                    groups = CSF_AT(uint8_t*, self, 0x4);
                    objects = CSF_AT(void*, groups + groupOffset, 0);
                    CSF_AT(void*, objects, objectIndex * 4) = nullptr;
                }

                groups = CSF_AT(uint8_t*, self, 0x4);
                const uint32_t objectCount =
                    CSF_AT(uint32_t, groups + groupOffset, 0x8);
                ++objectIndex;
                if (objectIndex >= objectCount)
                    break;
            } while (true);
        }

        ++groupIndex;
        groupOffset += 16;
    }

    uint8_t* group = CSF_AT(uint8_t*, self, 0x4);
    if (group)
    {
        uint8_t* end = CSF_AT(uint8_t*, self, 0x8);
        for (; group != end; group += 16)
        {
            void* objects = CSF_AT(void*, group, 0);
            if (objects)
            {
                Mem_Free(objects);
                CSF_AT(void*, group, 0) = nullptr;
            }
        }

        void* groups = CSF_AT(void*, self, 0x4);
        if (groups)
            Mem_FreeDebug(groups);
    }

    CSF_AT(void*, self, 0x4) = nullptr;
    CSF_AT(void*, self, 0x8) = nullptr;
    CSF_AT(void*, self, 0xC) = nullptr;
    return 0;
}

HOOK(0x00442130, DynamicObj_ResetSceneService, "auto");
