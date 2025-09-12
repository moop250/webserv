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

void set_env(Connection& connection, std::vector<std::string>& env, std::string cgi_path) {
	env.push_back("REQUEST_METHOD=" + connection.getRequest().getMethod());
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	env.push_back("SCRIPT_NAME=" + cgi_path);
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

// stuffs to do
void child_launch_CGI(Connection& connection, std::string& cgi_path, int in[2], int out[2], char **env) {
	std::vector<char*>	av;

	// to remake
	if (connection.getRequest().getFileType() == ".java") {
		cgi_path = "./cgi";
		av.push_back(const_cast<char*>("/bin/java"));
		av.push_back(const_cast<char*>("-cp"));
		av.push_back(const_cast<char*>(cgi_path.c_str()));
		av.push_back(const_cast<char*>("CGI"));
	} else {
		av.push_back(const_cast<char*>(cgi_path.c_str()));
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

// stuffs to do
int parse_cgi_output(Connection& connection, std::string& output) {

	(void)connection;
	(void)output;
	return 0;
}

// stuffs to do
int CGI_handler(Connection& connection) {
	std::string					cgi_path;
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

	// fix cgi_path for actual path, currently sending the file type (eg: .java)
	cgi_path = connection.getRequest().getFileType();

	set_env(connection, env_string, cgi_path);
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
			child_launch_CGI(connection, cgi_path, in, out, env);
			break ;
		default:
			parent_reap_output(connection, in, out, output);
			// std::cout << "CGI output: \n" << output << std::endl;
			waitpid(pid, &status, 0);
			parse_cgi_output(connection, output);
	}
	return 0;
}

// somehow pass fd to fdpoll to keep track of blocking. Need to work this flow out
// parse result to check for headers
// set headers and response body

// Pass fd to poll?
// Put CGI_FDin and CGI_FDout in Connection object.
// update poll fd table with those 2 fd.
// Pass poll fd table around?
// Put poll fd table in global?
// Hugo runs CGI? Create a new state, check after running handle_request, if set then run CGI handler with poll fd