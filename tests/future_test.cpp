#include <gtest/gtest.h>
#include <future>
#include <thread>
#include <chrono>
#include <stdexcept>

using namespace std;

// Test basic future and promise interaction
TEST(FutureTest, BasicFuturePromise) {
    promise<int> p;
    future<int> f = p.get_future();
    p.set_value(42);
    ASSERT_EQ(f.get(), 42);
}

// Test exception handling
TEST(FutureTest, ExceptionHandling) {
    promise<int> p;
    future<int> f = p.get_future();
    p.set_exception(make_exception_ptr(runtime_error("error")));
    EXPECT_THROW(f.get(), runtime_error);
}

// Test broken promise
TEST(FutureTest, BrokenPromise) {
    future<int> f;
    {
        promise<int> p;
        f = p.get_future();
    } // promise goes out of scope without setting a value
    EXPECT_THROW(f.get(), future_error);
}

// Test already retrieved future
TEST(FutureTest, AlreadyRetrieved) {
    promise<int> p;
    p.get_future();
    EXPECT_THROW(p.get_future(), future_error);
}

// Test already satisfied promise
TEST(FutureTest, AlreadySatisfied) {
    promise<int> p;
    p.set_value(42);
    EXPECT_THROW(p.set_value(43), future_error);
}

// Test void future
TEST(FutureTest, VoidFuture) {
    promise<void> p;
    future<void> f = p.get_future();
    p.set_value();
    f.get(); // Should not throw
}

// Test move semantics
TEST(FutureTest, MoveSemantics) {
    promise<int> p;
    future<int> f1 = p.get_future();
    future<int> f2 = move(f1);
    p.set_value(42);
    ASSERT_EQ(f2.get(), 42);
}

// Test async execution
TEST(FutureTest, AsyncExecution) {
    future<int> f = async(launch::async, [] { return 42; });
    ASSERT_EQ(f.get(), 42);
}

// Test timeout
TEST(FutureTest, Timeout) {
    promise<int> p;
    future<int> f = p.get_future();
    auto status = f.wait_for(chrono::milliseconds(100));
    ASSERT_EQ(status, future_status::timeout);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}