#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RwStreamReadChunkHeaderInfo, 0x00629CE0, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

CSF_FUNC(char, __cdecl, CollisionShapes_ConfigureScaledShape, 0x006B2000, (int a1, void* String2, float* a3));
CSF_FUNC(char, __cdecl, CollisionShapes_ConfigureNamedShape, 0x006B2180, (int a1, void* String2));
CSF_FUNC(char, __cdecl, CollisionShapes_ConfigureAlternateScaledShape, 0x006B2240, (int a1, void* String2, float* a3));

CSF_FUNC(int, __fastcall, C_00861a78_ConstructShapeWrapper, 0x006E3DA0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_00861a78_ConfigureFromEntry, 0x006E3E30, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00861b40_ConstructShapeWrapper, 0x006E3FC0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_00861b40_ConfigureFromEntry, 0x006E4070, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, C_00861b58_ConfigureFromEntry, 0x006E41B0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_00861b58_ConstructShapeWrapper, 0x006E4200, (void* self, void* edx, int a2));

CSF_FUNC(double, __cdecl, CollisionShapes_ApplyShapeParameter, 0x007FA460, (int a1, float a2, int a3));
CSF_FUNC(int, __cdecl, CollisionShapes_ConfigureDefaultShape, 0x007FA5B0, (int a1));
CSF_FUNC(uint32_t*, __cdecl, CollisionShapes_CreateShape, 0x0081B990, ());
CSF_FUNC(uint32_t*, __cdecl, CollisionShapes_ReadShapeChunk, 0x0081BA20, (uint32_t* a1));
CSF_FUNC(int, __cdecl, CollisionShapes_ReadShapeVariantBChunk, 0x0081C0B0, (uint32_t* a4));
CSF_FUNC(int, __cdecl, CollisionShapes_ReadShapeVariantAChunk, 0x0081CA00, (uint32_t* a4));

static uint32_t* __cdecl CollisionShapes_InitShapeEntry(uint32_t* entry)
{
    uint32_t* wrapper = nullptr;

    if (CSF_AT(uint32_t, entry, 0) == 3)
    {
        void* filename = CSF_AT(uint32_t, entry, 0x1C) < 0x10u
            ? static_cast<void*>(reinterpret_cast<uint8_t*>(entry) + 8)
            : CSF_AT(void*, entry, 8);

        uint32_t* stream = RwStreamOpen(2, 1, reinterpret_cast<int>(filename));
        if (!stream)
            return stream;

        alignas(8) uint8_t headerBytes[0x14];
        auto* header = reinterpret_cast<uint32_t*>(headerBytes);

        if (RwStreamReadChunkHeaderInfo(stream, header))
        {
            switch (header[0])
            {
            case 2311:
            {
                uint32_t* shape = CollisionShapes_ReadShapeChunk(stream);
                void* storage = Mem_Alloc(16);
                if (storage)
                {
                    wrapper = reinterpret_cast<uint32_t*>(
                        C_00861a78_ConstructShapeWrapper(
                            storage, nullptr, reinterpret_cast<int>(shape)));
                }

                if (!C_00861a78_ConfigureFromEntry(wrapper, nullptr, entry))
                {
                    if (wrapper)
                    {
                        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                            CSF_AT(void**, wrapper, 0)[1])(wrapper, nullptr, 1);
                        wrapper = nullptr;
                    }
                }
                break;
            }

            case 2313:
            {
                int shape = CollisionShapes_ReadShapeVariantAChunk(stream);
                void* storage = Mem_Alloc(8);
                if (storage)
                    wrapper = C_00861b40_ConstructShapeWrapper(storage, nullptr, shape);

                if (!C_00861b40_ConfigureFromEntry(wrapper, nullptr, entry) && wrapper)
                {
                    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, wrapper, 0)[1])(wrapper, nullptr, 1);
                    RwStreamClose(stream, nullptr);
                    return nullptr;
                }
                break;
            }

            case 2315:
            {
                int shape = CollisionShapes_ReadShapeVariantBChunk(stream);
                void* storage = Mem_Alloc(8);
                if (storage)
                    wrapper = C_00861b58_ConstructShapeWrapper(storage, nullptr, shape);

                if (!C_00861b58_ConfigureFromEntry(
                        wrapper, nullptr, reinterpret_cast<int>(entry)))
                {
                    if (wrapper)
                    {
                        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                            CSF_AT(void**, wrapper, 0)[1])(wrapper, nullptr, 1);
                        wrapper = nullptr;
                    }
                }
                break;
            }

            default:
                break;
            }
        }

        RwStreamClose(stream, nullptr);
        return wrapper;
    }

    uint32_t* shape = CollisionShapes_CreateShape();
    int shapeData = CSF_AT(int, shape, 0);

    switch (CSF_AT(uint32_t, entry, 0))
    {
    case 0:
        CollisionShapes_ConfigureDefaultShape(shapeData);
        break;

    case 1:
    {
        void* name = CSF_AT(uint32_t, entry, 0x38) < 0x10u
            ? static_cast<void*>(reinterpret_cast<uint8_t*>(entry) + 0x24)
            : CSF_AT(void*, entry, 0x24);
        CollisionShapes_ConfigureNamedShape(shapeData, name);
        break;
    }

    case 2:
    {
        void* name = CSF_AT(uint32_t, entry, 0x38) < 0x10u
            ? static_cast<void*>(reinterpret_cast<uint8_t*>(entry) + 0x24)
            : CSF_AT(void*, entry, 0x24);
        CollisionShapes_ConfigureScaledShape(
            shapeData, name, reinterpret_cast<float*>(
                reinterpret_cast<uint8_t*>(entry) + 0x3C));
        break;
    }

    case 4:
    {
        void* name = CSF_AT(uint32_t, entry, 0x38) < 0x10u
            ? static_cast<void*>(reinterpret_cast<uint8_t*>(entry) + 0x24)
            : CSF_AT(void*, entry, 0x24);
        CollisionShapes_ConfigureAlternateScaledShape(
            shapeData, name, reinterpret_cast<float*>(
                reinterpret_cast<uint8_t*>(entry) + 0x3C));
        break;
    }

    default:
        break;
    }

    CollisionShapes_ApplyShapeParameter(
        reinterpret_cast<int>(shape),
        CSF_AT(float, entry, 0x54),
        CSF_AT(uint32_t, entry, 0) != 1);

    void* storage = Mem_Alloc(16);
    if (storage)
    {
        wrapper = reinterpret_cast<uint32_t*>(
            C_00861a78_ConstructShapeWrapper(
                storage, nullptr, reinterpret_cast<int>(shape)));
    }

    C_00861a78_ConfigureFromEntry(wrapper, nullptr, entry);
    return wrapper;
}

HOOK(0x006B23E0, CollisionShapes_InitShapeEntry, "auto");
