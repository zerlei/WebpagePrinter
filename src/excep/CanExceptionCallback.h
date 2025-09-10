#pragma once
#include "PrintWorkFlowError.h"
#include <functional>
class CanExceptionCallback {

  public:
    explicit CanExceptionCallback(std::function<void()>                   callback,
                                  std::function<void(PrintWorkFlowError)> exception_callback)
        : callback(callback)
        , exception_callback(exception_callback) {}

    explicit CanExceptionCallback() = default;
    

    void correct() { callback(); }
    void         exception(PrintWorkFlowError e) { exception_callback(e); }

  private:
    std::function<void()> callback;
    std::function<void(PrintWorkFlowError)>   exception_callback;
};