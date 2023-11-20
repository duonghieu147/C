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

int isValid(int hh, int mm,int  dd, int mo, int yyyy) {
    if (hh < 0 || hh > 23) {
        return  11;
    }
    if (mm < 0 || mm > 59) {
        return 21;
    }
    if (dd < 1 || dd > 31) {
        return 31 ;
    }
    if (mo < 1 || mo > 12) {
        return 41 ;
    }
    if (yyyy <= 0) {
        return 51 ;
    }
    if ((mo == 4 || mo == 6 || mo == 9 || mo == 11) && dd > 30) {
        return 41 ;
    }
    if (mo == 2) {
        if ((yyyy % 4 == 0 && yyyy % 100 != 0) || (yyyy % 400 == 0)) {
            // Năm nhuận
            if (dd > 29) {
                return 41;
            }
        } else {
            // Năm không nhuận
            if (dd > 28) {
                return 41 ;
            }
        }
    }
    return -1;
}
int isCondition3(
        int datetime1_hour,int datetime1_minute,int datetime1_day, int datetime1_month,int  datetime1_year,
    int datetime2_hour,int  datetime2_minute,int  datetime2_day,int  datetime2_month,int  datetime2_year) {
    if (datetime2_year < datetime1_year ||
        (datetime2_year == datetime1_year &&
         (datetime2_month < datetime1_month ||
          (datetime2_month == datetime1_month &&
           (datetime2_day < datetime1_day ||
            (datetime2_day == datetime1_day &&
             (datetime2_hour < datetime1_hour ||
              (datetime2_hour == datetime1_hour && datetime2_minute < datetime1_minute)))))))) {
        return 0; 
    }
}
int concatenateIntegers(int a, int b) {
    char strA[10], strB[10];
    sprintf(strA, "%d", a);
    sprintf(strB, "%d", b);
    strcat(strA, strB);
    int result = atoi(strA);
    return result;
}
int checkTime(char *raw_time)
{
    int hh1, mm1, dd1, mo1, yy1;
    int hh2, mm2, dd2, mo2, yy2;

    // Đọc giá trị datetime1 từ raw_time
    sscanf(raw_time, "%d:%d|%d/%d/%d-%d:%d|%d/%d/%d",
           &hh1, &mm1, &dd1, &mo1, &yy1,
           &hh2, &mm2, &dd2, &mo2, &yy2);
    
    char isTrueTime1 = isValid(hh1,mm1,dd1,mo1,yy1);
    if (isTrueTime1 > 0 ) {
        if (isTrueTime1 == 11) {
            return concatenateIntegers(isTrueTime1,hh1);
        }
        if (isTrueTime1 == 21) {
            return concatenateIntegers(isTrueTime1,mm1);
        }
        if (isTrueTime1 == 31) {
            return concatenateIntegers(isTrueTime1,dd1);
        }
        if (isTrueTime1 == 41) {
            return concatenateIntegers(isTrueTime1,mo1);
        }
        if (isTrueTime1 == 51) {
            return concatenateIntegers(isTrueTime1,yy1);
        }
    }
    char isTrueTime2 = isValid(hh2,mm2,dd2,mo2,yy2);
    if (isTrueTime2 > 0 ) {
        if (isTrueTime2 == 11) {
            return concatenateIntegers(isTrueTime2 +1,hh2);
        }
        if (isTrueTime2 == 21) {
            return concatenateIntegers(isTrueTime2 +1,mm2);
        }
        if (isTrueTime2 == 31) {
            return concatenateIntegers(isTrueTime2 +1,dd2);
        }
        if (isTrueTime2 == 41) {
            return concatenateIntegers(isTrueTime2 +1,mo2);
        }
        if (isTrueTime2 == 51) {
            return concatenateIntegers(isTrueTime2 +1,yy2);
        }
    }

    int isTrueDK3 =  isCondition3(hh1,mm1,dd1,mo1,yy1,hh2,mm2,dd2,mo2,yy2);
    if (isTrueDK3== 0) {
        return 0;
    }
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
    if (num <=0)  {
        return 0;
    }
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
    // Tìm vị trí của chuỗi "status:[" trong câu lệnh Edit
    char *statusPos = strstr(edit_cmd, "status:[");

    // Nếu không tìm thấy chuỗi "status:[", trả về giá trị mặc định IN_PROGRESS
    if (statusPos == NULL) {
        return IN_PROGRESS;
    }

    // Di chuyển con trỏ đến phần bắt đầu của giá trị trạng thái
    statusPos += strlen("status:[");

    // Lấy giá trị trạng thái từ chuỗi edit_cmd
    char statusStr[20];
    sscanf(statusPos, "%[^]]", statusStr);

    // So sánh giá trị trạng thái và trả về enum tương ứng
    if ((strcmp(statusStr, "I") == 0) || (strcmp(statusStr, "i") == 0)) {
        return IN_PROGRESS;
    } else if ((strcmp(statusStr, "D") == 0) || (strcmp(statusStr, "d") == 0)) {
        return DONE;
    } else if ((strcmp(statusStr, "A") == 0) || (strcmp(statusStr, "a") == 0)) {
        return ARCHIVED;
    }else {
        // Trạng thái không hợp lệ, trả về giá trị mặc định IN_PROGRESS
        return IN_PROGRESS;
    }
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

bool addTask(struct Task *array_tasks, int no_tasks, char *new_title, char *new_description, char *new_time)
{
    if (no_tasks < MAX_NO_TASKS && checkTitle(new_title) == -1 && checkDescription(new_description) == -1&& checkTime(new_time) == -1)
    {
        strcpy(array_tasks[no_tasks].title, new_title);
        strcpy(array_tasks[no_tasks].description, new_description);
        strcpy(array_tasks[no_tasks].time, new_time);

        // Gán giá trị cho các biến thành viên tương ứng (num và status)
        array_tasks[no_tasks].num = no_tasks + 1;  // Ví dụ: Số công việc có thể được thiết lập là số thứ tự của công việc
        array_tasks[no_tasks].status = IN_PROGRESS; // Ví dụ: Mặc định là công việc chưa hoàn thành
        (no_tasks)++;
        return true;
    }
    else
    {
        return false;
    }
}
// Hàm xóa một công việc từ mảng công việc
bool deleteTask(struct Task *array_tasks, int no_tasks, int num)
{
    // Tìm vị trí của công việc có số num trong mảng
    int index = -1;
    for (int i = 0; i < no_tasks; ++i)
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
        for (int i = index; i < no_tasks - 1; ++i)
        {
            array_tasks[i] = array_tasks[i + 1];
        }

        // Giảm số lượng công việc trong mảng
        (no_tasks)--;

        // Cập nhật lại số của các công việc phía sau
        for (int i = index; i < no_tasks; ++i)
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
    return -1; // Trả về -1 để thể hiện rằng công việc đã được in theo tuần
}
// Other functions

// ------ End: Student Answer ------

void runTodoApp()
{
    printf("----- Sample testcase 20 -----\n");
    printf("Test deleteTask:\n");
    struct Task array_tasks[5] = { // extra slot for new task, 6 should be replaced by MAX_NO_TASKS
        {1, "Course Intro to Programming - apple", "Room 701-H6 - orange", "07:00|01/10/2023-12:00|01/10/2023", IN_PROGRESS},
        {2, "Course Intro to Programming - banana", "Room 701-H6 - apple", "07:00|01/10/2023-12:00|01/10/2023", DONE},
        {3, "Course Intro to Programming - apple", "Room 701-H6 - orange", "07:00|01/10/2023-12:00|01/10/2023", ARCHIVED},
        {4, "Course Intro to Programming - banana", "Room 701-H6 - orange", "07:00|01/10/2023-12:00|01/10/2023", IN_PROGRESS},
        {5, "Course Intro to Programming - apple", "Room 701-H6 - banana", "07:00|01/10/2023-12:00|01/10/2023", DONE},
    };
    int no_tasks = 5;
    if (deleteTask(array_tasks, 5, 3)) {
        --no_tasks;
        printf("Delete task successfully\n");
        printAllTasks(array_tasks, no_tasks);
    }
    else {
        printf("Delete task failed\n");
    }
}

int main()
{
    runTodoApp();
    return 0;
}