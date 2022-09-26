#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h> // for using st_stat for knowing permission of the file
typedef long long element_type;
typedef char *stringg;
element_type const MAX_SIZE = 10000000;
element_type check_argument(element_type no_arg) // checks if the correct arguments are passed
{
    if (no_arg < 4)
    {
        return 0;
    }
    return 1;
}
// prints the thing on the screen
// void print_screen(stringg S)
// {
//     element_type length = strlen(S);
//     write(1, S, strlen(S));
// }
element_type check_descripter(element_type input, element_type output) // check if the inputted files are correct
{
    if (input < 0 || output < 0)
    {
        perror("Sorry, an error occured\n");
        return 0;
    }
    return 1;
}

void reverse_string(stringg intial, element_type length, stringg final) // for reversing string
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
// void print_progress(float amount_completed)
// {
//     char loading[32];
//     if (amount_completed > 0.9999)
//         sprintf(loading, "\r####################   100%%");
//     else
//     {
//         float percentage = 100 * amount_completed;
//         sprintf(loading, "\r %.2f%%", percentage);
//     }
//     write(1, loading, strlen(loading));
// }
int main(element_type argc, stringg argv[])
{
    // Initially we will take care if user passed the file correctly
    element_type check_arg = check_argument(argc); // Check if arguments are passed correctly
    if (check_arg)
    {
        stringg input_file_name = argv[2];
        stringg output_file_name = argv[1];
        stringg directory_name = argv[3];
        // stringg directory = argv[3];

        // now we will have file descripters
        element_type input_descripter = open(input_file_name, O_RDONLY, 0);   // input file opened for readonly
        element_type output_descripter = open(output_file_name, O_RDONLY, 0); // new file opened for read only
        element_type check_desc = check_descripter(input_descripter, output_descripter);
        if (check_desc)
        {
            stringg actual_buffer = (stringg)malloc(sizeof(char) * (MAX_SIZE));  // actual content store
            stringg reverse_buffer = (stringg)malloc(sizeof(char) * (MAX_SIZE)); // reversed content
            stringg temp_buffer = (stringg)malloc(sizeof(char) * (MAX_SIZE));
            element_type end_file_bytes = lseek(input_descripter, 0, SEEK_END);

            element_type offset_end = end_file_bytes;
            element_type starting = 1;
            element_type count = 0;
            element_type output_file_ptr = lseek(output_descripter, 0, SEEK_SET);
            element_type flag = 0;
            for (element_type i = 0; starting > 0; i++)
            {
                element_type bytes_read = read(input_descripter, actual_buffer, MAX_SIZE);
                count += bytes_read;
                offset_end = lseek(input_descripter, -1 * bytes_read, SEEK_CUR);
                starting = lseek(input_descripter, -1 * MAX_SIZE, SEEK_CUR);
                reverse_string(actual_buffer, bytes_read, reverse_buffer);
                // reverse buffer store content of actual buffer in reversed buffer
                read(output_descripter, temp_buffer, bytes_read);
                //  temp_buffer stores content of file we are to compare with
                if (strcmp(temp_buffer, reverse_buffer) != 0)
                {
                    flag = 1;
                    break;
                }
            }
            struct stat direc;
            stat(directory_name, &direc);
            stringg waah;
            // checks if the directory exists
            if (S_ISDIR(direc.st_mode))
            {
                waah = "Directory is created: Yes\n";
                write(1, waah, strlen(waah));
            }
            else
            {
                waah = "Directory is created: No\n";
                write(1, waah, strlen(waah));
            }
            if (flag == 1)
            {
                stringg writu = "Whether file contents are reversed in newfile: NO\n";
                write(1, writu, strlen(writu));
            }
            else
            {
                lseek(input_descripter, 0, SEEK_SET);
                element_type bytes_rea = read(input_descripter, actual_buffer, offset_end);
                reverse_string(actual_buffer, bytes_rea, reverse_buffer);
                stringg t = (stringg)malloc(sizeof(char) * MAX_SIZE);
                read(output_descripter, t, bytes_rea);
                if (strcmp(t, reverse_buffer) != 0)
                {
                    flag = 1;
                }
                if (flag == 1)
                {

                    stringg writu = "Whether file contents are reversed in newfile: NO\n";
                    write(1, writu, strlen(writu));
                }
                else
                {

                    stringg writu = "Whether file contents are reversed in newfile: YES\n";
                    write(1, writu, strlen(writu));
                }

                close(input_descripter);
                close(output_descripter);
                free(actual_buffer);
                free(reverse_buffer);
                free(temp_buffer);
                // NOW WE HAVE TO CHECK IF THE FILE HAVE DESIRED PERMISSIONS
                // so we have this access named thing to checkthis stuff
                struct stat input_file_data;
                stat(input_file_name, &input_file_data);
                // now we do bitiwise or to check if flag exists
                // stringg waah;
                //............................................................................//
               
                struct stat output_file_data;
                stat(output_file_name, &output_file_data);
                // now we do bitiwise or to check if flag exists
                // stringg change = "NOW FOR OUTPUT FILE\n";
                // write(1, change, strlen(change));
                if (output_file_data.st_mode & S_IRUSR)
                {
                    waah = "User has read permission on newfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "User has read permission on newfile:NO\n";
                    write(1, waah, strlen(waah));
                }

                if (output_file_data.st_mode & S_IWUSR)
                {
                    waah = "User has write permission on newfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "User has write permission on newfile:NO\n";
                    write(1, waah, strlen(waah));
                }
                if (output_file_data.st_mode & S_IXUSR)
                {
                    waah = "User has execute permission on newfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "User has execute permission on newfile:NO\n";
                    write(1, waah, strlen(waah));
                }
                if (output_file_data.st_mode & S_IRGRP)
                {
                    waah = "Group has read permission on newfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Group has read permission on newfile:NO\n";
                    write(1, waah, strlen(waah));
                }

                if (output_file_data.st_mode & S_IWGRP)
                {
                    waah = "Group has write permission on newfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Group has write permission on newfile:NO\n";
                    write(1, waah, strlen(waah));
                }
                if (output_file_data.st_mode & S_IXGRP)
                {
                    waah = "Group has execute permission on newfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Group has execute permission on newfile:NO\n";
                    write(1, waah, strlen(waah));
                }
                if (output_file_data.st_mode & S_IROTH)
                {
                    waah = "Others has read permission on newfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Others has read permission on newfile:NO\n";
                    write(1, waah, strlen(waah));
                }

                if (output_file_data.st_mode & S_IWOTH)
                {
                    waah = "Others has write permission on newfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Others has write permission on newfile:NO\n";
                    write(1, waah, strlen(waah));
                }
                if (output_file_data.st_mode & S_IXOTH)
                {
                    waah = "Others has execute permission on newfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Others has execute permission on newfile:NO\n";
                    write(1, waah, strlen(waah));
                }

                if (direc.st_mode & S_IRUSR)
                {
                    waah = "User has read permission on directory:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "User has read permission on directory:NO\n";
                    write(1, waah, strlen(waah));
                }
                // checks if user has write permission
                if (direc.st_mode & S_IWUSR)
                {
                    waah = "User has write permission on directory:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "User has write permission on directory:NO\n";
                    write(1, waah, strlen(waah));
                }
                // chechk if user has execute permissions
                if (direc.st_mode & S_IXUSR)
                {
                    waah = "User has execute permission on directory:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "User has execute permission on directory:NO\n";
                    write(1, waah, strlen(waah));
                }
                // check if group has read permission
                if (direc.st_mode & S_IRGRP)
                {
                    waah = "Group has read permission on directory:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Group has read permission on directory:NO\n";
                    write(1, waah, strlen(waah));
                }
                // check if group has write permission
                if (direc.st_mode & S_IWGRP)
                {
                    waah = "Group has write permission on directory:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Group has write permission on directory:NO\n";
                    write(1, waah, strlen(waah));
                }
                // check if group has execute permission
                if (direc.st_mode & S_IXGRP)
                {
                    waah = "Group has execute permission on directory:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Group has execute permission on directory:NO\n";
                    write(1, waah, strlen(waah));
                }
                // check if others has read permission
                if (direc.st_mode & S_IROTH)
                {
                    waah = "Others has read permission on directory:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Others has read permission on directory:NO\n";
                    write(1, waah, strlen(waah));
                }

                if (direc.st_mode & S_IWOTH)
                {
                    waah = "Others has write permission on directory:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Others has write permission on directory:NO\n";
                    write(1, waah, strlen(waah));
                }
                if (direc.st_mode & S_IXOTH)
                {
                    waah = "Others has execute permission on directory:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Others has execute permission on directory:NO\n";
                    write(1, waah, strlen(waah));
                }
                 // checks if user has read permission
                if (input_file_data.st_mode & S_IRUSR)
                {
                    waah = "User has read permission on oldfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "User has read permission on oldfile:NO\n";
                    write(1, waah, strlen(waah));
                }
                // checks if user has write permission
                if (input_file_data.st_mode & S_IWUSR)
                {
                    waah = "User has write permission on oldfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "User has write permission on oldfile:NO\n";
                    write(1, waah, strlen(waah));
                }
                // chechk if user has execute permissions
                if (input_file_data.st_mode & S_IXUSR)
                {
                    waah = "User has execute permission on oldfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "User has execute permission on oldfile:NO\n";
                    write(1, waah, strlen(waah));
                }
                // check if group has read permission
                if (input_file_data.st_mode & S_IRGRP)
                {
                    waah = "Group has read permission on oldfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Group has read permission on oldfile:NO\n";
                    write(1, waah, strlen(waah));
                }
                // check if group has write permission
                if (input_file_data.st_mode & S_IWGRP)
                {
                    waah = "Group has write permission on oldfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Group has write permission on oldfile:NO\n";
                    write(1, waah, strlen(waah));
                }
                // check if group has execute permission
                if (input_file_data.st_mode & S_IXGRP)
                {
                    waah = "Group has execute permission on oldfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Group has execute permission on oldfile:NO\n";
                    write(1, waah, strlen(waah));
                }
                // check if others has read permission
                if (input_file_data.st_mode & S_IROTH)
                {
                    waah = "Others has read permission on oldfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Others has read permission on oldfile:NO\n";
                    write(1, waah, strlen(waah));
                }

                if (input_file_data.st_mode & S_IWOTH)
                {
                    waah = "Others has write permission on oldfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Others has write permission on oldfile:NO\n";
                    write(1, waah, strlen(waah));
                }
                if (input_file_data.st_mode & S_IXOTH)
                {
                    waah = "Others has execute permission on oldfile:YES\n";
                    write(1, waah, strlen(waah));
                }
                else
                {
                    waah = "Others has execute permission on oldfile:NO\n";
                    write(1, waah, strlen(waah));
                }
                //............................................................................//
            }
        }
        else
        {
            perror("File descripter couldn't be accessed\n");
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
