#define _CRT_SECURE_NO_WARNINGS
#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Win32 import (kernel32), C linkage via the CRT's own header macros.
_CRT_BEGIN_C_HEADER
__declspec(dllimport) int __stdcall CloseHandle(void* handle);
_CRT_END_C_HEADER

CSF_FUNC(char, __fastcall, SceneLoader_LoadSceneResources, 0x004FEA80,
         (void* self, void* edx, char* a2, char a3));
CSF_FUNC(char, __stdcall, SceneLoader_ReadVisibilitySettings, 0x00501450,
         (int a1, void* a2));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_DestroyVisibilitySettings, 0x00505D70,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetRenderContext, 0x00652010,
         (void* self, void* edx));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0,
         (char* Str, int a2, char* a3));
CSF_FUNC(void, __cdecl, SceneLoader_PrepareSceneArchive, 0x00698960,
         (char* String1));
CSF_FUNC(uint8_t*, __cdecl, SceneLoader_CreateLoadTimer, 0x0069CDC0, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, SceneLoader_GetVisibilityProxy, 0x006CABD0,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_ConfigureVisibility, 0x006CAD20,
         (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_AttachVisibilityProxy, 0x006D6EE0,
         (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0,
         (char* dest, const char* src, size_t count));

CSF_GLOBAL(double, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);

static uint8_t* SceneLoader_GetSelectedEntry(C_0082d410* self)
{
    return CSF_AT(uint8_t*, self, 0x30)
         + 88 * CSF_AT(uint32_t, self, 0x14);
}

static char* SceneLoader_GetEntryName(uint8_t* entry)
{
    return CSF_AT(uint32_t, entry, 0x18) < 0x10
         ? reinterpret_cast<char*>(entry + 0x4)
         : CSF_AT(char*, entry, 0x4);
}

static int __fastcall CSceneLoader_FinishSceneLoad(
    C_0082d410* self, void* /*edx*/)
{
    SceneLoader_GetRenderContext(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    uint8_t* timer = SceneLoader_CreateLoadTimer();

    char archivePath[256];
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA5C))
    {
        std::strcpy(archivePath,
                    SceneLoader_GetEntryName(SceneLoader_GetSelectedEntry(self)));
        Path_ReplaceExtension(
            archivePath, 512, const_cast<char*>(CSF_STR(0x00830FEC, ".pak")));
        SceneLoader_PrepareSceneArchive(archivePath);
    }

    SceneLoader_LoadSceneResources(
        self, nullptr,
        SceneLoader_GetEntryName(SceneLoader_GetSelectedEntry(self)), 0);

    alignas(4) uint8_t visibilitySettings[0x94] = {};
    CSF_AT(uint32_t, visibilitySettings, 0x18) = 15;
    CSF_AT(uint32_t, visibilitySettings, 0x34) = 15;
    CSF_AT(uint32_t, visibilitySettings, 0x50) = 15;
    CSF_AT(uint32_t, visibilitySettings, 0x6C) = 15;

    char visibilityPath[0x200] = {};
    _strncpy(visibilityPath,
             SceneLoader_GetEntryName(SceneLoader_GetSelectedEntry(self)), 0x200);
    visibilityPath[0x1FF] = 0;
    Path_ReplaceExtension(
        visibilityPath, 512, const_cast<char*>(CSF_STR(0x0083F534, ".vis")));

    if (SceneLoader_ReadVisibilitySettings(
            reinterpret_cast<int>(visibilityPath), visibilitySettings))
    {
        char* modelName;
        if (CSF_AT(uint32_t,
                   reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x48) > 0)
        {
            uint8_t* configuration =
                reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
            modelName = CSF_AT(uint32_t, configuration, 0x4C) < 0x10
                      ? reinterpret_cast<char*>(configuration + 0x38)
                      : CSF_AT(char*, configuration, 0x38);
        }
        else
        {
            modelName =
                SceneLoader_GetEntryName(SceneLoader_GetSelectedEntry(self));
        }

        char modelPath[256];
        std::strcpy(modelPath, modelName);
        Path_ReplaceExtension(
            modelPath, 256, const_cast<char*>(CSF_STR(0x0083F56C, ".m3d")));

        alignas(4) uint8_t visibilityOptions[0x28] = {};
        CSF_AT(uint8_t, visibilityOptions, 0x1C) = 1;
        CSF_AT(uint8_t, visibilityOptions, 0x0) =
            CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0);

        CSF_AT(void*, visibilityOptions, 0x4) =
            CSF_AT(uint32_t, visibilitySettings, 0x18) < 0x10
            ? visibilitySettings + 0x4
            : CSF_AT(void*, visibilitySettings, 0x4);
        CSF_AT(void*, visibilityOptions, 0x8) =
            CSF_AT(uint32_t, visibilitySettings, 0x34) < 0x10
            ? visibilitySettings + 0x20
            : CSF_AT(void*, visibilitySettings, 0x20);
        CSF_AT(void*, visibilityOptions, 0x20) =
            CSF_AT(uint32_t, visibilitySettings, 0x50) < 0x10
            ? visibilitySettings + 0x3C
            : CSF_AT(void*, visibilitySettings, 0x3C);
        CSF_AT(void*, visibilityOptions, 0xC) =
            CSF_AT(uint32_t, visibilitySettings, 0x6C) < 0x10
            ? visibilitySettings + 0x58
            : CSF_AT(void*, visibilitySettings, 0x58);

        CSF_AT(int, visibilityOptions, 0x10) =
            CSF_AT(int, visibilitySettings, 0x70);
        CSF_AT(int, visibilityOptions, 0x14) =
            CSF_AT(int, visibilitySettings, 0x74);
        CSF_AT(int, visibilityOptions, 0x18) =
            CSF_AT(int, visibilitySettings, 0x78);
        CSF_AT(uint8_t, visibilityOptions, 0x1C) =
            CSF_AT(uint8_t, visibilitySettings, 0x7C);
        CSF_AT(char*, visibilityOptions, 0x24) = modelPath;

        void* context = reinterpret_cast<void*>(
            SceneLoader_GetRenderContext(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        SceneLoader_ConfigureVisibility(
            context, nullptr, reinterpret_cast<int>(visibilityOptions));
        void* proxy = SceneLoader_GetVisibilityProxy(context, nullptr);
        void* renderHandle = reinterpret_cast<void*>(
            RenderHandle_FromObject(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        SceneLoader_AttachVisibilityProxy(renderHandle, nullptr, proxy);

        CSF_AT(uint8_t, SceneLoader_GetSelectedEntry(self), 0x1C) = 1;
    }

    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, self, 0x14);
    CSF_AT(int, self, 0x14) = -1;

    if (timer)
    {
        double elapsedSeconds =
            ((double(__fastcall*)(void*, void*))
                CSF_AT(void**, timer, 0)[0x10 / 4])(timer, nullptr)
            * g_SceneLoadMillisecondsToSeconds;
        if (elapsedSeconds > CSF_AT(float, self, 0xDC))
            CSF_AT(float, self, 0xDC) = static_cast<float>(elapsedSeconds);

        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, timer, 0)[0])(timer, nullptr, 1);
    }

    void* loadHandle = CSF_AT(void*, self, 0x110);
    CSF_AT(uint8_t, self, 0xB5) = 0;
    CloseHandle(loadHandle);
    SceneLoader_DestroyVisibilitySettings(visibilitySettings, nullptr);
    return 0;
}

HOOK(0x00505E00, CSceneLoader_FinishSceneLoad, "auto");
