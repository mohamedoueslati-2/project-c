#include <stdio.h>
#include "login.h"

int main() {
    const char *filename = "responsable.txt";
    FILE *file = fopen(filename, "r");
    int file_created = 0;

    if (!file) {
        create_default_file(filename);
        file_created = 1;
    } else {
        fclose(file);
    }

    User responsable[100];
    int responsible_count = read_credentials(filename, responsable, 100, file_created);

    if (responsible_count == -1) {
        return 1;
    }

    char username[50];
    char password[50];

    print_header();

    printf("Enter username: ");
    scanf("%49s", username);  // Limiter la taille à 49 caractères pour éviter le dépassement

    printf("Enter password: ");
    scanf("%49s", password);  // Limiter la taille à 49 caractères également

    if (validate_credentials(responsable, responsible_count, username, password)) {
        printf("Login successful! Redirecting to dashboard...\n");
        display_dashboard(&responsable[0], responsable, responsible_count, filename);
    } else {
        printf("Invalid username or password.\n");
    }

    print_footer();

    return 0;
}