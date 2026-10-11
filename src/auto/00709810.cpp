#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, D3DXAssembleShader, 0x005A1A34, (int a1, int a2, int a3, int a4, int a5, uint32_t* a6, int a7));
// a5 is the "main" entry point string pointer; bound as int so the pointer is not truncated.
CSF_FUNC(int, __stdcall, D3DXCompileShader, 0x005A1B17, (int a1, int a2, int a3, int a4, int a5, int a6, int a7, int* a8, int a9, int* a10));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, OpenSceneFile, 0x00698350, (const char* path, const char* mode, int flags));

static int __stdcall GfxWater_CompileShaderFromFile(int path, uint32_t* outShader, int profile)
{
    *outShader = 0;
    void* file = OpenSceneFile(reinterpret_cast<const char*>(path), CSF_STR(0x008579F8, "rb"), 0);
    if (file) {
        int size = ((int(__fastcall*)(void*, void*))CSF_AT(void**, file, 0)[7])(file, nullptr);
        char* buffer = static_cast<char*>(Mem_Alloc(size + 1));
        ((void(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, file, 0)[2])(file, nullptr, buffer, size);
        buffer[size] = 0;

        int errors = 0;
        int errorsAddr = reinterpret_cast<int>(&errors);
        int cur;
        if (D3DXCompileShader(reinterpret_cast<int>(buffer), size, 0, 0,
                              reinterpret_cast<int>(CSF_STR(0x008634D4, "main")),
                              profile, 0, reinterpret_cast<int*>(outShader), errorsAddr, nullptr)
            && D3DXAssembleShader(reinterpret_cast<int>(buffer), size, 0, 0, 0, outShader, errorsAddr)) {
            cur = errors;
            if (cur) {
                void** obj = reinterpret_cast<void**>(cur);
                ((void(__stdcall*)(int))CSF_AT(void**, obj, 0)[3])(cur);
                cur = errors;
            }
            *outShader = 0;
        } else {
            cur = errors;
        }
        if (cur) {
            void** obj = reinterpret_cast<void**>(cur);
            ((void(__stdcall*)(int))CSF_AT(void**, obj, 0)[2])(cur);
        }
        if (buffer)
            Mem_Free(buffer);
        CloseSceneFile(file);
    }
    return *outShader != 0;
}

HOOK(0x00709810, GfxWater_CompileShaderFromFile, "auto");
