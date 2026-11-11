#include "get_next_line.h"

char *read_line(int fd, char *stash)
{
    char *buffer;
    char *temp;
    int bytes_read;

    buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
    if (!buffer)
        return (NULL);
    bytes_read = 1;
    while (bytes_read > 0 && !ft_strchr(stash, '\n'))
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read < 0)
        {
            free(buffer);
            free(stash);
            return (NULL);
        }
        buffer[bytes_read] = '\0';
        temp = ft_strjoin(stash, buffer);
        free(stash);
        stash = temp;
    }
    free(buffer);
    return (stash);
}

char *get_next_line(int fd)
{
    static char *stash;
    char        *line;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return NULL;
    stash = read_line(fd, stash);
    if (!stash)
        return (NULL);
    
}