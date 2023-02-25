#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

# define FAIL (1)
# define SUCCESS (0)

typedef struct s_back
{
	int		height;
	int		width;
	char	sym;
	char	*back;
}				t_back;

typedef struct s_circle
{
	float	x;
	float	y;
	float	r;
	char	sym;
	char	circle;
}				t_circle;
int	fill_background(FILE *fp, t_back *back)
{
	int	i;
	int	count;

	count = fscanf(fp, "%d %d %c\n", &back->width, &back->height, &back->sym); // caution
	if (count != 3)
		return (FAIL);
	back->back = (char *)malloc(sizeof(char) * back->width * back->height + 1);
	if (back->back == NULL)
		return (FAIL);
	i = 0;
	while (i < back->height * back->width)
	{
		back->back[i] = back->sym;
		i++;
	}
	back->back[i] = '\0';
	return (SUCCESS);
}

int	ft_check(t_circle *c, int x, int y)
{
	float	dist;

	dist = sqrtf(powf(x - c->x, 2) + powf(y - c->y, 2));
	if (dist <= c->r)
	{
		if (c->r - dist < 1)
			return (2);
		return (0);
	}
	return (FAIL);
}

void	ft_get_draw(t_back *back, t_circle *c)
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
			ret = ft_check(c, j, i);
			if (ret == 2 || (ret == 0 && c->circle == 'C'))
				back->back[j + i * back->width] = c->sym;
			j++;
		}
		i++;
	}
}

int	ft_circle(FILE *fp, t_back *back)
{
	t_circle	c;
	int			count;

	count = fscanf(fp, "%c %f %f %f %c\n", &c.circle, &c.x, &c.y, &c.r, &c.sym);
	while (count == 5)
	{
		if (c.r <= 0 || (c.circle != 'c' && c.circle != 'C')) // caution
			return (FAIL);
		ft_get_draw(back, &c);
		count = fscanf(fp, "%c %f %f %f %c\n", &c.circle, &c.x, &c.y, &c.r, &c.sym);
	}
	if (count != -1)
		return (FAIL);
	return (SUCCESS);
}

int	mini_paint(FILE *fp, t_back *back, char *filename)
{
	int	i;

	fp = fopen(filename, "r");
	if (fp == NULL)
		return (FAIL);
	if (fill_background(fp, back) == FAIL)
		return (FAIL);
	if (ft_circle(fp, back) == FAIL)
		return (FAIL);
	i = 0;
	while (i < back->width * back->height)
	{
		write(1, &back->back[i], back->width); // caution
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
		if (mini_paint(fp, &back, av[1]) == FAIL)
		{
			write(1, "Error: Operation file corrupted\n", 32);
			free(back.back);
			fclose(fp);
			return (1);
		}
	}
	else 
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	free(back.back);
	fclose(fp);
	return (0);
}
