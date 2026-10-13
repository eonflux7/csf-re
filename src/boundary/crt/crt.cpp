// Standalone boundary only. /MT UCRT + vcruntime from the xwin MSVC toolchain.
#ifdef CSF_STANDALONE
#define _CRT_SECURE_NO_WARNINGS
#include "globals.h"
#include "hook.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <cmath>
#include <cctype>
#include <cstdarg>
#include <float.h>
#include <process.h>
#include <malloc.h>
#include <exception>
#include <stdexcept>
#include <new>

// Use concrete wrappers (rather than addresses of overloaded C++ header functions).
#define FORWARD(R, N, P, E) \
    static R __cdecl boundary_##N P { return E; }
FORWARD(void, qsort, (void* p, size_t n, size_t s, int (__cdecl* cmp)(const void*, const void*)), std::qsort(p,n,s,cmp));
HOOK(0x007AEAE0, boundary_qsort, "boundary-crt");
FORWARD(int, atexit, (void (__cdecl* f)()), std::atexit(f));
HOOK(0x007AEE43, boundary_atexit, "boundary-crt");
FORWARD(void*, memcpy, (void* p, const void* q, size_t n), std::memcpy(p,q,n));
HOOK(0x007AEE60, boundary_memcpy, "boundary-crt");
FORWARD(int, vsprintf, (char* p, const char* f, va_list a), std::vsprintf(p,f,a));
HOOK(0x007AFCaa, boundary_vsprintf, "boundary-crt");
FORWARD(int, atoi, (const char* p), std::atoi(p));
HOOK(0x007AFD89, boundary_atoi, "boundary-crt");
FORWARD(double, floor, (double x), std::floor(x));
HOOK(0x007AFD90, boundary_floor, "boundary-crt");
FORWARD(char*, strncpy, (char* p, const char* q, size_t n), std::strncpy(p,q,n));
HOOK(0x007AFEC0, boundary_strncpy, "boundary-crt");
FORWARD(wchar_t*, wcscpy, (wchar_t* p, const wchar_t* q), std::wcscpy(p,q));
HOOK(0x007B004C, boundary_wcscpy, "boundary-crt");
FORWARD(wchar_t*, wcsncpy, (wchar_t* p, const wchar_t* q, size_t n), std::wcsncpy(p,q,n));
HOOK(0x007B0068, boundary_wcsncpy, "boundary-crt");
FORWARD(int, strncmp, (const char* p, const char* q, size_t n), std::strncmp(p,q,n));
HOOK(0x007B00B0, boundary_strncmp, "boundary-crt");
FORWARD(int, fclose, (FILE* p), std::fclose(p));
HOOK(0x007B01D7, boundary_fclose, "boundary-crt");
FORWARD(size_t, wcslen, (const wchar_t* p), std::wcslen(p));
HOOK(0x007B0499, boundary_wcslen, "boundary-crt");
FORWARD(double, atof, (const char* p), std::atof(p));
HOOK(0x007B04AF, boundary_atof, "boundary-crt");
FORWARD(int, rand, (), std::rand());
HOOK(0x007B04E7, boundary_rand, "boundary-crt");
FORWARD(void*, memchr, (const void* p, int c, size_t n), const_cast<void*>(std::memchr(p,c,n)));
HOOK(0x007B0510, boundary_memchr, "boundary-crt");
FORWARD(unsigned, controlfp, (unsigned v, unsigned m), _controlfp(v,m));
HOOK(0x007B070F, boundary_controlfp, "boundary-crt");
FORWARD(uintptr_t, beginthread, (void (__cdecl* f)(void*), unsigned s, void* a), _beginthread(f,s,a));
HOOK(0x007B07F5, boundary_beginthread, "boundary-crt");
FORWARD(int, vsnprintf, (char* p, size_t n, const char* f, va_list a), _vsnprintf(p,n,f,a));
HOOK(0x007B0BEB, boundary_vsnprintf, "boundary-crt");
FORWARD(int, stricmp, (const char* p, const char* q), _stricmp(p,q));
HOOK(0x007B0C41, boundary_stricmp, "boundary-crt");
FORWARD(void, free, (void* p), std::free(p));
HOOK(0x007B17CC, boundary_free, "boundary-crt");
FORWARD(int, tolower, (int c), std::tolower(c));
HOOK(0x007B1905, boundary_tolower, "boundary-crt");
FORWARD(int, isalpha, (int c), std::isalpha(c));
HOOK(0x007B1927, boundary_isalpha, "boundary-crt");
FORWARD(int, isdigit, (int c), std::isdigit(c));
HOOK(0x007B1966, boundary_isdigit, "boundary-crt");
FORWARD(int, isspace, (int c), std::isspace(c));
HOOK(0x007B19DF, boundary_isspace, "boundary-crt");
FORWARD(int, toupper, (int c), std::toupper(c));
HOOK(0x007B1B21, boundary_toupper, "boundary-crt");
FORWARD(char*, strchr, (char* p, int c), std::strchr(p,c));
HOOK(0x007B1B60, boundary_strchr, "boundary-crt");
FORWARD(double, ceil, (double x), std::ceil(x));
HOOK(0x007B1C40, boundary_ceil, "boundary-crt");
FORWARD(void*, malloc, (size_t n), std::malloc(n));
HOOK(0x007B1EC2, boundary_malloc, "boundary-crt");
FORWARD(void*, realloc, (void* p, size_t n), std::realloc(p,n));
HOOK(0x007B1ED4, boundary_realloc, "boundary-crt");
FORWARD(void*, calloc, (size_t n, size_t s), std::calloc(n,s));
HOOK(0x007B2081, boundary_calloc, "boundary-crt");
FORWARD(char*, strstr, (char* p, const char* q), std::strstr(p,q));
HOOK(0x007B2350, boundary_strstr, "boundary-crt");
FORWARD(int, fflush, (FILE* p), std::fflush(p));
HOOK(0x007B2BEF, boundary_fflush, "boundary-crt");
FORWARD(char*, strrchr, (char* p, int c), std::strrchr(p,c));
HOOK(0x007B2E90, boundary_strrchr, "boundary-crt");
FORWARD(void*, bsearch, (const void* k, const void* p, size_t n, size_t s, int (__cdecl* cmp)(const void*, const void*)), std::bsearch(k,p,n,s,cmp));
HOOK(0x007B2EBD, boundary_bsearch, "boundary-crt");
FORWARD(FILE*, fopen, (const char* p, const char* m), std::fopen(p,m));
HOOK(0x007B2FA5, boundary_fopen, "boundary-crt");
FORWARD(size_t, msize, (void* p), _msize(p));
HOOK(0x007B2FB8, boundary_msize, "boundary-crt");
FORWARD(char*, strupr, (char* p), _strupr(p));
HOOK(0x007B302E, boundary_strupr, "boundary-crt");
#undef FORWARD

