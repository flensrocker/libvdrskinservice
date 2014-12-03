#include "globals.h"

const char *libvdrskinservice::globals::lib_version = LIBVERSION;
int libvdrskinservice::globals::api_version = APIVERSION;

const char *libvdrskinservice::globals::LibVersion(void)
{
  return lib_version;
}

int libvdrskinservice::globals::ApiVersion(void)
{
  return api_version;
}
