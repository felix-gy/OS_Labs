
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_BUF 1024

int main(int argc, char * argv [])
{
    int processID = atoi(argv[1]);
    int fd;

    char * myfifo = "/tmp/myfifo3";
    char buf[MAX_BUF];

    fd = open(myfifo, O_RDONLY);
    
    for (int counter = 0;counter < 10; counter++)
    {
        read(fd, buf, MAX_BUF);
        printf("%d  Received: %s \n", counter,buf);
        sleep(1);
        kill(processID,2);
    }
    
    close(fd);

    return 0;
}


