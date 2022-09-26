# SYSTEM CALLS
## Prisha 


---

## Requirements

- GCC compiler
- Linux OS (preferably Ubuntu)


## Q1

- gcc q1.c
- ./a.out <input_file>
 ---
- SO, CHUNK_SIZE was taken to reverse file in bytes since reversing all at once consumes both time and memory so concept of CHUNK_SIZE was used , size of which was kept 10000000.

- Secondly ,it wasn't mentioned where "Assignment" folder had to be created so it was assumed to be created in  same folder where codes are.

- If the arguments are not sufficient to work with, then it throws error of less arguments.

- If the file descriptors are negative then also it throws a perror.



## Q2

- gcc q2.c
- ./a.out <file_path> <start_index> <end_index>
---
- SO, CHUNK_SIZE was taken to reverse file in bytes since reversing all at once consumes both time and memory so concept of CHUNK_SIZE was used , size of which was kept 1e6.

- File was handled in three parts each part used CHUNK_SIZES for reversing, writing same and reversing.

- Also, It wasn't mentioned where "Assignment" folder had to be created so it was assumed to be created in  same folder where codes are.

- If the arguments are not sufficient to work with, then it throws error of less arguments.

- If the file descriptors are negative then also it throws a perror.



## Q3

- gcc q3.c
- ./a.out  <new_file> <old_file> <directory_path>
---
- SO, CHUNK_SIZE was taken to reverse file in bytes since reversing all at once consumes both time and memory so concept of CHUNK_SIZE was used , size of which was kept 1e6.

- Since there was no info if we are print progress bar hence it wasn't printed.

- If the arguments are not sufficient to work with, then it throws error of less arguments.

 -  If the file descriptors are negative then also it throws a perror.

 - Permission are printed in the order as 
 - Directory exists or not
 - Content is reversed or not
 - permission with old file 
 - permission with newfile
 - permission with directory
 

