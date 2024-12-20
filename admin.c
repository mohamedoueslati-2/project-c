#include <stdio.h>
#include <string.h>
#include "admin.h"

void print_header() {
    printf("=====================================\n");
    printf("          LOGIN SYSTEM               \n");
    printf("=====================================\n");
}

void print_footer() {
    printf("=====================================\n");
}

int read_credentials(const char *filename, User *users, int max_users, int print_debug) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return -1;
    }

    int count = 0;
    while (fscanf(file, "%49s %49s", users[count].username, users[count].password) == 2 && count < max_users) {
        if (print_debug) {
            printf("Read user: %s, password: %s\n", users[count].username, users[count].password);
        }
        count++;
    }

    fclose(file);
    return count;
}

int validate_credentials(User *users, int user_count, const char *username, const char *password) {
    for (int i = 0; i < user_count; i++) {
        printf("Validating against user: %s, password: %s\n", users[i].username, users[i].password); // Debugging line
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1; // Valid credentials
        }
    }
    return 0; // Invalid credentials
}

void create_default_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "admin defaultpassword\n");
        fclose(file);
        printf("Default file created with admin account.\n");
    } else {
        perror("Could not create file");
    }
}

void update_credentials_file(const char *filename, User *users, int user_count) {
    FILE *file = fopen(filename, "w");
    if (file) {
        for (int i = 0; i < user_count; i++) {
            fprintf(file, "%s %s\n", users[i].username, users[i].password);
        }
        fclose(file);
    } else {
        perror("Could not update file");
    }
}

void update_technicians_file(const char *filename, User *users, int user_count) {
    FILE *file = fopen(filename, "w");
    if (file) {
        for (int i = 0; i < user_count; i++) {
            fprintf(file, "%s %s\n", users[i].username, users[i].password);
        }
        fclose(file);
    } else {
        perror("Could not update file");
    }
}

int read_technicians(const char *filename, User *users, int max_users) {
    return read_credentials(filename, users, max_users, 0);
}

void create_technicians_file_if_not_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        file = fopen(filename, "w");
        if (file) {
            fclose(file);
            printf("Technicians file created.\n");
        } else {
            perror("Could not create technicians file");
        }
    } else {
        fclose(file);
    }
}

void change_credentials(User *users, int user_count, const char *filename) {
    char current_password[50];
    char new_username[50];
    char new_password[50];

    printf("Enter current password: ");
    scanf("%49s", current_password);

    if (validate_credentials(users, user_count, users[0].username, current_password)) {
        printf("Enter new username: ");
        scanf("%49s", new_username);

        printf("Enter new password: ");
        scanf("%49s", new_password);

        strcpy(users[0].username, new_username);
        strcpy(users[0].password, new_password);

        update_credentials_file(filename, users, user_count);
        printf("Credentials updated successfully.\n");
    } else {
        printf("Invalid current password.\n");
    }
}

