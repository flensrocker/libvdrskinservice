#include "keyvaluelist.h"

using namespace libvdrskinservice;

// --- cSchedulesLock --------------------------------------------------------

cKeyValueContainerLock::cKeyValueContainerLock(const cKeyValueContainer& Container)
 : container(Container)
{
  isLocked = container.valuesLock.Lock(false);
}

cKeyValueContainerLock::~cKeyValueContainerLock()
{
  if (isLocked)
     container.valuesLock.Unlock();
}

bool cKeyValueContainerLock::IsLocked(void) const
{
  return isLocked;
}

bool cKeyValueContainerLock::IsForContainer(const cKeyValueContainer *Container) const
{
  return Container == &container;
}

// --- cKeyValueContainer ----------------------------------------------------

cKeyValueContainer::cKeyValueContainer(void)
{
}

cKeyValueContainer::~cKeyValueContainer(void)
{
}

bool cKeyValueContainer::AddStringChangeHandler(IValueChanged<cString> *Handler)
{
  return stringValues.AddHandler(Handler);
}

bool cKeyValueContainer::DelStringChangeHandler(IValueChanged<cString> *Handler)
{
  return stringValues.DelHandler(Handler);
}

bool cKeyValueContainer::AddIntChangeHandler(IValueChanged<int> *Handler)
{
  return intValues.AddHandler(Handler);
}

bool cKeyValueContainer::DelIntChangeHandler(IValueChanged<int> *Handler)
{
  return intValues.DelHandler(Handler);
}

bool cKeyValueContainer::AddLoopChangeHandler(IValueChanged< cList< cKeyValueList<cString> > > *Handler)
{
  return loopValues.AddHandler(Handler);
}

bool cKeyValueContainer::DelLoopChangeHandler(IValueChanged< cList< cKeyValueList<cString> > > *Handler)
{
  return loopValues.DelHandler(Handler);
}

void cKeyValueContainer::ClearValues(void)
{
  if (valuesLock.Lock(true)) {
     bool callString = stringValues.Count() > 0;
     bool callInt = intValues.Count() > 0;
     bool callLoop = loopValues.Count() > 0;

     stringValues.Clear();
     intValues.Clear();
     loopValues.Clear();

     if (callString)
        stringValues.CallHandler(NULL, NULL, true);
     if (callInt)
        intValues.CallHandler(NULL, NULL, true);
     if (callLoop)
        loopValues.CallHandler(NULL, NULL, true);

     valuesLock.Unlock();
     }
}

void cKeyValueContainer::AddString(const char *Key, cString *Value)
{
  if (valuesLock.Lock(true)) {
     stringValues.AddKeyValue(Key, Value);
     valuesLock.Unlock();
     }
}

void cKeyValueContainer::AddString(const char *Key, const char *Value)
{
  AddString(Key, new cString(Value));
}

void cKeyValueContainer::AddInt(const char *Key, int Value)
{
  if (valuesLock.Lock(true)) {
     intValues.AddKeyValue(Key, new int(Value));
     valuesLock.Unlock();
     }
}

void cKeyValueContainer::AddLoopValues(const char *LoopName, cKeyValueList<cString> *LoopValues)
{
  if (valuesLock.Lock(true)) {
     cKeyValuePair< cList< cKeyValueList<cString> > > *loop = loopValues.Find(LoopName);
     if (loop == NULL) {
        cList< cKeyValueList<cString> > *list = new cList< cKeyValueList<cString> >();
        list->Add(LoopValues);
        loopValues.AddKeyValue(LoopName, list);
        }
     else {
        loop->Value().Add(LoopValues);
        loopValues.CallHandler(LoopName, &(loop->Value()), false);
        }
     valuesLock.Unlock();
     }
}

const char *cKeyValueContainer::GetString(cKeyValueContainerLock& Lock, const char *Key) const
{
  if (!Lock.IsLocked() || !Lock.IsForContainer(this))
     return NULL;

  cKeyValuePair<cString> *item = stringValues.Find(Key);
  if (item == NULL)
     return NULL;
  return *(item->Value());
}

int cKeyValueContainer::GetInt(cKeyValueContainerLock& Lock, const char *Key) const
{
  if (!Lock.IsLocked() || !Lock.IsForContainer(this))
     return 0;

  cKeyValuePair<int> *item = intValues.Find(Key);
  if (item == NULL)
     return 0;
  return item->Value();
}

const cList< cKeyValueList<cString> > *cKeyValueContainer::GetLoopValues(cKeyValueContainerLock& Lock, const char *Key) const
{
  if (!Lock.IsLocked() || !Lock.IsForContainer(this))
     return NULL;

  cKeyValuePair< cList< cKeyValueList<cString> > > *item = loopValues.Find(Key);
  if (item == NULL)
     return NULL;
  return &(item->Value());
}

bool cKeyValueContainer::DelString(const char *Key)
{
  if (valuesLock.Lock(true)) {
     bool ret = stringValues.DelKeyValue(Key);
     valuesLock.Unlock();
     return ret;
  }
  return false;
}

bool cKeyValueContainer::DelInt(const char *Key)
{
  if (valuesLock.Lock(true)) {
     bool ret = intValues.DelKeyValue(Key);
     valuesLock.Unlock();
     return ret;
     }
  return false;
}

bool cKeyValueContainer::DelLoopValues(const char *LoopName)
{
  if (valuesLock.Lock(true)) {
     bool ret = loopValues.DelKeyValue(LoopName);
     valuesLock.Unlock();
     return ret;
     }
  return false;
}

// --- cGlobalContainers -----------------------------------------------------

cMutex cGlobalContainers::mutex;
cKeyValueList<cKeyValueContainer> cGlobalContainers::containers;

cKeyValueContainer *cGlobalContainers::Create(const char *Name)
{
  if ((Name == NULL) || (*Name == 0))
     return NULL;

  cMutexLock lock(&mutex);
  cKeyValuePair<cKeyValueContainer> *kv = containers.Find(Name);
  // first come, first serves
  if (kv != NULL)
     return NULL;

  cKeyValueContainer *c = new cKeyValueContainer();
  containers.AddKeyValue(Name, c);
  return c;
}

const cKeyValueContainer *cGlobalContainers::Get(const char *Name, IValueChanged<cString> *StringHandler, IValueChanged<int> *IntHandler, IValueChanged< cList< cKeyValueList<cString> > > *LoopHandler)
{
  if ((Name == NULL) || (*Name == 0))
     return NULL;

  cMutexLock lock(&mutex);
  cKeyValuePair<cKeyValueContainer> *kv = containers.Find(Name);
  if (kv == NULL)
     return NULL;

  cKeyValueContainer &c = kv->Value();
  if (StringHandler != NULL)
     c.AddStringChangeHandler(StringHandler);
  if (IntHandler != NULL)
     c.AddIntChangeHandler(IntHandler);
  if (LoopHandler != NULL)
     c.AddLoopChangeHandler(LoopHandler);
  return &c;
}
