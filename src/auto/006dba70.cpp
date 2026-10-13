#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __cdecl, RwImageCreate, 0x00625060, (int a1, int a2, int a3));
CSF_FUNC(uint32_t*, __cdecl, RwImageAllocatePixels, 0x006250C0, (uint32_t* a1));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));

static int __stdcall GfxRenderEngine_LoadRgbImage(const char* filename)
{
    if (!filename || !*filename)
        return 0;

    void* file = CoreFiles_OpenSceneFile(
        filename, const_cast<char*>(CSF_STR(0x008579F8, "rb")), 0);
    if (!file)
        return 0;

    const int fileSize =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, file, 0)[0x1C / 4])(file, nullptr);

    void* buffer = Mem_Alloc(static_cast<std::size_t>(fileSize));

    ((void(__fastcall*)(void*, void*, void*, int))
        CSF_AT(void**, file, 0)[8 / 4])(file, nullptr, buffer, fileSize);

    const int width = CSF_AT(int, buffer, 0);
    const int height = CSF_AT(int, buffer, 4);
    uint32_t* image = nullptr;

    if (width > 0 && height > 0)
    {
        image = RwImageCreate(width, height, 32);
        if (image)
        {
            RwImageAllocatePixels(image);

            std::uint8_t* destination =
                CSF_AT(std::uint8_t*, image, 0x14);
            const std::uint8_t* source =
                static_cast<const std::uint8_t*>(buffer) + 8;

            int rowsRemaining = height;
            do
            {
                int pixelsRemaining = width;
                if (pixelsRemaining > 0)
                {
                    do
                    {
                        destination[0] = source[0];
                        destination[1] = source[1];
                        destination[2] = source[2];
                        destination[3] = 0xFF;
                        source += 3;
                        destination += 4;
                    } while (--pixelsRemaining);
                }
            } while (--rowsRemaining);
        }
    }

    CloseSceneFile(file);
    Mem_Free(buffer);
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(image));
}

HOOK(0x006DBA70, GfxRenderEngine_LoadRgbImage, "auto");
