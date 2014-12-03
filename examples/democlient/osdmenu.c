#include <vdr/osdbase.h>

// Don't change these IDs, they're used by the skin
// to distinguish the different menus of this plugin.
enum eMenus {
  meListMain = 1,
  meListSub = 2,
  meDetail = 3
};

class cPlugOsdMenu : public libvdrskinservice::cPluginSkinOsdMenu {
private:
  int level;
  int numItems;
  bool isSubfolder;

  void SetMenu(void);

public:
  cPlugOsdMenu(int Level = 0, int NumItems = 10, bool IsSubfolder = false);
  virtual ~cPlugOsdMenu();
  virtual eOSState ProcessKey(eKeys key);
};


class cPlugDetailOsdMenu : public libvdrskinservice::cPluginSkinOsdMenu {
private:
  int element;

  void SetDetail(void);

public:
  cPlugDetailOsdMenu(int Element);
  virtual ~cPlugDetailOsdMenu();
  virtual eOSState ProcessKey(eKeys key);
};


cPlugOsdMenu::cPlugOsdMenu(int Level, int NumItems, bool IsSubfolder)
 : libvdrskinservice::cPluginSkinOsdMenu("democlient", "Demo Client")
 , level(Level)
 , numItems(NumItems)
 , isSubfolder(IsSubfolder)
{
  SetMenu();
}

cPlugOsdMenu::~cPlugOsdMenu(void)
{
}

eOSState cPlugOsdMenu::ProcessKey(eKeys key)
{
  eOSState state = cOsdMenu::ProcessKey(key);
  if (state == osUnknown) {
     switch (key) {
       case kOk: {
         int element = Current();
         if (element % 2)
            state = AddSubMenu(new cPlugDetailOsdMenu(element));
         else
            state = AddSubMenu(new cPlugOsdMenu(level + 1, element + 1, true));
         break;
         }
       default:
         break;
       }
     }
  return state;
}

void cPlugOsdMenu::SetMenu()
{
  eMenus menu = isSubfolder ? meListSub : meListMain;
  SetPluginMenu(menu, libvdrskinservice::mtList);
  Clear();
  ClearValues();

  for (int i = 0; i < numItems; i++) {
      libvdrskinservice::cPluginSkinOsdItem *item = new libvdrskinservice::cPluginSkinOsdItem();
      //add some tokens to the menu item
      cString text;
      if (i % 2)
         text = cString::sprintf("DetailItem %d/%d", level + 1, i + 1);
      else
         text = cString::sprintf("FolderItem %d/%d", level + 1, i + 1);
      item->SetText(*text);
      item->AddInt("itemnumber", i);
      item->AddString("menuitemtext", *text);

      cString text2 = "";
      for (int j = 0; j < 6; j++)
          text2 = cString::sprintf("%sCurrentItemText %d\n", *text2, i + 1);
      item->AddString("currentitemtext", text2);
      
      //Loop Token Example
      for (int row = 0; row < 20; row++) {
          libvdrskinservice::cKeyValueList<cString> *loopValues = new libvdrskinservice::cKeyValueList<cString>();
          for (int col = 0; col < 3; col++) {
              cString key = cString::sprintf("loop1[col%d]", col);
              cString *value = new cString(cString::sprintf("menuitem%d-%dx%d", i, row, col));
              loopValues->AddKeyValue(*key, value);
          }
          item->AddLoopValues("loop1", loopValues);
      }

      Add(item, (i == 0));
  }

  Display();
}


cPlugDetailOsdMenu::cPlugDetailOsdMenu(int Element)
 : libvdrskinservice::cPluginSkinOsdMenu("democlient", "Demo Client Detail")
 , element(Element)
{
  SetDetail();
}

cPlugDetailOsdMenu::~cPlugDetailOsdMenu(void)
{
}

eOSState cPlugDetailOsdMenu::ProcessKey(eKeys key)
{
  eOSState state = cOsdMenu::ProcessKey(key);
  if (state == osUnknown) {
     switch (key) {
       case kOk: {
         state = osBack;
         break;
         }
       default:
         break;
       }
     }

  return state;
}

void cPlugDetailOsdMenu::SetDetail()
{
  SetPluginMenu(meDetail, libvdrskinservice::mtText);
  Clear();
  ClearValues();
  
  SetText("Text to be displayed if skin doesn't know anything about this plugin");

  AddInt("menuitem", element);
  AddString("tabtext", "String value to be displayed if skin doesn't know anything about this plugin");

  //Loop Token Example
  for (int row = 0; row < 25; row++) {
      libvdrskinservice::cKeyValueList<cString> *loopValues = new libvdrskinservice::cKeyValueList<cString>();
      for (int col = 0; col < 10; col++) {
          cString key = cString::sprintf("loop1[col%d]", col);
          cString *value = new cString(cString::sprintf("row%d-col%d", row, col));
          loopValues->AddKeyValue(*key, value);
      }
      AddLoopValues("loop1", loopValues);
  }

  Display();
}
