#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void* , void* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(bool, __fastcall, CoreFiles_IsArchiveAccessLocked, 0x006973F0, (void* self, void* edx));
CSF_FUNC(char, __cdecl, PrefixBaseDirectoryIfRelative, 0x006975F0, (char* path));
CSF_FUNC(int, __cdecl, CoreFiles_OpenArchiveFile, 0x00697840, (int a1, int a2));
CSF_FUNC(char, __fastcall, MissionScript_HasPendingState, 0x0069AF10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CoreFiles_HasCachedFile, 0x0069E830, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, CoreFiles_OpenCachedFile, 0x0069E910, (void* self, void* edx, char* arg0));
CSF_FUNC(C_0085ec74*, __fastcall, C_0085ec74_Construct, 0x006A02B0, (C_0085ec74* self, void*));
CSF_FUNC(int, __cdecl, ZlibLog_CopyPathBackslashes, 0x006A02C0, (char* dst, const char* src, int));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));
CSF_FUNC(char*, __cdecl, _strchr, 0x007B1B60, (char* Str, int Val));

static void* __cdecl CoreFiles_OpenSceneFile(
    const char* filename, char* mode, int openFlags)
{
    char logBuffer[512];
    char normalizedPath[256];
    char relativePath[256];
    alignas(8) uint8_t logStringBytes[0x20];
    C_0082d410& logString =
        *reinterpret_cast<C_0082d410*>(logStringBytes);

    if (CSF_AT(uint8_t, g_pOptions, 0x01))
    {
        _sprintf(logBuffer, CSF_STR(0x0085E6B8, "%s %s"), filename, mode);

        CSF_AT(uint32_t, logStringBytes, 0x18) = 15;
        CSF_AT(uint32_t, logStringBytes, 0x14) = 0;
        reinterpret_cast<uint8_t*>(&logString.field_0004)[0] = 0;

        AssignStringRange_429B60(
            &logString, nullptr, logBuffer,
            static_cast<unsigned int>(std::strlen(logBuffer)));
        MenuManager_AppendPageName(
            reinterpret_cast<S_00470ab0*>(
                static_cast<uint8_t*>(g_pOptions) + 0x30),
            nullptr, &logString);

        if (CSF_AT(uint32_t, logStringBytes, 0x18) >= 0x10 &&
            logString.field_0004)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(logString.field_0004));
        }

        CSF_AT(uint32_t, logStringBytes, 0x18) = 15;
        CSF_AT(uint32_t, logStringBytes, 0x14) = 0;
        reinterpret_cast<uint8_t*>(&logString.field_0004)[0] = 0;
    }

    void* pendingState = CSF_AT(void*, g_pOptions, 0x64);
    if (pendingState &&
        MissionScript_HasPendingState(pendingState, nullptr))
    {
        pendingState = CSF_AT(void*, g_pOptions, 0x64);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, pendingState, 0)[4 / 4])(
                pendingState, nullptr);
    }

    void* file = nullptr;
    if (!filename)
        return nullptr;

    int accessMode = _strchr(mode, 119) != nullptr;
    if (_strchr(mode, 97))
        accessMode = 2;

    uint8_t* options = static_cast<uint8_t*>(g_pOptions);
    void* cache = CSF_AT(void*, options, 0x60);
    if (cache)
    {
        if (CoreFiles_HasCachedFile(
                cache, nullptr, const_cast<char*>(filename)))
        {
            file = CoreFiles_OpenCachedFile(
                CSF_AT(void*, g_pOptions, 0x60),
                nullptr, const_cast<char*>(filename));
        }
        options = static_cast<uint8_t*>(g_pOptions);
    }

    bool openDiskFile = false;
    if (!file)
    {
        if (!CSF_AT(uint8_t, options, 0x41) &&
            !CoreFiles_IsArchiveAccessLocked(options + 0x50, nullptr))
        {
            if (accessMode)
            {
                openDiskFile = true;
            }
            else
            {
                file = reinterpret_cast<void*>(
                    CoreFiles_OpenArchiveFile(
                        reinterpret_cast<int>(filename),
                        reinterpret_cast<int>(options + 0x50)));
                options = static_cast<uint8_t*>(g_pOptions);
            }
        }

        if (!file && !openDiskFile)
        {
            openDiskFile =
                accessMode ||
                CSF_AT(uint8_t, options, 0x40) ||
                CoreFiles_IsArchiveAccessLocked(options + 0x50, nullptr);
        }
    }

    if (openDiskFile)
    {
        void* storage = Mem_Alloc(8);
        file = storage
            ? C_0085ec74_Construct(
                  static_cast<C_0085ec74*>(storage), nullptr)
            : nullptr;

        const bool usePathDirectly =
            CSF_AT(uint8_t, g_pOptions, 0x68) != 0;
        char* destination = usePathDirectly ? normalizedPath : relativePath;
        const char* source = filename;
        char character;
        do
        {
            character = *source++;
            *destination++ = character;
        } while (character);

        if (!usePathDirectly)
        {
            PrefixBaseDirectoryIfRelative(relativePath);
            ZlibLog_CopyPathBackslashes(normalizedPath, relativePath, 0);
        }

        if (reinterpret_cast<bool(__fastcall*)(void*, void*, char*, int, int)>(
                CSF_AT(void**, file, 0)[4 / 4])(
                    file, nullptr, normalizedPath, accessMode, openFlags))
        {
            options = static_cast<uint8_t*>(g_pOptions);
            void* openFiles = options + 0x20;

            if (CSF_AT(uint32_t, options, 0x28) >=
                CSF_AT(uint32_t, options, 0x24))
            {
                // The packet also identifies this call address as a vtable reference.
                (void)CSF_VTBL(0x0041E5F0);
                S_0041e5f0_GrowArrayCapacity(
                    static_cast<S_0041e5f0*>(openFiles), nullptr,
                    CSF_AT(int, openFiles, 0x0C));
                options = static_cast<uint8_t*>(g_pOptions);
            }

            const uint32_t index = CSF_AT(uint32_t, openFiles, 0x08);
            void** entry = CSF_AT(void**, openFiles, 0x00) + index;
            CSF_AT(uint32_t, openFiles, 0x08) = index + 1;
            *entry = file;
        }
        else
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, file, 0)[0 / 4])(file, nullptr, 1);
            options = static_cast<uint8_t*>(g_pOptions);
            file = nullptr;
        }
    }

    if (CSF_AT(uint8_t, options, 0x41) && !file)
    {
        void* archive = options + 0x50;
        if (!CoreFiles_IsArchiveAccessLocked(archive, nullptr) && !accessMode)
        {
            return reinterpret_cast<void*>(
                CoreFiles_OpenArchiveFile(
                    reinterpret_cast<int>(filename),
                    reinterpret_cast<int>(archive)));
        }
    }

    return file;
}

HOOK(0x00698350, CoreFiles_OpenSceneFile, "auto");
