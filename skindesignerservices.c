#include "skindesignerservices.h"

skindesignerservice::ISkindesignerService* skindesignerservice::ISkindesignerService::skindesigner = NULL;

skindesignerservice::ISkindesignerService::ISkindesignerService(void)
{
  if (skindesigner != NULL)
     esyslog("skindesigner should only be loaded once");
  else
     skindesigner = this;
}

skindesignerservice::ISkindesignerService::~ISkindesignerService(void)
{
  if (skindesigner == this)
     skindesigner = NULL;
}

bool skindesignerservice::ISkindesignerService::CallRegisterPlugin(string name, map< int, string > menus)
{
  if (skindesigner)
     return skindesigner->OnRegisterPlugin(name, menus);
  return false;
}

skindesignerservice::ISDDisplayMenu*  skindesignerservice::ISkindesignerService::CallGetDisplayMenu()
{
  if (skindesigner)
     return skindesigner->OnGetDisplayMenu();
  return NULL;
}
