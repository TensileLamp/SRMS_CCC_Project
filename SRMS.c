#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STUD_FILE "students.txt"
#define CRED_FILE "credentials.txt"

//Simplifier Functions

void getString(char *msg, char *s, int size) {
    printf("%s", msg);
    fgets(s, size, stdin);
    s[strcspn(s, "\n")] = '\0';
}

int getInt(char *msg) {
    char temp[50];
    int x;
    while (1) {
        printf("%s", msg);
        fgets(temp, 50, stdin);
        if (sscanf(temp, "%d", &x) == 1)
            return x;
        printf("Invalid number!\n");
    }
}

float getFloat(char *msg, float min, float max) {
    char temp[50];
    float x;
    while (1) {
        printf("%s", msg);
        fgets(temp, 50, stdin);
        if (sscanf(temp, "%f", &x) == 1 && x >= min && x <= max)
            return x;
        printf("Invalid value! Enter between %.2f and %.2f.\n", min, max);
    }
}

//Login System

int checkLogin(char *username, char *password, char *role, int *studentId) {
    FILE *fp = fopen(CRED_FILE, "r");
    if (!fp) {
        printf("credentials.txt not found!\n");
        return 0;
    }

    char u[50], p[50], r;
    int sid;

    while (fscanf(fp, "%s %s %c %d", u, p, &r, &sid) == 4) {
        if (strcmp(username, u) == 0 && strcmp(password, p) == 0) {
            *role = r;
            *studentId = sid;
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

//Student Functions

int idExists(int id) {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) return 0;

    int sid;
    char name[50];
    int year;
    char sec;
    float cgpa;

    while (fscanf(fp, "%d %s %d %c %f", &sid, name, &year, &sec, &cgpa) == 5) {
        if (sid == id) { fclose(fp); return 1; }
    }
    fclose(fp);
    return 0;
}

void addStudent() {
    FILE *fp;

    printf("\n======[ ADD STUDENT ]======\n");

    int id;
    while (1) {
        id = getInt("Enter ID: ");
        if (!idExists(id)) break;
        printf("ID already exists! Try again.\n");
    }

    char name[50];
    getString("Enter Name: ", name, 50);

    int year = getInt("Enter Year: ");

    char sec[10];
    while (1) {
        getString("Enter Section (A-Z): ", sec, 10);
        if (strlen(sec) == 1 && isalpha(sec[0])) break;
        printf("Invalid section!\n");
    }

    float cgpa = getFloat("Enter CGPA (0-10): ", 0, 10);

    //save student
    fp = fopen(STUD_FILE, "a");
    fprintf(fp, "%d %s %d %c %.2f\n", id, name, year, toupper(sec[0]), cgpa);
    fclose(fp);

    //auto-gen credentials
    char uname[50], pass[50];
    sprintf(uname, "stu%d", id);
    sprintf(pass, "pass%d!", id);

    fp = fopen(CRED_FILE, "a");
    fprintf(fp, "%s %s S %d\n", uname, pass, id);
    fclose(fp);

    //admin display
    printf("\nStudent Added!\n");
    printf("Generated Login:\n");
    printf("Username: %s\n", uname);
    printf("Password: %s\n", pass);
}

void displayStudents() {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No data found.\n");
        return;
    }

    int id, year;
    char name[50];
    char sec;
    float cgpa;

    printf("\n======[ ALL STUDENTS ]======\n");

    while (fscanf(fp, "%d %s %d %c %f", &id, name, &year, &sec, &cgpa) == 5) {
        printf("\n|| ID: %d\n|| Name: %s\n|| Year: %d\n|| Section: %c\n|| CGPA: %.2f\n",
               id, name, year, sec, cgpa);
    }

    fclose(fp);
}

void searchStudent() {
    int sid = getInt("Enter ID to search: ");

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No records.\n");
        return;
    }

    int id, year;
    char name[50];
    char sec;
    float cgpa;

    while (fscanf(fp, "%d %s %d %c %f", &id, name, &year, &sec, &cgpa) == 5) {
        if (id == sid) {
            printf("\nFOUND!\nID: %d\nName: %s\nYear: %d\nSection: %c\nCGPA: %.2f\n",
                   id, name, year, sec, cgpa);
            fclose(fp);
            return;
        }
    }

    printf("Not found.\n");
    fclose(fp);
}

