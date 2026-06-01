#include "Task.h"

/**
 * @file Task.cpp
 * @brief Implementa el comportamiento de la estructura de datos de tarea.
 */

#include <iomanip>

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
Task::Task(int _id, string _title, string _desc, string _course, int _prio, Date _dl)
    : id(_id),
      priority(_prio),
      title(_title),
      description(_desc),
      courseName(_course),
      deadline(_dl),
      completed(false) {}

/**
 * @brief Construye una tarea vacia por defecto.
 */
Task::Task()
    : id(0),
      priority(3),
      title(""),
      description(""),
      courseName(""),
      deadline({1, 1, 2026}),
      completed(false) {}

/**
 * @brief Compara una tarea con un ID entero.
 *
 * @param _id Identificador que se comparara con esta tarea.
 * @return true si el ID de la tarea coincide con el ID proporcionado; de lo contrario false.
 */
bool Task::operator==(const int& _id) const {
    return id == _id;
}

/**
 * @brief Escribe los detalles de una tarea en un flujo de salida con formato.
 *
 * @param out Flujo de salida donde se escribira la tarea.
 * @param task Tarea que se imprimira.
 * @return Referencia al mismo flujo de salida.
 */
ostream& operator<<(ostream& out, const Task& task) {
    out << "\n [Task ID: " << setfill('0') << setw(3) << task.id << "]"
        << "\n  Title: " << task.title
        << "\n  Description: " << (task.description.empty() ? "-empty-" : task.description)
        << "\n  Course: " << task.courseName
        << "\n  Priority: " << task.priority
        << "\n  Due: "
        << setfill('0') << setw(2) << task.deadline.mm << "/"
        << setfill('0') << setw(2) << task.deadline.dd << "/"
        << task.deadline.yy << " (m/d/y)"
        << "\n  Status: " << (task.completed ? "Completed" : "Pending") << "\n";

    out << setfill(' ');
    return out;
}
