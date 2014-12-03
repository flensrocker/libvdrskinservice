#ifndef __LIBVDRSKINSERVICE_GLOBALS_H
#define __LIBVDRSKINSERVICE_GLOBALS_H

namespace libvdrskinservice {

  class globals {
  private:
    static const char *lib_version;
    static int api_version;

  public:
    static const char *LibVersion(void);
    static int ApiVersion(void);
  };

}

#endif // __LIBVDRSKINSERVICE_GLOBALS_H
