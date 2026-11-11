#include "get_next_line.h"

size_t ft_strlen(const char *s)
{
    size_t len;
    
    if (!s)
        return (0);
    len = 0;
    while (s[len])
        len++;
    return (len);
}

char *ft_strchr(const char *s, int c)
{
    if (!s)
        return (NULL);
    while (*s)
    {
        if (*s == (char)c)
            return (char *)s;
        s++;
    }
    if (c == '\0')
        return (char *)s;
    return (NULL);
}

char *ft_strdup(const char *s1)
{
    size_t len;
    char *dup;
    size_t i;

    if (!s1)
        return (NULL);
    len = ft_strlen(s1);
    dup = (char *)malloc(len + 1);
    if (!dup)
        return (NULL);
    i = 0;
    while (i < len)
    {
        dup[i] = s1[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}

