#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, MenuMessageBox_UpdateHorizontalLayout, 0x00529280, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMessageBox_UpdateLayout, 0x00529360, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuMessageBox_ApplyChildBounds, 0x00529590, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMessageBox_UpdateStaticLayout, 0x00529610, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuMessageBox_UpdateLayerLayout, 0x00529770, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuMessageBox_UpdateCombinedHeight, 0x00529A90, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuMessageBox_UpdateLayout, 0x0052A830, (void* self, void* edx));

// Slot 0x1CC/4 = 115 on the child object at +0x894.
// Slot 0x28/4 = 10 and 0x20/4 = 8 on the returned object.
static uint32_t* __fastcall CMenuMessageBox_RefreshLayoutAndCacheBounds(void* self, void* /*edx*/)
{
    MenuMessageBox_UpdateHorizontalLayout(self, nullptr);
    CMenuMessageBox_UpdateLayout(self, nullptr);
    MenuMessageBox_ApplyChildBounds(self, nullptr);
    CMenuMessageBox_UpdateStaticLayout(self, nullptr);
    MenuMessageBox_UpdateLayout(self, nullptr);
    MenuMessageBox_UpdateLayerLayout(self, nullptr);
    MenuMessageBox_UpdateCombinedHeight(self, nullptr);

    void* childObj = CSF_AT(void*, self, 0x894);
    void* queryObj = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, childObj, 0)[115])(childObj, nullptr);
    if (queryObj == nullptr)
    {
        return nullptr;
    }

    uint32_t* firstPair = ((uint32_t* (__fastcall*)(void*, void*))CSF_AT(void**, queryObj, 0)[10])(queryObj, nullptr);
    CSF_AT(uint32_t, self, 0x990) = firstPair[0];
    CSF_AT(uint32_t, self, 0x994) = firstPair[1];

    uint32_t* secondPair = ((uint32_t* (__fastcall*)(void*, void*))CSF_AT(void**, queryObj, 0)[8])(queryObj, nullptr);
    CSF_AT(uint32_t, self, 0x988) = secondPair[0];
    CSF_AT(uint32_t, self, 0x98C) = secondPair[1];

    return secondPair;
}

HOOK(0x0052AE50, CMenuMessageBox_RefreshLayoutAndCacheBounds, "auto");
