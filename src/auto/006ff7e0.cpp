#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, SceneLoader_CreateIndexedGeometry, 0x006B1DE0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, Rt2dSceneGetChildCount, 0x008013C0, (int a1));
CSF_FUNC(int, __cdecl, Rt2dSceneGetChild, 0x008013D0, (uint32_t* a1, int a2));
CSF_FUNC(int, __cdecl, Rt2dClumpGetClump, 0x00801520, (int a1));

static C_008656d0* __fastcall S_006ff7e0_CreateIndexedGeometry(
    S_006ff7e0* self, void* /*edx*/)
{
    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
    const uint16_t objectCount = CSF_AT(
        uint16_t, reinterpret_cast<void*>(self->field_0004), 0x0C);

    uint32_t entryOffset = 0;
    for (uint32_t remaining = objectCount; remaining != 0; --remaining)
    {
        void* entries = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0004), 0x10);
        void* object = CSF_AT(void*, entries, entryOffset);
        void* typeData = CSF_AT(void*, object, 0x5C);
        const int16_t type = CSF_AT(int16_t, typeData, 0);

        switch (type)
        {
        case 5:
        case 3:
        case 6:
            vertexCount += 8;
            indexCount += 24;
            break;

        case 2:
            vertexCount += 96;
            indexCount += 192;
            break;

        case 8:
        {
            const int clump = Rt2dClumpGetClump(
                reinterpret_cast<int>(object));
            const uint32_t childCount =
                static_cast<uint32_t>(Rt2dSceneGetChildCount(clump));

            for (uint32_t childIndex = 0; childIndex < childCount; ++childIndex)
            {
                void* child = reinterpret_cast<void*>(Rt2dSceneGetChild(
                    reinterpret_cast<uint32_t*>(clump),
                    static_cast<int>(childIndex)));
                void* childTypeData = CSF_AT(void*, child, 0x5C);
                const int16_t childType = CSF_AT(int16_t, childTypeData, 0);

                if (childType == 5 || childType == 3 || childType == 6)
                {
                    vertexCount += 8;
                    indexCount += 24;
                }
                else if (childType == 2)
                {
                    vertexCount += 96;
                    indexCount += 192;
                }
            }
            break;
        }
        }

        entryOffset += 12;
    }

    if (vertexCount == 0 || indexCount == 0)
    {
        vertexCount = 1;
        indexCount = 1;
    }

    void* loader = reinterpret_cast<void*>(GetGlobalInt());
    C_008656d0* geometry = reinterpret_cast<C_008656d0*>(
        SceneLoader_CreateIndexedGeometry(
            loader, nullptr,
            static_cast<int>(vertexCount), static_cast<int>(indexCount)));

    self->field_0024 = reinterpret_cast<uint32_t>(geometry);
    return geometry;
}

HOOK(0x006FF7E0, S_006ff7e0_CreateIndexedGeometry, "auto");
