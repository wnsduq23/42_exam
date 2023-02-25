#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_back
{
	int		height;
	int		width;
	char	sym;
	char	*back;
}	t_back;

typedef struct s_shape
{
	float	x;
	float	y;
	float	height;
	float	width;
	char	sym;
	char	shape;
}t_shape;

int	ft_back(FILE *fp, t_back *back)
{
	int	i;
	int	count;

	count = fscanf(fp, "%d %d %c\n", &back->width, &back->height, &back->sym);
	if (count != 3)
		return (1);
	if (back->width < 0 || back->height < 0 || back->width > 300 || back->height > 300)
		return (1);
	back->back = (char *)malloc(sizeof(char) * (back->width * back->height) + 1);
	if (back->back == NULL)
		return (1);
	i = 0;
	while (i < back->width * back->height)
	{
		back->back[i] = back->sym;
		i++;
	}
	back->back[i] = '\0';
	return (0);
}

int	ft_check(t_shape *s, float x, float y)
{
	if (x < s->x || x > s->x + s->width \
			|| y < s->y || y > s->y + s->height)
		return (1);
	if (x - s->x < 1 || s->width + s->x - x < 1 \
			|| y - s->y < 1 || s->y + s->height - y < 1)
		return (2);
	return (0);

}

void	ft_get_draw(t_back *back, t_shape *s)
{
	int	i;
	int	j;
	int	ret;

	i = 0;
	while (i < back->height)
	{
		j = 0;
		while (j < back->width)
		{
			ret = ft_check(s, j, i);
			if (ret == 2 || (ret == 0 && s->shape == 'R'))
				back->back[j + i * back->width] = s->sym;
			j++;
		}
		i++;
	}
}

int	ft_shape(FILE *fp, t_back *back)
{
	t_shape	s;
	int	count;

	count = fscanf(fp, "%c %f %f %f %f %c\n", &s.shape, &s.x, &s.y, &s.width, &s.height, &s.sym);
	while (count == 6)
	{
		if (s.width <= 0 || s.height <= 0 || (s.sym != 'r' && s.sym != 'R'))
			return (1);
		ft_get_draw(back, &s);
		count = fscanf(fp, "%c %f %f %f %f %c\n", &s.shape, &s.x, &s.y, &s.width, &s.height, &s.sym);
	}
	if (count != -1)
		return (1);
	return (0);
}
int	micro_paint(FILE *fp, char *filename, t_back *back)
{
	int	i;

	fp = fopen(filename, "r");
	if (fp == NULL)
		return (1);
	if (ft_back(fp, back) == 1)
		return (1);
	if (ft_shape(fp, back) == 1)
		return (1);
	i = 0;
	while (back->back[i] != '\0')
	{
		write(1, &back->back[i], back->width);
		write(1, "\n", 1);
		i += back->width;
	}
	return (0);
}

int	main(int ac, char **av)
{
	FILE	*fp;
	t_back	back;

	fp = NULL;
	if (ac == 2)
	{
		if (micro_paint(fp, av[1], &back) == 1)
		{
			// 
			free(back.back);
			fclose(fp);
		}
	}
	else {
		return (1);
	}
	free(back.back);
	fclose(fp);
	return (0);
}