void manage_technicians(User *users, int *user_count, const char *filename) {
    create_technicians_file_if_not_exists(filename);

    int choice;
    char username[50];
    char password[50];

    do {
        printf("\nManage Technicians\n");
        printf("List of Technicians:\n");
        for (int i = 0; i < *user_count; i++) {
            printf("%d. %s\n", i + 1, users[i].username);
        }
        printf("\nOptions:\n");
        printf("1. Add Technician\n");
        printf("2. Modify Technician\n");
        printf("3. Delete Technician\n");
        printf("4. Search Technician\n");
        printf("5. Back to Dashboard\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (*user_count < 100) {
                    printf("Enter new technician username: ");
                    scanf("%49s", username);
                    // Check if the username already exists
                    int exists = 0;
                    for (int i = 0; i < *user_count; i++) {
                        if (strcmp(users[i].username, username) == 0) {
                            exists = 1;
                            break;
                        }
                    }
                    if (exists) {
                        printf("Technician with username '%s' already exists.\n", username);
                    } else {
                        printf("Enter new technician password: ");
                        scanf("%49s", password);
                        strcpy(users[*user_count].username, username);
                        strcpy(users[*user_count].password, password);
                        (*user_count)++;
                        update_technicians_file(filename, users, *user_count);
                        printf("Technician added successfully.\n");
                    }
                } else {
                    printf("Cannot add more technicians. Maximum limit reached.\n");
                }
                break;
            case 2:
                printf("Enter technician number to modify: ");
                int modify_index;
                scanf("%d", &modify_index);
                if (modify_index > 0 && modify_index <= *user_count) {
                    printf("Enter new username: ");
                    scanf("%49s", users[modify_index - 1].username);
                    printf("Enter new password: ");
                    scanf("%49s", users[modify_index - 1].password);
                    update_technicians_file(filename, users, *user_count);
                    printf("Technician modified successfully.\n");
                } else {
                    printf("Invalid technician number.\n");
                }
                break;
            case 3:
                printf("Enter technician number to delete: ");
                int delete_index;
                scanf("%d", &delete_index);
                if (delete_index > 0 && delete_index <= *user_count) {
                    for (int i = delete_index - 1; i < *user_count - 1; i++) {
                        users[i] = users[i + 1];
                    }
                    (*user_count)--;
                    update_technicians_file(filename, users, *user_count);
                    printf("Technician deleted successfully.\n");
                } else {
                    printf("Invalid technician number.\n");
                }
                break;
            case 4:
                printf("Enter technician username to search: ");
                scanf("%49s", username);
                int found = 0;
                for (int i = 0; i < *user_count; i++) {
                    if (strcmp(users[i].username, username) == 0) {
                        printf("Technician found: %s\n", users[i].username);
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("Technician not found.\n");
                }
                break;
            case 5:
                printf("Returning to dashboard...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

void update_resources_file(const char *filename, Resource *resources, int resource_count) {
    FILE *file = fopen(filename, "w");
    if (file) {
        for (int i = 0; i < resource_count; i++) {
            fprintf(file, "%s %d %d\n", resources[i].name, resources[i].quantity, resources[i].available);
        }
        fclose(file);
    } else {
        perror("Could not update file");
    }
}

int read_resources(const char *filename, Resource *resources, int max_resources) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return -1;
    }

    int count = 0;
    while (fscanf(file, "%49s %d %d", resources[count].name, &resources[count].quantity, &resources[count].available) == 3 && count < max_resources) {
        count++;
    }

    fclose(file);
    return count;
}

void create_resources_file_if_not_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        file = fopen(filename, "w");
        if (file) {
            fclose(file);
            printf("Resources file created.\n");
        } else {
            perror("Could not create resources file");
        }
    } else {
        fclose(file);
    }
}

