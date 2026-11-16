*This project has been created as part of the 42 curriculum by callwood.*

# get_next_line

## Description

**get_next_line** is a 42 school programming project whose goal is to implement a C function capable of reading and returning a single line at a time from a file descriptor (`fd`). Calling the function in a loop allows reading an entire text stream (such as a file or standard input) line by line until EOF (End Of File) is reached.

The project introduces the concept of **static variables** in C, dynamic memory management across successive function calls, and buffered I/O handling via the `read()` system call. The bonus part extends this functionality to handle **multiple file descriptors simultaneously** using a single static variable.

---

## Algorithm Explanation and Justification

The implementation relies on a **persistent static stash (remainder buffer) combined with chunked buffered reading**.

### How It Works
1. **Buffered Reading (`read_line`)**:
   - A temporary heap buffer of size `BUFFER_SIZE + 1` is allocated.
   - Data is read in chunks of `BUFFER_SIZE` bytes using `read(fd, buffer, BUFFER_SIZE)` and concatenated onto a persistent `static char *stash` until a newline character (`'\n'`) is found in the stash or `read()` returns `0` (EOF).
2. **Line Extraction (`set_line`)**:
   - Once a newline or EOF is present in `stash`, the function scans up to the first `'\n'` (included) or `'\0'` and extracts that substring using `ft_substr()`. This newly allocated string is returned to the caller.
3. **State Preservation (`update_stash`)**:
   - Any characters remaining after the extracted `'\n'` are duplicated into a new string using `ft_strdup()`, the old `stash` is freed, and the static pointer is updated to hold the leftover data for the next call. If nothing remains after the newline or EOF is reached, `stash` is freed and set to `NULL`.
4. **Multi-FD Bonus Extension**:
   - Instead of a single `static char *stash`, the bonus version uses a single static array of pointers: `static char *stash[FD_MAX]`. Each open file descriptor indexes its own independent remainder string (`stash[fd]`), allowing interleaved calls across different file descriptors without losing each stream's reading state.

### Justification
- **Minimizes System Calls**: Reading `BUFFER_SIZE` bytes per `read()` call avoids the severe performance overhead of reading 1 byte at a time, while still adapting cleanly to arbitrary `BUFFER_SIZE` values (`1`, `42`, `1024`, `10000000`, etc.).
- **Strict Memory Control**: Every intermediate concatenation frees the previous pointer immediately, and the stash is completely freed as soon as EOF is reached or an error occurs, preventing memory leaks.
- **O(1) Descriptor Lookup (Bonus)**: Indexing `stash[fd]` directly by the integer file descriptor provides constant-time access to each stream's state while respecting the single static variable constraint.

---

## Instructions

### Requirements
- A C compiler (`cc`, `gcc`, or `clang`)
- Standard C library and POSIX `<unistd.h>`

### Mandatory Part (`main.c` & Compilation)

Create a `main.c` file to read a single file descriptor line by line until `EOF`:

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("test.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}
```

Compile and run the mandatory part with a custom `BUFFER_SIZE`:
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
./gnl
```

### Bonus Part — Multi-FD (`main_bonus.c` & Compilation)

Create a `main_bonus.c` file to test reading from multiple file descriptors simultaneously in alternating order:

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line_bonus.h"

int	main(void)
{
	int		fd1;
	int		fd2;
	char	*line1;
	char	*line2;

	fd1 = open("file1.txt", O_RDONLY);
	fd2 = open("file2.txt", O_RDONLY);
	if (fd1 < 0 || fd2 < 0)
		return (1);
	while (1)
	{
		line1 = get_next_line(fd1);
		line2 = get_next_line(fd2);
		if (!line1 && !line2)
			break ;
		if (line1)
		{
			printf("FD1: %s", line1);
			free(line1);
		}
		if (line2)
		{
			printf("FD2: %s", line2);
			free(line2);
		}
	}
	close(fd1);
	close(fd2);
	return (0);
}
```

Compile and run the bonus part:
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c -o gnl_bonus
./gnl_bonus
```

---

## Resources

### Classic References
- [`read(2)` — Linux Programmer's Manual](https://man7.org/linux/man-pages/man2/read.2.html) — Official documentation for the `read()` system call, return values, and file descriptor behavior.
- [`open(2)` — Linux Programmer's Manual](https://man7.org/linux/man-pages/man2/open.2.html) — Reference for file descriptor flags and error handling (`O_RDONLY`, invalid `fd`).
- [Static Variables in C — GeeksforGeeks](https://www.geeksforgeeks.org/static-variables-in-c/) — Documentation on the lifetime, initialization, and persistence of `static` variables across function calls.
- [42 Docs — get_next_line](https://harm-smits.github.io/42docs/projects/get_next_line) — Project guide detailing memory management rules, `BUFFER_SIZE` behavior, and edge cases.

### Use of AI
AI tools were used strictly as a learning and productivity assistant during this project:
- **Conceptual Review:** Clarifying the behavior of `static` pointers across successive function calls and verifying how the `static char *stash[FD_MAX]` array isolates state per file descriptor in the bonus part.
- **Testing & Edge Cases:** Generating test ideas (`BUFFER_SIZE=1`, `BUFFER_SIZE=10000000`, empty files, files without a trailing `\n`, `stdin`, and invalid `fd` values) to validate leak-free execution with Valgrind.
- **Documentation:** Structuring and proofreading this `README.md` file.
- All C source files (`get_next_line.c`, `get_next_line_utils.c`, and their `_bonus` counterparts), memory allocation logic, and debugging were written and carried out manually.