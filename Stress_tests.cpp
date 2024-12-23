#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <memory>

#include "shared_ptr.h"
#include "unique_ptr.h"
#include "weak_ptr.h"



struct TestObject {
    int id;
    TestObject(int i) : id(i) {}
};


void testSmartPointers(int numObjects) {
    std::cout << "Test with " << numObjects << " objects using custom smart pointers.\n";

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<shared_ptr<TestObject>> spVec;
    spVec.reserve(numObjects);
    for (int i = 0; i < numObjects; ++i) {
        spVec.emplace_back(shared_ptr<TestObject>(new TestObject(i)));
    }


    std::vector<unique_ptr<TestObject>> upVec;
    upVec.reserve(numObjects);
    for (int i = 0; i < numObjects; ++i) {
        upVec.emplace_back(unique_ptr<TestObject>(new TestObject(i)));
    }

    std::vector<weak_ptr<TestObject>> wpVec;
    wpVec.reserve(numObjects);
    for (int i = 0; i < numObjects; ++i) {
        wpVec.emplace_back(weak_ptr<TestObject>(spVec[i]));
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Time taken: " << duration.count() << " seconds.\n";
}

void testRawPointers(int numObjects) {
    std::cout << "Test with " << numObjects << " objects using raw pointers.\n";

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<TestObject*> ptrVec;
    ptrVec.reserve(numObjects);
    for (int i = 0; i < numObjects; ++i) {
        ptrVec.emplace_back(new TestObject(i));
    }

    for (auto ptr : ptrVec) {
        delete ptr;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Time taken: " << duration.count() << " seconds.\n";
}


void testSTLSmartPointers(int numObjects) {
    std::cout << "Test with " << numObjects << " objects using STL smart pointers.\n";

    auto start = std::chrono::high_resolution_clock::now();


    std::vector<std::shared_ptr<TestObject>> spVec;
    spVec.reserve(numObjects);
    for (int i = 0; i < numObjects; ++i) {
        spVec.emplace_back(std::make_shared<TestObject>(i));
    }


    std::vector<std::unique_ptr<TestObject>> upVec;
    upVec.reserve(numObjects);
    for (int i = 0; i < numObjects; ++i) {
        upVec.emplace_back(std::make_unique<TestObject>(i));
    }


    std::vector<std::weak_ptr<TestObject>> wpVec;
    wpVec.reserve(numObjects);
    for (int i = 0; i < numObjects; ++i) {
        wpVec.emplace_back(std::weak_ptr<TestObject>(spVec[i]));
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Time taken: " << duration.count() << " seconds.\n";
}

//int main(int argc, char* argv[]) {
//    if (argc < 2) {
//        std::cerr << "Usage: stress_tests <mode>\n";
//        std::cerr << "Modes:\n";
//        std::cerr << " 1 - Small number of objects (10^3)\n";
//        std::cerr << " 2 - Large number of objects (10^6)\n";
//        return 1;
//    }
//
//    int mode = std::atoi(argv[1]);
//    int numObjects;
//
//    switch (mode) {
//    case 1:
//        numObjects = 1000; // 10^3
//        break;
//    case 2:
//        numObjects = 1000000; // 10^6
//        break;
//    default:
//        std::cerr << "Invalid mode. Use 1 or 2.\n";
//        return 1;
//    }
//
//
//    testSmartPointers(numObjects);
//    testRawPointers(numObjects);
//    testSTLSmartPointers(numObjects);
//
//    return 0;
//}
