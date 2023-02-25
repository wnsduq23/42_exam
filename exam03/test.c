#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

# define FAIL 1

typedef struct s_back
{
	int		width;
	int		height;
	char	sym;
	char	*back;
}			t_back;

typedef struct s_circle
{
	char	circle;
	float	x;
	float	y;
	float	r;
	char	sym;
}			t_circle;

static int	ft_back(FILE *fp, t_back *back)
{
	int i;
	int	count;

	count = fscanf(fp, "%d %d %c\n", &back->width, &back->height, &back->sym);// caution!
	if (count != 3)// caution!
		return (FAIL);
	if (back->width <= 0 || back->width > 300 || back->height <= 0 || back->height > 300)// caution!
		return (FAIL);
	back->back = (char *)malloc(sizeof(char) * (back->width * back->height) + 1);
	if (back->back == NULL)
		return (FAIL);
	i = 0;
	while (i < back->height * back->width)
	{
		back->back[i] = back->sym;
		i++;
	}
	back->back[i] = '\0'; // caution!
	return (0);
}

static int	ft_check(t_circle *c, int x, int y)
{
	float	dist; // caution !

	dist = sqrtf(powf(x - c->x, 2) + powf(y - c->y, 2));
	if (dist <= c->r)
	{
		if (c->r - dist < 1) // caution !
			return (2);
		return (0);
	}
	return (FAIL);
}

static void	ft_get_draw(t_back *back, t_circle *c)
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
				back->back[i * back->width + j] = c->sym;
			j++;
		}
		i++;
	}
}

static int	ft_circle(FILE *fp, t_back *back)
{
	int			count;
	t_circle	c;

	count = fscanf(fp, "%c %f %f %f %c\n", &c.circle, &c.x, &c.y, &c.r, &c.sym); // have to add \n ? 
	while (count == 5)
	{
		if (c.r <= 0 || (c.circle != 'c' && c.circle != 'C'))
			return (FAIL);
		ft_get_draw(back, &c);
		count = fscanf(fp, "%c %f %f %f %c\n", &c.circle, &c.x, &c.y, &c.r, &c.sym);
	}
	if (count != -1) // caution !
		return (FAIL);
	return (0);
}

static int	mini_paint(FILE *fp, t_back *back, char *filename)
{
	int	i;

	fp = fopen(filename, "r");
	if (fp == NULL)
		return (1);
	if (ft_back(fp, back) == FAIL)
		return (FAIL);
	if (ft_circle(fp, back) == FAIL)
		return (FAIL);
	i = 0;
	while (back->back[i] != '\0')
	{
		write(1, &back->back[i], back->width); // caution!
		write(1, "\n", 1);
		i += back->width;
	}	
	return (0);
}

int	main(int ac, char **av)
{
	FILE *fp;
	t_back back;

	fp = NULL;
	if (ac == 2)
	{
		if (mini_paint(fp, &back, av[1]) == FAIL)
		{
			write(1, "Error : Operation\n", 18);
			free(back.back);
			fclose(fp);
			return (1);
		}
	}
	else {
		write(1, "Error : argument\n", 16);
		return (1);
	}
	free(back.back);
	fclose(fp);
	return (0);
}
