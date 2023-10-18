#ifndef SCHOOL_H_
#define SCHOOL_H_

/* Pointers to files to store records
    in case we delete record we store new data
    in inter and then rename it and delete the old
    data version.
*/
FILE *f_ptr,*inter;

// data required for student identification
struct student_data
{
    char name[20];
    char date_birth[20];
    char id[20];
    char add[20];
    char phone_no[20];
    float oa_score;
};

/* base node for any student consists
   of st_data and pointer 
*/
struct student_node
{
    struct student_data student;
    struct student_node *next;
};




// Functions prototype
void new_student(void);
void delete_student(void);
void student_score(void);
void student_rank(void);
void student_list(void);
void student_edit(void);
void init_data(struct student_data * ptr);
void read_line(char line[], int maxLength);
void insert_descending(struct student_node** head, struct student_data *data);
void print_scores(struct student_node * head);
void print_list(struct student_node * head);
void insert_ascending(struct student_node** head, struct student_data *data);
int compare_names(char *str1, char* str2);



/* this function displays the
    welcome screen and targeted activates.*/
void main_menu()
{
    int choice;
    printf("WARNINGS : 1- ALL RECORDS MUSTN'T EXCEED 20 DIGITS TILL NOW. \n");
    printf("           2- YOU MUST UPDATE OVERALL SCORES BEFOR CALLING SHOWING RANK. \n");
    printf("           3- THERE IS NO TYPE CHEKING SO CORRECTTNES OF INPUTS IS YOUR RESPONSIBILITY TILL NOW. \n");
    printf("           4- STUDENT ID MUST BE UNIQUE. \n");


    printf("   \nMENU\n\n");
    printf("1. ADD NEW STUDENT\n");
    printf("2. DELETE  STUDENT\n");
    printf("3. MODIFY  STUDENT RECODE\n");
    printf("4. SHOW STUDENTS LIST\n");
    printf("5. UPDATE SCORES\n");
    printf("6. SHOW RANK\n");
    printf("7. EXIT\n\n");
    printf("Enter your choice: ");
    scanf("%d",&choice);

    switch(choice)
    {
        case 1:
            new_student();
            break;
        case 2:
            delete_student();
            break;
        case 3:
            student_edit();
            break;
        case 4:
            student_list();
            break;
        case 5:
            student_score();
            break;
        case 6:
            student_rank();
            break;
        case 7:
            exit(0);
            break;
        default:
            break;
    }

}

/* this function creates a
    new student account. it asks about details of the
    student such as name, date of birth, ID, address
    and phone number. 
*/
void new_student()
{
    struct student_data std;

    init_data(&std);

    // here you can confirms the ID
    printf("Student ID : ");
    scanf("%s", std.id);

    printf("Name: "); 
    read_line(std.name, 20);

    printf("Date of birth (DD-MM-YYYY) : "); 
    scanf("%s", std.date_birth);

    printf("Address: "); 
    read_line(std.add, 20);
    
    printf("Phone NO: "); 
    read_line(std.phone_no, 20);

    printf("The record is sucessfully added\n");


    // Append open or create file for update, writing at end-of-file.
    f_ptr = fopen("data.txt","ab+");
    fwrite(&std, sizeof(std), 1, f_ptr);
    fclose(f_ptr);

    return;
}

/* this function is for
    deleting a customer account.
*/
void delete_student()
{
    struct student_data std_data;

    // define and init string id to compare with records
    char s_id[20];
    for(int i = 0;i < 20; i++)
    {
        s_id[i]='\0';
        // feild of intrest in search
        std_data.id[i]='\0';
    }

    printf("Enter student ID to delete: ");
    scanf("%s", s_id);

    f_ptr = fopen("data.txt","rb");
    inter = fopen("temp.txt", "wb");
    // reading data from data.txt structure by structure in std_data
    while(fread(&std_data, sizeof(std_data), 1, f_ptr) == 1){
        // writing all valid records in new file
        if(strcmp(s_id, std_data.id) != 0){
            fwrite(&std_data, sizeof(std_data), 1, inter);
        }

    }
    // delete old file and rename the new one
    fclose(f_ptr);
    fclose(inter);
    remove("data.txt");
    rename("temp.txt","data.txt");
    printf("record deleted\n");
    return;
}

