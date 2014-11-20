#include "pluginskin.h"

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
  cPluginSkinDisplayMenu *dm = dynamic_cast<cPluginSkinDisplayMenu*>(DisplayMenu());
  if (dm)
     dm->SetPluginMenu(*pluginName, MenuId, Type, init);
  init = false;
}

void libvdrskinservice::cPluginSkinOsdMenu::Display(void)
{
  if (type == mtText) {
     cSkinDisplayMenu *displayMenu = DisplayMenu();
     cPluginSkinDisplayMenu *dm = dynamic_cast<cPluginSkinDisplayMenu*>(displayMenu);

     if ((dm == NULL) || !dm->SetMenuValues(stringValues, intValues, loopValues)) {
        displayMenu->Clear();
        displayMenu->SetText(*text, false);
        }
     displayMenu->Flush();
     }
  else
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
  cPluginSkinDisplayMenu *dm = dynamic_cast<cPluginSkinDisplayMenu*>(DisplayMenu);
  if ((dm == NULL) || !dm->SetItemValues(stringValues, intValues, loopValues, Index, Current, Selectable))
     DisplayMenu->SetItem(Text(), Index, Current, Selectable);
}
