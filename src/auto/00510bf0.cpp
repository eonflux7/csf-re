#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GameBddLoader_AllocateTreeHead, 0x0050E9D0, ());
CSF_FUNC(uint32_t*, __fastcall, GameBddLoader_ConstructMaterials, 0x005102D0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, GameBddLoader_ConstructSounds, 0x00510420, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, GameBddLoader_ConstructDatabase, 0x00510680, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GameBddLoader_LogDatabase, 0x00519C90, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, GameBddLoader_LogMaterials, 0x0051AFD0, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, GameBddLoader_ConstructEffectsBase, 0x0051B4C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GameBddLoader_LogEffects, 0x0051B520, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, GameBddLoader_ConstructObjectsBase, 0x0051B600, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GameBddLoader_LogObjects, 0x0051B6F0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, GameBddLoader_LogSounds, 0x0051B950, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, GameBddLoader_LogWeapons, 0x0051BB30, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, GameBddLoader_ConstructWeaponsBase, 0x0051BF20, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GameBddLoader_FinalizeLoading, 0x005370C0, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, LengthPrefixedBuffer_TerminateAtLength, 0x00694C20, (void* self, void*));
CSF_FUNC(void**, __fastcall, LoadState_Construct, 0x00694C80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginRecord, 0x006951B0, (void* self, void* edx, char a2, char a3, int a4));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup_6951D0, 0x006951D0, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, OpenSceneFile, 0x00698350, (const char* path, const char* mode, int flags));
CSF_FUNC(void, __fastcall, GameBddLoader_WriteLoadingLog, 0x006986E0, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, ResetEmbeddedMemberAt8, 0x00712D10, (void** self, void* edx));

CSF_GLOBAL(const char*, g_pszMenu_P_Juego, 0x00899AF0);

