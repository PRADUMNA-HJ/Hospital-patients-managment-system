#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 50
#define MAX_DISEASE 50
#define MAX_TIME 26

typedef struct {
    int id;
    char name[MAX_NAME];
    int age;
    char gender;
    char disease[MAX_DISEASE];
    int roomNo;
    char admissionTime[MAX_TIME];
} Patient;

typedef struct PatientNode {
    Patient data;
    struct PatientNode *next;
} PatientNode;

PatientNode* create_patient_node(Patient data) {
    PatientNode *newNode = (PatientNode*)malloc(sizeof(PatientNode));
    if (newNode) {
        newNode->data = data;
        newNode->next = NULL;
    }
    return newNode;
}

void free_patients(PatientNode *head) {
    while (head) {
        PatientNode *temp = head;
        head = head->next;
        free(temp);
    }
}

PatientNode* add_patient(PatientNode *head) {
    Patient newPatient;
    printf("\nEnter Patient Details:\n");
    printf("ID: ");
    scanf("%d", &newPatient.id);
    getchar(); // Clear buffer
    
    printf("Name: ");
    fgets(newPatient.name, MAX_NAME, stdin);
    newPatient.name[strcspn(newPatient.name, "\n")] = 0;
    
    printf("Age: ");
    scanf("%d", &newPatient.age);
    getchar();
    
    printf("Gender (M/F): ");
    scanf("%c", &newPatient.gender);
    getchar();
    
    printf("Disease: ");
    fgets(newPatient.disease, MAX_DISEASE, stdin);
    newPatient.disease[strcspn(newPatient.disease, "\n")] = 0;
    
    printf("Room No: ");
    scanf("%d", &newPatient.roomNo);
    
    // Get current time
    time_t now = time(NULL);
    strftime(newPatient.admissionTime, MAX_TIME, "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    PatientNode *newNode = create_patient_node(newPatient);
    if (!head) return newNode;
    
    PatientNode *cur = head;
    while (cur->next) cur = cur->next;
    cur->next = newNode;
    return head;
}

void list_patients(PatientNode *head) {
    if (!head) {
        printf("No patients in the system.\n");
        return;
    }
    
    printf("\nPatient List:\n");
    printf("ID\tName\t\tAge\tGender\tDisease\t\tRoom\tAdmission Time\n");
    printf("----------------------------------------------------------------------------\n");
    
    PatientNode *cur = head;
    while (cur) {
        printf("%d\t%-15s\t%d\t%c\t%-15s\t%d\t%s\n",
               cur->data.id, cur->data.name, cur->data.age,
               cur->data.gender, cur->data.disease,
               cur->data.roomNo, cur->data.admissionTime);
        cur = cur->next;
    }
}

int save_to_file(PatientNode *head, const char *filename) {
    if (!head) {
        printf("No patients to save!\n");
        return 0;
    }

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("Cannot open file for writing!\n");
        return 0;
    }

    PatientNode *cur = head;
    while (cur) {
        if (fwrite(&cur->data, sizeof(Patient), 1, fp) != 1) {
            printf("Error writing to file!\n");
            fclose(fp);
            return 0;
        }
        cur = cur->next;
    }
    
    fclose(fp);
    printf("Data saved successfully!\n");
    return 1;
}

