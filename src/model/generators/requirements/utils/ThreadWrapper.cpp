#include "ThreadWrapper.h"

ThreadWrapper::ThreadWrapper() {

}

void ThreadWrapper::start() {
    thread_ = std::thread(&ThreadWrapper::run, this);
}

ThreadWrapper::~ThreadWrapper() {
    thread_.join();
}

void ThreadWrapper::run() {

}
