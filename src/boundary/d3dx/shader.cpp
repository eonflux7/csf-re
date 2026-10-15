// Standalone boundary only. The original links D3DX9's shader compiler statically (db/regions.yaml: d3dx9,
// compiler 5.04.00.3900); the game calls two entries of it to build its water and effect shaders from source
// (0x709810: compile with the "main" entry, assembler as the fallback, no constant table). Library code is not
// reimplemented: both forward to the D3DX9 runtime DLL (d3dx9_43.dll, DirectX End-User Runtime), loaded on first
// use. ID3DXBuffer has the same COM layout the game calls (Release at slot 2, GetBufferPointer/Size at 3/4).
#ifdef CSF_STANDALONE
#include <windows.h>
#include <cstdio>
#include <initializer_list>
#include "globals.h"
#include "hook.h"

namespace {
using CompileFn = HRESULT(WINAPI*)(const char*, UINT, const void*, void*, const char*, const char*, DWORD, void**,
                                   void**, void**);
using AssembleFn = HRESULT(WINAPI*)(const char*, UINT, const void*, void*, DWORD, void**, void**);

HMODULE runtime() {
    static HMODULE module;
    static bool tried;
    if (!tried) {
        tried = true;
        for (const char* name : {"d3dx9_43.dll", "d3dx9_42.dll", "d3dx9_41.dll"})
            if ((module = LoadLibraryA(name)))
                break;
        if (!module)
            std::fprintf(stderr, "csf: D3DX9 runtime not found (d3dx9_43.dll): shaders cannot be compiled\n");
    }
    return module;
}

HRESULT WINAPI CompileShader(const char* source, UINT length, const void* defines, void* include, const char* entry,
                             const char* profile, DWORD flags, void** shader, void** errors, void** constants) {
    auto compile = runtime() ? reinterpret_cast<CompileFn>(GetProcAddress(runtime(), "D3DXCompileShader")) : nullptr;
    if (!compile) {
        if (shader) *shader = nullptr;
        if (errors) *errors = nullptr;
        if (constants) *constants = nullptr;
        return E_FAIL;
    }
    return compile(source, length, defines, include, entry, profile, flags, shader, errors, constants);
}

HRESULT WINAPI AssembleShader(const char* source, UINT length, const void* defines, void* include, DWORD flags,
                              void** shader, void** errors) {
    auto assemble = runtime() ? reinterpret_cast<AssembleFn>(GetProcAddress(runtime(), "D3DXAssembleShader")) : nullptr;
    if (!assemble) {
        if (shader) *shader = nullptr;
        if (errors) *errors = nullptr;
        return E_FAIL;
    }
    return assemble(source, length, defines, include, flags, shader, errors);
}
}  // namespace

HOOK(0x005A1B17, CompileShader, "boundary-d3dx");
HOOK(0x005A1A34, AssembleShader, "boundary-d3dx");
#endif
