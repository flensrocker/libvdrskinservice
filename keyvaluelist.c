#include "keyvaluelist.h"

// --- cKeyValueContainer ----------------------------------------------------

libvdrskinservice::cKeyValueContainer::cKeyValueContainer(void)
 : stringValues(NULL)
 , intValues(NULL)
 , loopValues(NULL)
{
}

libvdrskinservice::cKeyValueContainer::~cKeyValueContainer(void)
{
  Clear();
}

void libvdrskinservice::cKeyValueContainer::Clear(void)
{
  delete stringValues;
  stringValues = NULL;
  delete intValues;
  intValues = NULL;
  delete loopValues;
  loopValues = NULL;
}

void libvdrskinservice::cKeyValueContainer::AddString(const char *Key, cString *Value)
{
  if (stringValues == NULL)
     stringValues = new cKeyValueList<cString>();

  stringValues->Add(new cKeyValuePair<cString>(Key, Value));
}

void libvdrskinservice::cKeyValueContainer::AddString(const char *Key, const char *Value)
{
  AddString(Key, new cString(Value));
}

void libvdrskinservice::cKeyValueContainer::AddInt(const char *Key, int Value)
{
  if (intValues == NULL)
     intValues = new cKeyValueList<int>();

  intValues->Add(new cKeyValuePair<int>(Key, new int(Value)));
}

void libvdrskinservice::cKeyValueContainer::AddLoop(const char *LoopName, cKeyValueList<cString> *LoopValues)
{
  if (loopValues == NULL)
     loopValues = new cKeyValueList< cList< cKeyValueList<cString> > >();

  cKeyValuePair< cList< cKeyValueList<cString> > > *loop = loopValues->Find(LoopName);
  if (loop == NULL) {
     loop = new cKeyValuePair< cList< cKeyValueList<cString> > >(LoopName, new cList< cKeyValueList<cString> >());
     loopValues->Add(loop);
     }
  loop->Value().Add(LoopValues);
}
