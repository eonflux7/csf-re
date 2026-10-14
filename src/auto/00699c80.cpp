#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Every callee below is called exactly as the original call site does it (convention, stack arguments, and the
// cleanup the callee performs: a `call dword ptr` without `add esp` leaves the pops to the callee, so the call
// through a __fastcall pointer with the same stack arguments is the same stack layout).
CSF_FUNC(int, __fastcall, C_0082d410_AssignResourceString, 0x00429C60, (void* self, void* edx, const char* text));
CSF_FUNC(int, __fastcall, S_0047ea40_DestroyPointerStorage, 0x00558790, (S_0047ea40* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, const char* mode, int openFlags));
CSF_FUNC(int, __cdecl, CoreFiles_CheckPageAvailability, 0x00698B20, (const char* path, char bypassList));
CSF_FUNC(char, __fastcall, S_00698c10_IsAtEnd, 0x00699020, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, S_00698c10_BeginParsing, 0x006991F0, (void* self, void* edx));
CSF_FUNC(void, __cdecl, CoreSortedTable_SortEntries, 0x00699220, (int a1));
// Returns bool in al; declared with the full eax so the function can return eax as the original does.
CSF_FUNC(std::uint32_t, __fastcall, S_00698c10_FindEntryByPath, 0x00699490, (void* self, void* edx, const char* name));
CSF_FUNC(char, __fastcall, S_00698c10_ParseNextEntry, 0x00699B10, (void* self, void* edx, void* entry));

