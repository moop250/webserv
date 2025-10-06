/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 11:33:24 by hoannguy          #+#    #+#             */
/*   Updated: 2025/10/06 11:33:24 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 30000

void send_error(char *message) {
	int len = strlen(message);

	printf("Content-Type: text/plain\r\n");
	printf("Status: 200 OK\r\n");
	printf("Content-Length: %d\r\n\r\n", len);
	printf("%s", message);
}

void send_response(char* input, char *body) {
	int	b_len = strlen(body);
	int	i_len = strlen(input);

	printf("Content-Type: text/html\r\n");
	printf("Status: 200 OK\r\n");
	printf("Content-Length: %d\r\n\r\n", 124 + b_len + i_len);
	printf("<!DOCTYPE html>\n");
	printf("<html><head><title>Result</title></head><body>\n");
	printf("<h1>Result</h1>\n");
	printf("<p>Before: %s</p>\n", input);
	printf("<p>After: %s</p>\n", body);
	printf("</body></html>");
}

char *get_input(char *input) {
	int		len;
	char	*tmp;
	int		j;

	len = strlen(input);
	tmp = malloc(sizeof(char) * len);
	j = 0;
	if (tmp == NULL) {
		send_error("Error: Malloc");
		return NULL;
	}
	for (int i = 0; i < len; i++) {
		if (input[i] == '%' && i + 2 < len) {
			char h1 = input[i + 1];
			char h2 = input[i + 2];
			if (h1 == '2' && h2 == '0')
				tmp[j] = ' ';
			else if ((h1 == '2' && h2 == 'A') || (h1 == '2' && h2 == 'a'))
				tmp[j] = '*';
			else if ((h1 == '2' && h2 == 'B') || (h1 == '2' && h2 == 'b'))
				tmp[j] = '+';
			else if ((h1 == '2' && h2 == 'F') || (h1 == '2' && h2 == 'f'))
				tmp[j] = '/';
			else {
				tmp[j] = '%';
				j++;
				continue;
			}
			i += 2;
			j++;
			continue;
		}
		if (input[i] == '+') {
			input[i] = ' ';
		}
		tmp[j] = input[i];
		j++;
	}
	return tmp;
}

char* read_post(void) {
	char	*line;
	
	line = malloc(sizeof(char) * MAX_LENGTH);
	if (line == NULL) {
		send_error("Error: Malloc");
		return NULL;
	}
	fgets(line, MAX_LENGTH, stdin);
	return line;
}

int simple_calculator(char *input) {
	int		len;
	int		result;
	char	**split;

	len = strlen(input);
	for (int i = 0; i < len; i++) {
		if (!(input[i] == '+' || input[i] == '-' || input[i] == '*'
			|| input[i] == '/' || input[i] == ' ' || isdigit(input[i])))
		{
			send_error("Error: Bad input");
			return -1;
		}
	}
	result = 0;
	printf("input: %s\n", input);
	return 0;
}

int main(int ac, char **av, char **env) {
	char*	method;
	char*	input;
	int		code;

	code = 0;
	method = getenv("REQUEST_METHOD");
	if (method == NULL) {
		send_error("Error: No request method");
		return -1;
	}
	for (int i = 0; method[i] != '\0' ; i++){
		method[i] = tolower(method[i]);
	}
	if (strcmp(method, "get") == 0) {
		input = getenv("QUERY_STRING");
		input = get_input(input);
		if (input == NULL)
			return -1;
		code = simple_calculator(input);
	} else if (strcmp(method, "post") == 0) {
		input = read_post();
		if (input == NULL)
			return -1;
		code = simple_calculator(input);
	} else {
		send_error("Error: Bad request method");
		return -1;
	}
	free(input);
	return code;
}