#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Localized caption strings selected by the current option variant.
CSF_GLOBAL(const char*, g_pszMenu_P_Option, 0x00899ACC);
CSF_GLOBAL(const char*, g_pszMenu_P_OptIng, 0x00899AD4);

// Re-initialises the configuration listbox with this screen and then assigns it
// the caption matching the current "in-game" / "out-of-game" option variant.
static int __fastcall CMenuControls_UpdateOptionText(CMenuControls* self, void* /*edx*/)
{
  // Give the listbox its owning screen (second data argument is null).
  {
    void* const listbox = reinterpret_cast<void*>(self->field_0018);
    void** const listboxVtbl = *reinterpret_cast<void***>(listbox);
    ((void(__fastcall*)(void*, void*, CMenuControls*, int))listboxVtbl[0x60 / 4])(listbox, nullptr, self, 0);
  }

  // Ask the screen whether it is currently in "in-game" mode.
  void** const selfVtbl = *reinterpret_cast<void***>(self);
  const unsigned char useInGameText =
      ((unsigned char(__fastcall*)(CMenuControls*, void*))selfVtbl[0x30 / 4])(self, nullptr);

  const char* text = g_pszMenu_P_OptIng;
  if (useInGameText == 0)
    text = g_pszMenu_P_Option;

  // Push the chosen caption onto the listbox.
  void* const listbox = reinterpret_cast<void*>(self->field_0018);
  void** const listboxVtbl = *reinterpret_cast<void***>(listbox);
  return ((int(__fastcall*)(void*, void*, const char*))listboxVtbl[0x58 / 4])(listbox, nullptr, text);
}

HOOK(0x00542B70, CMenuControls_UpdateOptionText, "auto");
