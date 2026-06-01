/**
 * @file main.cpp
 * @brief Punto de entrada de consola para Smart Academic Task Planner.
 */
#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <sstream>
#include <vector>
#include "TaskPlanner.h"

using namespace std;

const int MENU_EXIT = 13;

/**
 * @brief Determina si un anio es bisiesto.
 *
 * @param year Anio que se evaluara.
 * @return true si el anio es bisiesto; de lo contrario false.
 */
bool isLeapYear(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

/**
 * @brief Obtiene la cantidad de dias de un mes para un anio especifico.
 *
 * @param month Numero de mes de 1 a 12.
 * @param year Anio usado para evaluar febrero en anios bisiestos.
 * @return Cantidad de dias del mes proporcionado.
 */
int daysInMonth(int month, int year) {
    switch (month) {
        case 2:
            return isLeapYear(year) ? 29 : 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 31;
    }
}

/**
 * @brief Valida una fecha limite aceptada por el planificador.
 *
 * @param month Numero de mes.
 * @param day Numero de dia.
 * @param year Numero de anio.
 * @return true si la fecha es real y esta entre 2026 y 2028; de lo contrario false.
 */
bool isValidDate(int month, int day, int year) {
    return year >= 2026 && year <= 2028
        && month >= 1 && month <= 12
        && day >= 1 && day <= daysInMonth(month, year);
}

/**
 * @brief Lee y valida un ID de tarea desde la entrada estandar.
 *
 * @param prompt Mensaje mostrado antes de leer el ID.
 * @param id Parametro de salida donde se almacena el ID valido.
 * @return true si el ID es valido; de lo contrario false.
 */
bool readTaskId(const string& prompt, int& id) {
    cout << prompt;

    if (!(cin >> id) || id < 1 || id > 999) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n >> Invalid ID. Enter a number from 001 to 999.\n";
        return false;
    }

    return true;
}

/**
 * @brief Muestra al usuario las opciones del menu principal.
 */
void displayMenu() {
    cout << "\n|--- SMART ACADEMIC TASK PLANNER ---|\n"
         << "  1. Add new task\n"
         << "  2. Show all tasks\n"
         << "  3. Search a task\n"
         << "  4. Delete a task\n"
         << "  5. Mark task as completed\n"
         << "  6. Show action history\n"
         << "  7. Show pending tasks\n"
         << "  8. Add task dependency\n"
         << "  9. Show dependency graph\n"
         << "  10. Check if a task can be completed\n"
         << "  11. Save data\n"
         << "  12. Load data\n"
         << "  " << MENU_EXIT << ". Exit\n"
         << "\n >> Select an option [1 - " << MENU_EXIT << "]: ";
}

/**
 * @brief Ejecuta el menu interactivo del planificador de tareas.
 *
 * @return Codigo de salida del programa.
 */
