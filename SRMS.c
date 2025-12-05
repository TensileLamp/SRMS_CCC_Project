#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STDFILE "students.txt"
#define CRDFILE "credentials.txt"


typedef struct {
    int id;
    char name[50];
    int year;
    char section;
    float cgpa;
} Student;


//Helper FUnctions
void getString(char *msg, char *s, int size) {
    printf("%s", msg);
    fgets(s, size, stdin);
    if (s[0] == '\n') fgets(s, size, stdin);
    s[strcspn(s, "\n")] = '\0';
}

int getInt(char *msg) {
    char temp[50];
    int x;
    while (1) {
        printf("%s", msg);
        fgets(temp, 50, stdin);
        if (sscanf(temp, "%d", &x) == 1) return x;
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


//Credential Login
int checkLogin(char *username, char *password, char *role, int *studentId) {
    FILE *fp = fopen(CRDFILE, "r");
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


//Student Related Functions
int idExists(int id) {
    FILE *fp = fopen(STDFILE, "r");
    if (!fp) return 0;

    Student st;

    while (fscanf(fp, "%d %s %d %c %f",
                  &st.id, st.name, &st.year, &st.section, &st.cgpa) == 5) {
        if (st.id == id) { fclose(fp); return 1; }
    }

    fclose(fp);
    return 0;
}


void addStudent() {
    Student st;
    FILE *fp;

    printf("\n======[ ADD STUDENT ]======\n");

    while (1) {
        st.id = getInt("Enter ID: ");
        if (!idExists(st.id)) break;
        printf("ID already exists! Try again.\n");
    }

    getString("Enter Name (no spaces): ", st.name, 50);
    if (strchr(st.name, ' ')) {
        printf("Name cannot contain spaces!\n");
        return;
    }

    st.year = getInt("Enter Year: ");

    char sec[10];
    while (1) {
        getString("Enter Section (A-Z): ", sec, 10);
        if (strlen(sec) == 1 && isalpha(sec[0])) {
            st.section = toupper(sec[0]);
            break;
        }
        printf("Invalid section!\n");
    }

    st.cgpa = getFloat("Enter CGPA (0-10): ", 0, 10);

    fp = fopen(STDFILE, "a");
    fprintf(fp, "%d %s %d %c %.2f\n",
            st.id, st.name, st.year, st.section, st.cgpa);
    fclose(fp);

    char uname[50], pass[50];
    sprintf(uname, "stu%d", st.id);
    sprintf(pass, "pass%d!", st.id);

    fp = fopen(CRDFILE, "a");
    fprintf(fp, "%s %s S %d\n", uname, pass, st.id);
    fclose(fp);

    printf("\nStudent Added!\n");
    printf("Generated Login:\nUsername: %s\nPassword: %s\n", uname, pass);
}


void displayStudents() {
    FILE *fp = fopen(STDFILE, "r");
    if (!fp) {
        printf("No data found.\n");
        return;
    }

    Student st;

    printf("\n======[ ALL STUDENTS ]======\n");

    while (fscanf(fp, "%d %s %d %c %f",
                  &st.id, st.name, &st.year, &st.section, &st.cgpa) == 5) {

        printf("\n|| ID: %d\n|| Name: %s\n|| Year: %d\n|| Section: %c\n|| CGPA: %.2f\n",
               st.id, st.name, st.year, st.section, st.cgpa);
    }

    fclose(fp);
}


void searchStudent() {
    int sid = getInt("Enter ID to search: ");

    FILE *fp = fopen(STDFILE, "r");
    if (!fp) {
        printf("No records.\n");
        return;
    }

    Student st;

    while (fscanf(fp, "%d %s %d %c %f",
                  &st.id, st.name, &st.year, &st.section, &st.cgpa) == 5) {

        if (st.id == sid) {
            printf("\nFOUND!\n|| ID: %d\n|| Name: %s\n|| Year: %d\n|| Section: %c\n|| CGPA: %.2f\n",
                   st.id, st.name, st.year, st.section, st.cgpa);
            fclose(fp);
            return;
        }
    }

    printf("Not found.\n");
    fclose(fp);
}


void updateStudent() {
    int sid = getInt("Enter ID to update: ");

    FILE *fp = fopen(STDFILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp) {
        printf("No records.\n");
        return;
    }

    Student st;
    int found = 0;

    while (fscanf(fp, "%d %s %d %c %f",
                  &st.id, st.name, &st.year, &st.section, &st.cgpa) == 5) {

        if (st.id == sid) {
            found = 1;

            printf("\n1. Update Year\n2. Update Section\n3. Update CGPA\n");
            int ch = getInt("Choose: ");

            if (ch == 1) st.year = getInt("New Year: ");
            else if (ch == 2) {
                char t[10];
                while (1) {
                    getString("New Section (A-Z): ", t, 10);
                    if (strlen(t) == 1 && isalpha(t[0])) {
                        st.section = toupper(t[0]);
                        break;
                    }
                    printf("Invalid section!\n");
                }
            }
            else if (ch == 3) st.cgpa = getFloat("New CGPA: ", 0, 10);
        }

        fprintf(temp, "%d %s %d %c %.2f\n",
                st.id, st.name, st.year, st.section, st.cgpa);
    }

    fclose(fp);
    fclose(temp);

    remove(STDFILE);
    rename("temp.txt", STDFILE);

    if (found) printf("Updated successfully.\n");
    else printf("Student not found.\n");
}


void editCGPA() {
    int sid = getInt("Enter ID to edit CGPA: ");

    FILE *fp = fopen(STDFILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp) {
        printf("No records.\n");
        return;
    }

    Student st;
    int found = 0;

    while (fscanf(fp, "%d %s %d %c %f",
                  &st.id, st.name, &st.year, &st.section, &st.cgpa) == 5) {

        if (st.id == sid) {
            found = 1;
            printf("Current CGPA: %.2f\n", st.cgpa);
            st.cgpa = getFloat("Enter new CGPA (0-10): ", 0, 10);
        }

        fprintf(temp, "%d %s %d %c %.2f\n",
                st.id, st.name, st.year, st.section, st.cgpa);
    }

    fclose(fp);
    fclose(temp);

    remove(STDFILE);
    rename("temp.txt", STDFILE);

    if (found) printf("CGPA updated successfully.\n");
    else printf("Student not found.\n");
}


void deleteStudent() {
    int sid = getInt("Enter ID to delete: ");

    FILE *fp = fopen(STDFILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp) {
        printf("No records.\n");
        return;
    }

    Student st;
    int deleted = 0;

    while (fscanf(fp, "%d %s %d %c %f",
                  &st.id, st.name, &st.year, &st.section, &st.cgpa) == 5) {

        if (st.id == sid) {
            deleted = 1;
            continue;
        }

        fprintf(temp, "%d %s %d %c %.2f\n",
                st.id, st.name, st.year, st.section, st.cgpa);
    }

    fclose(fp);
    fclose(temp);

    remove(STDFILE);
    rename("temp.txt", STDFILE);


    FILE *cf = fopen(CRDFILE, "r");
    FILE *ct = fopen("credtemp.txt", "w");
    char u[50], p[50], r;
    int cid;

    if (cf) {
        while (fscanf(cf, "%s %s %c %d", u, p, &r, &cid) == 4) {
            if (cid == sid) continue;
            fprintf(ct, "%s %s %c %d\n", u, p, r, cid);
        }

        fclose(cf);
        fclose(ct);
        remove(CRDFILE);
        rename("credtemp.txt", CRDFILE);
    }

    if (deleted) printf("Deleted successfully.\n");
    else printf("Not found.\n");
}



//AUTO GENERATED TEACHER LOGIN
void addTeacher() {
    int count = 0;
    FILE *cf = fopen(CRDFILE, "r");
    if (cf) {
        char u[50], p[50], r;
        int cid;
        while (fscanf(cf, "%s %s %c %d", u, p, &r, &cid) == 4) {
            if (r == 'T') count++;
        }
        fclose(cf);
    }

    char user[50], pass[50];
    sprintf(user, "teach%d", count + 1);
    sprintf(pass, "tpass%d!", count + 1);

    FILE *fp = fopen(CRDFILE, "a");
    fprintf(fp, "%s %s T -1\n", user, pass);
    fclose(fp);

    printf("Teacher added successfully.\n");
    printf("Username: %s\nPassword: %s\n", user, pass);
}



//Menu Functions
void adminMenu() {
    while (1) {
        printf("\n======[ ADMIN MENU ]======\n");
        printf("1. Add Student\n2. Display\n3. Search\n4. Update\n5. Delete\n6. Add Teacher\n7. Logout\n");

        int c = getInt("Enter choice: ");
        if (c == 1) addStudent();
        else if (c == 2) displayStudents();
        else if (c == 3) searchStudent();
        else if (c == 4) updateStudent();
        else if (c == 5) deleteStudent();
        else if (c == 6) addTeacher();
        else if (c == 7) return;
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
    }
}


void studentMenu(int sid) {
    printf("\n--- YOUR DETAILS ---\n");

    FILE *fp = fopen(STDFILE, "r");
    if (!fp) {
        printf("No record found.\n");
        return;
    }

    Student st;

    while (fscanf(fp, "%d %s %d %c %f",
                  &st.id, st.name, &st.year, &st.section, &st.cgpa) == 5) {

        if (st.id == sid) {
            printf("|| ID: %d\n|| Name: %s\n|| Year: %d\n|| Section: %c\n|| CGPA: %.2f\n",
                   st.id, st.name, st.year, st.section, st.cgpa);
            break;
        }
    }

    fclose(fp);
}


//MAIN
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
