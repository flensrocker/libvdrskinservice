#ifndef __LIBVDRSKINSERVICE_VALUECHANGE_H
#define __LIBVDRSKINSERVICE_VALUECHANGE_H

#include <vdr/thread.h>
#include <vdr/tools.h>

namespace libvdrskinservice {

  template<class T> class IValueChanged : public cListObject {
  public:
    IValueChanged(void) {};
    virtual ~IValueChanged(void) {};

    // If Key == NULL, then all values of the list were changed/deleted.
    // Don't modify the signaled list in its ValueChanged function,
    // this can lead to deadlocks or endless loops.
    // If "Deleted" then NewValue is NULL.
    virtual void OnValueChanged(const char *Key, T *NewValue, bool Deleted) = 0;
  };


  template<class T> class cValueChangeHandler {
  private:
    cRwLock lock;
    cList< IValueChanged<T> > handlers;

    void CallHandlerUnlocked(const char *Key, T *Value, bool Deleted)
    {
      IValueChanged<T> *h;
      for (h = handlers.First(); h; h = handlers.Next(h))
          h->OnValueChanged(Key, Value, Deleted);
    }

  protected:
    void CallHandler(const char *Key, T *Value, bool Deleted)
    {
      if (lock.Lock(false)) {
         CallHandlerUnlocked(Key, Value, Deleted);
         lock.Unlock();
         }
    }

  public:
    cValueChangeHandler(void)
    {
    }

    virtual ~cValueChangeHandler(void)
    {
      if (lock.Lock(true)) {
         IValueChanged<T> *h;
         while ((h = handlers.First()) != NULL)
               handlers.Del(h, false);
         lock.Unlock();
         }
    }

    // Handlers will be called in the same thread as the one, which triggered the change.
    bool AddHandler(IValueChanged<T> *Handler)
    {
      if (lock.Lock(true)) {
         handlers.Add(Handler);
         lock.Unlock();
         return true;
         }
      return false;
    }

    bool DelHandler(IValueChanged<T> *Handler)
    {
      if (lock.Lock(true)) {
         handlers.Del(Handler, false);
         lock.Unlock();
         return true;
         }
      return false;
    }
  };

}

#endif // __LIBVDRSKINSERVICE_VALUECHANGE_H
