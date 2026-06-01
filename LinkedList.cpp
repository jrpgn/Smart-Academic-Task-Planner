#include "LinkedList.h"

/**
 * @file LinkedList.cpp
 * @brief Implementa la plantilla de lista enlazada usada por el planificador.
 */

#include "Task.h"

/** 
 * @brief Crea una lista enlazada vacia.
 */
template <class DataType>
LinkedList<DataType>::LinkedList() : start(nullptr) {}

/**
 * @brief Libera todos los nodos que pertenecen a la lista.
 */
template <class DataType>
LinkedList<DataType>::~LinkedList() {
    clear();
}

/**
 * @brief Elimina todos los nodos de la lista y la deja vacia.
 */
template <class DataType>
void LinkedList<DataType>::clear() {
    Node* current = start;

    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }

    start = nullptr;
}

/**
 * @brief Inserta un nuevo elemento al inicio de la lista.
 *
 * @param newData Elemento que se insertara.
 */
template <class DataType>
void LinkedList<DataType>::insertFront(const DataType& newData) {
    Node* newNode = new Node;
    newNode->info = newData;
    newNode->next = start;
    start = newNode;
}

/**
 * @brief Muestra cada nodo usando el operador de salida del tipo almacenado.
 */
template <class DataType>
void LinkedList<DataType>::displayAll() const {
    if (start == nullptr) {
        cout << "\n * No data found *\n";
        return;
    }

    Node* current = start;

    while (current != nullptr) {
        cout << current->info << '\n';
        current = current->next;
    }
}

/**
 * @brief Busca un nodo por su ID entero.
 *
 * @param id Identificador que se buscara.
 * @return Puntero al elemento almacenado si se encuentra; de lo contrario nullptr.
 */
template <class DataType>
DataType* LinkedList<DataType>::searchByID(int id) const {
    Node* current = start;

    while (current != nullptr) {
        if (current->info == id) {
            return &(current->info);
        }

        current = current->next;
    }

    return nullptr;
}

/**
 * @brief Elimina el primer nodo cuyo elemento coincide con el ID objetivo.
 *
 * @param target Identificador que se eliminara.
 * @return true si se elimino un nodo; de lo contrario false.
 */
template <class DataType>
bool LinkedList<DataType>::removeByID(int target) {
    if (start == nullptr) {
        return false;
    }

    if (start->info == target) {
        Node* ptr = start;
        start = start->next;
        delete ptr;
        return true;
    }

    Node* current = start->next;
    Node* previous = start;

    while (current != nullptr) {
        if (current->info == target) {
            previous->next = current->next;
            delete current;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

template class LinkedList<Task>;
