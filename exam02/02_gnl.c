#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// if (read fail or (ready_bytes == 0 and size == 0)
// return NULL
// if (read_bytes == 0 or current buffer_char is '\n')
// malloc(size + 2)
char	*res_line(size_t size_n_idx, int fd)
{
	char	*line;
	char	buf_c;
	char	read_bytes;

	line = 0;
	buf_c = 0;
	read_bytes = read(fd, &buf_c, 1);
	//fail or empty
	if (read_bytes < 0 || (read_bytes == 0 && size_n_idx == 0))
		return (0);
	//meet null or line feed
	if (read_bytes == 0 || buf_c == '\n')
		line = malloc(size_n_idx + 2);
	//when meet normal char not \n
	else
		line = res_line(size_n_idx + 1, fd);
	//insert char in line
	if (line)
	{
		//input buffer case
		line[size_n_idx] = buf_c;
		//input exit char (0) when buf_c is \n
		line[size_n_idx + 1] = line[size_n_idx + 1] * (buf_c != '\n');
		return (line);
	}
	return (line);
}

char	*gnl(int fd)
{
	if (fd < 0)
		return (0);
	return (res_line(0, fd));
}

int	main(void)
{
	char	*str;
	int		fd;

	fd = open("./test.txt", O_RDONLY);
	str = gnl(fd);
	printf("%s", str);
}
