#ifndef __LIBVDRSKINSERVICE_KEYVALUELIST_H
#define __LIBVDRSKINSERVICE_KEYVALUELIST_H

#include "valuechange.h"

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


  class cKeyValueContainer;


  // The current implementation doesn't handle large collections very well...
  template<class T> class cKeyValueList : public cListObject, public cList< cKeyValuePair<T> >, public cValueChangeHandler<T> {
  private:
    friend class cKeyValueContainer;

  public:
    // Not thread-safe!
    cKeyValuePair<T> *Find(const char *Key) const {
      for (cKeyValuePair<T> *kv = this->cList< cKeyValuePair<T> >::First(); kv; kv = this->cList< cKeyValuePair<T> >::Next(kv)) {
          if (strcmp(Key, kv->Key()) == 0)
             return kv;
          }
      return NULL;
      }

    // Not thread-safe!
    void AddKeyValue(const char *Key, T *Value) {
      cKeyValuePair<T> *item = Find(Key);
      if (item == NULL)
         this->Add(new cKeyValuePair<T>(Key, Value));
      else
         item->SetValue(Value);
      this->CallHandler(Key, Value, false);
      }

    // Not thread-safe!
    bool DelKeyValue(const char *Key) {
      cKeyValuePair<T> *item = Find(Key);
      if (item != NULL) {
         this->Del(item);
         this->CallHandler(Key, NULL, true);
         return true;
         }
      return false;
      }
  };


  class cKeyValueContainerLock {
  private:
    const cKeyValueContainer& container;
    bool isLocked;

  public:
    cKeyValueContainerLock(const cKeyValueContainer& Container);
    ~cKeyValueContainerLock(void);

    bool IsLocked(void) const;
    bool IsForContainer(const cKeyValueContainer *Container) const;
  };


  class cKeyValueContainer {
  private:
    friend class cKeyValueContainerLock;

    mutable cRwLock valuesLock;

    cKeyValueList<cString> stringValues;
    cKeyValueList<int> intValues;
    cKeyValueList< cList< cKeyValueList<cString> > > loopValues;

  public:
    cKeyValueContainer(void);
    virtual ~cKeyValueContainer(void);

    void ClearValues(void);

    bool AddStringChangeHandler(IValueChanged<cString> *Handler);
    bool DelStringChangeHandler(IValueChanged<cString> *Handler);
    bool AddIntChangeHandler(IValueChanged<int> *Handler);
    bool DelIntChangeHandler(IValueChanged<int> *Handler);
    bool AddLoopChangeHandler(IValueChanged< cList< cKeyValueList<cString> > > *Handler);
    bool DelLoopChangeHandler(IValueChanged< cList< cKeyValueList<cString> > > *Handler);

    // Value must be instantiated with new.
    void AddString(const char *Key, cString *Value);
    // For convenience.
    void AddString(const char *Key, const char *Value);
    void AddInt(const char *Key, int Value);
    // LoopValues must be instantiated with new.
    void AddLoopValues(const char *LoopName, cKeyValueList<cString> *LoopValues);

    // Copy the value if you need it longer than the lock is held.
    // The returned pointer points right into the value-lists, so they may get invalidated
    // when modifying the container.
    const char *GetString(cKeyValueContainerLock& Lock, const char *Key) const;
    int GetInt(cKeyValueContainerLock& Lock, const char *Key) const;
    const cList< cKeyValueList<cString> > *GetLoopValues(cKeyValueContainerLock& Lock, const char *LoopName) const;

    bool DelString(const char *Key);
    bool DelInt(const char *Key);
    bool DelLoopValues(const char *LoopName);
  };


  class cGlobalContainers {
  private:
    static cMutex mutex;
    static cKeyValueList<cKeyValueContainer> containers;

  public:
    // Returns NULL if a container with the same name already exists.
    // Don't delete it!
    static cKeyValueContainer *Create(const char *Name);
    // Returns NULL if a container with the given name does not exist.
    // Don't delete it!
    static const cKeyValueContainer *Get(const char *Name, IValueChanged<cString> *StringHandler, IValueChanged<int> *IntHandler, IValueChanged< cList< cKeyValueList<cString> > > *LoopHandler);
  };

}

#endif // __LIBVDRSKINSERVICE_KEYVALUELIST_H
