#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LENGTH_COMMAND 300

#define MAX_NO_TASKS 100

#define MAX_LENGTH_TITLE 100
#define MAX_LENGTH_DESCRIPTION 200
#define MAX_LENGTH_TIME 33

#define WEEK_CELL_FIRST_COL_WIDTH 10
#define WEEK_CELL_OTHER_COL_WIDTH 20

enum Status
{
    IN_PROGRESS,
    DONE,
    ARCHIVED
};
char *status_name[] = {"In Progress", "Done", "Archived"};
enum CommandType
{
    ADD,
    EDIT,
    SHOW,
    DELETE,
    QUIT,
    INVALID
};
char *command_name[] = {"ADD", "EDIT", "SHOW", "DELETE", "QUIT", "INVALID"};

struct Task
{
    int num;
    char title[MAX_LENGTH_TITLE + 1];
    char description[MAX_LENGTH_DESCRIPTION + 1];
    char time[MAX_LENGTH_TIME + 1];
    enum Status status;
};

void printTask(struct Task *task)
{
    printf("--------------------------------------------\n");
    printf("Num: #%d. Title: %s\n", task->num, task->title);
    printf("Description: %s\n", task->description);
    printf("Status: %s\n", status_name[task->status]);
    printf("--------------------------------------------\n");
}

void printUnsupportedTime(struct Task *task)
{
    printf("----- Show week view -----\n");
    printf("Error: Unsupported time with non-zero minutes: %s\n", task->time);
    printf("In Task:\n");
    printTask(task);
}

// ------ Begin: Student Answer ------

enum CommandType getCommandType(char *command)
{

    char *first = strdup(command);
    char *token = strtok(first, " ");
    if (strcmp(token, "Add") == 0)
    {
        return ADD;
    }
    else if (strcmp(token, "Edit") == 0)
    {
        return EDIT;
    }
    else if(strcmp(token, "Show") == 0)
    {
        return SHOW;
    }
    else if (strcmp(token, "Delete") == 0)
    {
        return DELETE;
    }
    else if (strcmp(token, "Quit") == 0)
    {
        return QUIT;
    } else {
        return INVALID;
    }
}

void getTitleFromAdd(char *command, char *out_title)
{
    // Tìm vị trí của dấu ngoặc vuông đầu tiên và cuối cùng
    char *start = strchr(command, '[');
    char *end = strchr(command, ']');

    // Kiểm tra xem có dấu ngoặc vuông hay không và lấy nội dung bên trong
    if (start != NULL && end != NULL)
    {
        strncpy(out_title, start + 1, end - start - 1);
        out_title[end - start - 1] = '\0'; // Kết thúc chuỗi
    }
}

void getDescriptionFromAdd(char *command, char *out_description)
{
    // Tìm vị trí của dấu ngoặc vuông thứ hai và thứ ba
    char *start = strchr(command, '[');
    start = strchr(start + 1, '['); // Tìm dấu ngoặc vuông thứ hai
    char *end = strchr(start, ']');

    // Kiểm tra xem có dấu ngoặc vuông thứ hai và thứ ba hay không và lấy nội dung bên trong
    if (start != NULL && end != NULL)
    {
        strncpy(out_description, start + 1, end - start - 1);
        out_description[end - start - 1] = '\0'; // Kết thúc chuỗi
    }
}

void getTimeFromAdd(char *command, char *out_time)
{
    // Tìm vị trí của dấu ngoặc vuông thứ ba và thứ tư
    char *start = strchr(command, '[');
    start = strchr(start + 1, '['); // Tìm dấu ngoặc vuông thứ hai
    start = strchr(start + 1, '['); // Tìm dấu ngoặc vuông thứ ba
    char *end = strchr(start, ']');

    // Kiểm tra xem có dấu ngoặc vuông thứ ba và thứ tư hay không và lấy nội dung bên trong
    if (start != NULL && end != NULL)
    {
        strncpy(out_time, start + 1, end - start - 1);
        out_time[end - start - 1] = '\0'; // Kết thúc chuỗi
    }
}

