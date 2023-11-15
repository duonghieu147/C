#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <stdlib.h>

#define MAX_TASKS 100

struct Task
{
    int num;
    char title[50];
    int status;
    char description[100];
    char date[20];
};

// Định nghĩa các kiểu lệnh
typedef enum
{
    SHOW,
    ADD,
    EDIT,
    DELETE,
    QUIT,
    INVALID
} CommandType;
// Định nghĩa các trạng thái công việc
typedef enum
{
    IN_PROGRESS,
    DONE,
    ARCHIVED
} StatusType;

void printTask(struct Task *task)
{
    printf("Title: %s\n", task->title);
    printf("Description: %s\n", task->description);
    printf("Date: %s\n", task->date);
}

// Hàm in ra tất cả các công việc trong mảng
void printAllTasks(struct Task *array_tasks, int no_tasks) {
    for (int i = 0; i < no_tasks; ++i) {
        printTask(&array_tasks[i]);
    }
}

// Hàm thêm một công việc mới vào mảng công việc
bool addTask(struct Task *array_tasks, int *no_tasks, char *new_title, char *new_description, char *new_time)
{
    if (*no_tasks < MAX_TASKS)
    {
        strcpy(array_tasks[*no_tasks].title, new_title);
        strcpy(array_tasks[*no_tasks].description, new_description);
        strcpy(array_tasks[*no_tasks].date, new_time);

        // Gán giá trị cho các biến thành viên tương ứng (num và status)
        array_tasks[*no_tasks].num = *no_tasks + 1; // Ví dụ: Số công việc có thể được thiết lập là số thứ tự của công việc
        array_tasks[*no_tasks].status = IN_PROGRESS;       // Ví dụ: Mặc định là công việc chưa hoàn thành
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

// Hàm chuyển đổi chuỗi lệnh thành enum CommandType
CommandType parseCommand(char *command)
{
    if (strncmp(command, "Add", 3) == 0)
    {
        return ADD;
    }
    else if (strncmp(command, "Edit", 4) == 0)
    {
        return EDIT;
    }
    else if (strncmp(command, "Show", 4) == 0)
    {
        return SHOW;
    }
    else if (strncmp(command, "Delete", 6) == 0)
    {
        return DELETE;
    }
    else if (strncmp(command, "Quit", 4) == 0)
    {
        return QUIT;
    }
    else
    {
        return INVALID;
    }
}

// void getTitleFromAdd(char *command, char *out_title)
// {
//     // Tìm vị trí của <title> trong chuỗi command
//     char *title_start = strstr(command, "<title>");

//     // Nếu không tìm thấy, đặt out_title thành chuỗi rỗng
//     if (title_start == NULL)
//     {
//         strcpy(out_title, "");
//         return;
//     }

//     // Di chuyển con trỏ đến vị trí bắt đầu của tiêu đề và tìm vị trí kết thúc
//     title_start += strlen("<title>");
//     char *title_end = strstr(title_start, "</title>");

//     // Nếu không tìm thấy kết thúc, đặt out_title thành chuỗi rỗng
//     if (title_end == NULL)
//     {
//         strcpy(out_title, "");
//         return;
//     }

//     // Sao chép tiêu đề vào out_title
//     strncpy(out_title, title_start, title_end - title_start);
//     // Kết thúc chuỗi out_title
//     out_title[title_end - title_start] = '\0';
// }

// void getDescriptionFromAdd(char *command, char *out_description)
// {
//     // Tìm vị trí của <description> trong chuỗi command
//     char *desc_start = strstr(command, "<description>");

//     // Nếu không tìm thấy, đặt out_description thành chuỗi rỗng
//     if (desc_start == NULL)
//     {
//         strcpy(out_description, "");
//         return;
//     }

//     // Di chuyển con trỏ đến vị trí bắt đầu của mô tả và tìm vị trí kết thúc
//     desc_start += strlen("<description>");
//     char *desc_end = strstr(desc_start, "</description>");

//     // Nếu không tìm thấy kết thúc, đặt out_description thành chuỗi rỗng
//     if (desc_end == NULL)
//     {
//         strcpy(out_description, "");
//         return;
//     }

//     // Sao chép mô tả vào out_description
//     strncpy(out_description, desc_start, desc_end - desc_start);
//     // Kết thúc chuỗi out_description
//     out_description[desc_end - desc_start] = '\0';
// }

// void getTimeFromAdd(char *command, char *out_time)
// {
//     // Tìm vị trí của <time> trong chuỗi command
//     char *time_start = strstr(command, "<time>");

//     // Nếu không tìm thấy, đặt out_time thành chuỗi rỗng
//     if (time_start == NULL)
//     {
//         strcpy(out_time, "");
//         return;
//     }

//     // Di chuyển con trỏ đến vị trí bắt đầu của thời gian và tìm vị trí kết thúc
//     time_start += strlen("<time>");
//     char *time_end = strstr(time_start, "</time>");

//     // Nếu không tìm thấy kết thúc, đặt out_time thành chuỗi rỗng
//     if (time_end == NULL)
//     {
//         strcpy(out_time, "");
//         return;
//     }

//     // Sao chép thời gian vào out_time
//     strncpy(out_time, time_start, time_end - time_start);
//     // Kết thúc chuỗi out_time
//     out_time[time_end - time_start] = '\0';
// }


int main()
{
    struct Task tasks[MAX_TASKS];
    int numTasks = 0;

    char command[100];

    // Vòng lặp chính của ứng dụng
    while (true)
    {
        printf("Enter a command: ");
        fgets(command, sizeof(command), stdin);

        // Loại bỏ ký tự xuống dòng từ chuỗi command
        command[strcspn(command, "\n")] = 0;

        // Phân tích lệnh và thực hiện các hàm tương ứng
        CommandType cmdType = parseCommand(command);

        switch (cmdType)
        {
        case ADD:
            // Thực hiện lệnh Add
            // Ví dụ: Add [Title] [Description] [Time]
            addTask(tasks, &numTasks, "Title", "Description", "Time");
            break;
        case EDIT:
            // Thực hiện lệnh Edit
            // Ví dụ: Edit #1 title:[New Title]
            // ...
            break;
        case SHOW:
            // Thực hiện lệnh Show
            // Ví dụ: Show all
            for (size_t i = 0; i < numTasks; i++)
            {
                printTask(&tasks[i]);
            }
            
            // ...
            break;
        case DELETE:
            // Thực hiện lệnh Delete
            // Ví dụ: Delete #1
            // ...
            break;
        case QUIT:
            // Thoát khỏi ứng dụng
            exit(0);
        default:
            break;
        }
    }

    return 0;
}