PatientNode* load_from_file(const char *filename) {
    FILE *fp = fopen(filename, "rb");  // Open in binary read mode
    if (!fp) {
        printf("Cannot open file for reading!\n");
        return NULL;
    }

    PatientNode *head = NULL;
    PatientNode *tail = NULL;
    Patient temp;

    while (fread(&temp, sizeof(Patient), 1, fp) == 1) {
        PatientNode *newNode = create_patient_node(temp);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    fclose(fp);
    printf("Data loaded successfully!\n");
    return head;
}

int export_to_csv(PatientNode *head, const char *filename) {
    if (!head) {
        printf("No patients to export!\n");
        return 0;
    }

    char fullpath[256];
    sprintf(fullpath, "C:\\Users\\Hp\\Desktop\\dsaP_3sem\\%s", filename);
    
    // Check if file exists
    FILE *check = fopen(fullpath, "r");
    int isNewFile = (check == NULL);
    if (check) fclose(check);
    
    // Open in append mode to preserve existing data
    FILE *fp = fopen(fullpath, "a");
    if (!fp) {
        printf("Cannot open CSV file at: %s\n", fullpath);
        perror("Error");
        return 0;
    }

    // Write header only for new files
    if (isNewFile) {
        fprintf(fp, "ID,Name,Age,Gender,Disease,RoomNo,AdmissionTime,ExportDate\n");
    }
    
    // Get current date/time for export timestamp
    time_t now = time(NULL);
    char exportTime[26];
    strftime(exportTime, sizeof(exportTime), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    // Write patient records with export timestamp
    PatientNode *cur = head;
    int count = 0;
    while (cur) {
        if (fprintf(fp, "%d,%s,%d,%c,%s,%d,%s,%s\n",
                cur->data.id,
                cur->data.name,
                cur->data.age,
                cur->data.gender,
                cur->data.disease,
                cur->data.roomNo,
                cur->data.admissionTime,
                exportTime) < 0) {
            printf("Error writing to CSV file\n");
            fclose(fp);
            return 0;
        }
        count++;
        cur = cur->next;
    }
    
    fflush(fp);
    fclose(fp);
    
    printf("Successfully appended %d records to:\n%s\n", count, fullpath);
    printf("Export timestamp: %s\n", exportTime);
    return 1;
}

// Add these functions before main():

PatientNode* search_patient(PatientNode *head, int id) {
    PatientNode *cur = head;
    while (cur) {
        if (cur->data.id == id) {
            // Print patient details when found
            printf("\nPatient Found:\n");
            printf("ID: %d\n", cur->data.id);
            printf("Name: %s\n", cur->data.name);
            printf("Age: %d\n", cur->data.age);
            printf("Gender: %c\n", cur->data.gender);
            printf("Disease: %s\n", cur->data.disease);
            printf("Room No: %d\n", cur->data.roomNo);
            printf("Admission Time: %s\n", cur->data.admissionTime);
            return cur;
        }
        cur = cur->next;
    }
    printf("Patient with ID %d not found!\n", id);
    return NULL;
}

PatientNode* delete_patient(PatientNode *head, int id) {
    if (!head) {
        printf("No patients in the system!\n");
        return NULL;
    }

    // If first node is the target
    if (head->data.id == id) {
        PatientNode *temp = head->next;
        free(head);
        printf("Patient deleted successfully!\n");
        return temp;
    }

    // Search for the node to delete
    PatientNode *cur = head;
    while (cur->next && cur->next->data.id != id) {
        cur = cur->next;
    }

    // If patient was not found
    if (!cur->next) {
        printf("Patient with ID %d not found!\n", id);
        return head;
    }

    // Delete the node
    PatientNode *temp = cur->next;
    cur->next = temp->next;
    free(temp);
    printf("Patient deleted successfully!\n");
    return head;
}

int main() {
    PatientNode *head = NULL;
    int choice;

    while (1) {
        printf("\nHospital Patient Management System\n");
        printf("1. Add Patient\n");
        printf("2. List Patients\n");
        printf("3. Search Patient\n");
        printf("4. Delete Patient\n");
        printf("5. Save to File\n");
        printf("6. Load from File\n");
        printf("7. Export to CSV\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear buffer

        switch (choice) {
            case 1:
                head = add_patient(head);
                break;
            case 2:
                list_patients(head);
                break;
                     case 3: {
                int search_id;
                printf("Enter patient ID to search: ");
                scanf("%d", &search_id);
                getchar(); // Clear buffer
                search_patient(head, search_id);
                break;
            }
            case 4: {
                int delete_id;
                printf("Enter patient ID to delete: ");
                scanf("%d", &delete_id);
                getchar(); // Clear buffer
                head = delete_patient(head, delete_id);
                break;
            }    
            case 5:
                save_to_file(head, "patients.dat");
                break;
            case 6: {
                PatientNode *newlist = load_from_file("patients.dat");
                if (newlist) {
                    free_patients(head);
                    head = newlist;
                }
                break;
            }
            case 7:
                export_to_csv(head, "patients.csv");
                break;
            case 8:
                free_patients(head);
                return 0;
            default:
                printf("Invalid choice!\n");
        }
        
        printf("\nPress Enter to continue...");
        getchar();
    }
    return 0;
}