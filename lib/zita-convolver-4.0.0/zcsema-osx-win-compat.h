#if defined(__WIN32__)

#include <semaphore.h>

class ZCsema
{
public:

    ZCsema (void) { init (0, 0); }
    ~ZCsema (void) { sem_destroy (&_sema); }

    ZCsema (const ZCsema&); // disabled
    ZCsema& operator= (const ZCsema&); // disabled

    int init (int s, int v) { return sem_init (&_sema, s, v); }
    int post (void) { return sem_post (&_sema); }
    int wait (void) { return sem_wait (&_sema); }
    int trywait (void) { return sem_trywait (&_sema); }

private:

    sem_t  _sema;
};

#define ZCSEMA_IS_IMPLEMENTED
#endif


// ADDED BY DZ TO MAKE IT COMPILE FOR OSX 10.5 or Windows
#if defined(__APPLE__) || defined(__WIN32__)
#include <errno.h>

inline int posix_memalign(void **memptr, size_t alignment, size_t size)
{
   *memptr = malloc(size);
   if (*memptr == NULL)
       return ENOMEM;
   return 0;
}
#endif
