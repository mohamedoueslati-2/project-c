#ifndef TECHNICIEN_H
#define TECHNICIEN_H

#include "admin.h"

void display_resources(Resource *resources, int resource_count);
void change_technician_password(User *technician, const char *filename);
void technician_dashboard(User *technician, const char *filename);
void create_request(User *technician, const char *filename);
void follow_requests(User *technician, const char *filename);
#endif // TECHNICIEN_H