int checkTitle(char *raw_title)
{
    int length = strlen(raw_title);

    if (length > MAX_LENGTH_TITLE)
    {
        return length;
    }

    if (isspace(raw_title[0]))
    {
        return 0;
    }
        if (isspace(raw_title[length- 1]))
    {
        return length - 1;
    }

    // if (isspace(raw_title[length - 1])) {
    //     for (int i = length - 1 ; i >=0 ; i--)
    //     {
    //         if (isspace(raw_title[length - 1])) {
    //             return i;
    //         }    
    //     }
        
    // }

    for (int i = 0; i < length; i++)
    {
        char currentChar = raw_title[i];

        if (!isalpha(currentChar) && !isdigit(currentChar) &&
            currentChar != ' ' && currentChar != ',' &&
            currentChar != '.' && currentChar != '-' &&
            currentChar != ':' && currentChar != '|' &&
            currentChar != '/')
        {
            return i; // Trả về vị trí của ký tự đầu tiên vi phạm điều kiện
        }
    }

    // Tiêu đề hợp lệ, trả về -1
    return -1;
}

int checkDescription(char *raw_description)
{
    int length = strlen(raw_description);

    if (length > MAX_LENGTH_DESCRIPTION)
    {
        return length;
    }

    if (isspace(raw_description[0]) || isspace(raw_description[length - 1]))
    {
        return 0;
    }

    for (int i = 0; i < length; i++)
    {
        char currentChar = raw_description[i];

        if (!isalpha(currentChar) && !isdigit(currentChar) &&
            currentChar != ' ' && currentChar != ',' &&
            currentChar != '.' && currentChar != '-' &&
            currentChar != ':' && currentChar != '|' &&
            currentChar != '/')
        {
            return i; // Trả về vị trí của ký tự đầu tiên vi phạm điều kiện
        }
    }

    // Tiêu đề hợp lệ, trả về -1
    return -1;
}

int isValidTime(int hour, int minute)
{
    return (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59);
}

int checkTime(char *raw_time)
{
    return -1;
}

void getTitleFromEdit(char *command, char *out_title)
{
    char *titlePos = strstr(command, "title:[");
    if (titlePos != NULL)
    {
        sscanf(titlePos + 7, "%[^]]", out_title);
    }
    else
    {
        out_title[0] = '\0';
    }
}

void getDescriptionFromEdit(char *command, char *out_description)
{
    char *descPos = strstr(command, "description:[");
    if (descPos != NULL)
    {
        sscanf(descPos + 13, "%[^]]", out_description);
    }
    else
    {
        out_description[0] = '\0';
    }
}

void getTimeFromEdit(char *command, char *out_time)
{
    char *timePos = strstr(command, "time:[");
    if (timePos != NULL)
    {
        sscanf(timePos + 6, "%[^]]", out_time);
    }
    else
    {
        out_time[0] = '\0';
    }
}
int getNumFromCommand(char *command)
{
    char *hashPos = strchr(command, '#');
    if (hashPos == NULL || hashPos[1] == '\0') {
        return -1;
    }

    int num = atoi(hashPos + 1);
    if (num == 0 && !(hashPos[1] == '0' && hashPos[2] == '\0')) {
        return 0;
    }

    return num;
}
int getFieldFromEdit(char *edit_cmd)
{   
    char *copied_edit_cmd = strdup(edit_cmd);
    char *token = strtok(copied_edit_cmd, " ");


    for (int i = 0; i < 2 && token != NULL; i++) {
        token = strtok(NULL, " ");
    }

    if (token != NULL) {
        if (strcmp(token, "title") == 0) {
            return 1; 
        } else if (strcmp(token, "description") == 0) {
            return 2; 
        } else if (strcmp(token, "time") == 0) {
            return 3; 
        } else if (strcmp(token, "status") == 0) {
            return 4; 
        }
    } else {
        return 0;
    }

}
enum Status getStatusFromEdit(char *edit_cmd)
{
    return IN_PROGRESS;
}
void printAllTasks(struct Task *array_tasks, int no_tasks)
{
    for (int i = 0; i < no_tasks; ++i)
    {
        printTask(&array_tasks[i]);
    }
}
void printTaskByNum(struct Task *array_tasks, int no_tasks, int num)
{
     for (int i = 0; i < no_tasks; i++) {
        if (array_tasks[i].num == num) {
            printTask(&array_tasks[i]);
            return;
        }
    }
}
void printHeadTasks(struct Task *array_tasks, int no_tasks, int quan)
{
    int numTasksToPrint = (quan < no_tasks) ? quan : no_tasks;

    for (int i = 0; i < numTasksToPrint; i++) {
        printTask(&array_tasks[i]);
    }
}
void printTailTasks(struct Task *array_tasks, int no_tasks, int quan)
{
    int numTasksToPrint = (quan < no_tasks) ? quan : no_tasks;
    int startIndex = no_tasks - numTasksToPrint;

    for (int i = startIndex; i < no_tasks; i++) {
        printTask(&array_tasks[i]);
    }
}
void printFilteredTasksByTitle(struct Task *array_tasks, int no_tasks, char *filter_title)
{
    for (int i = 0; i < no_tasks; i++) {
        if (strstr(array_tasks[i].title, filter_title) != NULL) {
            printTask(&array_tasks[i]);
        }
    }
}
void printFilteredTasksByDescription(struct Task *array_tasks, int no_tasks, char *filter_description)
{
    for (int i = 0; i < no_tasks; i++) {
        if (strstr(array_tasks[i].description, filter_description) != NULL) {
            printTask(&array_tasks[i]);
        }
    }
}
void printFilteredTasksByStatus(struct Task *array_tasks, int no_tasks, enum Status filter_status)
{
    for (int i = 0; i < no_tasks; i++) {
        if (array_tasks[i].status == filter_status) {
            // Print the task using the printTask function
            printTask(&array_tasks[i]);
        }
    }
}

