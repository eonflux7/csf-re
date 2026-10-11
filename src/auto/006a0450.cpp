#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Win32 types spelled with fundamental types, matching the kernel32 ABI (LONG = int32, DWORD = uint32, HANDLE = void*).
using FileHandleValue = void*;
using Win32Long = int32_t;
using Win32Dword = uint32_t;
using Win32LongPtr = int32_t*;

// Import slot the original calls through (IAT entry for kernel32!SetFilePointer).
using SetFilePointerFn = Win32Long(__stdcall*)(FileHandleValue, Win32Long, Win32LongPtr, Win32Dword);
CSF_GLOBAL(SetFilePointerFn, g_SetFilePointerImport, 0x82D230);

// Seek origin values used by the original: 1 = FILE_CURRENT, 2 = FILE_END, 0 = FILE_BEGIN.
static constexpr Win32Dword kSeekFromCurrent = 1;
static constexpr Win32Dword kSeekFromEnd = 2;
static constexpr Win32Dword kSeekFromBegin = 0;

// Returns true when the file position equals the file size, i.e. the read cursor is at EOF.
// The original saves the current position, seeks to the end, and restores the saved position.
static bool __fastcall C_0085ec74_IsFilePositionAtEnd(C_0085ec74* self, void* /*edx*/)
{
    FileHandleValue fileHandle = reinterpret_cast<FileHandleValue>(static_cast<uintptr_t>(self->field_0004));

    Win32Long savedPosition = g_SetFilePointerImport(fileHandle, 0, nullptr, kSeekFromCurrent);
    Win32Dword endPosition = static_cast<Win32Dword>(g_SetFilePointerImport(fileHandle, 0, nullptr, kSeekFromEnd));
    g_SetFilePointerImport(fileHandle, savedPosition, nullptr, kSeekFromBegin);

    return endPosition == static_cast<Win32Dword>(savedPosition);
}

HOOK(0x6A0450, C_0085ec74_IsFilePositionAtEnd, "auto");
