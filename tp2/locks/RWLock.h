#ifndef RWLock_h
#define RWLock_h
#include <iostream>

class RWLock {
    public:
        RWLock();
        void rlock();
        void wlock();
        void runlock();
        void wunlock();

    private:
        pthread_mutex_t mtx;
        pthread_mutex_t roomEmpty;
        pthread_cond_t  turn;   /* Event: someone else's turn */

        int readers;
};

#endif
