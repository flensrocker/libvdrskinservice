#include "pluginskin.h"

// --- cPluginSkinDisplayMenu ------------------------------------------------

libvdrskinservice::cPluginSkinDisplayMenu::cPluginSkinDisplayMenu(void)
{
}

libvdrskinservice::cPluginSkinDisplayMenu::~cPluginSkinDisplayMenu(void)
{
}

void libvdrskinservice::cPluginSkinDisplayMenu::SetPluginMenu(const char *PluginName, int MenuId, int Type, bool init)
{
}

bool libvdrskinservice::cPluginSkinDisplayMenu::SetItemValues(const cKeyValueContainer &Values, int Index, bool Current, bool Selectable)
{
  return false;
}

bool libvdrskinservice::cPluginSkinDisplayMenu::SetTextValues(const cKeyValueContainer &Values)
{
  return false;
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

libvdrskinservice::cPluginSkinOsdMenu::~cPluginSkinOsdMenu(void)
{
}

bool libvdrskinservice::cPluginSkinOsdMenu::Init(void) const
{
  return init;
}

const char *libvdrskinservice::cPluginSkinOsdMenu::PluginName(void) const
{
  return *pluginName;
}

libvdrskinservice::eMenuType libvdrskinservice::cPluginSkinOsdMenu::MenuType(void) const
{
  return type;
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

     if ((dm == NULL) || !dm->SetTextValues(*this)) {
        displayMenu->Clear();
        displayMenu->SetText(*text, false);
        }
     displayMenu->Flush();
     }
  else
     cOsdMenu::Display();
}

void libvdrskinservice::cPluginSkinOsdMenu::SetText(const char *Text)
{
  text = Text;
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
  if ((dm == NULL) || !dm->SetItemValues(*this, Index, Current, Selectable))
     DisplayMenu->SetItem(Text(), Index, Current, Selectable);
}
