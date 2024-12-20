#include <stdio.h>
#include <string.h>
#include "admin.h"
#include "technicien.h"

int main() {
    const char *responsable_filename = "responsable.txt";
    const char *technicien_filename = "techniciens.txt";
    FILE *file = fopen(responsable_filename, "r");
    int file_created = 0;

    if (!file) {
        create_default_file(responsable_filename);
        file_created = 1;
    } else {
        fclose(file);
    }

    User responsable[100];
    int responsible_count = read_credentials(responsable_filename, responsable, 100, 0);

    if (responsible_count == -1) {
        return 1;
    }

    User techniciens[100];
    int technicien_count = read_technicians(technicien_filename, techniciens, 100);

    char username[50];
    char password[50];
    int user_type;

    print_header();

    printf("Enter user type (1 for Responsable, 2 for Technicien): ");
    scanf("%d", &user_type);

    printf("Enter username: ");
    scanf("%49s", username);

    printf("Enter password: ");
    scanf("%49s", password);

    if (user_type == 1) {
        if (validate_credentials(responsable, responsible_count, username, password)) {
            printf("Login successful! Redirecting to dashboard...\n");
            display_dashboard(&responsable[0], responsable, responsible_count, responsable_filename);
        } else {
            printf("Invalid username or password.\n");
        }
    } else if (user_type == 2) {
        if (validate_credentials(techniciens, technicien_count, username, password)) {
            printf("Login successful! Redirecting to technician dashboard...\n");
            for (int i = 0; i < technicien_count; i++) {
                if (strcmp(techniciens[i].username, username) == 0) {
                    technician_dashboard(&techniciens[i], technicien_filename);
                    break;
                }
            }
        } else {
            printf("Invalid username or password.\n");
        }
    } else {
        printf("Invalid user type.\n");
    }

    print_footer();

    return 0;
}