static void __fastcall boundary_string_xran(void*, void*) {
    throw std::out_of_range(CSF_STR(0x008769F4, "invalid string position"));
}
static void __fastcall boundary_string_xlen(void*, void*) {
    throw std::length_error(CSF_STR(0x00876A0C, "string too long"));
}
HOOK(0x007AE981, boundary_string_xran, "boundary-crt");
HOOK(0x007AE9C1, boundary_string_xlen, "boundary-crt");

static int __cdecl boundary_sprintf(char* p, const char* f, ...) {
    va_list a; va_start(a,f); int r = std::vsprintf(p,f,a); va_end(a); return r;
}
static int __cdecl boundary_snprintf(char* p, size_t n, const char* f, ...) {
    va_list a; va_start(a,f); int r = _vsnprintf(p,n,f,a); va_end(a); return r;
}
static int __cdecl boundary_fprintf(FILE* p, const char* f, ...) {
    va_list a; va_start(a,f); int r = std::vfprintf(p,f,a); va_end(a); return r;
}
static int __cdecl boundary_sscanf(const char* p, const char* f, ...) {
    va_list a; va_start(a,f); int r = std::vsscanf(p,f,a); va_end(a); return r;
}
HOOK(0x007AF9BB, boundary_sprintf, "boundary-crt");
HOOK(0x007B02F5, boundary_snprintf, "boundary-crt");
HOOK(0x007B0228, boundary_fprintf, "boundary-crt");
HOOK(0x007B213C, boundary_sscanf, "boundary-crt");

// VC7.1 exception storage is vptr, message pointer, ownership int (12 bytes).
// Modern x86 std::exception has the same extent; its ownership flag is a bool.
static_assert(sizeof(std::exception) == 12);
static void* __fastcall boundary_exception_ctor(void* p, void*) {
    // Clear padding as old game code can inspect the entire ownership dword.
    std::memset(p,0,sizeof(std::exception));
    return ::new(p) std::exception();
}
static void* __fastcall boundary_exception_copy(void* p, void*, const std::exception* q) {
    std::memset(p,0,sizeof(std::exception));
    return ::new(p) std::exception(*q);
}
static void __fastcall boundary_exception_dtor(std::exception* p, void*) { p->~exception(); }
HOOK(0x007AF6A0, boundary_exception_ctor, "boundary-crt");
HOOK(0x007AF6B1, boundary_exception_copy, "boundary-crt");
HOOK(0x007AF6FB, boundary_exception_dtor, "boundary-crt");

extern "C" void __cdecl _ftol2();
extern "C" int __cdecl __CxxFrameHandler(void*, void*, void*, void*);
extern "C" void __stdcall _CxxThrowException(void*, void*);
// Preserve ST0 input / EDX:EAX output, and EH's non-C EAX FuncInfo argument.
static __declspec(naked) void boundary_ftol2() { __asm jmp _ftol2 }
static __declspec(naked) void boundary_frame_handler() { __asm jmp __CxxFrameHandler }
HOOK(0x007AF86C, boundary_ftol2, "boundary-crt");
HOOK(0x007AF226, boundary_frame_handler, "boundary-crt");
HOOK(0x007AF666, _CxxThrowException, "boundary-crt");

// These compiler helpers exist in sdk/lib/ucrt/x86/libucrt.lib. Naked
// forwarding preserves their x87 operand order and all register conventions.
extern "C" void __cdecl _CIasin();
extern "C" void __cdecl _CIpow();
extern "C" void __cdecl _CIfmod();
static __declspec(naked) void boundary_CIasin() { __asm jmp _CIasin }
static __declspec(naked) void boundary_CIpow() { __asm jmp _CIpow }
static __declspec(naked) void boundary_CIfmod() { __asm jmp _CIfmod }
HOOK(0x007AF8F0, boundary_CIasin, "boundary-crt");
HOOK(0x007AFA60, boundary_CIpow, "boundary-crt");
HOOK(0x007AFFFA, boundary_CIfmod, "boundary-crt");
#endif
