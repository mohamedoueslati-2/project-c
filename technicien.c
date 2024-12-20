#include <stdio.h>
#include <string.h>
#include "technicien.h"
void create_request(User *technician, const char *filename) {
    char resource_name[50];
    int quantity;

    printf("Enter resource name: ");
    scanf("%49s", resource_name);

    printf("Enter quantity: ");
    scanf("%d", &quantity);

    FILE *file = fopen(filename, "a");
    if (file) {
        fprintf(file, "%s %s %d 0\n", technician->username, resource_name, quantity);
        fclose(file);
        printf("Request created successfully.\n");
    } else {
        perror("Could not create request");
    }
}

void follow_requests(User *technician, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return;
    }

    char technician_username[50];
    char resource_name[50];
    int quantity;
    int status;

    printf("\nList of Requests:\n");
    while (fscanf(file, "%49s %49s %d %d", technician_username, resource_name, &quantity, &status) == 4) {
        if (strcmp(technician->username, technician_username) == 0) {
            printf("Resource: %s, Quantity: %d, Status: %s\n", resource_name, quantity, status == 0 ? "Pending" : (status == 1 ? "Approved" : "Rejected"));
        }
    }

    fclose(file);
}
void display_resources(Resource *resources, int resource_count) {
    printf("\nList of Resources:\n");
    for (int i = 0; i < resource_count; i++) {
        printf("%d. %s (Quantity: %d, Available: %s)\n", i + 1, resources[i].name, resources[i].quantity, resources[i].available ? "True" : "False");
    }
}

void change_technician_password(User *technician, const char *filename) {
    char new_password[50];

    printf("Enter new password: ");
    scanf("%49s", new_password);

    strcpy(technician->password, new_password);

    // Update the technicians file
    User techniciens[100];
    int technicien_count = read_technicians(filename, techniciens, 100);
    for (int i = 0; i < technicien_count; i++) {
        if (strcmp(techniciens[i].username, technician->username) == 0) {
            techniciens[i] = *technician;
            break;
        }
    }
    update_technicians_file(filename, techniciens, technicien_count);

    printf("Password updated successfully.\n");
}

void technician_dashboard(User *technician, const char *filename) {
    int choice;
    Resource resources[100];
    int resource_count = read_resources("resources.txt", resources, 100);

    do {
        printf("\nTechnician Dashboard\n");
        printf("1. View Resources\n");
        printf("2. Create Request\n");
        printf("3. Follow Requests\n");
        printf("4. Change Password\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display_resources(resources, resource_count);
                break;
            case 2:
                create_request(technician, "requests.txt");
                break;
            case 3:
                follow_requests(technician, "requests.txt");
                break;
            case 4:
                change_technician_password(technician, filename);
                break;
            case 5:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}