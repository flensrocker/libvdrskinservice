#ifndef __LIBVDRSKINSERVICE_SKINDESIGNERSERVICES_H
#define __LIBVDRSKINSERVICE_SKINDESIGNERSERVICES_H

using namespace std;

#include <string>
#include <map>
#include <vector>
#include <vdr/osdbase.h>

namespace skindesignerservice {

class ISDDisplayMenu : public cSkinDisplayMenu {
public:
  virtual void SetPluginMenu(string name, int menu, int type, bool init) = 0;
  virtual bool SetItemPlugin(map<string,string> *stringTokens, map<string,int> *intTokens, map<string,vector<map<string,string> > > *loopTokens, int Index, bool Current, bool Selectable) = 0;
  virtual bool SetPluginText(map<string,string> *stringTokens, map<string,int> *intTokens, map<string,vector<map<string,string> > > *loopTokens) = 0;
};

class ISkindesignerService {
private:
  static ISkindesignerService* skindesigner;

protected:
  ISkindesignerService(void);
  virtual ~ISkindesignerService(void);

  virtual bool OnRegisterPlugin(string name, map< int, string > menus) = 0;
  virtual ISDDisplayMenu*  OnGetDisplayMenu() = 0;

public:
  static bool CallRegisterPlugin(string name, map< int, string > menus);
  static ISDDisplayMenu* CallGetDisplayMenu();
};

}

#endif //__SKINDESIGNERSERVICES_H