// The original returns eax (the Hex-Rays prototype is `int ... @<eax>`), so every return below is the full eax of
// the path it takes: a check value, the bool-returning callee's eax, 0, or the eax left by CoreMemory_Free.
static int __fastcall S_00698c10_InitializeEntryFromPath(S_00698c10* self, void* /*edx*/, const char* path)
{
    // 0x699c80: push ebp ... sub esp,0x14 ; push esi ; push edi ; mov edi,[ebp+8] (path) ; push 0 ; push edi ; mov esi,ecx
    // 0x699c90: call 0x698b20 (cdecl, path, 0) ; add esp,8 ; test al,al ; je 0x699d98 (return eax)
    const int check = CoreFiles_CheckPageAvailability(path, 0);
    if (!static_cast<std::uint8_t>(check))
        return check;

    // 0x699ca0: push edi ; mov ecx,esi ; call 0x699490 (thiscall, path on the stack, ret 4) ; test al,al ; jne 0x699d98
    const std::uint32_t found = S_00698c10_FindEntryByPath(self, nullptr, path);
    if (static_cast<std::uint8_t>(found))
        return static_cast<int>(found);

    // 0x699cb0: push ebx ; push 0 ; push 0x83e130 ("r") ; push edi ; call 0x698350 (cdecl) ; mov ebx,eax ; add esp,0xc
    void* file = CoreFiles_OpenSceneFile(path, CSF_STR(0x0083E130, "r"), 0);
    // 0x699cc3: test ebx,ebx ; je 0x699d97 (pop ebx ... ret 4, with eax = 0)
    if (!file)
        return 0;

    // 0x699ccb: mov eax,[ebx] ; mov ecx,ebx ; call [eax+0x1c] (thiscall, no args) ; shr eax,1 ; lea edi,[eax+eax]
    const std::uint32_t size = reinterpret_cast<std::uint32_t(__fastcall*)(void*, void*)>(
        CSF_AT(void**, file, 0)[0x1C / 4])(file, nullptr);
    const std::uint32_t byteCount = (size >> 1) * 2;

    // 0x699cd7: push edi ; call 0x6910f0 (cdecl, one arg) ; add esp,4 ; mov [esi+0x18],eax
    self->field_0018 = static_cast<std::uint32_t>(CoreMemory_AllocUntagged(byteCount));

    // 0x699ce0: push edi ; mov [esi+0x18],eax ; mov edx,[ebx] ; push eax ; mov ecx,ebx ; call [edx+8]
    // The pushes are size then buffer, so the callee sees (buffer, size); no add esp follows, the callee pops them.
    reinterpret_cast<void(__fastcall*)(void*, void*, void*, std::uint32_t)>(
        CSF_AT(void**, file, 0)[8 / 4])(
            file, CSF_AT(void*, file, 0), reinterpret_cast<void*>(self->field_0018), byteCount);

    // 0x699cec: mov eax,[esi+0x18] ; lea ecx,[eax+edi] ; mov [esi+0x1c],ecx ; mov ecx,esi ; mov [esi+0x14],eax
    self->field_001c = self->field_0018 + byteCount;
    CSF_AT(std::uint32_t, self, 0x14) = self->field_0018;

    // [ebp-0x14] = the 0x10-byte entry (S_0047ea40); [ebp-2] = parsing flag (+0x12), [ebp-1] = reached-end flag (+0x13)
    alignas(8) std::uint8_t entryBytes[0x14];
    S_0047ea40& entry = *reinterpret_cast<S_0047ea40*>(entryBytes);
    char& parsing = CSF_AT(char, entryBytes, 0x12);
    char& reached = CSF_AT(char, entryBytes, 0x13);

    // 0x699cfa: call 0x6991f0 (thiscall, ecx=esi, ret 0) ; xor edi,edi ; test al,al ; mov [ebp-2],al ; je 0x699d7b
    parsing = static_cast<char>(S_00698c10_BeginParsing(self, nullptr));

    if (parsing)
    {
        // 0x699d08: mov edx,[esi] ; mov [ebp-1],0 ; mov [ebp-0x10],edi ; mov [ebp-0xc],edi ; mov [ebp-8],edi
        // The original writes entry+4 .. entry+0xF (12 bytes, memset) and never entry+0.
        reached = 0;
        std::memset(&entry.field_0004, 0, 12);

        // 0x699d17: call dword ptr [edx+8] with edx = [esi] (the callbacks table) and ecx = esi (left by 0x6991f0);
        // no stack arguments, the callee pops none.
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            reinterpret_cast<void**>(self->field_0000)[8 / 4])(
                self, reinterpret_cast<void*>(self->field_0000));

        // do { if (!reached) reached = SkipBlanks(this)  [0x699d1a..0x699d28]
        //      if (!reached) parsing = ParseNext(this, &entry)  [0x699d2d..0x699d38]
        // } while (parsing && !reached)  [0x699d3b..0x699d47]
        do
        {
            if (!reached)
                reached = S_00698c10_IsAtEnd(self, nullptr);   // 0x699d21: mov ecx,esi ; call 0x699020

            if (!reached)
                parsing = S_00698c10_ParseNextEntry(self, nullptr, &entry);  // 0x699d2f: lea eax,[ebp-0x14] ; push eax ; call 0x699b10 (ret 4)
        }
        while (parsing && !reached);

        // 0x699d49: mov ecx,[ebp+8] ; push ecx ; lea ecx,[esi+0x3c] ; call 0x429c60 (ret 4, path on the stack)
        C_0082d410_AssignResourceString(reinterpret_cast<std::uint8_t*>(self) + 0x3C, nullptr, const_cast<char*>(path));

        // 0x699d55: mov edx,[esi] ; lea eax,[ebp-0x14] ; push eax ; mov ecx,esi ; call dword ptr [edx+0x14]
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            reinterpret_cast<void**>(self->field_0000)[0x14 / 4])(
                self, reinterpret_cast<void*>(self->field_0000), &entry);

        // 0x699d60: lea ecx,[esi+4] ; push ecx ; call 0x699220 (cdecl) ; add esp,4
        CoreSortedTable_SortEntries(static_cast<int>(reinterpret_cast<std::uintptr_t>(
            reinterpret_cast<std::uint8_t*>(self) + 4)));

        // 0x699d69: mov edx,[esi] ; mov ecx,esi ; call dword ptr [edx+0xc]
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            reinterpret_cast<void**>(self->field_0000)[0x0C / 4])(
                self, reinterpret_cast<void*>(self->field_0000));

        // 0x699d73: lea ecx,[ebp-0x14] ; call 0x558790 (ret 0)
        S_0047ea40_DestroyPointerStorage(&entry, nullptr);
    }

    // 0x699d7b: push ebx ; call 0x697980 (cdecl, file) ; add esp,4 (the return value is not used)
    CloseSceneFile(file);

    // 0x699d81: mov eax,[esi+0x14] ; cmp eax,edi (edi = 0) ; je 0x699d97 (eax = 0 is returned)
    const std::uint32_t storage = CSF_AT(std::uint32_t, self, 0x14);
    if (storage == 0)
        return 0;

    // 0x699d8b: push eax ; call 0x691110 (cdecl) ; add esp,4 ; mov [esi+0x14],edi ; the return is the eax of the free
    const int freed = CoreMemory_Free(reinterpret_cast<void*>(storage));
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    return freed;
}

HOOK(0x00699C80, S_00698c10_InitializeEntryFromPath, "auto");