void updateStudent() {
    int sid = getInt("Enter ID to update: ");

    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp) {
        printf("No records.\n");
        return;
    }

    int id, year;
    char name[50];
    char sec;
    float cgpa;
    int found = 0;

    while (fscanf(fp, "%d %s %d %c %f", &id, name, &year, &sec, &cgpa) == 5) {
        if (id == sid) {
            found = 1;
            printf("\n1. Update Year\n2. Update Section\n3. Update CGPA\n");
            int ch = getInt("Choose: ");

            if (ch == 1) year = getInt("New Year: ");
            else if (ch == 2) {
                char t[10]; 
                getString("New Section: ", t, 10);
                sec = toupper(t[0]);
            }
            else if (ch == 3) cgpa = getFloat("New CGPA: ", 0, 10);
        }

        fprintf(temp, "%d %s %d %c %.2f\n", id, name, year, sec, cgpa);
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("Updated successfully.\n");
    else printf("Student not found.\n");
}


//specifically for Teacher logins
void editCGPA() {
    int sid = getInt("Enter ID to edit CGPA: ");

    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp) {
        printf("No records.\n");
        return;
    }

    int id, year;
    char name[50];
    char sec;
    float cgpa;
    int found = 0;

    while (fscanf(fp, "%d %s %d %c %f", &id, name, &year, &sec, &cgpa) == 5) {
        if (id == sid) {
            found = 1;
            printf("Current CGPA: %.2f\n", cgpa);
            cgpa = getFloat("Enter new CGPA (0-10): ", 0, 10);
        }
        fprintf(temp, "%d %s %d %c %.2f\n", id, name, year, sec, cgpa);
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("CGPA updated successfully.\n");
    else printf("Student not found.\n");
}


void deleteStudent() {
    int sid = getInt("Enter ID to delete: ");

    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp) {
        printf("No records.\n");
        return;
    }

    int id, year;
    char name[50];
    char sec;
    float cgpa;
    int deleted = 0;

    while (fscanf(fp, "%d %s %d %c %f", &id, name, &year, &sec, &cgpa) == 5) {
        if (id == sid) {
            deleted = 1;
            continue;
        }
        fprintf(temp, "%d %s %d %c %.2f\n", id, name, year, sec, cgpa);
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (deleted) printf("Deleted successfully.\n");
    else printf("Not found.\n");
}

//Sub-Login Menu's

void adminMenu() {
    while (1) {
        printf("\n======[ ADMIN MENU ]======\n");
        printf("1. Add Student\n2. Display\n3. Search\n4. Update\n5. Delete\n6. Logout\n");

        int c = getInt("Enter choice: ");
        if (c == 1) addStudent();
        else if (c == 2) displayStudents();
        else if (c == 3) searchStudent();
        else if (c == 4) updateStudent();
        else if (c == 5) deleteStudent();
        else if (c == 6) return;
    }
}

void teacherMenu() {
    while (1) {
        printf("\n======[ TEACHER MENU ]======\n");
        printf("1. Display Students\n2. Search\n3. Edit CGPA\n4. Logout\n");

        int c = getInt("Choice: ");

        if (c == 1) displayStudents();
        else if (c == 2) searchStudent();
        else if (c == 3) editCGPA();
        else if (c == 4) return;
        else printf("Invalid choice!\n");
    }
}


void studentMenu(int sid) {
    printf("\n--- YOUR DETAILS ---\n");

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No record found.\n");
        return;
    }

    int id, year;
    char name[50];
    char sec;
    float cgpa;

    while (fscanf(fp, "%d %s %d %c %f", &id, name, &year, &sec, &cgpa) == 5) {
        if (id == sid) {
            printf("|| ID: %d\n|| Name: %s\n|| Year: %d\n|| Section: %c\n|| CGPA: %.2f\n",
                   id, name, year, sec, cgpa);
            break;
        }
    }

    fclose(fp);
}


int main() {
    char username[50], password[50];

    while (1) {
        printf("\n======[ MAIN MENU ]======\n1. Login\n2. Exit\n");
        int ch = getInt("Enter choice: ");

        if (ch == 2) break;

        printf("=========================\n");
        getString("Username: ", username, 50);
        getString("Password: ", password, 50);

        char role;
        int sid;

        if (checkLogin(username, password, &role, &sid)) {
            if (role == 'A') adminMenu();
            else if (role == 'T') teacherMenu();
            else if (role == 'S') studentMenu(sid);
        }
        else printf("Invalid login!\n");
    }

    return 0;
}

