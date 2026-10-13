#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Local Win32 layouts: <windows.h> is not an allowed include.
struct AppMainWindowClass {
    std::uint32_t style;
    void* windowProc;
    int classExtra;
    int windowExtra;
    void* instance;
    void* icon;
    void* cursor;
    void* background;
    const char* menuName;
    const char* className;
};

struct AppMainLogBrush {
    std::uint32_t style;
    std::uint32_t color;
    std::uintptr_t hatch;
};

// Import slots checked against the complete disassembly and the PE import table.
using RegisterClassProc = std::uint16_t (__stdcall*)(const AppMainWindowClass*);
using GetSystemMetricsProc = int (__stdcall*)(int);
using CreateWindowProc = void* (__stdcall*)(
    std::uint32_t exStyle, const char* className, const char* windowName, std::uint32_t style,
    int x, int y, int width, int height, void* parent, void* menu, void* instance, void* param);
using ShowWindowProc = int (__stdcall*)(void*, int);
using UpdateWindowProc = int (__stdcall*)(void*);
using SetFocusProc = void* (__stdcall*)(void*);
CSF_GLOBAL(RegisterClassProc, RegisterClassA, 0x0082D2DC);
CSF_GLOBAL(GetSystemMetricsProc, GetSystemMetrics, 0x0082D2BC);
CSF_GLOBAL(CreateWindowProc, CreateWindowExA, 0x0082D2D8);
CSF_GLOBAL(ShowWindowProc, ShowWindow, 0x0082D2D4);
CSF_GLOBAL(UpdateWindowProc, UpdateWindow, 0x0082D2D0);
CSF_GLOBAL(SetFocusProc, SetFocus, 0x0082D2CC);

