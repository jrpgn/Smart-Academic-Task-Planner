# Smart Academic Task Planner

## 2. Introduccion

Smart Academic Task Planner es una aplicacion de consola desarrollada en C++ para organizar tareas academicas. El sistema permite registrar tareas por curso, asignarles prioridad, fecha limite y estado, consultar informacion rapidamente, manejar tareas pendientes, registrar dependencias entre actividades y guardar los datos en archivos de texto.


## 3. Descripcion del problema

Los estudiantes suelen manejar tareas, examenes, proyectos y entregas de distintas clases al mismo tiempo. Cuando estas actividades se registran de forma desordenada, es facil olvidar fechas, repetir informacion o intentar completar tareas que dependen de otras actividades previas.

El problema principal que resuelve esta aplicacion es la gestion organizada de tareas academicas, considerando tres necesidades importantes: almacenar informacion de cada tarea, encontrar tareas de forma eficiente por ID y controlar relaciones de dependencia entre tareas. Ademas, el sistema conserva un historial de acciones y permite persistir la informacion para no perder los datos al cerrar el programa.

## 4. Objetivos del proyecto

- Desarrollar una aplicacion funcional en C++ orientada a la gestion de tareas academicas.
- Aplicar estructuras de datos vistas en clase dentro de un problema practico.
- Permitir crear, consultar, eliminar y completar tareas.
- Usar una tabla hash para acelerar la busqueda por ID.
- Mantener una cola de tareas pendientes.
- Registrar un historial de acciones mediante una pila.
- Representar dependencias entre tareas mediante un grafo dirigido.
- Validar datos importantes como IDs, prioridades y fechas.
- Guardar y cargar informacion desde archivos externos.

## 5. Diseno general del sistema

El sistema esta organizado en varios archivos:

- `main.cpp`: contiene el menu principal, la captura de datos del usuario y las validaciones iniciales de entrada.
- `Task.h`: declara la estructura `Task`, la estructura `Date` y las operaciones asociadas.
- `Task.cpp`: implementa los constructores de `Task`, la comparacion por ID y la impresion formateada.
- `LinkedList.h`: declara una lista enlazada generica usada para almacenar las tareas.
- `LinkedList.cpp`: implementa la lista enlazada e instancia explicitamente `LinkedList<Task>`.
- `TaskPlanner.h`: declara la clase principal del sistema y sus estructuras internas.
- `TaskPlanner.cpp`: implementa la logica de negocio: tareas, historial, cola de pendientes, hash table, dependencias y persistencia.

La clase central es `TaskPlanner`. Esta clase actua como controlador del sistema porque coordina todas las estructuras de datos. Las tareas se almacenan en una lista enlazada, pero tambien se indexan en una tabla hash para poder buscarlas mas rapido por ID. Las acciones recientes se guardan en una pila, las tareas pendientes en una cola y las dependencias en un grafo representado con una lista de adyacencia.

El flujo general de uso ocurre desde el menu de consola. El usuario selecciona una opcion, `main.cpp` valida los datos basicos y luego llama a los metodos publicos de `TaskPlanner`.

## 6. Estructuras de datos utilizadas

### Lista enlazada

La lista enlazada se implementa en `LinkedList.h` y almacena objetos `Task`. Cada nodo contiene una tarea y un puntero al siguiente nodo. Se usa como estructura principal de almacenamiento en memoria.

Operaciones principales:

- Insertar tareas al frente.
- Recorrer e imprimir todas las tareas.
- Buscar por ID.
- Eliminar por ID.
- Convertir la lista a `vector` para guardar los datos.

### Tabla hash

La tabla hash esta integrada en `TaskPlanner`. Usa un arreglo de tamano fijo `HASH_SIZE = 101` y nodos enlazados para manejar colisiones mediante encadenamiento.

La llave es el ID de la tarea y el valor es un puntero a la tarea almacenada en la lista enlazada. Esto evita recorrer toda la lista cada vez que se busca una tarea.

