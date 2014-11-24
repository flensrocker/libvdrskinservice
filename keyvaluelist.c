#include "keyvaluelist.h"

using namespace libvdrskinservice;

// --- cSchedulesLock --------------------------------------------------------

cKeyValueContainerLock::cKeyValueContainerLock(const cKeyValueContainer& Container, eListType ListType)
 : container(Container)
 , listType(ListType)
{
  switch (listType) {
    case ltString:
      locked = container.stringLock.Lock(false);
      break;
    case ltInt:
      locked = container.intLock.Lock(false);
      break;
    case ltLoopValues:
      locked = container.loopLock.Lock(false);
      break;
    }
}

cKeyValueContainerLock::~cKeyValueContainerLock()
{
  if (locked) {
     switch (listType) {
       case ltString:
         container.stringLock.Unlock();
         break;
       case ltInt:
         container.intLock.Unlock();
         break;
       case ltLoopValues:
         container.loopLock.Unlock();
         break;
       }
     }
}

// --- cKeyValueContainer ----------------------------------------------------

cKeyValueContainer::cKeyValueContainer(void)
{
}

cKeyValueContainer::~cKeyValueContainer(void)
{
  if (changeLock.Lock(true)) {
     IValueChanged *ch;
     while ((ch = changeHandlers.First()) != NULL)
           changeHandlers.Del(ch, false);
     changeLock.Unlock();
     }
}

bool cKeyValueContainer::AddChangeHandler(IValueChanged *ChangeHandler)
{
  if (changeLock.Lock(true)) {
     changeHandlers.Add(ChangeHandler);
     changeLock.Unlock();
     return true;
     }
  return false;
}

bool cKeyValueContainer::DelChangeHandler(IValueChanged *ChangeHandler)
{
  if (changeLock.Lock(true)) {
     changeHandlers.Del(ChangeHandler, false);
     changeLock.Unlock();
     return true;
     }
  return false;
}

void cKeyValueContainer::CallChangeHandlers(eListType ListType, const char *Key, bool Deleted)
{
  if (changeLock.Lock(false)) {
     IValueChanged *ch;
     for (ch = changeHandlers.First(); ch; ch = changeHandlers.Next(ch))
         ch->OnValueChanged(ListType, Key, Deleted);
     changeLock.Unlock();
     }
}

void cKeyValueContainer::Clear(void)
{
  if (stringLock.Lock(true)) {
     stringValues.Clear();
     CallChangeHandlers(ltString, NULL, true);
     stringLock.Unlock();
     }
  if (intLock.Lock(true)) {
     intValues.Clear();
     CallChangeHandlers(ltInt, NULL, true);
     intLock.Unlock();
     }
  if (loopLock.Lock(true)) {
     loopValues.Clear();
     CallChangeHandlers(ltLoopValues, NULL, true);
     loopLock.Unlock();
     }
}

void cKeyValueContainer::AddString(const char *Key, cString *Value)
{
  if (stringLock.Lock(true)) {
     stringValues.AddKeyValue(Key, Value);
     CallChangeHandlers(ltString, Key, false);
     stringLock.Unlock();
     }
}

void cKeyValueContainer::AddString(const char *Key, const char *Value)
{
  AddString(Key, new cString(Value));
}

void cKeyValueContainer::AddInt(const char *Key, int Value)
{
  if (intLock.Lock(true)) {
     intValues.AddKeyValue(Key, new int(Value));
     CallChangeHandlers(ltInt, Key, false);
     intLock.Unlock();
     }
}

void cKeyValueContainer::AddLoopValues(const char *LoopName, cKeyValueList<cString> *LoopValues)
{
  if (loopLock.Lock(true)) {
     cKeyValuePair< cList< cKeyValueList<cString> > > *loop = loopValues.Find(LoopName);
     if (loop == NULL) {
        loop = new cKeyValuePair< cList< cKeyValueList<cString> > >(LoopName, new cList< cKeyValueList<cString> >());
        loopValues.Add(loop);
        }
     loop->Value().Add(LoopValues);
     CallChangeHandlers(ltLoopValues, LoopName, false);
     loopLock.Unlock();
     }
}

const char *cKeyValueContainer::GetString(cKeyValueContainerLock& Lock, const char *Key) const
{
  if (!Lock.Locked())
     return NULL;

  cKeyValuePair<cString> *item = stringValues.Find(Key);
  if (item == NULL)
     return NULL;
  return *(item->Value());
}

int cKeyValueContainer::GetInt(cKeyValueContainerLock& Lock, const char *Key) const
{
  if (!Lock.Locked())
     return 0;

  cKeyValuePair<int> *item = intValues.Find(Key);
  if (item == NULL)
     return 0;
  return item->Value();
}

const cList< cKeyValueList<cString> > *cKeyValueContainer::GetLoopValues(cKeyValueContainerLock& Lock, const char *Key) const
{
  if (!Lock.Locked())
     return NULL;

  cKeyValuePair< cList< cKeyValueList<cString> > > *item = loopValues.Find(Key);
  if (item == NULL)
     return NULL;
  return &(item->Value());
}

bool cKeyValueContainer::DelString(const char *Key)
{
  if (stringLock.Lock(true)) {
     bool ret = stringValues.DelKeyValue(Key);
     if (ret)
        CallChangeHandlers(ltString, Key, true);
     stringLock.Unlock();
     return ret;
  }
  return false;
}

bool cKeyValueContainer::DelInt(const char *Key)
{
  if (intLock.Lock(true)) {
     bool ret = intValues.DelKeyValue(Key);
     if (ret)
        CallChangeHandlers(ltInt, Key, true);
     intLock.Unlock();
     return ret;
     }
  return false;
}

bool cKeyValueContainer::DelLoopValues(const char *LoopName)
{
  if (loopLock.Lock(true)) {
     bool ret = loopValues.DelKeyValue(LoopName);
     if (ret)
        CallChangeHandlers(ltLoopValues, LoopName, true);
     loopLock.Unlock();
     return ret;
     }
  return false;
}
