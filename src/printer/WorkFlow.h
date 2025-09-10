#pragma once

#include "DataPack.h"
#include <functional>
#include <mutex>
#include <optional>
#include <queue>
enum State { IsWorking, IsWaiting };
template <typename T>
struct PrinterWorkFlow {
    State           state = State::IsWaiting;
    T               first_step;
    PrinterDataPack data_pack;
    std::mutex      mutex;
    explicit PrinterWorkFlow()
        : data_pack([this]() { this->startWork(); }) {}

    void addWorkQueue(QJsonObject source, std::move_only_function<void(QJsonObject)>&& resp,
                      std::optional<int> uid) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            print_page_list.push(std::make_tuple(source, std::move(resp), uid));
        }
        if (state == State::IsWaiting) {
            startWork();
        }
    }
    void startWork() {
        std::lock_guard<std::mutex> lock(mutex);
        if (!print_page_list.empty()) {

            state = State::IsWorking;
            data_pack.setData(print_page_list.front());
            print_page_list.pop();

            first_step.work(data_pack);

        } else {
            state = State::IsWaiting;
        }
    }

  private:
    std::queue<std::tuple<QJsonObject, std::move_only_function<void(QJsonObject)>, std::optional<int>>>
        print_page_list;
};