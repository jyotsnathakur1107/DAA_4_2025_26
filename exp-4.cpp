#include <bits/stdc++.h>
using namespace std;

#define CAPACITY 100

int heapArr[CAPACITY];
int heapSize = 0;

// Heapify down
void heapDown(int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heapSize && heapArr[left] < heapArr[smallest])
        smallest = left;
    if (right < heapSize && heapArr[right] < heapArr[smallest])
        smallest = right;

    if (smallest != idx)
    {
        swap(heapArr[idx], heapArr[smallest]);
        heapDown(smallest);
    }
}

// Heapify up
void heapUp(int idx)
{
    while (idx > 0 && heapArr[(idx - 1) / 2] > heapArr[idx])
    {
        swap(heapArr[(idx - 1) / 2], heapArr[idx]);
        idx = (idx - 1) / 2;
    }
}

// Insert element
void insertHeap(int value)
{
    if (heapSize == CAPACITY)
    {
        cout << "Heap overflow\n";
        return;
    }

    heapArr[heapSize] = value;
    heapSize++;
    heapUp(heapSize - 1);
}

// Delete root element
void deleteRoot()
{
    if (heapSize == 0)
    {
        cout << "Heap is empty\n";
        return;
    }

    heapArr[0] = heapArr[heapSize - 1];
    heapSize--;
    heapDown(0);
}

// Search element
int findValue(int value)
{
    for (int i = 0; i < heapSize; i++)
        if (heapArr[i] == value)
            return i;
    return -1;
}

// Delete a specific value
void deleteValue(int value)
{
    int index = findValue(value);
    if (index == -1)
    {
        cout << "Value not found\n";
        return;
    }

    heapArr[index] = heapArr[heapSize - 1];
    heapSize--;

    heapDown(index);
    heapUp(index);
}

// Print heap
void displayHeap()
{
    for (int i = 0; i < heapSize; i++)
        cout << heapArr[i] << " ";
    cout << endl;
}

int main()
{
    insertHeap(10);
    insertHeap(5);
    insertHeap(3);
    insertHeap(8);
    insertHeap(2);

    cout << "Heap: ";
    displayHeap();

    deleteRoot();
    cout << "After deleting root: ";
    displayHeap();

    deleteValue(8);
    cout << "After deleting 8: ";
    displayHeap();

    int key = 5;
    int pos = findValue(key);

    if (pos != -1)
        cout << key << " found at index " << pos << endl;
    else
        cout << key << " not found\n";

    return 0;
}
