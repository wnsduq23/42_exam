#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <unistd.h> // write
#include <math.h>	// powf, sqrtf	

# define FAIL 1

typedef struct s_back
{
	int		width;
	int		height;
	char	symbol; // what kinda background
	char	*back;
}	t_back;

typedef struct s_circle
{
	char	circle; // ?
	float	x;
	float	y;
	float	r; // ?
	char	symbol;
}			t_circle;

static int	ft_back(FILE *fp, t_back *back)
{
	int	count;
	int	i;

	count = fscanf(fp, "%d %d %c\n", &back->width, &back->height, &back->symbol);
	if (count != 3)
		return (FAIL);
	if (back->width <= 0 || back->width > 300 \
			|| back->height <= 0 || back->height > 300)
		return (FAIL);
	back->back = (char *)malloc(sizeof(char) * back->width * back->height + 1); // \0 causes + 1
	if (back->back == NULL)
		return (FAIL);
	i = 0;
	while (i < back->width * back->height)
	{
		back->back[i] = back->symbol;
		i++;
	}
	back->back[i] = '\0';
	return (0);
}

static int	ft_check(t_circle *circle, float x, float y)
{
	float	dist;

	dist = sqrtf(powf(x - circle->x, 2) + powf(y - circle->y, 2));
	if (dist <= circle->r)
	{
		if (circle->r - dist < 1)
			return (2); // ? 
		return (0);
	}
	return (1);
}

static void	ft_get_draw(t_back *back, t_circle *circle)
{
	int	i;
	int	j;
	int	ret;

	i = 0;
	while(i < back->width)
	{
		j = 0;
		while (j < back->width)
		{
			ret = ft_check(circle, j, i);
			if ((ret == 2) || (ret == 0 && circle->circle == 'C'))
				back->back[i * back->width + j] = circle->symbol;
			j++;
		}
		i++;
	}
}

static int	ft_circle(FILE *fp, t_back *back)
{
	int			count;
	t_circle	circle;

	count = fscanf(fp, "%c %f %f %f %c\n",
			&circle.circle, &circle.x, &circle.y, &circle.r, &circle.symbol);
	while (count == 5)
	{
		if (circle.r <= 0 || (circle.circle != 'c' && circle.circle != 'C'))
			return (FAIL);
		ft_get_draw(back, &circle);
		count = fscanf(fp, "%c %f %f %f %c\n", 
				&circle.circle, &circle.x, &circle.y, &circle.r, &circle.symbol);
	}
	if (count != -1)
		return (1);
	return (0);
}

static int	mini_paint(FILE *fp, t_back *back, char *filename)
{
	int	i;

	fp = fopen(filename, "r");
	if (fp == NULL)
		return (FAIL);
	if (ft_back(fp, back) == FAIL)
		return (FAIL);
	if (ft_circle(fp, back) == FAIL)
		return (FAIL);
	i = 0;
	while (back->back[i] != '\0')
	{
		write(1, &back->back[i], back->width);
		write(1, "\n", 1);
		i = i + back->width;
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
			write(1, "Error : Operation file corrupted\n", 32);
			free(back.back);
			fclose(fp);
			return (1);
		}
	}
	else
	{
		write(1, "Error : argument\n", 16);
		return (1);
	}

	free(back.back);
	fclose(fp);
	return (0);
}

/* receive filename as argument
* open the file
* 
*/
