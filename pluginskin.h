#ifndef __LIBVDRSKINSERVICE_PLUGINSKIN_H
#define __LIBVDRSKINSERVICE_PLUGINSKIN_H

#include <vdr/osdbase.h>
#include <vdr/skins.h>

namespace libvdrskinservice {

  template<class T> class cKeyValuePair : public cListObject {
    private:
      cString  key;
      T  *value;

    public:
      cKeyValuePair(const char *Key, T *Value) {
        key = Key;
        value = Value;
        }

      virtual ~cKeyValuePair(void) { delete value; }

      virtual int Compare(const cListObject &ListObject) const
      {
        cKeyValuePair* kv = (cKeyValuePair*)&ListObject;
        return strcmp(*key, *(kv->key));
      }

      const char *Key(void) { return *key; }
      T &Value(void) const { return *value; }
  };


  template<class T> class cKeyValueList : public cListObject, public cList< cKeyValuePair<T> > {
    public:
      cKeyValuePair<T> *Find(const char *Key) {
        for (cKeyValuePair<T> *kv = this->cList< cKeyValuePair<T> >::First(); kv; kv = this->cList< cKeyValuePair<T> >::Next(kv)) {
            if (strcmp(Key, kv->Key()) == 0)
               return kv;
            }
        return NULL;
        }
  };


  class cTokenStore {
  protected:
    cKeyValueList<cString> *stringValues;
    cKeyValueList<int> *intValues;
    cKeyValueList< cList< cKeyValueList<cString> > > *loopValues;

  public:
    cTokenStore(void);
    virtual ~cTokenStore(void);

    void ClearTokens(void);
    void AddStringToken(const char *Key, cString *Value); // Value must be instantiated with new
    void AddIntToken(const char *Key, int Value);
    void AddLoopToken(const char *LoopName, cKeyValueList<cString> *Tokens); // Value must be instantiated with new
  };


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


  class cPluginSkinOsdMenu : public cOsdMenu, protected cTokenStore {
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


  class cPluginSkinOsdItem : public cOsdItem, public cTokenStore {
  public:
    cPluginSkinOsdItem(eOSState State = osUnknown);
    virtual ~cPluginSkinOsdItem(void);

    virtual void SetMenuItem(cSkinDisplayMenu *DisplayMenu, int Index, bool Current, bool Selectable);
  };

}

#endif // __LIBVDRSKINSERVICE_PLUGINSKIN_H