void manage_resources(Resource *resources, int *resource_count, const char *filename) {
    create_resources_file_if_not_exists(filename);

    int choice;
    char name[50];
    int quantity;

    do {
        printf("\nManage Resources\n");
        printf("List of Resources:\n");
        for (int i = 0; i < *resource_count; i++) {
            printf("%d. %s (Quantity: %d, Available: %s)\n", i + 1, resources[i].name, resources[i].quantity, resources[i].available ? "True" : "False");
        }
        printf("\nOptions:\n");
        printf("1. Add Resource\n");
        printf("2. Modify Resource\n");
        printf("3. Delete Resource\n");
        printf("4. Back to Dashboard\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (*resource_count < 100) {
                    printf("Enter new resource name: ");
                    scanf("%49s", name);
                    // Check if the resource already exists
                    int exists = 0;
                    for (int i = 0; i < *resource_count; i++) {
                        if (strcmp(resources[i].name, name) == 0) {
                            exists = 1;
                            break;
                        }
                    }
                    if (exists) {
                        printf("Resource with name '%s' already exists.\n", name);
                    } else {
                        printf("Enter quantity: ");
                        scanf("%d", &quantity);
                        strcpy(resources[*resource_count].name, name);
                        resources[*resource_count].quantity = quantity;
                        resources[*resource_count].available = quantity > 0 ? 1 : 0;
                        (*resource_count)++;
                        update_resources_file(filename, resources, *resource_count);
                        printf("Resource added successfully.\n");
                    }
                } else {
                    printf("Cannot add more resources. Maximum limit reached.\n");
                }
                break;
            case 2:
                printf("Enter resource number to modify: ");
                int modify_index;
                scanf("%d", &modify_index);
                if (modify_index > 0 && modify_index <= *resource_count) {
                    printf("Enter new name: ");
                    scanf("%49s", resources[modify_index - 1].name);
                    printf("Enter new quantity: ");
                    scanf("%d", &resources[modify_index - 1].quantity);
                    resources[modify_index - 1].available = resources[modify_index - 1].quantity > 0 ? 1 : 0;
                    update_resources_file(filename, resources, *resource_count);
                    printf("Resource modified successfully.\n");
                } else {
                    printf("Invalid resource number.\n");
                }
                break;
            case 3:
                printf("Enter resource number to delete: ");
                int delete_index;
                scanf("%d", &delete_index);
                if (delete_index > 0 && delete_index <= *resource_count) {
                    for (int i = delete_index - 1; i < *resource_count - 1; i++) {
                        resources[i] = resources[i + 1];
                    }
                    (*resource_count)--;
                    update_resources_file(filename, resources, *resource_count);
                    printf("Resource deleted successfully.\n");
                } else {
                    printf("Invalid resource number.\n");
                }
                break;
            case 4:
                printf("Returning to dashboard...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
}

void validate_requests(const char *request_filename, const char *resource_filename) {
    FILE *file = fopen(request_filename, "r+");
    if (!file) {
        perror("Could not open file");
        return;
    }

    Resource resources[100];
    int resource_count = read_resources(resource_filename, resources, 100);

    char technician_username[50];
    char resource_name[50];
    int quantity;
    int status;
    long pos;

    printf("\nList of Requests:\n");
    while ((pos = ftell(file)) != -1 && fscanf(file, "%49s %49s %d %d", technician_username, resource_name, &quantity, &status) == 4) {
        if (status == 0) {
            printf("Technician: %s, Resource: %s, Quantity: %d, Status: Pending\n", technician_username, resource_name, quantity);
            printf("Approve (1) or Reject (2) this request? ");
            int choice;
            scanf("%d", &choice);
            fseek(file, pos, SEEK_SET);
            fprintf(file, "%s %s %d %d\n", technician_username, resource_name, quantity, choice);
            fseek(file, 0, SEEK_CUR);

            if (choice == 1) {
                // Update resource quantity
                for (int i = 0; i < resource_count; i++) {
                    if (strcmp(resources[i].name, resource_name) == 0) {
                        resources[i].quantity -= quantity;
                        if (resources[i].quantity < 0) {
                            resources[i].quantity = 0;
                        }
                        resources[i].available = resources[i].quantity > 0 ? 1 : 0;
                        break;
                    }
                }
            }
        }
    }

    fclose(file);

    // Update the resources file
    FILE *resource_file = fopen(resource_filename, "w");
    if (resource_file) {
        for (int i = 0; i < resource_count; i++) {
            fprintf(resource_file, "%s %d %d\n", resources[i].name, resources[i].quantity, resources[i].available);
        }
        fclose(resource_file);
    } else {
        perror("Could not update resources file");
    }
}

void display_dashboard(User *user, User *users, int user_count, const char *filename) {
    int choice;
    User technicians[100];
    int technician_count = read_technicians("techniciens.txt", technicians, 100);
    Resource resources[100];
    int resource_count = read_resources("resources.txt", resources, 100);

    do {
        printf("\nDashboard\n");
        printf("Username: %s\n", user->username);
        printf("1. Change Username and Password\n");
        printf("2. Logout\n");
        printf("3. Manage Technicians\n");
        printf("4. Manage Resources\n");
        printf("5. Validate Requests\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                change_credentials(users, user_count, filename);
                break;
            case 2:
                printf("Logging out...\n");
                break;
            case 3:
                manage_technicians(technicians, &technician_count, "techniciens.txt");
                break;
            case 4:
                manage_resources(resources, &resource_count, "resources.txt");
                break;
            case 5:
                validate_requests("requests.txt", "resources.txt");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 2);
}