// The host CRT initializes our own C++ bindings first. Game startup is explicit.
#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "globals.h"
#include "runtime.h"

// db/functions.csv: _WinMain@16; parked in queue/00509250.md.
CSF_FUNC(int, __stdcall, Game_WinMain, 0x00509250, (HINSTANCE, HINSTANCE, LPSTR, int));

// CSF_HANG_SECONDS=N: after N seconds, suspend the main thread, print its registers and every stack word that
// points into this exe (MAINBASE for tools/dbg/stdsym.py, which names them from the link map) and exit 3. For a
// run that neither traps nor crashes.
static HANDLE g_mainThread;
static DWORD WINAPI hang_dump(void* seconds) {
    Sleep(static_cast<DWORD>(reinterpret_cast<uintptr_t>(seconds)) * 1000);
    SuspendThread(g_mainThread);
    CONTEXT c{};
    c.ContextFlags = CONTEXT_FULL;
    GetThreadContext(g_mainThread, &c);
    const auto base = reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));
    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const uintptr_t end = base + nt->OptionalHeader.SizeOfImage;
    std::fprintf(stderr, "csf: HANG after %u s\nMAINBASE %08X\nEIP %08X\n", static_cast<unsigned>(
        reinterpret_cast<uintptr_t>(seconds)), static_cast<unsigned>(base), static_cast<unsigned>(c.Eip));
    std::fprintf(stderr, "  eax=%08lX ebx=%08lX ecx=%08lX edx=%08lX esi=%08lX edi=%08lX ebp=%08lX esp=%08lX\n",
                 c.Eax, c.Ebx, c.Ecx, c.Edx, c.Esi, c.Edi, c.Ebp, c.Esp);
    MEMORY_BASIC_INFORMATION m{};
    VirtualQuery(reinterpret_cast<void*>(c.Esp), &m, sizeof m);
    const uintptr_t top = reinterpret_cast<uintptr_t>(m.BaseAddress) + m.RegionSize;
    int shown = 0;
    for (uintptr_t a = c.Esp; a + 4 <= top && shown < 200; a += 4) {
        const uint32_t w = *reinterpret_cast<const uint32_t*>(a);
        if (w >= base && w < end) {
            std::fprintf(stderr, "STK %08X %08X\n", static_cast<unsigned>(a), w);
            ++shown;
        }
    }
    TerminateProcess(GetCurrentProcess(), 3);
    return 0;
}

// CSF_NO_DIALOGS: the game's message boxes (MessageBoxW/A through the original's IAT slots 0x82D290/0x82D2C4)
// print their text and answer OK. A full-screen run otherwise blocks on an invisible modal box (sound init's error
// dialog from 0x69CE50 looked like a hang).
static int WINAPI no_dialog_a(HWND, LPCSTR text, LPCSTR caption, UINT type);
static int WINAPI no_dialog_w(HWND window, LPCWSTR text, LPCWSTR caption, UINT type) {
    char t[2048]{}, c[256]{};  // UTF-8: %ls stops at the first character outside the C locale (Spanish text)
    if (text) WideCharToMultiByte(CP_UTF8, 0, text, -1, t, sizeof t - 1, nullptr, nullptr);
    if (caption) WideCharToMultiByte(CP_UTF8, 0, caption, -1, c, sizeof c - 1, nullptr, nullptr);
    return no_dialog_a(window, t, c, type);
}
static int WINAPI no_dialog_a(HWND, LPCSTR text, LPCSTR caption, UINT type) {
    std::fprintf(stderr, "csf: MessageBox [%s] %s (type %X): answered OK\n", caption ? caption : "", text ? text : "", type);
    return IDOK;
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR command, int show) {
    std::setvbuf(stderr, nullptr, _IONBF, 0);  // progress lines must survive a crash
    csf::initialize_data();
    char quiet[8]{};
    if (GetEnvironmentVariableA("CSF_NO_DIALOGS", quiet, sizeof quiet) && quiet[0] != '0') {
        *reinterpret_cast<void**>(CSF_ADDR_DATA(0x0082D290)) = reinterpret_cast<void*>(&no_dialog_w);
        *reinterpret_cast<void**>(CSF_ADDR_DATA(0x0082D2C4)) = reinterpret_cast<void*>(&no_dialog_a);
    }
    const int result = csf::run_original_initializers();
    if (result) {
        std::fprintf(stderr, "csf: C initializer failed: %d\n", result);
        return result;
    }
    char dry[8]{};
    if (GetEnvironmentVariableA("CSF_DRYRUN", dry, sizeof dry) == 1 && !std::strcmp(dry, "1")) {
        std::fputs("csf: dry run completed; WinMain not called\n", stderr);
        return 0;
    }
    char hang[16]{};
    if (GetEnvironmentVariableA("CSF_HANG_SECONDS", hang, sizeof hang) && std::atoi(hang) > 0) {
        DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_mainThread, 0, FALSE,
                        DUPLICATE_SAME_ACCESS);
        CreateThread(nullptr, 0, hang_dump, reinterpret_cast<void*>(static_cast<uintptr_t>(std::atoi(hang))), 0, nullptr);
    }
    return Game_WinMain(instance, previous, command, show);
}