CSF_FUNC(unsigned int, __fastcall, DestroyRecord_43E300, 0x0043E300, (std::uint32_t* self, void* edx));
CSF_FUNC(int, __fastcall, AppMain_ConstructLaunchRecord, 0x004FB820, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AppMain_LoadLaunchRecord, 0x004FB8F0, (void* self, void* edx, void* a2));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __fastcall, AppMain_InitializeApplicationServices, 0x00506BE0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AppMain_InitializeApplication, 0x00506C10, (void* self, void* edx));
CSF_FUNC(void, __cdecl, AppMain_InitializeRuntime, 0x005074E0, ());
CSF_FUNC(int, __cdecl, AppMain_ShutdownRuntime, 0x00507540, ());
CSF_FUNC(int, __cdecl, AppMain_RunMessageLoop, 0x00508CE0, ());
CSF_FUNC(int, __cdecl, AppMain_SetWindowClientSize, 0x00508F50, (void* window, std::int32_t width, std::int32_t height));
CSF_FUNC(int, __cdecl, AppMain_InitializeWindowServices, 0x00509060, ());
CSF_FUNC(char, __cdecl, AppMain_ApplyConfiguration, 0x005091A0, ());
CSF_FUNC(void*, __fastcall, GetInlineOrHeapBuffer, 0x0050B8B0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __cdecl, AppMain_ShutdownGame, 0x0050D1A0, ());
CSF_FUNC(int, __fastcall, MenuCommon_SetFlagAndNotifyChild, 0x0050D7C0, (void* self, void*, int value));
CSF_FUNC(char, __cdecl, GameBddLoader_CreateMenuManager, 0x0050FBD0, (int window, char* archiveName));
CSF_FUNC(char, __fastcall, GameBddLoader_RebuildDatabases, 0x00510BF0, (void* self, void*));
CSF_FUNC(int, __cdecl, AppMain_InitializeSubsystems, 0x00519120, ());
CSF_FUNC(int, __cdecl, AppMain_ReleaseSubsystems, 0x00519140, ());
CSF_FUNC(int, __cdecl, AppMain_StopSubsystems, 0x00519170, ());
CSF_FUNC(int, __cdecl, MenuMpOnline_Sub_54E780, 0x0054E780, ());
CSF_FUNC(int, __cdecl, AppMain_ReleaseMultiplayerConfiguration, 0x00558160, ());
CSF_FUNC(char, __cdecl, AppMain_LoadMultiplayerConfiguration, 0x0055A970, (char* a2));
CSF_FUNC(int, __cdecl, GetValue_8B08F8, 0x0055A9C0, ());
CSF_FUNC(int, __cdecl, AppMain_ReleaseNetworkConfiguration, 0x0055A9D0, ());
CSF_FUNC(char, __cdecl, AppMain_LoadNetworkConfiguration, 0x0055BBE0, (char* a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(unsigned int, __cdecl, CopyTrucoStringToWide, 0x00695590, (const char* src, uint16_t* dst, int maxCount));
CSF_FUNC(int, __cdecl, AppMain_FormatDiagnostic, 0x006969C0, (void* output, int capacity, const char* format, ...));
CSF_FUNC(char, __cdecl, Options_SetFlag1, 0x00697300, (char a1));
CSF_FUNC(char, __cdecl, SetOptionsByte41, 0x00697390, (char value));
CSF_FUNC(char*, __cdecl, Options_GetStr4, 0x00697450, ());
CSF_FUNC(void*, __cdecl, AppMain_SetWorkingDirectory, 0x00697C00, (void* a1));
CSF_FUNC(char*, __cdecl, AppMain_InitializeFileServices, 0x00698310, (char a1, int a2));
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(char, __cdecl, AppMain_MountPatchArchive, 0x00698680, (char* a1));
CSF_FUNC(int, __cdecl, AppMain_ShutdownFileServices, 0x00698820, ());
CSF_FUNC(void*, __cdecl, StoreWindowHandle, 0x0069CDE0, (void* hWnd));
CSF_FUNC(int, __cdecl, AppMain_ShowError, 0x0069CDF0, (const char* caption, const char* format, ...));
CSF_FUNC(char*, __cdecl, AppMain_GetDiagnosticText, 0x0069D4B0, (void* exception, char* output));
CSF_FUNC(void, __fastcall, AppMain_ActivateDiagnosticServices, 0x0069D4F0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, AppMain_GetDiagnosticServices, 0x0069D530, ());
CSF_FUNC(void, __cdecl, SetFlag_8BB3EC, 0x0069D570, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __cdecl, GameConfig_ReleaseCachedInstance_753A40, 0x00753A40, ());
CSF_FUNC(char, __cdecl, GameConfig_InitializeSingleton, 0x007557C0, (char* configName));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, GameConfig_ReleaseCachedInstance_756A20, 0x00756A20, ());
CSF_FUNC(char, __cdecl, GameConfig_InitializeSecondarySingleton, 0x00759FC0, (char* configName));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(int, __cdecl, AppMain_ReleaseVideoConfiguration, 0x0075FE00, ());
CSF_FUNC(char, __cdecl, AppMain_LoadVersionConfiguration, 0x00760F40, (void* a1));
CSF_FUNC(int, __cdecl, AppMain_ReleaseAimingConfiguration, 0x00783320, ());
CSF_FUNC(char, __cdecl, AppMain_LoadAimingConfiguration, 0x007833D0, (int a2));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));
CSF_FUNC(int, __cdecl, AppMain_RunWideCommand, 0x007B00E9, (const wchar_t* command));
CSF_FUNC(int, __cdecl, _fclose, 0x007B01D7, (void* Stream));
CSF_FUNC(int, __cdecl, _fprintf, 0x007B0228, (void* Stream, const char* Format, ...));
CSF_FUNC(void*, __cdecl, __wfopen, 0x007B02E2, (const wchar_t* FileName, const wchar_t* Mode));
CSF_FUNC(int, __cdecl, __snprintf, 0x007B02F5, (void* Buffer, uint32_t BufferCount, const char* Format, ...));

using AppMainCreateMutexProc = void* (__stdcall*)(void* attributes, int initialOwner, const char* name);
using AppMainWaitForSingleObjectProc = std::uint32_t (__stdcall*)(void* handle, std::uint32_t milliseconds);
using AppMainInitCommonControlsProc = void (__stdcall*)();
using AppMainGetCurrentDirectoryProc = std::uint32_t (__stdcall*)(std::uint32_t size, char* buffer);
using AppMainLoadIconProc = void* (__stdcall*)(void* instance, const char* name);
using AppMainLoadCursorProc = void* (__stdcall*)(void* instance, const char* name);
using AppMainCreateBrushProc = void* (__stdcall*)(const AppMainLogBrush* brush);

