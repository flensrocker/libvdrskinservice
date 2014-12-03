/*
 * democlient.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include <vdr/plugin.h>
#include <libvdrskinservice/pluginskin.h>
#include "osdmenu.c"

static const char *VERSION        = "0.0.1";
static const char *DESCRIPTION    = "VDR-SkinService-Lib Test Client";
static const char *MAINMENUENTRY  = "Democlient";

class cPluginDemoclient : public cPlugin {
private:
  // Add any member variables or functions you may need here.
public:
  cPluginDemoclient(void);
  virtual ~cPluginDemoclient();
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return DESCRIPTION; }
  virtual const char *CommandLineHelp(void);
  virtual bool ProcessArgs(int argc, char *argv[]);
  virtual bool Initialize(void);
  virtual bool Start(void);
  virtual void Stop(void);
  virtual void Housekeeping(void);
  virtual void MainThreadHook(void);
  virtual cString Active(void);
  virtual time_t WakeupTime(void);
  virtual const char *MainMenuEntry(void) { return MAINMENUENTRY; }
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
  virtual bool Service(const char *Id, void *Data = NULL);
  virtual const char **SVDRPHelpPages(void);
  virtual cString SVDRPCommand(const char *Command, const char *Option, int &ReplyCode);
  };

cPluginDemoclient::cPluginDemoclient(void)
{

}

cPluginDemoclient::~cPluginDemoclient()
{
}

const char *cPluginDemoclient::CommandLineHelp(void)
{
  return NULL;
}

bool cPluginDemoclient::ProcessArgs(int argc, char *argv[])
{
  return true;
}

bool cPluginDemoclient::Initialize(void)
{
  return true;
}

bool cPluginDemoclient::Start(void)
{
  return true;
}

void cPluginDemoclient::Stop(void)
{
  // Stop any background activities the plugin is performing.
}

void cPluginDemoclient::Housekeeping(void)
{
  // Perform any cleanup or other regular tasks.
}

void cPluginDemoclient::MainThreadHook(void)
{
  // Perform actions in the context of the main program thread.
  // WARNING: Use with great care - see PLUGINS.html!
}

cString cPluginDemoclient::Active(void)
{
  // Return a message string if shutdown should be postponed
  return NULL;
}

time_t cPluginDemoclient::WakeupTime(void)
{
  // Return custom wakeup time for shutdown script
  return 0;
}

cOsdObject *cPluginDemoclient::MainMenuAction(void)
{
  cOsdObject *menu = new cPlugOsdMenu();  
  return menu;
}

cMenuSetupPage *cPluginDemoclient::SetupMenu(void)
{
  // Return a setup menu in case the plugin supports one.
  return NULL;
}

bool cPluginDemoclient::SetupParse(const char *Name, const char *Value)
{
  // Parse your own setup parameters and store their values.
  return false;
}

bool cPluginDemoclient::Service(const char *Id, void *Data)
{
  // Handle custom service requests from other plugins
  return false;
}

const char **cPluginDemoclient::SVDRPHelpPages(void)
{
  // Return help text for SVDRP commands this plugin implements
  return NULL;
}

cString cPluginDemoclient::SVDRPCommand(const char *Command, const char *Option, int &ReplyCode)
{
  // Process SVDRP commands this plugin implements
  return NULL;
}

VDRPLUGINCREATOR(cPluginDemoclient); // Don't touch this!
