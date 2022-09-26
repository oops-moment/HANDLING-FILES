#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

typedef long long element_type;
typedef char *stringg;
element_type const MAX_SIZE = 10000000;           // 1<<18
element_type check_argument(element_type no_arg) // checks if number of arguments are adequate ie 4
{
    if (no_arg < 4)
    {
        return 0;
    }
    return 1;
}
element_type check_descripter(element_type input, element_type output) // checks if given file input are valid
{
    if (input < 0 || output < 0)
    {
        perror("Sorry, an error occured\n");
        return 0;
    }
    return 1;
}
stringg absolute_name(stringg input_name) // correspond to actual name of file
{
    stringg answer;
    element_type index = -1;
    for (element_type i = strlen(input_name); i >= 0; i--)
    {
        if (input_name[i] == '/')
        {
            index = i;
            break;
        }
    }
    // basically actual name of the file start after index
    if (index == -1)
        return input_name;
    element_type length_name = strlen(input_name) - index;
    answer = (stringg)malloc((length_name) * sizeof(char));
    for (long long j = 0; j < length_name; j++)
    {
        answer[j] = input_name[j + index + 1];
    }

    return answer;
}
void reverse_string(stringg intial, element_type length, stringg final) // reverses the string
{
    for (element_type i = length - 1; i >= 0; i--)
    {
        final[i] = intial[length - i - 1];
    }
    if (length < MAX_SIZE)
    {
        final[length] = '\0';
    }
}
void print_progress(float amount_completed) // for printing the progress
{
    char loading[35];
    if (amount_completed > 0.9999)
        sprintf(loading, "\r....................  100%%");
    else
    {
        float percentage = 100 * amount_completed;
        sprintf(loading, "\r.................... %.2f%%", percentage);
    }
    write(1, loading, strlen(loading));
}
int main(element_type argc, stringg argv[])
{
    // Initially we will take care if user passed the file correctly
    element_type check_arg = check_argument(argc);
    if (check_arg)
    {
        stringg directory = "Assignment";
        mkdir(directory, S_IRUSR | S_IRWXG | S_IWUSR | S_IXUSR);
        stringg input_file_name = argv[1]; // takes input of input file
        stringg start_index = argv[2];     // starting index
        stringg end_index = argv[3];       // ending index
        stringg output_file_name = absolute_name(input_file_name);
        stringg final_output_file_name = malloc(sizeof(char) * (15 + strlen(output_file_name)));
        // 15 for Assignement 2
        sprintf(final_output_file_name, "Assignment/2_%s", output_file_name);
        // changes final output file name to Assignment/2..
        // now we will have file descripters
        element_type input_descripter = open(input_file_name, O_RDONLY, 0);
        element_type output_descripter = open(final_output_file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        element_type check_desc = check_descripter(input_descripter, output_descripter);
        if (check_desc)
        {
            stringg actual_buffer = (stringg)malloc(sizeof(char) * (MAX_SIZE));                 // allocating size to actual string
            stringg reverse_buffer = (stringg)malloc(sizeof(char) * (MAX_SIZE));                // allocating size to reversed string
            element_type filesize = lseek(input_descripter, 0, SEEK_END);                       // total file size
            element_type end_file_bytes = lseek(input_descripter, atoi(start_index), SEEK_SET); // first portion to be reversed is considered as file where end bytes are start index
            element_type offset_end = end_file_bytes;
            element_type starting = lseek(input_descripter, end_file_bytes - MAX_SIZE, SEEK_SET);
            element_type count = 0;
            if (offset_end > 0)
            {
                for (element_type i = 0; starting > 0; i++)
                {
                    element_type bytes_read = 0;
                    bytes_read = read(input_descripter, actual_buffer, MAX_SIZE);
                    // printf("bytes read %d", bytes_read);
                    count += bytes_read;
                    offset_end = lseek(input_descripter, -1 * bytes_read, SEEK_CUR);
                    starting = lseek(input_descripter, -1 * MAX_SIZE, SEEK_CUR);
                    reverse_string(actual_buffer, bytes_read, reverse_buffer);
                    //  printf("%s", reverse_buffer);
                    write(output_descripter, reverse_buffer, bytes_read);
                    print_progress((double)count / filesize);
                }
            }
            lseek(input_descripter, 0, SEEK_SET);
            element_type bytes_read = read(input_descripter, actual_buffer, offset_end);
            reverse_string(actual_buffer, bytes_read, reverse_buffer);
            write(output_descripter, reverse_buffer, bytes_read);
            // till here we have inverted bits till start index
            // now we will write rest bits as it is
            end_file_bytes = lseek(input_descripter, atoi(start_index), SEEK_SET);
            offset_end = end_file_bytes;
            // starting for part 2 will be startindex
            starting = atoi(start_index);
            lseek(input_descripter, atoi(start_index), SEEK_SET);
            while ((starting + MAX_SIZE) <= atoi(end_index))
            {
                element_type bytes_read = 0;
                bytes_read = read(input_descripter, actual_buffer, MAX_SIZE);
                // printf("bytes read are %d\n", bytes_read);
                // printf("string is %s\n", actual_buffer);
                count += bytes_read;
                // printf("the value of starting is %d\n", starting);
                starting += bytes_read;
                // printf("the value of starting is %d\n", starting);
                write(output_descripter, actual_buffer, bytes_read);
                print_progress((double)count / filesize);
            }
            lseek(input_descripter, starting, SEEK_SET);
            bytes_read = read(input_descripter, actual_buffer, atoi(end_index) - starting + 1);
            write(output_descripter, actual_buffer, bytes_read);
            // reads the bytes that haven't to be inverted
            // till here we have inverted the first part and written the actual part as it is
            // now we will invert last part
            end_file_bytes = lseek(input_descripter, 0, SEEK_END);
            offset_end = end_file_bytes;
            // starting = atoi(end_index) + 1;
            starting = end_file_bytes - MAX_SIZE;
            // starting for third part will be will be the end index
            for (element_type i = 0; starting > atoi(end_index); i++)
            {
                element_type bytes_read = read(input_descripter, actual_buffer, MAX_SIZE);
                // printf("bytes read are %d\n", bytes_read);
                // printf("string is %s\n",actual_buffer);
                count += bytes_read;
                offset_end = lseek(input_descripter, -1 * bytes_read, SEEK_CUR);
                starting = lseek(input_descripter, -1 * MAX_SIZE, SEEK_CUR);
                reverse_string(actual_buffer, bytes_read, reverse_buffer);
                write(output_descripter, reverse_buffer, bytes_read);
                print_progress((double)count / filesize);
            }
            lseek(input_descripter, atoi(end_index) + 1, SEEK_SET);
            bytes_read = read(input_descripter, actual_buffer, offset_end - atoi(end_index) - 1);
            reverse_string(actual_buffer, bytes_read, reverse_buffer);
            write(output_descripter, reverse_buffer, bytes_read);
            // here it ends

            print_progress(1);

            write(1, "\n", 1);
            close(input_descripter);
            close(output_descripter);
            free(actual_buffer);
            free(reverse_buffer);
            free(final_output_file_name);
        }
        else
        {
            exit(1);
        }
    }
    else
    {
        stringg argument_error = "Number of Arguments not adequate";
        write(2, argument_error, strlen(argument_error));
        exit(1);
    }
    return 0;
}
