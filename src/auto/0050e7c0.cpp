#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

static int __fastcall GameBddLoader_PrepareMenuForScene(void* self, void* /*edx*/)
{
    void* database = CSF_AT(void*, self, 0xE0);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, database, 0)[5])(database, nullptr);

    database = CSF_AT(void*, self, 0xD4);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, database, 0)[4])(database, nullptr);

    database = CSF_AT(void*, self, 0xDC);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, database, 0)[5])(database, nullptr);

    database = CSF_AT(void*, self, 0xD8);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, database, 0)[3])(database, nullptr);

    database = CSF_AT(void*, self, 0xE4);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, database, 0)[5])(database, nullptr);

    database = CSF_AT(void*, self, 0xE8);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, database, 0)[5])(database, nullptr);

    char buffer[0x100];
    std::memset(buffer, 0, sizeof(buffer));

    _sprintf(buffer, CSF_STR(0x008405AC, "%s\\Anims.bdd"),
             reinterpret_cast<const char*>(CSF_ADDR_DATA(0x008405BC)));
    database = CSF_AT(void*, self, 0xD8);
    ((int(__fastcall*)(void*, void*, char*))CSF_AT(void**, database, 0)[1])(
        database, nullptr, buffer);

    _sprintf(buffer, CSF_STR(0x0084059C, "%s\\Objetos.bdd"),
             reinterpret_cast<const char*>(CSF_ADDR_DATA(0x008405BC)));
    database = CSF_AT(void*, self, 0xDC);
    ((int(__fastcall*)(void*, void*, char*))CSF_AT(void**, database, 0)[1])(
        database, nullptr, buffer);

    _sprintf(buffer, CSF_STR(0x0084058C, "%s\\Armas.bdd"),
             reinterpret_cast<const char*>(CSF_ADDR_DATA(0x008405BC)));
    database = CSF_AT(void*, self, 0xD4);
    ((int(__fastcall*)(void*, void*, char*))CSF_AT(void**, database, 0)[1])(
        database, nullptr, buffer);

    _sprintf(buffer, CSF_STR(0x0084057C, "%s\\Efectos.bdd"),
             reinterpret_cast<const char*>(CSF_ADDR_DATA(0x008405BC)));
    database = CSF_AT(void*, self, 0xE0);
    ((int(__fastcall*)(void*, void*, char*))CSF_AT(void**, database, 0)[1])(
        database, nullptr, buffer);

    _sprintf(buffer, CSF_STR(0x0084056C, "%s\\Sonidos.bdd"),
             reinterpret_cast<const char*>(CSF_ADDR_DATA(0x008405BC)));
    database = CSF_AT(void*, self, 0xE4);
    ((int(__fastcall*)(void*, void*, char*))CSF_AT(void**, database, 0)[1])(
        database, nullptr, buffer);

    _sprintf(buffer, CSF_STR(0x00840558, "%s\\Materiales.bdd"),
             reinterpret_cast<const char*>(CSF_ADDR_DATA(0x008405BC)));
    database = CSF_AT(void*, self, 0xE8);
    return ((int(__fastcall*)(void*, void*, char*))CSF_AT(void**, database, 0)[1])(
        database, nullptr, buffer);
}

HOOK(0x0050E7C0, GameBddLoader_PrepareMenuForScene, "auto");
