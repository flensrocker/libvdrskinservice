#ifndef __LIBVDRSKINSERVICE_PLUGINSKIN_H
#define __LIBVDRSKINSERVICE_PLUGINSKIN_H

#include <vdr/osdbase.h>
#include <vdr/skins.h>

#include "keyvaluelist.h"

namespace libvdrskinservice {

  class cPluginSkinDisplayMenu : public cSkinDisplayMenu {
  public:
    cPluginSkinDisplayMenu(void) {};
    virtual ~cPluginSkinDisplayMenu(void) {};

    virtual void SetPluginMenu(const char *PluginName, int MenuId, int Type, bool init) {};
    virtual bool SetItemValues(const cKeyValueContainer &Values, int Index, bool Current, bool Selectable) { return false; };
    virtual bool SetTextValues(const cKeyValueContainer &Values) { return false; };
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
