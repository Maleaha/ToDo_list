#include <stdio.h>     // Functions for input and output operations
#include <string.h>    // Functions for handling strings
#include <time.h>      // For working with date and time
#include <stdbool.h>   // Defines the boolean data type in C
#include <ctype.h>     // Provides functions for character handling
#include <stdlib.h>    // Contains general-purpose functions in C
#include <unistd.h>    // Access to various operating system functionalities

int tLength = 0;  // Global variable for todo length
FILE *fp;         // Declares a pointer variable named fp of type FILE*.
struct Todo       // Will contain the title of task and status
{
    char title[50];       // Title of task
    _Bool isCompleted;    // Status of completion
} todos[20];             // Total tasks can be 20

void saveToFile()
{
    size_t i;
    fp = fopen("todos.bin", "wb");  // Open write (binary file), mode=write
    if (fp == NULL)
    {
        printf("Can't save your todo\n");
    }
    else
    {
        for (i = 0; i < tLength; i++)
        {
            fwrite(&todos[i], sizeof(struct Todo), 1, fp);  // fwrite(address, bytes, records_at_a_time, file pointer)
        }
        fclose(fp);  // Close the file
    }
}

void getFileSize()
{
    fseek(fp, 0L, SEEK_END);  // fseek(file pointer, offset, symbolic constant)
    unsigned long int size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    tLength = size / sizeof(struct Todo);
}

void readFromFile()
{
    size_t i;
    fp = fopen("todos.bin", "rb");  // Mode: read
    if (fp == NULL)
    {
        printf("We are not able to find any todos file\n");
    }
    else
    {
        getFileSize();
        for (i = 0; i < tLength; i++)
        {
            fread(&todos[i], sizeof(struct Todo), 1, fp);
        }
        fclose(fp);
    }
}

void addTodo()
{
    // Check if the todo list is full
    if (tLength >= 20)
    {
        printf("Todo list is full. Delete some todos to add new ones.\n");
        return;
    }

    // Prompt for the todo title
    printf("Type your todo: ");
    fgets(todos[tLength].title, sizeof(todos[tLength].title), stdin);

    // Remove the trailing newline character
    size_t len = strlen(todos[tLength].title);
    if (len > 0 && todos[tLength].title[len - 1] == '\n')
        todos[tLength].title[len - 1] = '\0';

    todos[tLength].isCompleted = 0; // Initialize to false
    tLength++;
}

void printAllTodo()
{
    int i;
    printf("+----+-------------------------------------+-------------+\n");
    printf("| ID |            Todo Title               |  Completed  |\n");
    printf("+----+-------------------------------------+-------------+\n");

    for (i = 0; i < tLength; i++)
    {
        printf("|%3d | %-35s |%-13s|\n", i + 1, todos[i].title, (!todos[i].isCompleted) ? "  No  " : "  Yes ");
        printf("+----+-------------------------------------+-------------+\n");
    }
}

void markAsComplete()
{
    int todoId;
    printf("Enter the ID of the todo: ");
    scanf("%d", &todoId);
    todoId--;  // Decrement of 1 to adjust it to the array index (it starts from 0)
    if (todoId < 0 || todoId >= tLength)
    {
        printf("!!INVALID TODO ID!!\n");
    }
    else
    {
        todos[todoId].isCompleted = true;  // Set the completion status to true
        printf("Todo marked as completed.\n");
    }

    saveToFile();   // Save the updated todos to file
    clearScreen();  // Clear the screen after marking a todo as complete
}

void deleteTodo()
{
    int todoId;
    printf("Enter the ID of the todo: ");
    scanf("%d", &todoId);
    todoId--;
    if (todoId < 0 || todoId >= tLength)
    {
        printf("!!INVALID TODO ID!!\n");
    }
    else
    {
        memmove(todos + todoId, todos + todoId + 1, (tLength - todoId - 1) * sizeof(*todos));
        tLength--;
        printf("Your todo has been deleted.\n");
    }

    saveToFile();   // Save the updated todos to file
    clearScreen();  // Clear the screen after deleting a todo
}

void ShowOptions()
{
    char userChoice;
    printf("Type 'A' to add, 'D' to delete, 'C' to mark complete, or 'Q' to quit: ");
    userChoice = getchar();
    userChoice = toupper(userChoice);  // If lowercase entered, it's changed to uppercase and processed like that
    getchar();
    switch (userChoice)  // Testing against conditions
    {
        case 'A':
            system("COLOR 02");
            addTodo();        // Call to add a todo
            break;
        case 'D':
            system("COLOR 03");
            deleteTodo();     // Call to delete a todo
            break;
        case 'C':
            system("COLOR 04");
            markAsComplete(); // Call to mark a todo as complete
            break;
        case 'Q':
            system("COLOR 05");
            exit(0);          // Exiting the program
            break;
        default:
            printf("Command not found\n");  // If anything else is entered, display an error message
            ShowOptions();
            break;
    }

    printAllTodo();   // Print all todos after each operation
    getchar();
    ShowOptions();
}

void isThisFirstTime()
{
    if (access("todos.bin", F_OK) != -1)
    {
        readFromFile();    // Read todos from file if it exists
    }
    else
    {
        addTodo();         // Add a todo if file doesn't exist
    }

    saveToFile();          // Save todos to file
    printAllTodo();        // Print all todos
    ShowOptions();         // Show options to the user
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");   // For Windows
#else
    system("clear"); // For Linux and macOS
#endif
}

int main()
{
    isThisFirstTime();
    return 0;
}
