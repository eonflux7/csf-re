#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AllocateAlignedBuffer, 0x00690EB0, (std::size_t size, int alignment, int option1, int option2));
CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x00690F70, (void* block));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, OpenSceneFile, 0x00698350, (const char* path, const char* mode, int flags));
CSF_FUNC(bool, __cdecl, MenuManager_CheckPageAvailability, 0x00698B20, (const char* name, char mode));
CSF_FUNC(char, __cdecl, Audio_ReadWaveHeader, 0x00712B70, (int* a1, uint32_t* a2));
CSF_FUNC(char*, __cdecl, __strupr, 0x007B302E, (char* String));
CSF_FUNC(int, __cdecl, Audio_SetSoundIdentity, 0x007DFBD0, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, Audio_SetSoundData, 0x007DFF20, (int sound, int offset, void* data, std::size_t size));
CSF_FUNC(int, __cdecl, Audio_PollSoundLoad, 0x007E0170, (int sound));
CSF_FUNC(unsigned int, __cdecl, RtGUIDGenerate, 0x007E0D10, (unsigned int* a1));
CSF_FUNC(int, __cdecl, Audio_CreateSound, 0x007F1850, (int a1, int a2, uint32_t* a3, char a4, int a5, int a6, int a7, int a8, int a9));

static const auto* const g_AudioRates =
    reinterpret_cast<const std::uint32_t*>(CSF_ADDR_DATA(0x008A85A8));
static const auto* const g_AudioFormats =
    reinterpret_cast<const std::uint8_t*>(CSF_ADDR_DATA(0x008A85B0));
static const auto* const g_AudioFormatData =
    reinterpret_cast<const std::uint32_t*>(CSF_ADDR_DATA(0x008A85B4));

static int __fastcall C_00863ba8_LoadSoundFile(
    C_00863ba8* self, void* /*edx*/, const char* path,
    const char* name, char forceMono, int quality)
{
    if (!path || !*path)
        return 0;

    if (self->field_0031) {
        MenuManager_CheckPageAvailability(path, 0);
        return 0;
    }

    alignas(8) std::uint8_t header[0x30]{};
    alignas(8) std::uint8_t sourceFormat[0x1C]{};
    alignas(8) std::uint8_t targetFormat[0x20]{};
    CSF_AT(C_00863ba8*, targetFormat, 0x1C) = self;

    void* data = nullptr;
    std::size_t dataSize = 0;
    void* file = OpenSceneFile(path, CSF_STR(0x008579F8, "rb"), 0);
    if (file) {
        if (Audio_ReadWaveHeader(reinterpret_cast<int*>(file),
                                 reinterpret_cast<uint32_t*>(header)) &&
            CSF_AT(std::uint16_t, header, 0x22) == 16) {
            dataSize = CSF_AT(std::size_t, header, 0x28);
            data = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(
                    AllocateAlignedBuffer(dataSize, 64, 0, 0)));
            ((void(__fastcall*)(void*, void*, void*, std::size_t))
                CSF_AT(void**, file, 0)[8 / 4])(
                    file, nullptr, data, dataSize);
        }
        CloseSceneFile(file);
    }

    if (!data) {
        int result = CSF_AT(int, header, 0x2C);
        CSF_AT(C_00863ba8*, targetFormat, 0x1C)->field_0030 = 1;
        return result;
    }

    CSF_AT(std::uint8_t, sourceFormat, 0x0D) =
        CSF_AT(std::uint8_t, header, 0x16);
    CSF_AT(std::size_t, sourceFormat, 0x08) = dataSize;
    CSF_AT(std::size_t, targetFormat, 0x08) = dataSize;
    CSF_AT(std::uint32_t, sourceFormat, 0x00) =
        CSF_AT(std::uint32_t, header, 0x18);
    CSF_AT(std::uint8_t, sourceFormat, 0x0C) =
        CSF_AT(std::uint8_t, header, 0x22);
    CSF_AT(std::uintptr_t, sourceFormat, 0x04) =
        reinterpret_cast<std::uintptr_t>(CSF_ADDR_DATA(0x00883C0C));

    if (forceMono ||
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0xA8 / 4])(self, nullptr) == 1) {
        CSF_AT(std::uint8_t, targetFormat, 0x0D) = 1;
    } else {
        CSF_AT(std::uint8_t, targetFormat, 0x0D) =
            CSF_AT(std::uint8_t, sourceFormat, 0x0D);
    }

    int deviceMode = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0xA8 / 4])(self, nullptr);
    CSF_AT(std::uint32_t, targetFormat, 0x00) =
        g_AudioRates[4 * deviceMode + quality];

    deviceMode = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0xA8 / 4])(self, nullptr);
    CSF_AT(std::uint8_t, targetFormat, 0x0C) =
        g_AudioFormats[16 * deviceMode];

    deviceMode = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0xA8 / 4])(self, nullptr);
    CSF_AT(std::uint32_t, targetFormat, 0x04) =
        g_AudioFormatData[4 * deviceMode];

    if (CSF_AT(std::uint8_t, targetFormat, 0x0D) == 2 &&
        CSF_AT(std::uint32_t, targetFormat, 0x00) <
            CSF_AT(std::uint32_t, sourceFormat, 0x00) &&
        (CSF_AT(std::size_t, sourceFormat, 0x08) & 7) != 0) {
        CSF_AT(std::size_t, sourceFormat, 0x08) -=
            CSF_AT(std::size_t, sourceFormat, 0x08) & 7;
        CSF_AT(std::size_t, targetFormat, 0x08) =
            CSF_AT(std::size_t, sourceFormat, 0x08);
    }

    int sound = Audio_CreateSound(
        static_cast<int>(self->field_0004),
        reinterpret_cast<int>(sourceFormat),
        reinterpret_cast<uint32_t*>(targetFormat),
        forceMono, 0, 0, 0, 0, 0);

    if (!sound) {
        self->field_0031 = 1;
        FreeBlock_690F70(data);
        self->field_0030 = 1;
        return 0;
    }

    alignas(8) std::uint8_t nameBytes[0x200];
    alignas(8) std::uint8_t guidBytes[0x10];
    std::memset(nameBytes, 0, sizeof(nameBytes));
    char* uppercaseName = reinterpret_cast<char*>(nameBytes);
    std::strcpy(uppercaseName, name);
    __strupr(uppercaseName);
    RtGUIDGenerate(reinterpret_cast<unsigned int*>(guidBytes));
    Audio_SetSoundIdentity(sound, reinterpret_cast<int>(guidBytes),
                           reinterpret_cast<int>(uppercaseName));
    Audio_SetSoundData(sound, 0, data,
                       CSF_AT(std::size_t, sourceFormat, 0x08));
    while (Audio_PollSoundLoad(sound)) {
    }

    C_00863ba8* owner = CSF_AT(C_00863ba8*, targetFormat, 0x1C);
    FreeBlock_690F70(data);
    owner->field_0030 = 1;
    return sound;
}

HOOK(0x0070F630, C_00863ba8_LoadSoundFile, "auto");
