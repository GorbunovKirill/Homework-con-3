#include <iostream>
#include <mutex>
#include <thread>
#include <algorithm>

class Data {
public:
    Data(int value) : value(value) {}

    int getValue() const { return value; }

    void setValue(int newValue) { value = newValue; }

    std::mutex mtx;

    int value;
};

void swap_with_lock(Data& a, Data& b) {
    std::lock(a.mtx, b.mtx);
    std::lock_guard<std::mutex> lockA(a.mtx, std::adopt_lock);
    std::lock_guard<std::mutex> lockB(b.mtx, std::adopt_lock);


    std::swap(a.value, b.value);
}

void swap_with_scoped_lock(Data& a, Data& b) {
    std::scoped_lock lock(a.mtx, b.mtx);

    std::swap(a.value, b.value);
}

void swap_with_unique_lock(Data& a, Data& b) {
    std::unique_lock<std::mutex> lockA(a.mtx);
    std::unique_lock<std::mutex> lockB(b.mtx);


    std::swap(a.value, b.value);
}

int main() {
    Data data1(10);
    Data data2(20);

    std::cout << "Before swap: Data1 = " << data1.getValue() << ", Data2 = " 
        << data2.getValue() << std::endl;

    swap_with_lock(data1, data2);
    std::cout << "After swap_with_lock: Data1 = " << data1.getValue() << ", Data2 = " 
        << data2.getValue() << std::endl;

    data1.setValue(10);
    data2.setValue(20);

    swap_with_scoped_lock(data1, data2);
    std::cout << "After swap_with_scoped_lock: Data1 = " << data1.getValue() 
        << ", Data2 = " << data2.getValue() << std::endl;

    data1.setValue(10);
    data2.setValue(20);

    swap_with_unique_lock(data1, data2);
    std::cout << "After swap_with_unique_lock: Data1 = " << data1.getValue() 
        << ", Data2 = " << data2.getValue() << std::endl;

    return 0;
}
