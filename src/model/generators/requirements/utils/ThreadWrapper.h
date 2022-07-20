#ifndef TIKTOKGEN_THREADWRAPPER_H
#define TIKTOKGEN_THREADWRAPPER_H

#include <thread>

class ThreadWrapper {
public:
    ThreadWrapper();

    void start();

    virtual ~ThreadWrapper();

protected:
    std::thread thread_;

    virtual void run();
};


#endif //TIKTOKGEN_THREADWRAPPER_H
