#include "SmartListSequence.h"

#include <cassert>
#include <iostream>

void testSequenceAppend() {
    SmartListSequence<int> sequence;
    sequence.append(10);
    sequence.append(20);
    assert(sequence.getLength() == 2);
    assert(sequence.get(0) == 10);
    assert(sequence.get(1) == 20);
}

void testSequencePrepend() {
    SmartListSequence<int> sequence;
    sequence.prepend(30);
    sequence.prepend(40);
    assert(sequence.getLength() == 2);
    assert(sequence.get(0) == 40);
    assert(sequence.get(1) == 30);
}

void testSequenceInsertAt() {
    int arr[] = { 10, 20, 30 };
    SmartListSequence<int> sequence(arr, 3);
    sequence.insertAt(1, 15);
    assert(sequence.getLength() == 4);
    assert(sequence.get(0) == 10);
    assert(sequence.get(1) == 15);
    assert(sequence.get(2) == 20);
    assert(sequence.get(3) == 30);
}

void testSequenceGetByIndex() {
    int arr[] = { 1, 2, 3, 4 };
    SmartListSequence<int> sequence(arr, 4);
    assert(sequence.get(0) == 1);
    assert(sequence.get(1) == 2);
    assert(sequence.get(2) == 3);
    assert(sequence.get(3) == 4);
    try {
        sequence.get(4);
        assert(false);
    }
    catch (const std::out_of_range&) {
        assert(true);
    }
}

void testSequenceSubSeq() {
    int arr[] = { 5, 10, 15, 20 };
    SmartListSequence<int> sequence(arr, 4);
    unique_ptr<Sequence<int>> subSequence = sequence.getSubSequence(1, 2);
    assert(subSequence->getLength() == 2);
    assert(subSequence->get(0) == 10);
    assert(subSequence->get(1) == 15);
}

void testSequenceConcat() {
    int arr1[] = { 1, 2 };
    int arr2[] = { 3, 4 };
    SmartListSequence<int> sequence1(arr1, 2);
    SmartListSequence<int> sequence2(arr2, 2);
    unique_ptr<Sequence<int>> uPtr2(new SmartListSequence<int>(std::move(sequence2)));
    sequence1.concat(std::move(uPtr2));
    assert(sequence1.getLength() == 4);
    assert(sequence1.get(0) == 1);
    assert(sequence1.get(1) == 2);
    assert(sequence1.get(2) == 3);
    assert(sequence1.get(3) == 4);
}

void runSmartListSequenceTests() {
    testSequenceAppend();
    testSequencePrepend();
    testSequenceInsertAt();
    testSequenceGetByIndex();
    testSequenceSubSeq();
    testSequenceConcat();
    std::cout << "Smart list sequence functionality tests passed\n";
}

//int main() {
//    runSmartListSequenceTests();
//}
