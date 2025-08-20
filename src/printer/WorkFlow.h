#pragma once
#include "DataPack.h"
template <typename T>
struct PrinterWorkFlow {

    T               first_step;
    PrinterDataPack data_pack;
    void            startWork() { first_step.work(data_pack); }
};