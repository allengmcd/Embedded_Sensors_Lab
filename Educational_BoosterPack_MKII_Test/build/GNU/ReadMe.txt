When adding new files make sure that their location is in the source path 
and includes path in the Makefile. Next, add the name of the source file 
along with its extension in C_SRCS or RTOS_SRCS. In this case the C_SRCS are
what ever is in the test code folder whereas the RTOS_SRCS is what is ever in
the TalRTOS folder. Finally, if there are any new groups add them under 
RTOS_SRCS and then add in the files. Then add that group to C_SRCS under where
RTOS_SRCS is being added to C_SRCS.
