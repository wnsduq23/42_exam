#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

int	ft_putstr(char	*str, int len);
void ft_put_dec(long num, int base_len, char *base, int *print_len);
void ft_put_num(unsigned num, unsigned base_len, char *base, int *print_len);
int	ft_putstr(char	*str, int len)
{
	while (str && *str && str[len] && ++len);
	return (str ? write(1, str, len) : write(1, "(null)", 6));
}
void ft_put_dec(long num, int base_len, char *base, int *print_len)
{
	if (num < 0)
	{
		*print_len += write(1, "-", 1);
		ft_put_num(-num, base_len, base, print_len);
	}
	else
		ft_put_num(num, base_len, base, print_len);
}
void ft_put_num(unsigned num, unsigned base_len, char *base, int *print_len)
{
	if (num >= base_len)
		ft_put_num(num / base_len, base_len, base, print_len);
	*print_len += (int)write(1, &base[num % base_len], 1);
}
int	ft_printf(const char *format, ...)
{
	int	print_len = 0;
	va_list	ap;

	va_start(ap, format);
	while (*format)
	{
		if (*format == '%' && *(format + 1) == 's' && (format += 2))
			print_len += (int)ft_putstr(va_arg(ap, char *), 0);
		else if (*format == '%' && *(format + 1) == 'x' && (format += 2))
			ft_put_num(va_arg(ap, int), 16, "0123456789abcdef", &print_len);
		else if (*format == '%' && *(format + 1) == 'd' && (format += 2))
			ft_put_dec(va_arg(ap, int), 10, "0123456789", &print_len);
		else
			print_len += write(1, format++, 1);
	}
	va_end(ap);
	return (print_len);
}

int	main(void)
{
	ft_printf("testing %d\n", 1234);
	ft_printf("testing %x\n", 1234);
	ft_printf("testing %s\n", "1234");
	printf("testing %d\n", 1234);
	printf("testing %x\n", 1234);
	printf("testing %s\n", "1234");
	return (0);
}