### Pila

La pila `stack<string> actionHistory` guarda las acciones realizadas, como agregar, eliminar, completar una tarea o agregar una dependencia. Se utiliza porque el historial se muestra desde la accion mas reciente hacia la mas antigua.

### Cola

La cola `queue<int> pendingTasks` almacena los IDs de las tareas pendientes. Se guardan IDs en lugar de punteros para evitar referencias invalidas si una tarea se elimina de la lista.

### Grafo dirigido

El grafo de dependencias se representa con `map<int, vector<int> > dependencyGraph`. La llave es el ID de una tarea y el vector contiene los IDs de las tareas que deben completarse antes.

Una relacion `taskId -> prerequisiteId` significa que la tarea `taskId` depende de `prerequisiteId`.

### Vectores

Se usan vectores para almacenar temporalmente tareas cargadas desde archivo, dependencias, prerrequisitos y listas de nodos visitados durante busquedas DFS.

## 7. Explicacion de funcionalidades implementadas

La aplicacion permite agregar tareas con ID, curso, titulo, descripcion, prioridad y fecha limite. Antes de crear una tarea se valida que el ID este entre `001` y `999`, que la prioridad sea `1`, `2` o `3`, y que la fecha sea real dentro del rango permitido de 2026 a 2028.

Tambien permite mostrar todas las tareas, buscar una tarea por ID, eliminar tareas y marcar tareas como completadas. La busqueda se realiza mediante la tabla hash, lo que mejora el rendimiento frente a una busqueda lineal en la lista enlazada.

El sistema maneja una cola de tareas pendientes. Cuando se agrega una tarea nueva, su ID entra a la cola. Al mostrar tareas pendientes, el programa revisa cada ID y solo imprime las tareas que todavia existen y no han sido completadas.

El historial de acciones se administra con una pila. Cada accion importante se agrega al tope de la pila y se muestra en orden de mas reciente a mas antiguo.

Otra funcionalidad importante es el manejo de dependencias. El usuario puede indicar que una tarea depende de otra. El sistema valida que ambas tareas existan, que no sean la misma, que la dependencia no este duplicada y que no cree un ciclo en el grafo. Para esto se usa una busqueda en profundidad.

Antes de completar una tarea, el sistema verifica si todos sus prerrequisitos directos estan completados. Si hay tareas previas pendientes, muestra sus IDs y evita que la tarea se marque como completada.

Finalmente, la aplicacion implementa persistencia en tres archivos: `tasks.txt`, `history.txt` y `dependencies.txt`. La opcion de guardar escribe las tareas, historial y dependencias; la opcion de cargar reconstruye la lista enlazada, tabla hash, cola, pila y grafo. Durante la carga se validan los archivos antes de reemplazar los datos actuales.

## 8. Analisis de complejidad de operaciones clave

Sea `n` la cantidad de tareas, `m` la cantidad de dependencias y `k` la cantidad de tareas en una misma posicion de la tabla hash por colisiones.

- Agregar tarea: `O(n)` en esta implementacion, porque despues de insertar en la lista se busca el ID dentro de la lista para obtener el puntero. La insercion al frente es `O(1)` y la insercion en hash es `O(k)`.
- Buscar tarea por ID: promedio `O(1)` usando la tabla hash; peor caso `O(n)` si muchas tareas caen en la misma posicion.
- Mostrar todas las tareas: `O(n)`, porque recorre toda la lista enlazada.
- Eliminar tarea: promedio `O(n + m)`. Aunque la busqueda inicial por hash es eficiente, eliminar de la lista requiere recorrido lineal y luego se eliminan referencias en el grafo.
- Marcar tarea como completada: `O(p)` para revisar sus prerrequisitos directos, donde `p` es la cantidad de prerrequisitos de esa tarea.
- Agregar dependencia: `O(n + m)` en el peor caso, porque valida existencia con hash y usa DFS para comprobar que no se forme un ciclo.
- Mostrar historial: `O(h)`, donde `h` es la cantidad de acciones registradas.
- Mostrar pendientes: `O(q)`, donde `q` es la cantidad de IDs almacenados en la cola.
- Guardar datos: `O(n + h + m)`, porque escribe tareas, historial y dependencias.
- Cargar datos: `O(n^2 + m(n + m))` en el peor caso por las validaciones de duplicados, existencia de tareas y ciclos durante la reconstruccion.

