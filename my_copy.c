#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_SIZE 4096

/* Writes error messages to stderr */
void write_error(const char *msg) {
    write(STDERR_FILENO, msg, strlen(msg));
}

int main(int argc, char *argv[]) {
    int src_fd, dst_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    char answer;

    /* Validate arguments */
    if (argc != 3) {
        write_error("Usage: ./copy_my <source_file> <destination_file>\n");
        return 1;
    }

    /* Open source file */
    src_fd = open(argv[1], O_RDONLY);
    if (src_fd < 0) {
        write_error("Error: Cannot open source file\n");
        return 1;
    }

    /* Check if destination file exists */
    if (access(argv[2], F_OK) == 0) {
        const char *msg =
            "Destination file already exists. Overwrite? (y/n): ";
        write(STDOUT_FILENO, msg, strlen(msg));

        while (1) {
            if (read(STDIN_FILENO, &answer, 1) <= 0)
                continue;

            if (answer == 'n') {
                write(STDOUT_FILENO,
                      "Copy canceled by user.\n",
                      23);
                close(src_fd);
                return 0;
            } else if (answer == 'y') {
                break;
            } else {
                write(STDOUT_FILENO,
                      "\nPlease enter y or n: ",
                      23);
            }
        }
    }

    /* Open destination file */
    dst_fd = open(argv[2],
                  O_WRONLY | O_CREAT | O_TRUNC,
                  0644);
    if (dst_fd < 0) {
        write_error("Error: Cannot open destination file\n");
        close(src_fd);
        return 1;
    }

    /* Copy loop */
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            write_error("Error: Write failed\n");
            close(src_fd);
            close(dst_fd);
            return 1;
        }
    }

    if (bytes_read < 0) {
        write_error("Error: Read failed\n");
    }

    /* Close files */
    close(src_fd);
    close(dst_fd);

    return 0;
}
