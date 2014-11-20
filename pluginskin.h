#ifndef __LIBVDRSKINSERVICE_PLUGINSKIN_H
#define __LIBVDRSKINSERVICE_PLUGINSKIN_H

#include <vdr/osdbase.h>
#include <vdr/skins.h>

#include "keyvaluelist.h"

namespace libvdrskinservice {

  class cPluginSkinDisplayMenu : public cSkinDisplayMenu {
  public:
    virtual void SetPluginMenu(const char *PluginName, int MenuId, int Type, bool init) = 0;
    virtual bool SetItemPlugin(const cKeyValueList<cString> *StringValues, const cKeyValueList<int> *IntValues, const cKeyValueList< cList< cKeyValueList<cString> > > *LoopValues, int Index, bool Current, bool Selectable) { return false; };
    virtual bool SetMenuValues(const cKeyValueList<cString> *StringValues, const cKeyValueList<int> *IntValues, const cKeyValueList< cList< cKeyValueList<cString> > > *LoopValues) = 0;
  };


  class cPluginSkin : public cSkin {
  private:
    static cVector<cPluginSkin*> pluginSkins;
    cPluginSkinDisplayMenu *currentDisplayMenu;

  protected:
    // override this instead of DisplayMenu
    virtual cPluginSkinDisplayMenu *OnDisplayMenu(void) = 0;

  public:
    cPluginSkin(const char *Name, cTheme *Theme = NULL);
    virtual ~cPluginSkin(void);

    virtual cSkinDisplayMenu *DisplayMenu(void);

    static cPluginSkinDisplayMenu *CurrentDisplayMenu(void);
  };


  enum eMenuType {
      mtList,
      mtText
  };


  class cPluginSkinOsdMenu : public cOsdMenu, protected cKeyValueContainer {
  private:
    bool init;
    cString pluginName;
    eMenuType type;
    cString text;

  protected:
    bool Init(void) const { return init; }
    const char *PluginName(void) const { return *pluginName; }
    eMenuType MenuType(void) const { return type; }

    void SetPluginMenu(int MenuId, eMenuType Type);
    void SetText(const char *Text) { text = Text; }

  public:
    cPluginSkinOsdMenu(const char *PluginName, const char *Title, int c0 = 0, int c1 = 0, int c2 = 0, int c3 = 0, int c4 = 0);
    virtual ~cPluginSkinOsdMenu(void) {};

    virtual void Display(void);
  };


  class cPluginSkinOsdItem : public cOsdItem, public cKeyValueContainer {
  public:
    cPluginSkinOsdItem(eOSState State = osUnknown);
    virtual ~cPluginSkinOsdItem(void);

    virtual void SetMenuItem(cSkinDisplayMenu *DisplayMenu, int Index, bool Current, bool Selectable);
  };

}

#endif // __LIBVDRSKINSERVICE_PLUGINSKIN_H
