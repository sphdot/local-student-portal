#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define MAX_SLOTS 4
#define NAME_LEN 64
#define DATA_FILE "std.txt"

typedef struct {
    int id;
    char name[NAME_LEN];
    bool occupied;
} Student;

static void clear_screen(void) {
    system("cls");
}

static void pause_screen(void) {
    printf("\npress enter to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static bool read_line(char *buf, size_t size) {
    if (!fgets(buf, (int)size, stdin)) {
        return false;
    }
    buf[strcspn(buf, "\r\n")] = '\0';
    return true;
}

static int read_int(const char *prompt) {
    char line[32];
    char *endptr;
    long val;

    while (1) {
        printf("%s", prompt);
        if (!read_line(line, sizeof(line))) {
            return -1;
        }
        if (line[0] == '\0') {
            continue;
        }
        val = strtol(line, &endptr, 10);
        if (*endptr == '\0') {
            return (int)val;
        }
        printf("invalid number. try again.\n");
    }
}

static void get_data_path(char *out_path, size_t size) {
    DWORD len = GetModuleFileNameA(NULL, out_path, (DWORD)size);
    if (len == 0 || len >= size) {
        snprintf(out_path, size, "%s", DATA_FILE);
        return;
    }
    char *last_slash = strrchr(out_path, '\\');
    if (last_slash) {
        *(last_slash + 1) = '\0';
        strncat(out_path, DATA_FILE, size - strlen(out_path) - 1);
    } else {
        snprintf(out_path, size, "%s", DATA_FILE);
    }
}

static void load_students(Student students[], int count) {
    char path[MAX_PATH];
    get_data_path(path, sizeof(path));

    FILE *fp = fopen(path, "r");
    if (!fp) {
        return;
    }

    int slot, id;
    char name[NAME_LEN];
    while (fscanf(fp, "%d\t%d\t%63[^\r\n]\n", &slot, &id, name) == 3) {
        if (slot >= 1 && slot <= count) {
            students[slot - 1].id = id;
            strncpy(students[slot - 1].name, name, NAME_LEN - 1);
            students[slot - 1].name[NAME_LEN - 1] = '\0';
            students[slot - 1].occupied = true;
        }
    }
    fclose(fp);
}

static void save_students(const Student students[], int count) {
    char path[MAX_PATH];
    get_data_path(path, sizeof(path));

    FILE *fp = fopen(path, "w");
    if (!fp) {
        printf("\nerror: could not write to %s\n", DATA_FILE);
        return;
    }

    for (int i = 0; i < count; i++) {
        if (students[i].occupied) {
            fprintf(fp, "%d\t%d\t%s\n", i + 1, students[i].id, students[i].name);
        }
    }
    fclose(fp);
}

static void print_slots(const Student students[], int count) {
    for (int i = 0; i < count; i++) {
        if (students[i].occupied) {
            printf("%d - id: %d, name: %s\n", i + 1, students[i].id, students[i].name);
        } else {
            printf("%d - empty\n", i + 1);
        }
    }
}

static int select_slot(const Student students[], int count) {
    clear_screen();
    print_slots(students, count);

    while (1) {
        int slot = read_int("\nselect slot (1-4): ");
        if (slot >= 1 && slot <= count) {
            return slot - 1;
        }
        printf("invalid slot. choose between 1 and %d.\n", count);
    }
}

static void create_student(Student students[], int count) {
    int idx = select_slot(students, count);
    if (students[idx].occupied) {
        printf("\nslot %d is already occupied.\n", idx + 1);
        pause_screen();
        return;
    }

    printf("\nenter student name: ");
    read_line(students[idx].name, NAME_LEN);
    students[idx].id = read_int("enter student id: ");
    students[idx].occupied = true;

    save_students(students, count);
    printf("\nstudent added to slot %d.\n", idx + 1);
    pause_screen();
}

static void read_students(const Student students[], int count) {
    clear_screen();
    printf("*** student records ***\n");
    printf("------------------------------\n");
    print_slots(students, count);
    pause_screen();
}

static void update_student(Student students[], int count) {
    int idx = select_slot(students, count);
    if (!students[idx].occupied) {
        printf("\nslot %d is empty.\n", idx + 1);
        pause_screen();
        return;
    }

    printf("\nenter new student name: ");
    read_line(students[idx].name, NAME_LEN);
    students[idx].id = read_int("enter new student id: ");

    save_students(students, count);
    printf("\nslot %d updated.\n", idx + 1);
    pause_screen();
}

static void delete_student(Student students[], int count) {
    int idx = select_slot(students, count);
    if (!students[idx].occupied) {
        printf("\nslot %d is already empty.\n", idx + 1);
        pause_screen();
        return;
    }

    students[idx].id = 0;
    students[idx].name[0] = '\0';
    students[idx].occupied = false;

    save_students(students, count);
    printf("\nslot %d deleted.\n", idx + 1);
    pause_screen();
}

int main(void) {
    Student students[MAX_SLOTS] = {0};
    load_students(students, MAX_SLOTS);

    bool running = true;
    while (running) {
        clear_screen();
        printf("*** student portal ***\n");
        printf("------------------------------\n");
        printf("0: exit | 1: create | 2: read | 3: update | 4: delete\n");

        int choice = read_int("\n--> ");
        switch (choice) {
            case 0:
                running = false;
                printf("\nexiting student portal.\n");
                break;
            case 1:
                create_student(students, MAX_SLOTS);
                break;
            case 2:
                read_students(students, MAX_SLOTS);
                break;
            case 3:
                update_student(students, MAX_SLOTS);
                break;
            case 4:
                delete_student(students, MAX_SLOTS);
                break;
            default:
                printf("\ninvalid option.\n");
                pause_screen();
                break;
        }
    }

    return 0;
}