static char __fastcall GameBddLoader_RebuildDatabases(void* self, void* /*edx*/)
{
    void* object = CSF_AT(void*, self, 0xE0);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
            (object, nullptr, 1);
        CSF_AT(void*, self, 0xE0) = nullptr;
    }

    object = CSF_AT(void*, self, 0xD4);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
            (object, nullptr, 1);
        CSF_AT(void*, self, 0xD4) = nullptr;
    }

    object = CSF_AT(void*, self, 0xDC);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
            (object, nullptr, 1);
        CSF_AT(void*, self, 0xDC) = nullptr;
    }

    object = CSF_AT(void*, self, 0xD8);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
            (object, nullptr, 1);
        CSF_AT(void*, self, 0xD8) = nullptr;
    }

    object = CSF_AT(void*, self, 0xE4);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
            (object, nullptr, 1);
        CSF_AT(void*, self, 0xE4) = nullptr;
    }

    object = CSF_AT(void*, self, 0xE8);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])
            (object, nullptr, 1);
        CSF_AT(void*, self, 0xE8) = nullptr;
    }

    object = Mem_Alloc(36);
    CSF_AT(void*, self, 0xD8) =
        object ? GameBddLoader_ConstructDatabase(object, nullptr) : nullptr;

    object = Mem_Alloc(24);
    CSF_AT(void*, self, 0xDC) =
        object ? GameBddLoader_ConstructSounds(object, nullptr, CSF_AT(int, self, 0xD8))
               : nullptr;

    object = Mem_Alloc(56);
    CSF_AT(void*, self, 0xD4) =
        object ? GameBddLoader_ConstructMaterials(object, nullptr, CSF_AT(int, self, 0xD8))
               : nullptr;

    object = Mem_Alloc(20);
    if (object)
    {
        GameBddLoader_ConstructEffectsBase(object, nullptr);
        CSF_AT(void**, object, 0) = CSF_VTBL(0x00840698);
        void* head = reinterpret_cast<void*>(GameBddLoader_AllocateTreeHead());
        CSF_AT(void*, object, 0x0C) = head;
        CSF_AT(std::uint8_t, head, 0x11) = 1;
        head = CSF_AT(void*, object, 0x0C);
        CSF_AT(void*, head, 4) = head;
        head = CSF_AT(void*, object, 0x0C);
        CSF_AT(void*, head, 0) = head;
        head = CSF_AT(void*, object, 0x0C);
        CSF_AT(void*, head, 8) = head;
        CSF_AT(std::uint32_t, object, 0x10) = 0;
    }
    CSF_AT(void*, self, 0xE0) = object;

    object = Mem_Alloc(36);
    if (object)
    {
        GameBddLoader_ConstructWeaponsBase(object, nullptr);
        CSF_AT(void**, object, 0) = CSF_VTBL(0x008406B0);
        void* head = reinterpret_cast<void*>(GameBddLoader_AllocateTreeHead());
        CSF_AT(void*, object, 0x1C) = head;
        CSF_AT(std::uint8_t, head, 0x11) = 1;
        head = CSF_AT(void*, object, 0x1C);
        CSF_AT(void*, head, 4) = head;
        head = CSF_AT(void*, object, 0x1C);
        CSF_AT(void*, head, 0) = head;
        head = CSF_AT(void*, object, 0x1C);
        CSF_AT(void*, head, 8) = head;
        CSF_AT(std::uint32_t, object, 0x20) = 0;
    }
    CSF_AT(void*, self, 0xE4) = object;

    object = Mem_Alloc(100);
    if (object)
    {
        GameBddLoader_ConstructObjectsBase(object, nullptr);
        CSF_AT(void**, object, 0) = CSF_VTBL(0x008406D0);
        void* head = reinterpret_cast<void*>(GameBddLoader_AllocateTreeHead());
        CSF_AT(void*, object, 0x5C) = head;
        CSF_AT(std::uint8_t, head, 0x11) = 1;
        head = CSF_AT(void*, object, 0x5C);
        CSF_AT(void*, head, 4) = head;
        head = CSF_AT(void*, object, 0x5C);
        CSF_AT(void*, head, 0) = head;
        head = CSF_AT(void*, object, 0x5C);
        CSF_AT(void*, head, 8) = head;
        CSF_AT(std::uint32_t, object, 0x60) = 0;
    }
    CSF_AT(void*, self, 0xE8) = object;

    CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalInt()), 4) = 1;

    object = CSF_AT(void*, self, 0x14);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, object, 0)[144 / 4])
        (object, nullptr);

    object = CSF_AT(void*, self, 0x14);
    ((void(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, object, 0)[96 / 4])
        (object, nullptr, CSF_AT(void*, self, 0x30), 0);

    object = CSF_AT(void*, self, 0x14);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, object, 0)[148 / 4])
        (object, nullptr);

    object = CSF_AT(void*, self, 0x14);
    ((void(__fastcall*)(void*, void*, const char*))CSF_AT(void**, object, 0)[88 / 4])
        (object, nullptr, g_pszMenu_P_Juego);

    object = CSF_AT(void*, self, 0x14);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, object, 0)[144 / 4])
        (object, nullptr);

    object = CSF_AT(void*, self, 0x18);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[16 / 4])
        (object, nullptr, 0);

    GameBddLoader_FinalizeLoading();
    GameBddLoader_WriteLoadingLog(0, 0);

    alignas(8) std::uint8_t logBytes[0x918];
    S_00694cd0& log = *reinterpret_cast<S_00694cd0*>(logBytes);
    LoadState_Construct(&log, nullptr);
    CoreZlibLog_BeginRecord(&log, nullptr, 0, 0, 0);
    GameBddLoader_LogDatabase(CSF_AT(void*, self, 0xD8), nullptr, &log);
    GameBddLoader_LogSounds(CSF_AT(void*, self, 0xDC), nullptr, &log);
    GameBddLoader_LogMaterials(CSF_AT(void*, self, 0xD4), nullptr, &log);
    GameBddLoader_LogEffects(CSF_AT(void*, self, 0xE0), nullptr, &log);
    GameBddLoader_LogWeapons(CSF_AT(void*, self, 0xE4), nullptr, &log);
    GameBddLoader_LogObjects(CSF_AT(void*, self, 0xE8), nullptr, &log);
    CoreZlibLog_EndGroup_6951D0(&log, nullptr);

    void* file = OpenSceneFile(
        CSF_STR(0x00840744, "BDDS.log"), CSF_STR(0x00840044, "w"), 0);
    if (file)
    {
        void** fileVtable = CSF_AT(void**, file, 0);
        std::uint32_t length = log.field_000c;
        int buffer = LengthPrefixedBuffer_TerminateAtLength(&log, nullptr);
        ((void(__fastcall*)(void*, void*, int, std::uint32_t))fileVtable[12 / 4])
            (file, nullptr, buffer, length);
        CloseSceneFile(file);
    }

    object = CSF_AT(void*, self, 0x14);
    ((void(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, object, 0)[96 / 4])
        (object, nullptr, CSF_AT(void*, self, 0x18), 0);

    object = CSF_AT(void*, self, 0x14);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, object, 0)[148 / 4])
        (object, nullptr);

    return static_cast<char>(reinterpret_cast<std::uintptr_t>(
        ResetEmbeddedMemberAt8(reinterpret_cast<void**>(&log), nullptr)));
}

HOOK(0x00510BF0, GameBddLoader_RebuildDatabases, "auto");