CSF_GLOBAL(AppMainCreateMutexProc, g_AppMainCreateMutex, 0x0082D19C);
CSF_GLOBAL(AppMainWaitForSingleObjectProc, g_AppMainWaitForSingleObject, 0x0082D20C);
CSF_GLOBAL(AppMainInitCommonControlsProc, g_AppMainInitCommonControls, 0x0082D01C);
CSF_GLOBAL(AppMainGetCurrentDirectoryProc, g_AppMainGetCurrentDirectory, 0x0082D194);
CSF_GLOBAL(AppMainLoadIconProc, g_AppMainLoadIcon, 0x0082D2E4);
CSF_GLOBAL(AppMainLoadCursorProc, g_AppMainLoadCursor, 0x0082D2E0);
CSF_GLOBAL(AppMainCreateBrushProc, g_AppMainCreateBrush, 0x0082D040);

CSF_GLOBAL(std::uint8_t, g_AppMainStartupMode, 0x00899910);
CSF_GLOBAL(int, g_value_8B056C, 0x008B056C);
CSF_GLOBAL(std::uint16_t, g_AppMainDirectorySeparator, 0x0083FBFC);
CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);
CSF_GLOBAL(std::uint16_t, g_AppMainCrashFileMode, 0x0083F914);
CSF_GLOBAL(char, g_AppMainMemoryReadLabel, 0x0083F9A8);

using OpenKeyProc = int (__stdcall*)(void*, const char*, int, int, void**);
using CloseKeyProc = int (__stdcall*)(void*);
using MessageBoxProc = int (__stdcall*)(void*, const char*, const char*, unsigned int);
using GetDesktopWindowProc = void* (__stdcall*)();
using ExitProcessProc = void (__stdcall*)(unsigned int);
CSF_GLOBAL(OpenKeyProc, AppMain_OpenKey, 0x0082D010);
CSF_GLOBAL(CloseKeyProc, AppMain_CloseKey, 0x0082D000);
CSF_GLOBAL(MessageBoxProc, AppMain_MessageBox, 0x0082D2C4);
CSF_GLOBAL(GetDesktopWindowProc, AppMain_GetDesktopWindow, 0x0082D2C8);
CSF_GLOBAL(ExitProcessProc, AppMain_ExitProcess, 0x0082D198);

// clang-cl recognizes this MSVC SEH intrinsic without <windows.h>.
void* __cdecl _exception_info();

static int AppMain_ExceptionFilter(void* exception, char* text, char* diagnostic)
{
    void* record = CSF_AT(void*, exception, 0);
    if (CSF_AT(std::uint32_t, record, 0) != 0xC0000005u) {
        char* detail = AppMain_GetDiagnosticText(exception, diagnostic);
        __snprintf(text, 0x8000,
            CSF_STR(0x0083F9B0, "Exception: %X\nAddress: %p\n%s"),
            CSF_AT(std::uint32_t, record, 0), CSF_AT(void*, record, 0x0C), detail);
    } else {
        const char* access = (CSF_AT(std::uint8_t, record, 0x14) & 1)
            ? CSF_STR(0x0083F9A0, "WRITE") : &g_AppMainMemoryReadLabel;
        char* detail = AppMain_GetDiagnosticText(exception, diagnostic);
        __snprintf(text, 0x8000,
            CSF_STR(0x0083F958, "Exception: %X (Access Violation)\nAddress: %p\nCan't %s memory at %p\n%s"),
            CSF_AT(std::uint32_t, record, 0), CSF_AT(void*, record, 0x0C),
            access, CSF_AT(void*, record, 0x18), detail);
    }
    return 1;
}