/* this function has been used
    for updating a particular student account.
*/
void student_edit()
{
    struct student_data std_data;

    // take id as input to modifiy record
    char std_id[20];
    int found_flag = 0;

    printf("Enter Student ID. to modify: ");
    scanf("%s", std_id);

    f_ptr = fopen("data.txt","rb+");

    while(fread(&std_data, sizeof(std_data), 1, f_ptr) == 1)
    {

        if(strcmp(std_id, std_data.id) == 0){

            printf("Name : ");
            read_line(std_data.name, 20);

            printf("Date of birth (DD-MM-YYYY) : "); 
            scanf("%s",std_data.date_birth);

            printf("Address: ");
            read_line(std_data.add, 20);

            printf("Phone Number: ");
            read_line(std_data.phone_no, 20);

            fseek(f_ptr, -sizeof(std_data), SEEK_CUR);
            fwrite(&std_data,sizeof(std_data), 1, f_ptr);

            found_flag = 1;
            break;
        }
    }

    if(found_flag)
    {
        printf("The record is sucessfully modified\n\n");
    }
    else
        printf("No Record Found\n");

    fclose(f_ptr);
    return;
}

/*  this function, you can view
    the students information such as name, date of
    birth, ID, address and phone number in table
    form (alphabetically ascending).
*/
void student_list()
{
    struct student_data std_data;
    struct student_node *head = NULL;

    f_ptr = fopen("data.txt","rb+");

    int flag = 0;
    while(fread(&std_data, sizeof(std_data), 1, f_ptr) == 1)
    {
        insert_ascending(&head, &std_data);
        flag++;
    }


    if (flag)
        print_list(head);
    else
        printf("Error in Student List\n");


    fclose(f_ptr);
    return;
}

void print_list(struct student_node * head) 
{
    printf("\nDISCLAIMER : ALL RECORDS SORTED ASCENDING ONLY BY NAME.\n\n");
    struct student_node* current = head;

    while (current != NULL) {
        printf("Name: %-20s \t", current->student.name);
        printf("ID: %-20s \t", current->student.id);
        printf("DOB:%-20s \t", current->student.date_birth);
        printf("Address: %-20s \t", current->student.add);
        printf("Phone NO: %-20s \n", current->student.phone_no);
        current = current->next;
    }
    return;
}

int compare_names(char *str1, char* str2) 
{
    int result = strcmp(str1, str2);
    
    if (result < 0) {
        return 1; // str1 comes before str2
    } else {
        return 0; // str1 does not come before str2
    }
}

void insert_ascending(struct student_node** head, struct student_data *data) 
{

    struct student_node *new_node = (struct student_node *) malloc(sizeof(struct student_node));
   
    if(new_node != NULL)
    {
        strcpy(new_node->student.add, data->add);
        strcpy(new_node->student.date_birth, data->date_birth);
        strcpy(new_node->student.id, data->id);
        strcpy(new_node->student.name, data->name);
        strcpy(new_node->student.phone_no, data->phone_no);
        new_node->student.oa_score = data->oa_score;
        
        new_node->next = NULL;


        if (*head == NULL) 
        {
            *head = new_node;
        } 
        else 
        {
            struct student_node *current = *head;
            struct student_node *prev = NULL;            
            while (current != NULL && compare_names(current->student.name, data->name)) 
            {
                prev = current;
                current = current->next;
            }

            if (prev == NULL) 
            {
                new_node->next = *head;
                *head = new_node;
            } 
            else 
            {
                prev->next = new_node;
                new_node->next = current;
            }
        }
    }
    else
    {
        printf("Somthing wrong with insert decending\n");
        exit(0);
    }

    return;
}