int main() {
    TaskPlanner planner;
    
    int choice = 0;

    do {
        displayMenu();
        
        // Valida la entrada del menu.
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n >> Invalid input. Enter a digit between 1 and " << MENU_EXIT << ". \n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                string title, desc, course;
                int id, priority;

                if (!readTaskId("\n Enter new task ID [001 - 999]: ", id)) {
                    break;
                }

                // Esta busqueda usa internamente la tabla hash.
                if (planner.searchTaskByID(id) != nullptr) {
                    cout << "\n * A task with ID " 
                         << setfill('0') << setw(3) << id 
                         << " already exists. *\n";
                    cout << setfill(' ');
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << " Enter the course name: ";
                getline(cin, course);

                while (course.empty()) {
                    cout << " >> Course name cannot be empty. Enter a course: ";
                    getline(cin, course);
                }

                cout << " Enter a title for the task: ";
                getline(cin, title);

                while (title.empty()) {
                    cout << " >> Title cannot be empty. Enter a title: ";
                    getline(cin, title);
                }

                cout << " Enter a description for the task (optional): ";
                getline(cin, desc);

                cout << " Enter priority (1 = high, 2 = medium, 3 = low): ";

                while (!(cin >> priority) || priority < 1 || priority > 3) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << " >> Invalid input. Enter 1, 2, or 3: ";
                }

                cout << " Enter task's deadline using format mm dd yyyy"
                     << "\n\t (year must be between 2026 and 2028): ";

                string dateInput;
                int m, d, y;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                while (true) {
                    getline(cin, dateInput);

                    if (dateInput.empty()) {
                        cout << " >> Date cannot be empty. Enter a deadline: ";
                        continue;
                    }

                    stringstream ss(dateInput);

                    if (ss >> m >> d >> y) {
                        if (isValidDate(m, d, y)) {
                            break;
                        } else {
                            cout << " >> Invalid date. Check month length and year range, then try again: ";
                        }
                    } else {
                        cout << " >> Invalid date format. Try again using mm dd yyyy: ";
                    }
                }

                if (planner.addTask(id, title, desc, course, priority, m, d, y)) {
                    cout << "\n * Task successfully created. *\n";
                } else {
                    cout << "\n * Task could not be created. ID may already exist. *\n";
                }

                cout << "\n >> Press Enter to continue...";
                cin.get();
                break;
            }

            case 2: {
                cout << "\n All Tasks (newest first):\n";
                planner.displayTasks();

                cout << "\n >> Press Enter to continue...";
                cin.get();
                break;
            }

            case 3: {
                int searchID;

                if (!readTaskId("\n Enter the ID of the task to search: ", searchID)) {
                    break;
                }

                // Esta busqueda usa internamente la tabla hash.
                Task* foundTask = planner.searchTaskByID(searchID);

                if (foundTask != nullptr) {
                    cout << "\n Task " 
                         << setfill('0') << setw(3) << searchID 
                         << " found:\n" << *foundTask;
                    cout << setfill(' ');
                } else {
                    cout << "\n * Task " 
                         << setfill('0') << setw(3) << searchID 
                         << " NOT found. *\n";
                    cout << setfill(' ');
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n >> Press Enter to continue...";
                cin.get();
                break;
            }

            case 4: {
                int taskToRemove;

                if (!readTaskId("\n Enter the ID of the task to be deleted: ", taskToRemove)) {
                    break;
                }

                if (planner.removeTask(taskToRemove)) {
                    cout << "\n * Task " 
                         << setfill('0') << setw(3) << taskToRemove 
                         << " deleted. *\n";
                    cout << setfill(' ');
                } else {
                    cout << "\n * Task " << taskToRemove << " does NOT exist. *\n";
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n >> Press Enter to continue...";
                cin.get();
                break;
            }

            case 5: {
                int taskToComplete;

                if (!readTaskId("\n Enter the ID of the task to mark as completed: ", taskToComplete)) {
                    break;
                }

                vector<int> blockers = planner.getUncompletedPrerequisites(taskToComplete);

                if (planner.markTaskCompleted(taskToComplete)) {
                    cout << "\n * Task " 
                         << setfill('0') << setw(3) << taskToComplete 
                         << " marked as completed. *\n";
                    cout << setfill(' ');
                } else if (planner.searchTaskByID(taskToComplete) != nullptr && !blockers.empty()) {
                    cout << "\n * Task "
                         << setfill('0') << setw(3) << taskToComplete
                         << " cannot be completed yet. Pending prerequisites: ";

                    for (size_t i = 0; i < blockers.size(); i++) {
                        cout << setfill('0') << setw(3) << blockers[i];

                        if (i + 1 < blockers.size()) {
                            cout << ", ";
                        }
                    }

                    cout << ". *\n";
                    cout << setfill(' ');
                } else {
                    cout << "\n * Task " << taskToComplete << " does NOT exist. *\n";
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n >> Press Enter to continue...";
                cin.get();
                break;
            }

            case 6: {
                planner.showActionsHistory();

                cout << "\n >> Press Enter to continue...";
                cin.get();
                break;
            }

            case 7: {
                planner.showPendingTasks();

                cout << "\n >> Press Enter to continue...";
                cin.get();
                break;
            }

            case 8: {
                int taskId;
                int prerequisiteId;

                if (!readTaskId("\n Enter the task ID that will depend on another task: ", taskId)) {
                    break;
                }

                if (!readTaskId(" Enter the prerequisite task ID: ", prerequisiteId)) {
                    break;
                }

                if (planner.addDependency(taskId, prerequisiteId)) {
                    cout << "\n * Dependency registered: Task "
                         << setfill('0') << setw(3) << taskId
                         << " depends on Task "
                         << setfill('0') << setw(3) << prerequisiteId
                         << ". *\n";
                    cout << setfill(' ');
                } else {
                    cout << "\n * Dependency could not be registered. Verify that both tasks exist,"
                         << " are different, are not duplicated, and do not create a cycle. *\n";
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n >> Press Enter to continue...";
                cin.get();
                break;
            }

            case 9: {
                planner.displayDependencyGraph();

                cout << "\n >> Press Enter to continue...";
                cin.get();
                break;
            }

            case 10: {
                int taskId;

                if (!readTaskId("\n Enter the task ID to verify: ", taskId)) {
                    break;
                }

                Task* task = planner.searchTaskByID(taskId);

                if (task == nullptr) {
                    cout << "\n * Task " << taskId << " does NOT exist. *\n";
                } else if (planner.canTaskBeCompleted(taskId)) {
                    cout << "\n * Task "
                         << setfill('0') << setw(3) << taskId
                         << " can be completed. *\n";
                    cout << setfill(' ');
                } else {
                    vector<int> blockers = planner.getUncompletedPrerequisites(taskId);

                    cout << "\n * Task "
                         << setfill('0') << setw(3) << taskId
                         << " cannot be completed yet. Pending prerequisites: ";

                    for (size_t i = 0; i < blockers.size(); i++) {
                        cout << setfill('0') << setw(3) << blockers[i];

                        if (i + 1 < blockers.size()) {
                            cout << ", ";
                        }
                    }

                    cout << ". *\n";
                    cout << setfill(' ');
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n >> Press Enter to continue...";
                cin.get();
                break;
            }

            case 11: {
                cout << "\n Saving planner data...\n";

                if (planner.saveData()) {
                    cout << " * Data saved successfully in tasks.txt, history.txt, and dependencies.txt. *\n";
                } else {
                    cout << " * Data could not be saved. Check file permissions and try again. *\n";
                }

                cout << "\n >> Press Enter to continue...";
                cin.get();
                break;
            }

            case 12: {
                char confirm;

                cout << "\n Loading data will replace the current in-memory planner."
                     << "\n Continue? (y/n): ";
                cin >> confirm;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (confirm != 'y' && confirm != 'Y') {
                    cout << "\n * Load cancelled. *\n";
                } else if (planner.loadData()) {
                    cout << "\n * Data loaded successfully. *\n";
                } else {
                    cout << "\n * Data could not be loaded. Verify that tasks.txt, history.txt,"
                         << " and dependencies.txt exist and are valid. *\n";
                }

                cout << "\n >> Press Enter to continue...";
                cin.get();
                break;
            }

            case MENU_EXIT: {
                cout << "\n Exiting Task Planner...\n";
                break;
            }

            default: {
                cout << "\n >> Invalid input. Enter a digit between 1 and " << MENU_EXIT << ". \n";
                break;
            }
        }

    } while (choice != MENU_EXIT);

    return 0;
}
