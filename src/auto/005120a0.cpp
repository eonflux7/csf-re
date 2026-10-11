#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CSaveGameManager_ResetArchives, 0x00511FF0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, ProfilesSavegames_ConstructArchiveWrapper, 0x0054C140, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_CreateReferenceBuffer, 0x0054D860, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_008443c8_Construct, 0x0054E500, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_0085e5c4_Construct, 0x00691A20, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_CreateReferenceArchive, 0x00691C80, (void* self, void* edx, int a2));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SetOptionsByteAt40, 0x00697330, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte40, 0x00697350, ());
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(uint32_t*, __fastcall, C_0085e748_Construct, 0x0069A080, (void* self, void* edx, int a2, char a3));
CSF_FUNC(uint32_t*, __fastcall, C_0085e760_Construct, 0x0069A370, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_CreateReferenceRegistry, 0x0069AD30, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, ProfilesSavegames_ConstructFileStream, 0x0069D6A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

using GetCurrentDirectoryWProc = uint32_t (__stdcall*)(uint32_t, wchar_t*);
using SetCurrentDirectoryWProc = int (__stdcall*)(const wchar_t*);
CSF_GLOBAL(GetCurrentDirectoryWProc, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWProc, g_SetCurrentDirectoryW, 0x0082D190);

static char __fastcall CSaveGameManager_OpenArchive(
    CSaveGameManager* self, void* /*edx*/, int contextAddress,
    int fileArgument, char writing)
{
    alignas(8) uint8_t directoryBytes[0x80C];
    auto* previousDirectory = reinterpret_cast<wchar_t*>(directoryBytes);
    void* context = reinterpret_cast<void*>(contextAddress);

    const char previousFileOption = GetOptionsByteAt2();
    if (previousFileOption)
        SceneLoader_SetFileOption(0);

    g_GetCurrentDirectoryW(0x400u, previousDirectory);
    const char previousOption104 = GetOptionsByte104();
    StoreOptionsByte104(1);

    const wchar_t* saveDirectory;
    if (self->field_0074 < 8)
        saveDirectory = reinterpret_cast<const wchar_t*>(&self->field_0060);
    else
        saveDirectory = reinterpret_cast<const wchar_t*>(self->field_0060);
    g_SetCurrentDirectoryW(saveDirectory);

    const char previousOption40 = GetOptionsByte40();
    if (!previousOption40)
        SetOptionsByteAt40(1);

    CSF_AT(char, context, 0x14) = writing;
    CSaveGameManager_ResetArchives(self, nullptr);

    if (writing)
    {
        CSF_AT(uint32_t, context, 0x18) =
            CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xB44);
    }

    void* allocation = Mem_Alloc(12);
    uint32_t* fileStream = allocation
        ? ProfilesSavegames_ConstructFileStream(allocation, nullptr)
        : nullptr;
    self->field_001c = reinterpret_cast<uint32_t>(fileStream);

    bool opened;
    if (writing)
    {
        opened = reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, fileStream, 0)[0x10 / 4])(
                fileStream, nullptr, fileArgument);
    }
    else
    {
        opened = reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, fileStream, 0)[0x14 / 4])(
                fileStream, nullptr, fileArgument);
    }

    if (!opened)
    {
        void* stream = reinterpret_cast<void*>(self->field_001c);
        if (stream)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, stream, 0)[0])(
                    stream, nullptr, 1);
            self->field_001c = 0;
        }

        SetOptionsByteAt40(previousOption40);
        g_SetCurrentDirectoryW(previousDirectory);
        StoreOptionsByte104(previousOption104);
        SceneLoader_SetFileOption(previousFileOption);
        return 0;
    }

    allocation = Mem_Alloc(20);
    self->field_0024 = reinterpret_cast<uint32_t>(
        allocation
            ? C_0085e748_Construct(
                allocation, nullptr, static_cast<int>(self->field_001c),
                writing ? 0 : 1)
            : nullptr);

    allocation = Mem_Alloc(12);
    self->field_0020 = reinterpret_cast<uint32_t>(
        allocation
            ? C_0085e760_Construct(
                allocation, nullptr, static_cast<int>(self->field_0024))
            : nullptr);

    allocation = Mem_Alloc(8);
    self->field_0028 = reinterpret_cast<uint32_t>(
        allocation
            ? C_008443c8_Construct(
                allocation, nullptr, static_cast<int>(self->field_0020))
            : nullptr);

    if (writing)
    {
        allocation = Mem_Alloc(20);
        self->field_0038 = reinterpret_cast<uint32_t>(
            allocation ? C_0085e5c4_Construct(allocation, nullptr) : nullptr);

        CSF_AT(uint8_t, context, 0) = 0;
        CSF_AT(uint32_t, context, 0x0C) = self->field_0038;
    }
    else
    {
        allocation = Mem_Alloc(8);
        self->field_0030 = reinterpret_cast<uint32_t>(
            allocation
                ? SceneLoader_CreateReferenceBuffer(allocation, nullptr)
                : nullptr);

        allocation = Mem_Alloc(24);
        self->field_0034 = reinterpret_cast<uint32_t>(
            allocation
                ? SceneLoader_CreateReferenceArchive(
                    allocation, nullptr, static_cast<int>(self->field_0030))
                : nullptr);

        allocation = Mem_Alloc(12);
        // The packet also lists this constructor address as a vtable reference.
        (void)CSF_VTBL(0x0069AD30);
        self->field_003c = reinterpret_cast<uint32_t>(
            allocation
                ? SceneLoader_CreateReferenceRegistry(allocation, nullptr)
                : nullptr);

        CSF_AT(uint8_t, context, 0) = 0;
        CSF_AT(uint32_t, context, 0x08) = self->field_0034;
        CSF_AT(uint32_t, context, 0x10) = self->field_003c;
    }

    if (self->field_0004)
    {
        allocation = Mem_Alloc(168);
        uint32_t* wrapper = allocation
            ? ProfilesSavegames_ConstructArchiveWrapper(allocation, nullptr)
            : nullptr;

        // Deliberately retains the original null write on allocation failure.
        CSF_AT(uint32_t, wrapper, 0xA4) = self->field_0028;
        self->field_002c = reinterpret_cast<uint32_t>(wrapper);
        CSF_AT(uint32_t, context, 0x04) = reinterpret_cast<uint32_t>(wrapper);
    }
    else
    {
        CSF_AT(uint32_t, context, 0x04) = self->field_0028;
    }

    SetOptionsByteAt40(previousOption40);
    g_SetCurrentDirectoryW(previousDirectory);
    StoreOptionsByte104(previousOption104);
    SceneLoader_SetFileOption(previousFileOption);
    return 1;
}

HOOK(0x005120A0, CSaveGameManager_OpenArchive, "auto");
