#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RwStreamWriteReal, 0x00629BE0, (uint32_t* a1, char* a2, unsigned int a3));
CSF_FUNC(uint32_t*, __cdecl, RwStreamWriteInt32, 0x00629C70, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, S_006abb80_ComputeAxisAlignedBounds, 0x006AC000, (S_006abb80* self, void* , float* maxX, float* minX, float* maxY, float* minY, float* maxZ, float* minZ));
CSF_FUNC(bool, __cdecl, GfxObjectProps_WritePropertyString, 0x006B90B0, (uint32_t* a1, int a2, int a3, int a4, int a5));

CSF_GLOBAL(int32_t, g_ObjectPropertyVersion, 0x008A7E18);
CSF_GLOBAL(uint8_t*, g_ObjectPropertyTable, 0x008BB478);

static uint32_t* __cdecl GfxObjectProps_WriteObjectProperties(
    uint32_t* stream, int /*unused*/, int objectOffset)
{
    uint8_t* object = CSF_AT(uint8_t*, g_ObjectPropertyTable, objectOffset + 4);

    int32_t version = g_ObjectPropertyVersion;
    RwStreamWriteInt32(stream, &version, 4);

    uint32_t* values = static_cast<uint32_t*>(Mem_Alloc(52));
    values[0] = CSF_AT(uint32_t, object, 0x00);

    uint8_t* properties = CSF_AT(uint8_t*, object, 0x10);
    values[1] = properties ? CSF_AT(uint8_t, properties, 0x0C) : 0;

    properties = CSF_AT(uint8_t*, object, 0x10);
    values[2] = properties ? CSF_AT(uint32_t, properties, 0x14) : 0;

    properties = CSF_AT(uint8_t*, object, 0x10);
    values[3] = properties ? CSF_AT(uint32_t, properties, 0x18) : 0;

    properties = CSF_AT(uint8_t*, object, 0x10);
    values[4] = properties ? CSF_AT(uint32_t, properties, 0x10) : 32;

    values[5] = (CSF_AT(uint32_t, object, 0x04) >> 6) & 1;
    values[6] = (CSF_AT(uint32_t, object, 0x04) >> 1) & 1;

    properties = CSF_AT(uint8_t*, object, 0x10);
    values[7] = properties ? CSF_AT(uint32_t, properties, 0x08) : -1;

    properties = CSF_AT(uint8_t*, object, 0x10);
    values[8] = properties ? CSF_AT(uint8_t, properties, 0x0D) : 2;

    values[9] = (CSF_AT(uint32_t, object, 0x04) >> 7) & 1;
    values[10] = (CSF_AT(uint32_t, object, 0x04) >> 8) & 1;
    values[11] = (CSF_AT(uint32_t, object, 0x04) >> 9) & 1;
    values[12] = CSF_AT(uint32_t, object, 0x18);

    RwStreamWriteInt32(stream, values, 52);
    Mem_Free(values);

    properties = CSF_AT(uint8_t*, object, 0x10);
    uint32_t realBits = properties ? CSF_AT(uint32_t, properties, 0x1C) : 0;
    RwStreamWriteReal(stream, reinterpret_cast<char*>(&realBits), 4);

    properties = CSF_AT(uint8_t*, object, 0x10);
    realBits = properties ? CSF_AT(uint32_t, properties, 0x20) : 0;
    RwStreamWriteReal(stream, reinterpret_cast<char*>(&realBits), 4);

    properties = CSF_AT(uint8_t*, object, 0x10);
    realBits = properties ? CSF_AT(uint32_t, properties, 0x24) : 0;
    RwStreamWriteReal(stream, reinterpret_cast<char*>(&realBits), 4);

    properties = CSF_AT(uint8_t*, object, 0x10);
    realBits = properties ? CSF_AT(uint32_t, properties, 0x28) : 0;
    RwStreamWriteReal(stream, reinterpret_cast<char*>(&realBits), 4);

    properties = CSF_AT(uint8_t*, object, 0x10);
    realBits = properties ? CSF_AT(uint32_t, properties, 0x2C) : 0;
    RwStreamWriteReal(stream, reinterpret_cast<char*>(&realBits), 4);

    properties = CSF_AT(uint8_t*, object, 0x10);
    int stringValue = properties ? CSF_AT(int, properties, 0x00) : 0;
    GfxObjectProps_WritePropertyString(stream, stringValue, 0, 0, 0);

    properties = CSF_AT(uint8_t*, object, 0x10);
    stringValue = properties ? CSF_AT(int, properties, 0x04) : 0;
    GfxObjectProps_WritePropertyString(stream, stringValue, 0, 0, 0);

    int32_t hasBounds = CSF_AT(uint32_t, object, 0x14) != 0;
    RwStreamWriteInt32(stream, &hasBounds, 4);

    S_006abb80* boundsObject = CSF_AT(S_006abb80*, object, 0x14);
    if (boundsObject)
    {
        float maxX, minX, maxY, minY, maxZ, minZ;
        S_006abb80_ComputeAxisAlignedBounds(
            boundsObject, nullptr, &maxX, &minX, &maxY, &minY, &maxZ, &minZ);

        float bounds[6];
        bounds[0] = minX;
        bounds[3] = maxY;
        bounds[1] = maxX;
        bounds[2] = minY;
        bounds[4] = minZ;
        bounds[5] = maxZ;
        RwStreamWriteReal(stream, reinterpret_cast<char*>(bounds), 24);
    }

    return stream;
}

HOOK(0x006B9190, GfxObjectProps_WriteObjectProperties, "auto");
