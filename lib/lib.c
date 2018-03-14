#include <string.h>

/*
 *convert string to hex
 *
 */
unsigned int atohex(const char *str)
{
	unsigned int num = 0;
	int len = strlen(str);
	int i = 0;
	if ((str[0] == '0' && str[1] == 'x') || (str[0] == '0' && str[1] == 'X'))
		i += 2;
	for (; i < len; i++) {
		if (str[i] <= '9')
			num += ((str[i] - '0' +  0) << ((len - i - 1) * 4));
		else if (str[i] <= 'Z')
			num += ((str[i] - 'A' + 10) << ((len - i - 1) * 4));
		else
			num += ((str[i] - 'a' + 10) << ((len - i - 1) * 4));
	}

	return num;
}
