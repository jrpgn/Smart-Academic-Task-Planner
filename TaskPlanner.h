#ifndef TASKPLANNER_H
#define TASKPLANNER_H

/**
 * @file TaskPlanner.h
 * @brief Declara el controlador principal del planificador y sus estructuras de datos.
 */

#include <stack>
#include <queue>
#include <string>
#include <vector>
#include <map>

#include "Task.h"
#include "LinkedList.h"

using namespace std;

/**
 * @brief Controlador principal del planificador de tareas academicas.
 *
 * TaskPlanner es propietario de las estructuras de datos en memoria usadas por
 * la aplicacion: una lista enlazada para almacenar tareas, una tabla hash para
 * busqueda por ID, una pila para el historial de acciones, una cola para IDs de
 * tareas pendientes y un grafo dirigido para dependencias entre tareas.
 */
class TaskPlanner {
private:
    /** @brief Lista enlazada principal que almacena objetos Task. */
    LinkedList<Task> taskList;

    /** @brief Pila de acciones recientes del planificador, con la mas nueva arriba. */
    stack<string> actionHistory;

    /**
     * @brief Cola de IDs de tareas pendientes.
     *
     * Se almacenan IDs en lugar de punteros para evitar referencias invalidas
     * despues de eliminar una tarea.
     */
    queue<int> pendingTasks;

    /** @brief Numero de cubetas usadas por la tabla hash interna. */
    static const int HASH_SIZE = 101;

    /**
     * @brief Nodo usado para encadenamiento separado en la tabla hash.
     */
    struct HashNode {
        /** @brief ID de tarea usado como llave hash. */
        int key;

        /** @brief Puntero a la tarea almacenada en la lista enlazada. */
        Task* value;

        /** @brief Siguiente nodo en la misma cubeta hash. */
        HashNode* next;
    };

    /** @brief Cubetas de la tabla hash. */
    HashNode* hashTable[HASH_SIZE];

    /**
     * @brief Convierte un ID de tarea en un indice de la tabla hash.
     *
     * @param key ID de tarea.
     * @return Indice valido de cubeta.
     */
    int hashFunction(int key) const;

    /**
     * @brief Libera todos los nodos de la tabla hash.
     */
    void clearHashTable();

    /**
     * @brief Inserta un puntero a tarea en la tabla hash.
     *
     * @param id ID de tarea usado como llave.
     * @param taskPtr Puntero a la tarea almacenada en la lista enlazada.
     * @return true si se inserto; de lo contrario false.
     */
    bool insertHash(int id, Task* taskPtr);

    /**
     * @brief Busca un puntero a tarea en la tabla hash.
     *
     * @param id ID de tarea que se buscara.
     * @return Puntero a la tarea si se encuentra; de lo contrario nullptr.
     */
    Task* searchHash(int id) const;

    /**
     * @brief Elimina una referencia de tarea de la tabla hash.
     *
     * @param id ID de tarea que se eliminara.
     * @return true si se elimino un nodo hash; de lo contrario false.
     */
    bool removeHash(int id);

    /**
     * @brief Grafo dirigido de prerrequisitos de tareas.
     *
     * La llave es un ID de tarea y los valores son IDs de tareas que deben
     * completarse antes de esa tarea.
     */
    map<int, vector<int> > dependencyGraph;

    /**
     * @brief Verifica si ya existe una dependencia directa.
     *
     * @param taskId Tarea que depende de otra tarea.
     * @param prerequisiteId Tarea prerrequisito requerida.
     * @return true si la dependencia ya existe; de lo contrario false.
     */
    bool dependencyExists(int taskId, int prerequisiteId) const;

    /**
     * @brief Verifica si existe un camino entre dos tareas en el grafo.
     *
     * @param startId ID de tarea inicial.
     * @param targetId ID de tarea objetivo.
     * @return true si existe un camino; de lo contrario false.
     */
    bool hasPath(int startId, int targetId) const;

    /**
     * @brief Auxiliar DFS recursivo usado por hasPath().
     *
     * @param currentId ID de tarea actual que se esta explorando.
     * @param targetId ID de tarea objetivo.
     * @param visited IDs ya visitados durante esta busqueda.
     * @return true si se puede alcanzar el objetivo; de lo contrario false.
     */
    bool hasPathDFS(int currentId, int targetId, vector<int>& visited) const;

    /**
     * @brief Elimina cada arista de dependencia relacionada con una tarea eliminada.
     *
     * @param id ID de la tarea eliminada.
     */
    void removeDependencyReferences(int id);