/* this function has been
   used for updating score of all students.
   This Function MUST be called before student_rank().
*/
void student_score()
{
    struct student_data std_data;

    float oa_score;
    int flag = 0;

    f_ptr = fopen("data.txt","rb+");

    while(fread(&std_data, sizeof(std_data), 1, f_ptr) == 1)
    {
        printf("For student name : %s\n", std_data.name);
        printf("With student ID : %s\n", std_data.id);
        printf("Please put Overall score : ");
        scanf("%f", &std_data.oa_score);

        fseek(f_ptr, -sizeof(std_data), SEEK_CUR);
        fwrite(&std_data,sizeof(std_data), 1, f_ptr);
        flag++;
        printf("Overall score has been set successfully\n\n");
    }

    if (!flag)
    {
        printf("Unsucssefull score assesment\n");
    }

    fclose(f_ptr);
    return;
}

/* this function can sort all
    students according to overall score.
*/
void student_rank()
{
    struct student_data std_data;
    struct student_node *head = NULL;

    f_ptr = fopen("data.txt","rb+");

    int flag = 0;
    while(fread(&std_data, sizeof(std_data), 1, f_ptr) == 1)
    {
        insert_descending(&head, &std_data);
        flag++;
    }


    if (flag)
        print_scores(head);
    else
        printf("Error in Student Ranking\n");


    fclose(f_ptr);
    return;
}

/* A function to sort all nodes in decending order
*/
void insert_descending(struct student_node** head, struct student_data *data) 
{

    struct student_node *new_node = (struct student_node *) malloc(sizeof(struct student_node));
   
    if(new_node != NULL)
    {
        strcpy(new_node->student.add, data->add);
        strcpy(new_node->student.date_birth, data->date_birth);
        strcpy(new_node->student.id, data->id);
        strcpy(new_node->student.name, data->name);
        strcpy(new_node->student.phone_no, data->phone_no);
        new_node->student.oa_score = data->oa_score;
        
        new_node->next = NULL;


        if (*head == NULL) 
        {
            *head = new_node;
        } 
        else 
        {
            struct student_node *current = *head;
            struct student_node *prev = NULL;            

            while (current != NULL && ((current->student.oa_score) > (data->oa_score))) 
            {
                prev = current;
                current = current->next;
            }

            if (prev == NULL) 
            {
                new_node->next = *head;
                *head = new_node;
            } 
            else 
            {
                prev->next = new_node;
                new_node->next = current;
            }
        }
    }
    else
    {
        printf("Somthing wrong with insert decending\n");
        exit(0);
    }

    return;
}
/* Printing all valid nodes.
*/
void print_scores(struct student_node * head) 
{
    printf("\nDISCLAIMER : ALL RECORDS SORTED DESCENDING ONLY BY OVERALL SCORE.\n\n");

    struct student_node* current = head;

    while (current != NULL) {
        printf("Student: %-20s \t", current->student.name);
        printf("With ID: %-20s \t", current->student.id);
        printf("Overall Score: %.2f \n", current->student.oa_score);
        current = current->next;
    }
    return;
}

void init_data(struct student_data * ptr)
{

    // we shold generlize max count
    for(int i = 0; i < 20; i++)
    {
        ptr->add[i] = '\0';
        ptr->date_birth[i] = '\0';
        ptr->id[i] = '\0';
        ptr->name[i] = '\0';
        ptr->phone_no[i] = '\0';

    }
    
    ptr->oa_score = 0;
    return;
}

void read_line(char line[], int maxLength) 
{
    int i = 0;
    char c;

    // Read characters until the newline or maxLength is reached
    c = getchar();

    while ((c = getchar()) != '\n' && i < maxLength - 1) {
        line[i] = c;
            
        i++;
    }

    // Null-terminate the line
    line[i] = '\0';
}

#endif
