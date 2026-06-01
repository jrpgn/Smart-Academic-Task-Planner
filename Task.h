#ifndef TASK_H
#define TASK_H

/**
 * @file Task.h
 * @brief Define las estructuras de datos de tarea y fecha usadas por el planificador.
 */

#include <iostream>
#include <string>

using namespace std;

/**
 * @brief Representa la fecha limite de una tarea.
 */
struct Date {
    /** @brief Mes de la fecha limite. */
    int mm;

    /** @brief Dia de la fecha limite. */
    int dd;

    /** @brief Año de la fecha limite. */
    int yy;
};
/** @brief Determina si un año es bisiesto.
 *
 * @param year Año que se evaluara.
 * @return true si el año es bisiesto; de lo contrario false.
 */
bool isLeapYear(int year);

/** @brief Obtiene la cantidad de dias de un mes para un año especifico.
 *
 * @param month Numero de mes de 1 a 12.
 * @param year Año usado para evaluar febrero en años bisiestos.
 * @return Cantidad de dias del mes proporcionado.
 */
int daysInMonth(int month, int year);

/**
 * @brief Representa una tarea academica gestionada por el planificador.
 */
struct Task {
    /** @brief Identificador unico de la tarea en el rango 001 a 999. */
    int id;

    /** @brief Prioridad de la tarea, donde 1 es alta, 2 media y 3 baja. */
    int priority;

    /** @brief Titulo corto de la tarea. */
    string title;

    /** @brief Descripcion opcional de la tarea. */
    string description;

    /** @brief Nombre del curso asociado con la tarea. */
    string courseName;

    /** @brief Fecha limite asignada a la tarea. */
    Date deadline;

    /** @brief Estado de finalizacion de la tarea. */
    bool completed;

    /**
     * @brief Construye una tarea con los valores proporcionados por el usuario.
     *
     * @param _id Identificador unico de la tarea.
     * @param _title Titulo de la tarea.
     * @param _desc Descripcion de la tarea.
     * @param _course Nombre del curso.
     * @param _prio Prioridad de la tarea.
     * @param _dl Fecha limite de la tarea.
     */
    Task(int _id, string _title, string _desc, string _course, int _prio, Date _dl);

    /**
     * @brief Construye una tarea vacia por defecto.
     */
    Task();

    /**
     * @brief Compara una tarea con un ID entero.
     *
     * @param _id Identificador que se comparara con esta tarea.
     * @return true si el ID de la tarea coincide con el ID proporcionado; de lo contrario false.
     */
    bool operator==(const int& _id) const;

    /**
     * @brief Escribe los detalles de una tarea en un flujo de salida con formato.
     *
     * @param out Flujo de salida donde se escribira la tarea.
     * @param task Tarea que se imprimira.
     * @return Referencia al mismo flujo de salida.
     */
    friend ostream& operator<<(ostream& out, const Task& task);
};

#endif