bool addTask(struct Task *array_tasks, int *no_tasks, char *new_title, char *new_description, char *new_time)
{
    if (*no_tasks < MAX_NO_TASKS)
    {
        strcpy(array_tasks[*no_tasks].title, new_title);
        strcpy(array_tasks[*no_tasks].description, new_description);
        strcpy(array_tasks[*no_tasks].time, new_time);

        // Gán giá trị cho các biến thành viên tương ứng (num và status)
        array_tasks[*no_tasks].num = *no_tasks + 1;  // Ví dụ: Số công việc có thể được thiết lập là số thứ tự của công việc
        array_tasks[*no_tasks].status = IN_PROGRESS; // Ví dụ: Mặc định là công việc chưa hoàn thành
        (*no_tasks)++;
        return true;
    }
    else
    {
        return false;
    }
}
// Hàm xóa một công việc từ mảng công việc
bool deleteTask(struct Task *array_tasks, int *no_tasks, int num)
{
    // Tìm vị trí của công việc có số num trong mảng
    int index = -1;
    for (int i = 0; i < *no_tasks; ++i)
    {
        if (array_tasks[i].num == num)
        {
            index = i;
            break;
        }
    }

    // Kiểm tra xem có công việc có số num hay không
    if (index != -1)
    {
        // Dời các công việc phía sau công việc cần xóa để lấp đầy vị trí đã xóa
        for (int i = index; i < *no_tasks - 1; ++i)
        {
            array_tasks[i] = array_tasks[i + 1];
        }

        // Giảm số lượng công việc trong mảng
        (*no_tasks)--;

        // Cập nhật lại số của các công việc phía sau
        for (int i = index; i < *no_tasks; ++i)
        {
            array_tasks[i].num = i + 1;
        }

        // Trả về giá trị true để thông báo việc xóa công việc thành công
        return true;
    }
    else
    {
        // Trả về giá trị false để thông báo không tìm thấy công việc có số num
        return false;
    }
}
int printWeekTime(struct Task *array_tasks, int no_tasks, char *date)
{
    return -1;
}
// Other functions

// ------ End: Student Answer ------

void runTodoApp()
{
    // Example of command Add
    char command[MAX_LENGTH_COMMAND + 1];

    while (true)
    {
        // Sample input:
        // Add [Course Intro to Programming] [Room 701-H6] [07:00|01/10/2023-12:00|01/10/2023]
        fgets(command, MAX_LENGTH_COMMAND + 1, stdin);
        command[strlen(command) - 1] = '\0';

        enum CommandType commandType = getCommandType(command);
        printf("Command     : %s\n", command);
        printf("Command type: %s\n", command_name[commandType]);

        break; // only one loop for simple test
               // actual app will break when encounter QUIT command
    }
}

int main()
{
    runTodoApp();
    return 0;
}
