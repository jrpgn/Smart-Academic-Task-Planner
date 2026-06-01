#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/**
 * @file LinkedList.h
 * @brief Declara una plantilla generica de lista enlazada simple.
 */

#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief Lista enlazada simple generica usada por el planificador.
 *
 * @tparam DataType Tipo almacenado en cada nodo. El tipo debe soportar
 * comparacion con un ID entero y salida por flujo cuando se usen las
 * operaciones de busqueda o visualizacion.
 */
template <class DataType>
class LinkedList {
private:
    /**
     * @brief Nodo interno de la lista enlazada simple.
     */
    struct Node {
        /** @brief Dato almacenado en este nodo. */
        DataType info;

        /** @brief Puntero al siguiente nodo de la lista. */
        Node* next;
    };

    /** @brief Puntero al primer nodo de la lista. */
    Node* start;

public:
    /**
     * @brief Crea una lista enlazada vacia.
     */
    LinkedList();

    /**
     * @brief Libera todos los nodos que pertenecen a la lista.
     */
    ~LinkedList();

    /**
     * @brief Inserta un nuevo elemento al inicio de la lista.
     *
     * @param newData Elemento que se insertara.
     */
    void insertFront(const DataType& newData);

    /**
     * @brief Elimina todos los nodos de la lista.
     */
    void clear();

    /**
     * @brief Muestra todos los elementos en orden de insercion, del mas nuevo al mas antiguo.
     */
    void displayAll() const;

    /**
     * @brief Busca un elemento por ID entero.
     *
     * @param id Identificador que se buscara.
     * @return Puntero al elemento almacenado si se encuentra; de lo contrario nullptr.
     */
    DataType* searchByID(int id) const;

    /**
     * @brief Elimina el primer elemento cuyo ID coincide con el objetivo.
     *
     * @param target Identificador del elemento que se eliminara.
     * @return true si se elimino un elemento; de lo contrario false.
     */
    bool removeByID(int target);

    /**
     * @brief Copia todos los elementos de la lista en un vector.
     *
     * @return Vector que contiene los elementos de la lista.
     */
    vector<DataType> toVector() const;
};

#endif