    /**
     * @brief Limpia cada estructura de datos en memoria propiedad del planificador.
     */
    void clearAllData();

    /**
     * @brief Agrega una tarea cargada desde almacenamiento sin registrar una accion nueva.
     *
     * @param task Tarea reconstruida desde archivo.
     * @return true si la tarea fue aceptada; de lo contrario false.
     */
    bool addLoadedTask(const Task& task);

    /**
     * @brief Agrega una dependencia cargada desde almacenamiento sin registrar una accion nueva.
     *
     * @param taskId ID de la tarea dependiente.
     * @param prerequisiteId ID de la tarea prerrequisito.
     * @return true si la dependencia fue aceptada; de lo contrario false.
     */
    bool addLoadedDependency(int taskId, int prerequisiteId);

public:
    /**
     * @brief Crea un planificador vacio.
     */
    TaskPlanner();

    /**
     * @brief Libera los nodos de la tabla hash asignados dinamicamente.
     */
    ~TaskPlanner();

    /**
     * @brief Agrega una nueva tarea al planificador.
     *
     * @param id ID unico de la tarea.
     * @param title Titulo de la tarea.
     * @param desc Descripcion opcional de la tarea.
     * @param course Nombre del curso.
     * @param prio Prioridad de 1 a 3.
     * @param m Mes de la fecha limite.
     * @param d Dia de la fecha limite.
     * @param y Anio de la fecha limite.
     * @return true si la tarea fue creada; de lo contrario false.
     */
    bool addTask(int id, string title, string desc, string course, int prio, int m, int d, int y);

    /**
     * @brief Muestra cada tarea almacenada en el planificador.
     */
    void displayTasks() const;

    /**
     * @brief Busca una tarea por ID.
     *
     * @param id ID de tarea que se buscara.
     * @return Puntero a la tarea si se encuentra; de lo contrario nullptr.
     */
    Task* searchTaskByID(int id) const;

    /**
     * @brief Elimina una tarea y sus referencias de dependencia.
     *
     * @param id ID de tarea que se eliminara.
     * @return true si la tarea fue eliminada; de lo contrario false.
     */
    bool removeTask(int id);

    /**
     * @brief Marca una tarea como completada si todos los prerrequisitos estan completos.
     *
     * @param id ID de tarea que se completara.
     * @return true si la tarea fue marcada como completada; de lo contrario false.
     */
    bool markTaskCompleted(int id);

    /**
     * @brief Agrega una dependencia entre dos tareas existentes.
     *
     * @param taskId ID de la tarea dependiente.
     * @param prerequisiteId ID de la tarea prerrequisito.
     * @return true si la dependencia fue registrada; de lo contrario false.
     */
    bool addDependency(int taskId, int prerequisiteId);

    /**
     * @brief Muestra el grafo de dependencias como una lista de adyacencia.
     */
    void displayDependencyGraph() const;

    /**
     * @brief Verifica si una tarea puede completarse.
     *
     * @param id ID de tarea que se verificara.
     * @return true si la tarea existe y todos los prerrequisitos estan completados.
     */
    bool canTaskBeCompleted(int id) const;

    /**
     * @brief Obtiene los prerrequisitos directos que todavia estan pendientes.
     *
     * @param id ID de tarea que se inspeccionara.
     * @return IDs de tareas prerrequisito no completadas.
     */
    vector<int> getUncompletedPrerequisites(int id) const;

    /**
     * @brief Obtiene todos los prerrequisitos directos de una tarea.
     *
     * @param id ID de tarea que se inspeccionara.
     * @return IDs de tareas prerrequisito directas.
     */
    vector<int> getPrerequisites(int id) const;

    /**
     * @brief Guarda tareas, historial y dependencias en archivos de texto.
     *
     * @param folderPath Carpeta donde se escribiran los archivos.
     * @return true si todos los archivos fueron guardados; de lo contrario false.
     */
    bool saveData(const string& folderPath = ".") const;

    /**
     * @brief Carga tareas, historial y dependencias desde archivos de texto.
     *
     * @param folderPath Carpeta desde donde se leeran los archivos.
     * @return true si todos los archivos fueron validos y cargados; de lo contrario false.
     */
    bool loadData(const string& folderPath = ".");

    /**
     * @brief Muestra el historial de acciones desde la mas nueva hasta la mas antigua.
     */
    void showActionsHistory() const;

    /**
     * @brief Muestra las tareas que todavia estan pendientes.
     */
    void showPendingTasks() const;
};

#endif