static int __stdcall AppMain_Run(
    void* instance,
    void* /*previousInstance*/,
    char* /*commandLine*/,
    int showCommand)
{
    void* mutex = g_AppMainCreateMutex(
        nullptr, 1, CSF_STR(0x0083F8C4, "CommXPC Pyro Studios 2005"));
    if (g_AppMainWaitForSingleObject(mutex, 0x64u) == 258)
        return 0;

    alignas(8) std::uint8_t directoryBytes[0x100];
    char* directory = reinterpret_cast<char*>(directoryBytes);
    std::memset(directory, 0, sizeof(directoryBytes));

    const bool returnStartupFlag = g_AppMainStartupMode == 48;

    g_AppMainInitCommonControls();
    AppMain_InitializeRuntime();
    SceneLoader_NoOp();
    AppMain_InitializeSubsystems();
    AppMain_InitializeFileServices(0, 30);

    g_AppMainGetCurrentDirectory(0x100u, directory);
    if (directory[std::strlen(directory) - 1] != 92) {
        char* end = directory;
        while (*end)
            ++end;
        const std::uint16_t separator = g_AppMainDirectorySeparator;
        std::memcpy(end, &separator, sizeof(separator));
    }
    AppMain_SetWorkingDirectory(directory);

    if (!GameConfig_InitializeSecondarySingleton(
            const_cast<char*>(CSF_STR(0x0083FBE8, "Config/Juego.cfg")))) {
        AppMain_ShowError(
            CSF_STR(0x0083F688, "ERROR"),
            CSF_STR(0x0083FBA8, "No se pudo abrir el archivo de preferencias 'Config/Juego.cfg'"));
        GameConfig_ReleaseCachedInstance_756A20();
        AppMain_ShutdownFileServices();
        AppMain_StopSubsystems();
        AppMain_ReleaseSubsystems();
        AppMain_ShutdownRuntime();
        return 0;
    }

    if (!AppMain_LoadVersionConfiguration(
            const_cast<char*>(CSF_STR(0x0083FB94, "Config/JuegoVer.cfg")))) {
        AppMain_ShowError(
            CSF_STR(0x0083F688, "ERROR"),
            CSF_STR(0x0083FB50, "No se pudo abrir el archivo de preferencias 'Config/JuegoVer.cfg'"));
    }

    AppMain_ApplyConfiguration();
    if (CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xCF))
        SetFlag_8BB3EC();

    if (!GameConfig_InitializeSingleton(
            const_cast<char*>(CSF_STR(0x0083FB3C, "Config/JuegoPla.cfg")))) {
        AppMain_ShowError(
            CSF_STR(0x0083F688, "ERROR"),
            CSF_STR(0x0083FAF8, "No se pudo abrir el archivo de preferencias 'Config/JuegoPla.cfg'"));
    }

    if (!AppMain_LoadAimingConfiguration(
            reinterpret_cast<int>(CSF_STR(0x0083FAE4, "Config/Punteria.cfg")))) {
        AppMain_ShowError(
            CSF_STR(0x0083F688, "ERROR"),
            CSF_STR(0x0083FAA0, "No se pudo abrir el archivo de preferencias 'Config/Punteria.cfg'"));
        AppMain_ReleaseAimingConfiguration();
    }

    if (!AppMain_LoadNetworkConfiguration(
            const_cast<char*>(CSF_STR(0x0083FA8C, "Config/Red.cfg")))) {
        AppMain_ShowError(
            CSF_STR(0x0083F688, "ERROR"),
            CSF_STR(0x0083FA4C, "No se pudo abrir el archivo de preferencias 'Config/Red.cfg'"));
        AppMain_ReleaseNetworkConfiguration();
    }

    if (!AppMain_LoadMultiplayerConfiguration(
            const_cast<char*>(CSF_STR(0x0083FA38, "Config/MultiP.cfg")))) {
        AppMain_ShowError(
            CSF_STR(0x0083F688, "ERROR"),
            CSF_STR(0x0083F9F8, "No se pudo abrir el archivo de preferencias 'Config/MultiP.cfg'"));
        AppMain_ReleaseMultiplayerConfiguration();
    }

    if (g_value_8B056C) {
        AppMain_InitializeApplication(
            reinterpret_cast<void*>(GetGlobalValue_8B056C()), nullptr);
        AppMain_InitializeApplicationServices(
            reinterpret_cast<void*>(GetGlobalValue_8B056C()), nullptr);
    }

    void* recordMemory = Mem_Alloc(60);
    void* launchRecord = recordMemory
        ? reinterpret_cast<void*>(AppMain_ConstructLaunchRecord(recordMemory, nullptr))
        : nullptr;

    void* configuration = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* launchSettings;
    if (CSF_AT(std::uint32_t, configuration, 0x30) < 0x10u)
        launchSettings = static_cast<std::uint8_t*>(configuration) + 0x1C;
    else
        launchSettings = CSF_AT(void*, configuration, 0x1C);
    AppMain_LoadLaunchRecord(launchRecord, nullptr, launchSettings);

    if (CSF_AT(std::uint8_t, launchRecord, 0x39)) {
        if (g_value_8B056C)
            CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalValue_8B056C()), 0x21) = 1;
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA5C) = 0;
    } else {
        AppMain_MountPatchArchive(
            const_cast<char*>(CSF_STR(0x0083F9EC, "Patch.pak")));
    }

    SetOptionsByte41(
        CSF_AT(char, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA5D));
    Options_SetFlag1(CSF_AT(char, launchRecord, 0x39));

    if (CSF_AT(std::uint8_t, launchRecord, 0x38)) {
        void* diagnosticServices = AppMain_GetDiagnosticServices();
        AppMain_ActivateDiagnosticServices(diagnosticServices, nullptr);
    }

    AppMain_InitializeWindowServices();

    alignas(8) std::uint8_t windowClassBytes[0x28];
    AppMainWindowClass& windowClass =
        *reinterpret_cast<AppMainWindowClass*>(windowClassBytes);
    alignas(8) std::uint8_t brushBytes[0x14];
    AppMainLogBrush& brush = *reinterpret_cast<AppMainLogBrush*>(brushBytes);

    windowClass.className = CSF_STR(0x0083F9DC, "WindowClassName");
    windowClass.windowProc = reinterpret_cast<void*>(CSF_ADDR_FN(0x00508D40));
    windowClass.style = 3;
    windowClass.instance = instance;
    windowClass.icon =
        g_AppMainLoadIcon(instance, reinterpret_cast<const char*>(std::uintptr_t{0x72}));
    windowClass.cursor =
        g_AppMainLoadCursor(nullptr, reinterpret_cast<const char*>(std::uintptr_t{0x7F00}));

    brush.style = 1;
    brush.color = 0;
    brush.hatch = 0;
    windowClass.background = g_AppMainCreateBrush(&brush);
    windowClass.menuName = nullptr;
    windowClass.classExtra = 0;
    windowClass.windowExtra = 0;

    if (!RegisterClassA(&windowClass))
        return 0;

    void* window;
    if (CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x11C)) {
        const int screenHeight = GetSystemMetrics(1);
        const int screenWidth = GetSystemMetrics(0);
        window = CreateWindowExA(
            8u, windowClass.className,
            CSF_STR(0x0083F9D0, "CommX PC"), 0x90000000u,
            0, 0, screenWidth, screenHeight,
            nullptr, nullptr, instance, nullptr);
    } else {
        window = CreateWindowExA(
            0, windowClass.className,
            CSF_STR(0x0083F9D0, "CommX PC"), 0xCA0000u,
            static_cast<int>(0x80000000u), 0,
            static_cast<int>(0x80000000u), 0,
            nullptr, nullptr, instance, nullptr);
        if (!window)
            return 0;

        std::int32_t width = static_cast<std::int32_t>(static_cast<std::int64_t>(
            CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xB0)));
        std::int32_t height = static_cast<std::int32_t>(static_cast<std::int64_t>(
            CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xB4)));

        void* windowServices =
            CSF_AT(void*, reinterpret_cast<void*>(GetGlobalValue_8B056C()), 0x18);
        if (windowServices) {
            void* display = CSF_AT(void*, windowServices, 0x0C);
            if (display) {
                void* dimensions =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, display, 0)[0x20 / 4])(
                            CSF_AT(void*, windowServices, 0x0C), nullptr);
                width = CSF_AT(std::int32_t, dimensions, 0);

                dimensions =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, display, 0)[0x20 / 4])(display, nullptr);
                height = CSF_AT(std::int32_t, dimensions, 0x04);
            }
        }
        AppMain_SetWindowClientSize(window, width, height);
    }

    if (!window)
        return 0;

    StoreWindowHandle(window);
    ShowWindow(window, showCommand);
    UpdateWindow(window);
    SetFocus(window);

    char* archiveName;
    if (CSF_AT(std::uint32_t, launchRecord, 0x34) < 0x10u)
        archiveName = reinterpret_cast<char*>(launchRecord) + 0x20;
    else
        archiveName = CSF_AT(char*, launchRecord, 0x20);

    if (GameBddLoader_CreateMenuManager(reinterpret_cast<int>(window), archiveName)) {
        if (CSF_AT(std::uint8_t, launchRecord, 0x39)) {
            GameBddLoader_RebuildDatabases(
                reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
        } else {
            void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, state, 0)[0x10 / 4])(state, nullptr, 1);

            void* menu = reinterpret_cast<void*>(GetGlobalState_50D190());
            MenuCommon_SetFlagAndNotifyChild(menu, nullptr, 1);
            // The original saves this and pushes it below the diagnostic call's
            // two consumed arguments. 0069D4B0 never reads that extra slot.
            Options_GetStr4();
            // These four buffers are contiguous in the original stack frame.
            alignas(8) std::uint8_t crashBytes[0x11000];
            char* text = reinterpret_cast<char*>(crashBytes);
            wchar_t* quotedPath = reinterpret_cast<wchar_t*>(crashBytes + 0x8000);
            char* diagnostic = reinterpret_cast<char*>(crashBytes + 0x8800);
            wchar_t* path = reinterpret_cast<wchar_t*>(crashBytes + 0x10800);
            alignas(8) std::uint8_t modeBytes[8];
            wchar_t* mode = reinterpret_cast<wchar_t*>(modeBytes);
            __try {
                AppMain_RunMessageLoop();
            } __except (AppMain_ExceptionFilter(_exception_info(), text, diagnostic)) {
                void* key;
                if (AppMain_OpenKey(reinterpret_cast<void*>(std::uintptr_t{0x80000001u}),
                        CSF_STR(0x0083F930, "Software\\Pyro Studios\\CSFDeveloper"),
                        0, 0xF003F, &key) == 0) {
                    AppMain_CloseKey(key);
                    void* services = CSF_AT(void*,
                        reinterpret_cast<void*>(GetGlobalValue_8B056C()), 0x18);
                    const int formattingMode = g_NumberFormattingMode;
                    void* directoryText = GetInlineOrHeapBuffer(services, nullptr);
                    AppMain_FormatDiagnostic(path, 0x400,
                        CSF_STR(0x0083F924, "{w0}\\{s1}"), directoryText,
                        CSF_STR(0x0083F7E8, "crash.txt"), formattingMode);
                    const int quotedFormattingMode = g_NumberFormattingMode;
                    directoryText = GetInlineOrHeapBuffer(services, nullptr);
                    AppMain_FormatDiagnostic(quotedPath, 0x400,
                        CSF_STR(0x0083F918, "\"{w0}\\{s1}\""), directoryText,
                        CSF_STR(0x0083F7E8, "crash.txt"), quotedFormattingMode);
                    CopyTrucoStringToWide(reinterpret_cast<const char*>(&g_AppMainCrashFileMode),
                        reinterpret_cast<std::uint16_t*>(mode), 4);
                    _sprintf(diagnostic, CSF_STR(0x0083F90C, "%s\n\n%s"),
                        CSF_STR(0x0083F7F8, "Please, if this crash cannot be easily reproduced, send this call stack information to developers along with the information about the executable, as it can be valuable to fix this crash. Thank you."),
                        text);
                    void* file = __wfopen(path, mode);
                    if (file) {
                        _fprintf(file, diagnostic);
                        _fclose(file);
                        AppMain_RunWideCommand(quotedPath);
                    }
                    AppMain_MessageBox(AppMain_GetDesktopWindow(), diagnostic,
                        CSF_STR(0x0083F8F4, "Something bad happened"), 0x10);
                    AppMain_ExitProcess(1);
                }
            }
        }
    }

    AppMain_ShutdownGame();
    GameConfig_ReleaseCachedInstance_756A20();
    SceneLoader_SetFileOption(0);

    void* network = reinterpret_cast<void*>(GetValue_8B08F8());
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, network, 0)[0x08 / 4])(network, nullptr);

    SceneLoader_SetFileOption(1);
    AppMain_ReleaseVideoConfiguration();
    GameConfig_ReleaseCachedInstance_753A40();
    AppMain_ReleaseAimingConfiguration();
    AppMain_ReleaseNetworkConfiguration();
    AppMain_ReleaseMultiplayerConfiguration();
    MenuMpOnline_Sub_54E780();
    AppMain_ShutdownFileServices();
    AppMain_StopSubsystems();
    AppMain_ReleaseSubsystems();
    DestroyRecord_43E300(static_cast<std::uint32_t*>(launchRecord), nullptr);
    Mem_Free(launchRecord);
    AppMain_ShutdownRuntime();
    return returnStartupFlag;
}

HOOK(0x00509250, AppMain_Run, "auto");
