#pragma once
#include "DataPack.h"
#include <QList>
#include <future>
#include <mutex>
enum State { IsWorking, IsWaiting };
template <typename T>
struct PrinterWorkFlow {
    State           state = State::IsWaiting;
    T               first_step;
    PrinterDataPack data_pack;
    std::mutex      mutex;
    explicit PrinterWorkFlow()
        : data_pack([this]() { this->startWork(); }) {}

    void addWorkQueue(QJsonObject source, std::promise<QJsonObject>&& promise) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            print_page_list.append({source, std::move(promise)});
        }
        if (state == State::IsWaiting) {
            startWork();
        }
    }
    void startWork() {
        std::lock_guard<std::mutex> lock(mutex);
        if (!print_page_list.empty()) {
            state = State::IsWorking;
            data_pack.updateData(print_page_list.front());
            print_page_list.pop_front();
            first_step.work(data_pack);
        } else {
            state = State::IsWaiting;
        }
    }

  private:
    QList<std::tuple<QJsonObject, std::promise<QJsonObject>>> print_page_list;
};