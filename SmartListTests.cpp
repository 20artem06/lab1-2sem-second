#include "SmartList.h"


#include <cassert>
#include <iostream>

void testAppend() {
    SmartList<int> list;
    list.append(10);
    list.append(20);
    assert(list.getLength() == 2);
    assert(list.getByIndex(0) == 10);
    assert(list.getByIndex(1) == 20);
}

void testPrepend() {
    SmartList<int> list;
    list.prepend(30);
    list.prepend(40);
    assert(list.getLength() == 2);
    assert(list.getByIndex(0) == 40);
    assert(list.getByIndex(1) == 30);
}



void testGetByIndex() {
    int arr[] = { 1, 2, 3, 4 };
    SmartList<int> list(arr, 4);
    assert(list.getByIndex(0) == 1);
    assert(list.getByIndex(1) == 2);
    assert(list.getByIndex(2) == 3);
    assert(list.getByIndex(3) == 4);
    try {
        list.getByIndex(4);
        assert(false);
    }
    catch (const std::out_of_range&) {
        assert(true);
    }
}

void testSubList() {
    int arr[] = { 5, 10, 15, 20 };
    SmartList<int> list(arr, 4);
    unique_ptr<SmartList<int>> subList = list.getSubList(1, 2);
    assert(subList->getLength() == 2);
    assert(subList->getByIndex(0) == 10);
    assert(subList->getByIndex(1) == 15);
}

void testConcatenate() {
    int arr1[] = { 1, 2 };
    int arr2[] = { 3, 4 };
    SmartList<int> list1(arr1, 2);
    SmartList<int> list2(arr2, 2);
    unique_ptr<SmartList<int>> combinedList = list1.concatenate(list2);
    assert(combinedList->getLength() == 4);
    assert(combinedList->getByIndex(0) == 1);
    assert(combinedList->getByIndex(1) == 2);
    assert(combinedList->getByIndex(2) == 3);
    assert(combinedList->getByIndex(3) == 4);
}

void testAssignmentOperator() {
    int arr[] = { 1, 2, 3 };
    SmartList<int> list(arr, 3);
    SmartList<int> assignedList;
    assignedList = list;
    assert(assignedList.getLength() == 3);
    assert(assignedList.getByIndex(0) == 1);
    assert(assignedList.getByIndex(1) == 2);
    assert(assignedList.getByIndex(2) == 3);
    assignedList.append(4);
    assert(list.getLength() == 3);
    assert(assignedList.getLength() == 4);
}

void testDestructor() {
    {
        SmartList<int> tempList;
        tempList.append(1);
        tempList.append(2);
        tempList.append(3);
        assert(tempList.getLength() == 3);
    }
}

void runSmartListTest() {
    testAppend();
    testPrepend();

    testGetByIndex();
    testSubList();
    testConcatenate();
    testDestructor();
    testAssignmentOperator();
    std::cout << "Smart list functionality tests passed\n";
}

//int main() {
//    runSmartListTest();
//}
