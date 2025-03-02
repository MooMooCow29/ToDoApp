#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <iomanip>  // For std::get_time

// Enum for task priority
enum Priority { Low, Medium, High };

// Enum for task recurrence
enum Recurrence { None, Daily, Weekly, Monthly };

// Structure to store task data
struct Task {
    std::string description;
    Priority priority;
    bool isCompleted;
    std::string dueDate;          // Due date in format YYYY-MM-DD
    std::vector<std::string> tags; // Tags for categorizing tasks
    Recurrence recurrence;        // Recurrence for repeating tasks
    std::string completionDate;   // Date when task was completed (if any)

    // Constructor to initialize default values
    Task()
        : priority(Low), isCompleted(false), recurrence(None) {
    }
};

//
// Function: displayMenu
// Description: Shows the main menu options to the user.
//
void displayMenu() {
    std::cout << "\nTodo List Application Menu:\n";
    std::cout << "1. Add a Task\n";
    std::cout << "2. Show All Tasks\n";
    std::cout << "3. Mark Task as Completed\n";
    std::cout << "4. Delete Task\n";
    std::cout << "5. Sort Tasks by Priority\n";
    std::cout << "6. Search Tasks\n";
    std::cout << "7. Backup Data\n";
    std::cout << "8. Restore Data\n";
    std::cout << "9. View Task Progress\n";
    std::cout << "10. Check for Due Tasks\n";
    std::cout << "11. Exit\n";
}

//
// Function: addTask
// Description: Prompts the user to enter details for a new task and adds it to the list.
//
void addTask(std::vector<Task>& tasks) {
    Task newTask;
    std::cout << "Enter task description: ";
    std::getline(std::cin, newTask.description);

    int priorityChoice;
    std::cout << "Enter task priority (0 = Low, 1 = Medium, 2 = High): ";
    std::cin >> priorityChoice;
    std::cin.ignore(); // Clear newline from input buffer
    newTask.priority = static_cast<Priority>(priorityChoice);

    // Ask for comma-separated tags
    std::string tagLine;
    std::cout << "Enter tags (comma separated, e.g., Work,Urgent): ";
    std::getline(std::cin, tagLine);
    std::stringstream ss(tagLine);
    std::string singleTag;
    while (std::getline(ss, singleTag, ',')) {
        // Trim spaces (if needed) and add non-empty tag
        if (!singleTag.empty())
            newTask.tags.push_back(singleTag);
    }

    // Ask for due date in YYYY-MM-DD format
    std::cout << "Enter due date (YYYY-MM-DD): ";
    std::getline(std::cin, newTask.dueDate);

    // Ask for recurrence type
    int recurrenceChoice;
    std::cout << "Enter recurrence (0 = None, 1 = Daily, 2 = Weekly, 3 = Monthly): ";
    std::cin >> recurrenceChoice;
    std::cin.ignore();
    newTask.recurrence = static_cast<Recurrence>(recurrenceChoice);

    tasks.push_back(newTask);
}

//
// Function: showTasks
// Description: Displays all tasks with their details.
//
void showTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "No tasks to display.\n";
        return;
    }
    for (size_t i = 0; i < tasks.size(); ++i) {
        std::cout << i + 1 << ". " << tasks[i].description
            << " | Priority: " << (tasks[i].priority == High ? "High" : (tasks[i].priority == Medium ? "Medium" : "Low"))
            << " | Due: " << tasks[i].dueDate
            << " | Status: " << (tasks[i].isCompleted ? "Completed" : "Pending")
            << " | Recurrence: "
            << (tasks[i].recurrence == Daily ? "Daily" : (tasks[i].recurrence == Weekly ? "Weekly" : (tasks[i].recurrence == Monthly ? "Monthly" : "None")))
            << " | Tags: ";
        for (const auto& tag : tasks[i].tags)
            std::cout << tag << " ";
        // Optionally show completion date if task is completed
        if (tasks[i].isCompleted)
            std::cout << "| Completed on: " << tasks[i].completionDate;
        std::cout << "\n";
    }
}

//
// Function: markTaskAsCompleted
// Description: Marks a task as completed and records the current date.
//
void markTaskAsCompleted(std::vector<Task>& tasks) {
    showTasks(tasks);
    size_t taskNumber;
    std::cout << "Enter task number to mark as completed: ";
    std::cin >> taskNumber;
    std::cin.ignore();
    if (taskNumber > 0 && taskNumber <= tasks.size()) {
        tasks[taskNumber - 1].isCompleted = true;
        // Get current date in YYYY-MM-DD format
        time_t now = time(0);
        struct tm localTime;
        localtime_s(&localTime, &now); // Use localtime_s for safety
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);
        tasks[taskNumber - 1].completionDate = buffer;
        std::cout << "Task marked as completed on " << buffer << ".\n";
    }
    else {
        std::cout << "Invalid task number.\n";
    }
}