## 9. Retos encontrados y soluciones adoptadas

Uno de los retos principales fue combinar varias estructuras sin duplicar datos de forma peligrosa. La solucion fue almacenar las tareas reales en la lista enlazada y usar la tabla hash solo como indice mediante punteros a esas tareas.

Otro reto fue evitar referencias invalidas. En la cola de pendientes se decidio guardar IDs en lugar de punteros, porque una tarea puede ser eliminada y un puntero podria quedar apuntando a memoria liberada.

El manejo de dependencias tambien presento un reto importante: evitar ciclos. Para solucionarlo, antes de insertar una nueva dependencia se revisa si ya existe un camino desde el prerrequisito hacia la tarea. Si ese camino existe, agregar la nueva relacion cerraria un ciclo y se rechaza.

La persistencia en archivos requirio cuidar los campos de texto. Como los titulos, descripciones o cursos podrian contener caracteres especiales, se implementaron funciones para escapar tabs, saltos de linea y barras invertidas al guardar, y para restaurarlos al cargar.

Finalmente, se agregaron validaciones para evitar datos corruptos: IDs fuera de rango, prioridades invalidas, fechas imposibles, tareas duplicadas, dependencias repetidas o referencias a tareas inexistentes.

## 10. Resultados obtenidos

El resultado es una aplicacion de consola funcional que permite administrar tareas academicas y demuestra el uso integrado de varias estructuras de datos.

El sistema logra:

- Crear tareas con informacion academica completa.
- Consultar tareas rapidamente por ID.
- Mostrar todas las tareas registradas.
- Eliminar tareas y limpiar sus dependencias.
- Marcar tareas como completadas.
- Bloquear la finalizacion de tareas con prerrequisitos pendientes.
- Registrar y mostrar historial de acciones.
- Mostrar tareas pendientes.
- Registrar y visualizar dependencias.
- Guardar y cargar informacion desde archivos.

La aplicacion cumple con el enfoque del proyecto original porque convierte las estructuras de datos en partes utiles de un sistema real, no solo en ejemplos aislados.

## 11. Conclusiones

Smart Academic Task Planner demuestra como diferentes estructuras de datos pueden trabajar juntas dentro de una misma aplicacion. La lista enlazada funciona como almacenamiento principal, la tabla hash mejora la busqueda, la pila organiza el historial, la cola mantiene el orden de tareas pendientes y el grafo permite modelar dependencias academicas.

El proyecto tambien muestra la importancia de validar entradas y proteger la consistencia interna del sistema. Las dependencias, los archivos y los IDs requieren controles para evitar errores logicos. Gracias a estas validaciones, la aplicacion es mas estable y cercana a una herramienta util para estudiantes.

En conclusion, el sistema cumple su objetivo academico y practico: permite organizar tareas mientras evidencia el uso correcto de estructuras de datos fundamentales en C++.

## 12. Posibles mejoras futuras

- Ordenar tareas por fecha limite o prioridad.
- Permitir busquedas por curso, titulo o estado.
- Implementar una interfaz grafica o una version web.
- Agregar edicion de tareas existentes.
- Eliminar definitivamente de la cola los IDs de tareas ya completadas o borradas.
- Mostrar dependencias de forma mas visual.
- Implementar un orden topologico para sugerir el mejor orden de trabajo.
- Permitir fechas fuera del rango 2026-2028.
- Agregar pruebas automatizadas para validar operaciones clave.
- Usar archivos en formato JSON o CSV estructurado para facilitar interoperabilidad.
