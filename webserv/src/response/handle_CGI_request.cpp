/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_CGI_request.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:29:51 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/07 12:29:51 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "Config.hpp"
#include "request_handler.hpp"
#include "support_file.hpp"

void set_env(Connection& connection, std::vector<std::string>& env) {
	env.push_back("REQUEST_METHOD=" + connection.getRequest().getMethod());
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	env.push_back("SERVER_NAME=" + connection.getRequest().getHost());
	env.push_back("SERVER_PORT=" + connection.getRequest().getPort());
	env.push_back("HTTPS=off");
	env.push_back("QUERY_STRING=" + connection.getRequest().getQuery());
	if (connection.getRequest().getMethod() == "POST") {
		env.push_back("CONTENT_LENGTH=" + size_to_string(connection.getRequest().getContentLength()));
		env.push_back("CONTENT_TYPE=" + connection.getRequest().getContentType());
	}
	// HTTP_COOKIE	bonus?
	// PATH_INFO	Maybe?
}

char **build_env(std::vector<std::string>& env, std::vector<char*>& pointer) {
	for (size_t i = 0; i < env.size(); ++i) {
		pointer.push_back(const_cast<char*>(env[i].c_str()));
	}
	pointer.push_back(NULL);
	return &pointer[0];
}

void print_env(char **env) {
	size_t	i;

	i = 0;
	while (env[i] != NULL) {
		std::cout << env[i] << std::endl;
		i++;
	}
}

void child_launch_CGI(Connection& connection, int in[2], int out[2], char **env) {
	std::vector<char*>	av;
	std::string			path;
	std::string			cgi_path;

//	std::cout << "CGI launch\n";
	path = connection.getRequest().getPath();
	if (connection.getRequest().getFileType() == ".java") {
		cgi_path = path;
		av.push_back(const_cast<char*>("/bin/java"));
		av.push_back(const_cast<char*>("-cp"));
		av.push_back(const_cast<char*>(cgi_path.c_str()));
		av.push_back(const_cast<char*>("CGI"));
	} else if (connection.getRequest().getFileType() == ".cpp") {
		cgi_path = path + "RPN";
		av.push_back(const_cast<char*>(cgi_path.c_str()));
	} else if (connection.getRequest().getFileType() == ".py") {
		cgi_path = path + "main.py";
		av.push_back(const_cast<char*>("/usr/bin/python3"));
		av.push_back(const_cast<char*>(cgi_path.c_str()));
	} else {
		exit(-1);
	}

	av.push_back(NULL);
	dup2(in[0], STDIN_FILENO);
	close(in[0]);
	close(in[1]);
	dup2(out[1], STDOUT_FILENO);
	close(out[1]);
	close(out[0]);
	execve(av[0], &av[0], env);
	exit(-1);
}

// stuffs to do
// update pollfd here
// put a timeout here
int parent_reap_output(Connection& connection, int in[2], int out[2], std::string& output) {
	std::string	body;
	char		buffer[4096];
	long		n;
	size_t		total;
	long		written;

	close(in[0]);
	close(out[1]);
	if (connection.getRequest().getMethod() == "POST") {
		total = 0;
		body = connection.getRequest().getBody();
		while (total < body.size()) {
			// Blocking here
			written = write(in[1], body.c_str(), body.size() - total);
			if (written < 0) {
				close(in[1]);
				error_response(connection, INTERNAL_ERROR);
				return -1;
			}
			if (written == 0)
				break;
			total += written;
		}
	}
	close(in[1]);
	while (true) {
		// Blocking here
		n = read(out[0], buffer, sizeof(buffer));
		if (n > 0)
			output.append(buffer, n);
		if (n == 0) {
			break;
		}
		if (n < 0) {
			close(out[0]);
			error_response(connection, INTERNAL_ERROR);
			return -1;
		}
	}
	close(out[0]);
	return 0;
}

int parse_cgi_output(Connection& connection, std::string& output) {
	std::string::size_type	end_pos;
	std::string::size_type	colon_pos;
	std::string				header;
	std::string				key;
	std::string				value;
	std::string::size_type	space_pos;
	int						code;

	while (true) {
		end_pos = output.find("\r\n");
		if (end_pos == std::string::npos) {
			error_response(connection, INTERNAL_ERROR);
			return -1;
		}
		if (end_pos == 0) {
			output.erase(0, 2);
			break;
		}
		header = output.substr(0, end_pos);
		colon_pos = header.find(":");
		if (colon_pos == std::string::npos) {
			error_response(connection, INTERNAL_ERROR);
			return -1;
		}
		key = output.substr(0, colon_pos);
		size_t start = colon_pos + 1;
		while (start < header.size() && (header[start] == ' ' || header[start] == '\t'))
			start++;
		value = header.substr(start);
		if (key == "Content-Type") {
			connection.getResponse().setHeader("Content-Type", value);
			connection.getResponse().setContentType(value);
		}
		if (key == "Content-Length") {
			connection.getResponse().setHeader("Content-Length", value);
			connection.getResponse().setContentLength(std::atoi(value.c_str()));
		}
		if (key == "Status") {
			space_pos = value.find(" ");
			if (space_pos == std::string::npos) {
				code = std::atoi(value.c_str());
			} else {
				code = std::atoi(value.substr(0, space_pos).c_str());
			}
			connection.getResponse().setCode(code);
			connection.getResponse().setCodeMessage(error_message(code));
		}
		output.erase(0, end_pos + 2);
	}
	if (output.empty()) {
		error_response(connection, INTERNAL_ERROR);
		return -1;
	}
	connection.getResponse().setBody(output);
	if (connection.getResponse().getCode() == -1) {
		code = 200;
		connection.getResponse().setCode(code);
		connection.getResponse().setCodeMessage(error_message(code));
	}
	if (connection.getResponse().getContentLength() == 0)
		connection.getResponse().setHeader("Content-Length", size_to_string(output.size()));
	if (connection.getResponse().getContentType().empty())
		connection.getResponse().setHeader("Content-Type", "text/plain");
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	return 0;
}

// stuffs to do
int CGI_handler(Connection& connection) {
	std::vector<std::string>	env_string;
	std::vector<char*>			env_pointer;
	char						**env;
	pid_t						pid;
	int							in[2];
	int							out[2];
	std::string					output;
	int							status;
	
	if (connection.getRequest().getMethod() == "DELETE") {
		error_response(connection, METHOD_NOT_ALLOWED);
		return -1;
	}
	set_env(connection, env_string);
	env = build_env(env_string, env_pointer);
	if (pipe(in) < 0) { 
		error_response(connection, INTERNAL_ERROR);
		return -1;
	}
	if (pipe(out) < 0) {
		close(in[0]);
		close(in[1]);
		error_response(connection, INTERNAL_ERROR);
		return -1;
	}
	status = 0;
	pid = fork();
	switch (pid) {
		case -1:
			error_response(connection, INTERNAL_ERROR);
			return -1;
		case 0:
			child_launch_CGI(connection, in, out, env);
			break ;
		default:
			parent_reap_output(connection, in, out, output);
			// timeout here
			waitpid(pid, &status, 0);
			return parse_cgi_output(connection, output);
	}
	return 0;
}