//
// Function: deleteTask
// Description: Deletes a task from the list based on user input.
//
void deleteTask(std::vector<Task>& tasks) {
    showTasks(tasks);
    size_t taskNumber;
    std::cout << "Enter task number to delete: ";
    std::cin >> taskNumber;
    std::cin.ignore();
    if (taskNumber > 0 && taskNumber <= tasks.size()) {
        tasks.erase(tasks.begin() + taskNumber - 1);
        std::cout << "Task deleted successfully.\n";
    }
    else {
        std::cout << "Invalid task number.\n";
    }
}

//
// Function: sortTasks
// Description: Sorts tasks in descending order by priority (High first).
//
void sortTasks(std::vector<Task>& tasks) {
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.priority > b.priority;
        });
    std::cout << "Tasks sorted by priority.\n";
}

//
// Function: searchTasks
// Description: Searches for tasks that contain the provided query string in their description.
//
void searchTasks(const std::vector<Task>& tasks, const std::string& query) {
    bool found = false;
    for (const auto& task : tasks) {
        if (task.description.find(query) != std::string::npos) {
            std::cout << "Found task: " << task.description << "\n";
            found = true;
        }
    }
    if (!found)
        std::cout << "No tasks found with the given description.\n";
}

//
// Function: backupData
// Description: Saves all tasks to a backup file ("backup.txt").
void backupData(const std::vector<Task>& tasks) {
    std::ofstream backupFile("backup.txt");
    if (!backupFile) {
        std::cerr << "Error: Could not open backup file for writing.\n";
        return;
    }
    for (const auto& task : tasks) {
        backupFile << task.description << "\n";
        backupFile << task.dueDate << "\n";
        backupFile << (task.isCompleted ? "Completed" : "Not Completed") << "\n";
        backupFile << task.recurrence << "\n";
        backupFile << task.completionDate << "\n"; // Save completion date (or empty if not completed)
        for (size_t i = 0; i < task.tags.size(); ++i) {
            backupFile << task.tags[i];
            if (i < task.tags.size() - 1)
                backupFile << ",";
        }
        backupFile << "\n";
    }
    std::cout << "Data backed up successfully.\n";
}

//
// Function: restoreData
// Description: Loads tasks from the backup file ("backup.txt").
void restoreData(std::vector<Task>& tasks) {
    std::ifstream backupFile("backup.txt");
    if (!backupFile) {
        std::cout << "No backup file found.\n";
        return;
    }
    tasks.clear(); // Clear current tasks before restoring
    std::string line;
    while (std::getline(backupFile, line)) {
        Task restoredTask;  // Create a new Task for each record
        restoredTask.description = line;
        if (!std::getline(backupFile, restoredTask.dueDate)) break;
        if (!std::getline(backupFile, line)) break;
        restoredTask.isCompleted = (line == "Completed");
        if (!std::getline(backupFile, line)) break;
        restoredTask.recurrence = static_cast<Recurrence>(std::stoi(line));
        std::string compDate;
        if (!std::getline(backupFile, compDate)) break;
        restoredTask.completionDate = compDate;
        // Read tags (comma-separated)
        if (!std::getline(backupFile, line)) break;
        restoredTask.tags.clear();
        std::stringstream ss(line);
        std::string tag;
        while (std::getline(ss, tag, ',')) {
            if (!tag.empty())
                restoredTask.tags.push_back(tag);
        }
        tasks.push_back(restoredTask);
    }
    std::cout << "Data restored successfully.\n";
}

// Main function
int main() {
    std::vector<Task> tasks; // A list to store all tasks
    int choice;

    // Loop to show the menu and handle user input
    do {
        displayMenu(); // Show the menu
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        // Input validation to handle invalid choice (not an integer)
        while (std::cin.fail()) {
            std::cin.clear();  // Clears error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discards invalid input
            std::cout << "Invalid input. Please enter a valid number for your choice: ";
            std::cin >> choice;
        }

        std::cin.ignore(); // Ignore the newline character left by std::cin

        switch (choice) {
        case 1: // Add a new task
            addTask(tasks);
            break;
        case 2: // Show all tasks
            showTasks(tasks);
            break;
        case 3: // Mark task as completed
            markTaskAsCompleted(tasks);
            break;
        case 4: // Delete a task
            deleteTask(tasks);
            break;
        case 5: // Sort tasks by priority
            sortTasks(tasks);
            break;
        case 6: // Search for tasks
        {
            std::string query;
            std::cout << "Enter search query: ";
            std::getline(std::cin, query);
            searchTasks(tasks, query);
        }
        break;
        case 7: // Backup tasks
            backupData(tasks);
            break;
        case 8: // Restore tasks
            restoreData(tasks);
            break;
        case 9: // View task progress (not implemented yet)
            std::cout << "Task progress: (not implemented yet)" << std::endl;
            break;
        case 10: // Check for due tasks (not implemented yet)
            std::cout << "Checking for due tasks: (not implemented yet)" << std::endl;
            break;
        case 11: // Exit the program
            std::cout << "Exiting Todo List Application...\n";
            break;
        default:
            std::cout << "Invalid choice, please try again.\n";
            break;
        }

    } while (choice != 11); // Continue the loop until user chooses to exit
    return 0;
}

