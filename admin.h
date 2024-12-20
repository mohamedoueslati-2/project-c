#ifndef ADMIN_H
#define ADMIN_H

typedef struct {
    char username[50];
    char password[50];
} User;

typedef struct {
    char name[50];
    int quantity;
    int available; // 1 for true, 0 for false
} Resource;
typedef struct {
    char technician_username[50];
    char resource_name[50];
    int quantity;
    int status; // 0: Pending, 1: Approved, 2: Rejected
} Request;
void print_header();
void print_footer();
int read_credentials(const char *filename, User *users, int max_users, int print_debug);
int validate_credentials(User *users, int user_count, const char *username, const char *password);
void create_default_file(const char *filename);
void display_dashboard(User *user, User *users, int user_count, const char *filename);
void manage_technicians(User *users, int *user_count, const char *filename);
void update_technicians_file(const char *filename, User *users, int user_count);
int read_technicians(const char *filename, User *users, int max_users);
void create_technicians_file_if_not_exists(const char *filename);
void manage_resources(Resource *resources, int *resource_count, const char *filename);
void update_resources_file(const char *filename, Resource *resources, int resource_count);
int read_resources(const char *filename, Resource *resources, int max_resources);
void create_resources_file_if_not_exists(const char *filename);
void validate_requests(const char *request_filename, const char *resource_filename);
#endif // ADMIN_H