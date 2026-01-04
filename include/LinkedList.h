#pragma once

#include "Card.h"
using namespace std;

class Node {
public:
    Card data;
    unique_ptr<Node> next;

    Node(const Card& data) : data(data), next(nullptr) {}
};

class LinkedList {
protected:
    unique_ptr<Node> head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    virtual ~LinkedList() = default; // Smart pointers handle deletion

    bool isEmpty() const { return head == nullptr; }
    int getSize() const { return size; }
};