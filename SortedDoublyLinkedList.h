#ifndef SORTED_DOUBLY_LINKED_LIST_H
#define SORTED_DOUBLY_LINKED_LIST_H

#include <iostream>
using namespace std;

template<class T>
class SortedDoublyLinkedList {
    private:
        struct Link {
            T data1;
            T data2;
            Link *next;         // Points to next
            Link *prev;         // Points to previous
        };

        Link *head;
        int size;
    public:
        SortedDoublyLinkedList() { head = NULL; size = 0; }
        void displayTop(int size);
        int getSize() { return size; }
        void printList();
        void addList(const T& data1, const T& data2);
        int getTotal();
        void moveToFront(const T& data);
        T inList(const T& data);
        ~SortedDoublyLinkedList() {
            Link *ptr = head;
            while (ptr != NULL) {
                delete ptr;
                ptr = ptr->next;
            }
        }
};

// ============================================================================
// displayTop.
//
// Input -> how many to display.
// Output -> the top N elements to stdout.
// ============================================================================
template<class T>
void SortedDoublyLinkedList<T>::displayTop(int size) {
    Link *ptr = head;
    int i = 0;
    while (i < size) {
        cout << FBLACK_GREEN << i << "). " << ptr->data1 << ", ";
        ptr = ptr->next;
        i++;
    }

    cout << RST << endl;
}

// ============================================================================
// inList.
//
// Input -> The value to find.
// Output -> True if found, else false.
// ============================================================================
template<class T>
T SortedDoublyLinkedList<T>::inList(const T& data1) {
    Link *ptr = head;

    while (ptr != NULL) {
        if (ptr->data1 == data1) {
            break;
        }

        ptr = ptr->next;
    }

    if (ptr != NULL) return ptr->data2;

    return "We could not find that word.";
}

// ============================================================================
// moveToFront.
//
// Input -> nothing.
// Output -> nothing.
// ============================================================================
template<class T>
void SortedDoublyLinkedList<T>::moveToFront(const T& data) {
    if (data == head->data) {
        cout << "Priority for value " << data << " is already set." << endl;
    }

    // Find the target link.
    Link *ptr = head;
    Link *tmp = head;
    while (ptr != NULL) {
        if (ptr->data == data) {
            break;
        }

        ptr = ptr->next;
    }

    // Set the new null
    if (ptr->next == NULL) {
        ptr->prev->next = NULL;

        return;
    }

    ptr->prev->next = ptr->next;
    ptr->next->prev = ptr->prev;
    head = ptr;
    ptr->prev = NULL;
    ptr->next = tmp;
}

// ============================================================================
// printList.
//
// Input -> nothing.
// Output -> prints the list.
// ============================================================================
template<class T>
void SortedDoublyLinkedList<T>::printList() {
    cout << "\n\nPrinting SortedDoublyLinkedList.\n";
    cout << "NULL ";

    Link *ptr = head;
    while (ptr != NULL) {
        cout << " <- " << ptr->data << " -> ";
        ptr = ptr->next;
    }

    cout << "NULL" << endl;
}

// ============================================================================
// addList
//
// Input -> The value to add.
// Output -> nothing.
// ============================================================================
template<class T>
void SortedDoublyLinkedList<T>::addList(const T& data1, const T& data2) {
    // Add to empty list
    if (head == NULL) {
        Link *temp = new Link;
        temp->prev = NULL;
        temp->next = NULL;
        temp->data1 = data1;
        temp->data2 = data2;
        head = temp;
        size++;

        return;
    }

    // Add to front
    Link *ptr = head;
    if (head->data1 > data1) {
        Link *temp = new Link;
        temp->data1 = data1;
        temp->data2 = data2;
        temp->prev = NULL;
        temp->next = ptr;
        head = temp;
        size++;

        return;
    }

    // Add to single link
    ptr = head;
    if (getTotal() < 2) {
        if (data1 <= head->data1) {
            Link *temp = new Link;
            temp->data1 = data1;
            temp->data2 = data2;
            temp->prev = NULL;
            temp->next = ptr;
            ptr->prev = temp;
            head = temp;
            size++;

            return;                
        }

        if (data1 > head->data1) {
            Link *temp = new Link;
            temp->data1 = data1;
            temp->data2 = data2;
            temp->prev = ptr;
            temp->next = NULL;
            ptr->next = temp;
            size++;

            return;
        }
    }

    // Find position of next link.
    Link *prev = head;
    Link *after = head->next;
    while (data1 > prev->data1) {
        if (data1 < after->data1) {
            break;
        }

        prev = prev->next;
        after = after->next;

        if (after == NULL) break;
    }

    // Insert
    Link *temp = new Link;
    temp->data1 = data1;
    temp->data2 = data2;
    temp->next = after;
    temp->prev = prev;
    prev->next = temp;
    size++;

}


// ============================================================================
// getTotal.
//
// Input -> nothing.
// Output -> total number of links in the list.
// ============================================================================
template<class T>
int SortedDoublyLinkedList<T>::getTotal() {
    if (head == NULL) {
        return 0;
    }

    int i = 0;
    Link *ptr = head;
    while (ptr != NULL) {
        i++;
        ptr = ptr->next;
    }

    return i;
}

#endif /* SORTED_DOUBLY_LINKED_LIST_H */
