#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <sys/stat.h>
#include <direct.h> // For _mkdir on Windows

void pressAnyKey() {
    printf("Press any key to continue...");
    _getch(); // Wait for a key press
}

// Function to hide password input
void getPassword(char *password) {
    int i = 0;
    char ch;

    printf("Enter your password: ");
    while ((ch = _getch()) != '\r' && i < 49) { // '\r' is Enter key in Windows
        if (ch == '\b' && i > 0) { // Handle backspace
            printf("\b \b");
            i--;
        } else if (ch != '\b') {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

// Define a structure to hold user information
struct User {
    char name[50];
    char email[50];
    char password[50];
};

// Function to write user data to file
void saveUserToFile(struct User *user) {
    struct stat st = {0};
    if (stat("database", &st) == -1) {
        _mkdir("database"); // Create the folder
    }

    FILE *file = fopen("database/users.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(file, "%s | %s | %s\n", user->name, user->email, user->password);
    fclose(file);
}

// Dashboard functionality after successful login
void dashboard() {
    char input[10];

    while (1) {
        system("cls");
        printf("Welcome to the Dashboard!\n");
        printf("Type 'logout' to return to the main menu.\n\n");
        printf("Enter your choice: ");
        fgets(input, 10, stdin);
        input[strcspn(input, "\n")] = 0; // Remove newline

        if (strcmp(input, "logout") == 0) {
            break; // Break out of the dashboard and return to the main menu
        } else {
            printf("Unknown command. Please type 'logout' to exit.\n");
        }
    }
}

int loginUser() {
    char email[50], password[50], fileLine[150];
    char storedName[50], storedEmail[50], storedPassword[50];
    int found = 0;

    while (1) {
    	printf("---Login---\n");
    	printf("Type (0) to return to the menu\n\n");
        printf("Enter your email: ");
        fgets(email, 50, stdin);
        email[strcspn(email, "\n")] = 0; // Remove trailing newline

        if (strcmp(email, "0") == 0) {
            return 0; // Return to main menu
        }

        getPassword(password);

        FILE *file = fopen("database/users.txt", "r");
        if (file == NULL) {
            printf("Error opening file or no users found!\n");
            return 0;
        }

        while (fgets(fileLine, 150, file)) {
            sscanf(fileLine, "%[^|]| %[^|]| %[^|\n]", storedName, storedEmail, storedPassword);
            storedEmail[strcspn(storedEmail, " ")] = 0;
            storedPassword[strcspn(storedPassword, " ")] = 0;

            if (strcmp(email, storedEmail) == 0 && strcmp(password, storedPassword) == 0) {
                found = 1;
                break;
            }
        }

        fclose(file);

        if (found) {
            dashboard(); // Call the dashboard after successful login
            return found;
        } else {
        	system("cls");
            printf("Invalid email or password.\n");
        }
    }
}

void registerUser(struct User *user) {
    while (1) {
    	printf("---Register---\n");
    	printf("Type (0) to return to the menu\n\n");
        printf("Enter your name: ");
        fgets(user->name, 50, stdin);
        user->name[strcspn(user->name, "\n")] = 0; // Remove trailing newline
		
        if (strcmp(user->name, "0") == 0) {
            return; 
        }

        printf("Enter your email: ");
        fgets(user->email, 50, stdin);
        user->email[strcspn(user->email, "\n")] = 0; // Remove trailing newline

        getPassword(user->password);

        printf("\nRegistration Complete!\n");
        pressAnyKey();

        saveUserToFile(user);
        break; // Exit the loop after registration
    }
}

int main() {
    int choice;
    struct User newUser;

    while (1) {
    	system("cls");
        printf("\n--- Menu ---\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume the newline after scanf

        switch (choice) {
            case 1:
                printf("\n--- Register ---\n");
                system("cls");
                registerUser(&newUser);
                break;
            case 2:
                printf("\n--- Login ---\n");
                system("cls");
                loginUser();
                break;
            case 3:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice, please try again.\n");
                pressAnyKey();
        }
    }

    return 0;
}

