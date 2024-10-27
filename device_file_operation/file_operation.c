#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/mychar0"
#define BUFFER_SIZE 1024

int main() {
    int fd;
    char write_buffer[BUFFER_SIZE] = "Hello from user space!";
    char read_buffer[BUFFER_SIZE];

    // Open the device file
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device file");
        return -1;
    }

    // Write to the device file
    if (write(fd, write_buffer, strlen(write_buffer)) < 0) {
        perror("Failed to write to the device file");
        close(fd);
        return -1;
    }
    printf("Data written to the device: %s\n", write_buffer);

    // Read from the device file
  /*  if (lseek(fd, 0, SEEK_SET) < 0) {
        perror("Failed to reset the read pointer");
        close(fd);
        return -1;
    }*/
    if (read(fd, read_buffer, BUFFER_SIZE) < 0) {
        perror("Failed to read from the device file");
        close(fd);
        return -1;
    }
    printf("Data read from the device: %s\n", read_buffer);

    close(fd);
    return 0;
}
