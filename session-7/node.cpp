// UID - 24BCS12303

#include <iostream>
using namespace std;

class Node {
public:
    int data;
    Node* next;

    Node(int value) {
        data = value;
        next = nullptr;
    }
};

Node* insert_begin(int value, Node* head) {
    Node* newNode = new Node(value);
    newNode->next = head;
    return newNode;
}

Node* insert_end(int value, Node* head) {
    if (head == nullptr)
        return new Node(value);

    Node* temp = head;
    while (temp->next != nullptr)
        temp = temp->next;

    temp->next = new Node(value);
    return head;
}

Node* insert_at_index(int value, int index, Node* head) {
    if (index == 0)
        return insert_begin(value, head);

    Node* temp = head;
    for (int i = 0; i < index - 1 && temp != nullptr; i++)
        temp = temp->next;

    if (temp == nullptr) {
        cout << "Invalid index" << endl;
        return head;
    }

    Node* newNode = new Node(value);
    newNode->next = temp->next;
    temp->next = newNode;
    return head;
}

Node* insert_after_value(int value, int after, Node* head) {
    Node* temp = head;
    while (temp != nullptr && temp->data != after)
        temp = temp->next;

    if (temp == nullptr) {
        cout << "Value not found" << endl;
        return head;
    }

    Node* newNode = new Node(value);
    newNode->next = temp->next;
    temp->next = newNode;
    return head;
}

Node* delete_begin(Node* head) {
    if (head == nullptr) {
        cout << "List is empty" << endl;
        return head;
    }

    Node* temp = head;
    head = head->next;
    delete temp;
    return head;
}

Node* delete_end(Node* head) {
    if (head == nullptr) {
        cout << "List is empty" << endl;
        return head;
    }

    if (head->next == nullptr) {
        delete head;
        return nullptr;
    }

    Node* temp = head;
    while (temp->next->next != nullptr)
        temp = temp->next;

    delete temp->next;
    temp->next = nullptr;
    return head;
}

Node* delete_at_index(int index, Node* head) {
    if (head == nullptr) {
        cout << "List is empty" << endl;
        return head;
    }

    if (index == 0)
        return delete_begin(head);

    Node* temp = head;
    for (int i = 0; i < index - 1 && temp->next != nullptr; i++)
        temp = temp->next;

    if (temp->next == nullptr) {
        cout << "Invalid index" << endl;
        return head;
    }

    Node* delNode = temp->next;
    temp->next = delNode->next;
    delete delNode;
    return head;
}

Node* delete_by_value(int value, Node* head) {
    if (head == nullptr) {
        cout << "List is empty" << endl;
        return head;
    }

    if (head->data == value)
        return delete_begin(head);

    Node* temp = head;
    while (temp->next != nullptr && temp->next->data != value)
        temp = temp->next;

    if (temp->next == nullptr) {
        cout << "Value not found" << endl;
        return head;
    }

    Node* delNode = temp->next;
    temp->next = delNode->next;
    delete delNode;
    return head;
}

void display(Node* head) {
    Node* temp = head;
    while (temp != nullptr) {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << "NULL" << endl;
}

int main() {
    Node* head = nullptr;

    head = insert_begin(10, head);
    head = insert_end(20, head);
    head = insert_at_index(15, 1, head);
    head = insert_after_value(25, 20, head);

    display(head);

    head = delete_begin(head);
    head = delete_end(head);
    head = delete_at_index(1, head);
    head = delete_by_value(15, head);

    display(head);

    return 0;
}
