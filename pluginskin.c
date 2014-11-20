#include "pluginskin.h"

// --- cPluginSkin -----------------------------------------------------------

cVector<libvdrskinservice::cPluginSkin*> libvdrskinservice::cPluginSkin::pluginSkins;

libvdrskinservice::cPluginSkin::cPluginSkin(const char *Name, cTheme *Theme)
 : cSkin(Name, Theme)
 , currentDisplayMenu(NULL)
{
  pluginSkins.Append(this);
}

libvdrskinservice::cPluginSkin::~cPluginSkin(void)
{
  for (int i = 0; i < pluginSkins.Size(); i++) {
      if (pluginSkins.At(i) == this) {
         pluginSkins.Remove(i);
         break;
         }
      }
}

cSkinDisplayMenu *libvdrskinservice::cPluginSkin::DisplayMenu(void)
{
  cPluginSkinDisplayMenu *displayMenu = OnDisplayMenu();
  currentDisplayMenu = displayMenu;
  return displayMenu;
}

libvdrskinservice::cPluginSkinDisplayMenu *libvdrskinservice::cPluginSkin::CurrentDisplayMenu(void)
{
  cSkin *current = Skins.Current();
  for (int i = 0; i < pluginSkins.Size(); i++) {
      cPluginSkin *skin = pluginSkins.At(i);
      if (skin == current)
         return skin->currentDisplayMenu;
      }
  return NULL;
}

// --- cPluginSkinOsdMenu ----------------------------------------------------

libvdrskinservice::cPluginSkinOsdMenu::cPluginSkinOsdMenu(const char *PluginName, const char *Title, int c0, int c1, int c2, int c3, int c4)
 : cOsdMenu(Title, c0, c1, c2, c3, c4)
 , init(true)
 , pluginName(PluginName)
 , type(mtList)
 , text("")
{
}

void libvdrskinservice::cPluginSkinOsdMenu::SetPluginMenu(int MenuId, eMenuType Type)
{
  type = Type;
  cPluginSkinDisplayMenu *displayMenu = cPluginSkin::CurrentDisplayMenu();
  if (displayMenu)
     displayMenu->SetPluginMenu(*pluginName, MenuId, Type, init);
  init = false;
}

void libvdrskinservice::cPluginSkinOsdMenu::Display(void)
{
  cPluginSkinDisplayMenu *displayMenu = cPluginSkin::CurrentDisplayMenu();
  if (type == mtText) {
     if (displayMenu && displayMenu->SetMenuValues(stringValues, intValues, loopValues))
        displayMenu->Flush();
     else {
        DisplayMenu()->Clear();
        DisplayMenu()->SetText(*text, false);
        DisplayMenu()->Flush();
        }
     return;
     }
  cOsdMenu::Display();
}

// --- cPluginSkinOsdItem ----------------------------------------------------

libvdrskinservice::cPluginSkinOsdItem::cPluginSkinOsdItem(eOSState State)
 : cOsdItem(State)
{
}

libvdrskinservice::cPluginSkinOsdItem::~cPluginSkinOsdItem(void)
{
}

void libvdrskinservice::cPluginSkinOsdItem::SetMenuItem(cSkinDisplayMenu *DisplayMenu, int Index, bool Current, bool Selectable)
{
  cPluginSkinDisplayMenu *displayMenu = cPluginSkin::CurrentDisplayMenu();
  if ((displayMenu == NULL) || !displayMenu->SetItemPlugin(stringValues, intValues, loopValues, Index, Current, Selectable))
     DisplayMenu->SetItem(Text(), Index, Current, Selectable);
}