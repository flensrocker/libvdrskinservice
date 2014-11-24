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

      void SetValue(T *Value) {
        delete value;
        value = Value;
        }
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

      void AddKeyValue(const char *Key, T *Value) {
        cKeyValuePair<T> *item = Find(Key);
        if (item == NULL)
           this->Add(new cKeyValuePair<T>(Key, Value));
        else
           item->SetValue(Value);
        }

      bool DelKeyValue(const char *Key) {
        cKeyValuePair<T> *item = Find(Key);
        if (item != NULL) {
           this->Del(item);
           return true;
           }
        return false;
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
    void AddLoopValues(const char *LoopName, cKeyValueList<cString> *LoopValues); // LoopValues must be instantiated with new

    const char *GetString(const char *Key) const;
    int GetInt(const char *Key) const;
    const cList< cKeyValueList<cString> > *GetLoopValues(const char *LoopName) const;

    bool DelString(const char *Key);
    bool DelInt(const char *Key);
    bool DelLoopValues(const char *LoopName);
  };

}

#endif // __LIBVDRSKINSERVICE_KEYVALUELIST_H
