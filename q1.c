#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // Library that refers to file control
#include <unistd.h> // Define the constants
#include <string.h>
#include <sys/stat.h> // Basically defines user permissions

typedef long long element_type;
typedef char *stringg;
element_type const MAX_SIZE = 10000000;          // Basically that's the chunk size .. we divide the content into checks
element_type check_argument(element_type no_arg) //  check arguments check if arguments are sufficient
{
    if (no_arg < 2)
    {
        stringg error_is = "NO OF ARGUMENTS ARE INVALID\n";
        write(1, error_is, strlen(error_is));
        return 0;
    }
    return 1;
}
element_type check_descripter(element_type input, element_type output) // CHECK if file descripters are accurate or not
{                                                                      // if file descipter are negative then invalid
    if (input < 0 || output < 0)
    {
        perror("Sorry, an error occured\n");
        return 0;
    }
    return 1;
}
stringg absolute_name(stringg input_name) // we have to keep final file name as 1_filename .. where file name is after last this
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
    if (index == -1) // we already have been given correct name
    {
        return input_name;
    }
    element_type length_name = strlen(input_name) - index;  // basically that's the length of name of file
    answer = (stringg)malloc((length_name) * sizeof(char)); // allocating memory for final answer
    for (long long j = 0; j < length_name; j++)
    {
        answer[j] = input_name[j + index + 1];
    }

    return answer;
}
// function to reverse the string then each element is replaced by i and length-i;
void reverse_string(stringg intial, element_type length, stringg final)
{
    element_type actual_length = length - 1;
    for (element_type i = actual_length; i >= 0; i--)
    {
        final[i] = intial[length - i - 1];
    }
    if (length < MAX_SIZE)
    {
        final[length] = '\0';
    }
    // thats for terminating the string
}
// Basically it prints the progress of the file reversed
void print_progress(float amount_completed)
{
    char loading[35]; // it stores the percentage
    if (amount_completed >= 0.9999)
    {
        sprintf(loading, "\r....................  100%%"); // basically modulo 100
    }
    else
    {
        float percentage = 100 * amount_completed;
        sprintf(loading, "\r.................... %.2f%%", percentage);
    }
    write(1, loading, strlen(loading));
}

int main(element_type argc, stringg argv[])
{
    element_type check_arg = check_argument(argc);
    if (check_arg)
    {
        stringg directory = "Assignment";                        // name of the directory to be created
        mkdir(directory, S_IRUSR | S_IRWXG | S_IWUSR | S_IXUSR); // mkdir: make directory and giving permission lie read/write/execute
        stringg input_file_name = argv[1];
        // input file name passed as argument 1
        stringg output_file_name = absolute_name(input_file_name);
        // output file named as 1_input_file_name.
        stringg final_output_file_name = malloc(sizeof(char) * (12 + strlen(output_file_name)));
        sprintf(final_output_file_name, "Assignment/1_%s", output_file_name);

        // now we will have file descripters
        element_type input_descripter = open(input_file_name, O_RDONLY, 0);
        // file1 opened only for read only (input file opened for read only)
        element_type output_descripter = open(final_output_file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        // opening output file only for writing / truncating
        element_type check_desc = check_descripter(input_descripter, output_descripter);
        // checking if file descripters are correct
        if (check_desc)
        {
            stringg actual_buffer = (stringg)malloc(sizeof(char) * (MAX_SIZE));
            // stores the actual file content without inversion
            stringg reverse_buffer = (stringg)malloc(sizeof(char) * (MAX_SIZE));
            // stores the reversed content
            element_type end_file_bytes = lseek(input_descripter, 0, SEEK_END);
            // end _file_byte stores total bytes of the file for accounting the progress of file
            element_type offset_end = end_file_bytes; // kept for track like the progress of the file
            element_type starting = 1;
            element_type count = 0;
            for (element_type i = 0; starting > 0; i++)
            {
                element_type bytes_read = read(input_descripter, actual_buffer, MAX_SIZE); // bytes read equal to CHUNK_SIZE while loop run
                count += bytes_read;                                                       // needed to calculate progress of the file
                offset_end = lseek(input_descripter, -1 * bytes_read, SEEK_CUR);           // points to the last read byte
                starting = lseek(input_descripter, -1 * MAX_SIZE, SEEK_CUR);               // starting goes to behind to see what maximum can be read

                reverse_string(actual_buffer, bytes_read, reverse_buffer); // reverse string reverse the actual buffer and store the answer in reverse buffer
                write(output_descripter, reverse_buffer, bytes_read);      // writes the reversed buffer onto new file
                print_progress((double)count / end_file_bytes);            // fractional progress sent
            }
            // basically it reads all those bytes that couln't be read in chunk_size they have to be read seperately
            lseek(input_descripter, 0, SEEK_SET);
            element_type bytes_read = read(input_descripter, actual_buffer, offset_end);
            reverse_string(actual_buffer, bytes_read, reverse_buffer);
            write(output_descripter, reverse_buffer, bytes_read);
            print_progress(1); // prints 100% progress  directly
            write(1, "\n", 1);
            close(input_descripter);
            close(output_descripter);
            free(actual_buffer);
            free(reverse_buffer);
            free(final_output_file_name);
        }
        else
        {
            perror("File descripters can't be accessed\n");
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
