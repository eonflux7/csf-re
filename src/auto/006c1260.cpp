#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, _rwDeviceSystemRequest, 0x006269C0, (int a1, int a2, uint32_t* a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RwStreamRead, 0x006293E0, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(int, __cdecl, RwStreamFindChunk, 0x00629AE0, (uint32_t* a1, int a2, uint32_t* a3, unsigned int* a4));
CSF_FUNC(int, __cdecl, RwTexDictionaryCreate, 0x00631650, ());
CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTextures, 0x006316E0, (int dictionary, int (__cdecl* callback)(uint32_t*, int), int context));
CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));
CSF_FUNC(int, __cdecl, RwTexDictionaryAddTexture, 0x006318E0, (void* dictionary, void* texture));
CSF_FUNC(char*, __cdecl, RwTexDictionaryFindNamedTexture, 0x00631950, (int dict, const char* name));
CSF_FUNC(int, __cdecl, RwTexDictionaryDestroy, 0x00632340, (int dictionary));
CSF_FUNC(int, __cdecl, _rwPluginRegistryReadDataChunks, 0x0063CB30, (int a1, uint32_t* a2, int a3));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

static int __stdcall GfxTexdict_ReadDictionaryForMerge(int streamAddress, int existingDictionary)
{
    auto* stream = reinterpret_cast<uint32_t*>(streamAddress);
    uint32_t chunkLength;
    unsigned int chunkVersion;

    if (!RwStreamFindChunk(stream, 1, &chunkLength, &chunkVersion)
        || chunkVersion < 0x35000
        || chunkVersion > 0x37002)
    {
        return 0;
    }

    auto* device = static_cast<uint8_t*>(g_pRwEngineInstance) + 16;
    uint32_t header = 0;
    if (static_cast<uint32_t>(RwStreamRead(stream, &header, chunkLength)) != chunkLength)
        return 0;

    uint32_t deviceId = 0;
    _rwDeviceSystemRequest(reinterpret_cast<int>(device), 22, &deviceId, 0, 0);
    deviceId = static_cast<uint8_t>(deviceId);

    const uint16_t dictionaryDeviceId = static_cast<uint16_t>(header >> 16);
    if (deviceId && dictionaryDeviceId && deviceId != dictionaryDeviceId)
        return 0;

    const int dictionary = RwTexDictionaryCreate();
    if (!dictionary)
        return 0;

    uint16_t texturesRemaining = static_cast<uint16_t>(header);
    int texture = existingDictionary;

    if (texturesRemaining)
    {
        do
        {
            --texturesRemaining;

            if (!RwStreamFindChunk(stream, 21, &chunkLength, &chunkVersion)
                || chunkVersion < 0x35000
                || chunkVersion > 0x37002)
            {
                goto failure;
            }

            using ReadNativeTexture = int (__cdecl*)(int, int*, int);
            if (!CSF_AT(ReadNativeTexture, g_pRwEngineInstance, 0xB0)(
                    streamAddress, &texture, static_cast<int>(chunkLength))
                || !texture
                || !_rwPluginRegistryReadDataChunks(
                    static_cast<int>(CSF_ADDR_DATA(0x008A6410)), stream, texture))
            {
                goto failure;
            }

            if (RwTexDictionaryFindNamedTexture(
                    existingDictionary, reinterpret_cast<const char*>(texture + 16)))
            {
                RwTextureDestroy(texture);
            }
            else
            {
                RwTexDictionaryAddTexture(
                    reinterpret_cast<void*>(dictionary),
                    reinterpret_cast<void*>(texture));
            }
        } while (texturesRemaining);
    }

    if (!_rwPluginRegistryReadDataChunks(
            static_cast<int>(CSF_ADDR_DATA(0x008A6428)), stream, dictionary))
    {
        goto failure;
    }

    return dictionary;

failure:
    RwTexDictionaryForAllTextures(
        dictionary,
        reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(
            CSF_ADDR_FN(0x006C0FD0)),
        0);
    RwTexDictionaryDestroy(dictionary);
    return 0;
}

HOOK(0x006C1260, GfxTexdict_ReadDictionaryForMerge, "auto");
