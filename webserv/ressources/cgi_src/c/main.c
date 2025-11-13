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
#include "tinyexpr.h"

#define MAX_LENGTH 30000

void send_error(char *message, char *input) {
	int m_len = strlen(message);
	int	i_len = strlen(input);

	printf("Content-Type: text/plain\r\n");
	printf("Status: 200 OK\r\n");
	printf("Content-Length: %d\r\n\r\n", 8 + m_len + i_len);
	printf("%s\n", message);
	printf("Input: %s", input);
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
	fflush(stdout);
}

char *get_input(char *input) {
	int		len;
	char	*tmp;
	int		j;

	len = strlen(input);
	tmp = malloc(sizeof(char) * (len + 1));
	j = 0;
	if (tmp == NULL) {
		send_error("Error: Malloc", "error");
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
			else if ((h1 == '2' && h2 == 'D') || (h1 == '2' && h2 == 'd'))
				tmp[j] = '-';
			else if ((h1 == '3' && h2 == 'D') || (h1 == '3' && h2 == 'd'))
				tmp[j] = '=';
			else if ((h1 == '2' && h2 == '8'))
				tmp[j] = '(';
			else if ((h1 == '2' && h2 == '9'))
				tmp[j] = ')';
			else if ((h1 == '2' && h2 == 'E') || (h1 == '2' && h2 == 'e'))
				tmp[j] = '.';
			else if ((h1 == '5' && h2 == 'E') || (h1 == '5' && h2 == 'e'))
				tmp[j] = '^';
			else if ((h1 == '2' && h2 == '5') || (h1 == '2' && h2 == '5'))
				tmp[j] = '%';
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
	tmp[j] = '\0'; 
	return tmp;
}

char* read_post(void) {
	char	*line;
	char	*content_len;
	int		len;
	
	content_len = getenv("CONTENT_LENGTH");
	if (content_len == NULL) {
		send_error("Error: Invalid CONTENT_LENGTH", "error");
		return NULL;
	}
	len = atoi(content_len);
	if (len <= 0) {
		send_error("Error: atoi error", "error");
		return NULL;
	}
	line = malloc(sizeof(char) * (len + 1));
	if (line == NULL) {
		send_error("Error: Malloc", "error");
		return NULL;
	}
	fread(line, 1, len, stdin);
	line[len] = '\0';
	return line;
}

// tinyexpr pour math
// https://github.com/codeplea/tinyexpr?tab=readme-ov-file
int simple_calculator(char *input) {
	double	result;
	char	*buffer;
	int		error;

	// test
	// sqrt(5^2+7^2+11^2+(8-2)^2) -> sqrt%285%5E2%2B7%5E2%2B11%5E2%2B%288-2%29%5E2%29
	// cos(5) -> cos%285%29
	// support: tinyexpr.c line 162
	result = te_interp(input, &error);
	if (error != 0) {
		send_error("Error: Bad Input", input);
		return -1;
	}
	buffer = malloc(64);
	if (buffer == NULL) {
		send_error("Error: Malloc", "error");
		return -1;
	}
	sprintf(buffer, "%f", result);
	send_response(input, buffer);
	free(buffer);
	return 0;
}

char *remove_name(char *input) {
	int	i;

	i = 0;
	while (input[i] != '\0') {
		if (input[i] == '=') {
			input = &input[i + 1];
			break;
		}
		i++;
	}
	return input;
}

int main(void) {
	char*	method;
	char*	input;
	int		code;

	code = 0;
	while(1) {}
	method = getenv("REQUEST_METHOD");
	if (method == NULL) {
		send_error("Error: No request method", "error");
		return -1;
	}
	for (int i = 0; method[i] != '\0' ; i++){
		method[i] = tolower(method[i]);
	}
	if (strcmp(method, "get") == 0) {
		input = getenv("QUERY_STRING");
		if (input == NULL) {
			send_error("Error: No query string", "error");
			return -1;
		}
		input = remove_name(input);
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
		send_error("Error: Bad request method", "error");
		return -1;
	}
	free(input);
	return code;
}