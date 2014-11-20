#ifndef __LIBVDRSKINSERVICE_KEYVALUELIST_H
#define __LIBVDRSKINSERVICE_KEYVALUELIST_H

#include <vdr/tools.h>

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


  class cKeyValueContainer {
  protected:
    cKeyValueList<cString> *stringValues;
    cKeyValueList<int> *intValues;
    cKeyValueList< cList< cKeyValueList<cString> > > *loopValues;

  public:
    cKeyValueContainer(void);
    virtual ~cKeyValueContainer(void);

    void Clear(void);
    void AddString(const char *Key, cString *Value); // Value must be instantiated with new
    void AddString(const char *Key, const char *Value);
    void AddInt(const char *Key, int Value);
    void AddLoop(const char *LoopName, cKeyValueList<cString> *LoopValues); // Value must be instantiated with new
  };

}

#endif // __LIBVDRSKINSERVICE_KEYVALUELIST_H
