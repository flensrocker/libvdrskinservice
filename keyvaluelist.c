#include "keyvaluelist.h"

using namespace libvdrskinservice;

// --- cKeyValueContainer ----------------------------------------------------

cKeyValueContainer::cKeyValueContainer(void)
 : stringValues(NULL)
 , intValues(NULL)
 , loopValues(NULL)
{
  stringValues = new cKeyValueList<cString>();
  intValues = new cKeyValueList<int>();
  loopValues = new cKeyValueList< cList< cKeyValueList<cString> > >();
}

cKeyValueContainer::~cKeyValueContainer(void)
{
  delete stringValues;
  delete intValues;
  delete loopValues;
}

void cKeyValueContainer::Clear(void)
{
  stringValues->Clear();
  intValues->Clear();
  loopValues->Clear();
}

void cKeyValueContainer::AddString(const char *Key, cString *Value)
{
  stringValues->AddKeyValue(Key, Value);
}

void cKeyValueContainer::AddString(const char *Key, const char *Value)
{
  AddString(Key, new cString(Value));
}

void cKeyValueContainer::AddInt(const char *Key, int Value)
{
  intValues->AddKeyValue(Key, new int(Value));
}

void cKeyValueContainer::AddLoopValues(const char *LoopName, cKeyValueList<cString> *LoopValues)
{
  cKeyValuePair< cList< cKeyValueList<cString> > > *loop = loopValues->Find(LoopName);
  if (loop == NULL) {
     loop = new cKeyValuePair< cList< cKeyValueList<cString> > >(LoopName, new cList< cKeyValueList<cString> >());
     loopValues->Add(loop);
     }
  loop->Value().Add(LoopValues);
}

const char *cKeyValueContainer::GetString(const char *Key) const
{
  cKeyValuePair<cString> *item = stringValues->Find(Key);
  if (item == NULL)
     return NULL;
  return *(item->Value());
}

int cKeyValueContainer::GetInt(const char *Key) const
{
  cKeyValuePair<int> *item = intValues->Find(Key);
  if (item == NULL)
     return 0;
  return item->Value();
}

const cList< cKeyValueList<cString> > *cKeyValueContainer::GetLoopValues(const char *Key) const
{
  cKeyValuePair< cList< cKeyValueList<cString> > > *item = loopValues->Find(Key);
  if (item == NULL)
     return NULL;
  return &(item->Value());
}

bool cKeyValueContainer::DelString(const char *Key)
{
  return stringValues->DelKeyValue(Key);
}

bool cKeyValueContainer::DelInt(const char *Key)
{
  return intValues->DelKeyValue(Key);
}

bool cKeyValueContainer::DelLoopValues(const char *LoopName)
{
  return loopValues->DelKeyValue(LoopName);
}
