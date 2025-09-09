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
// void child_launch_CGI(Connection& connection, std::string& cgi_path, int in[2], int out[2], char **env) {

// 	if (connection.getRequest().getFileType() == ".java") {
// 		char		*argv[3];
// 		std::string	
// 		argv[0] = 
// 	} else {
// 		argv[0] = (char*)cgi_path.c_str();
// 	}
// 	argv[1] = NULL;
// 	dup2(in[0], STDIN_FILENO);
// 	close(in[0]);
// 	close(in[1]);
// 	dup2(out[1], STDOUT_FILENO);
// 	close(out[1]);
// 	close(out[0]);
// }

// // stuffs to do
// // update pollfd here
// void parent_reap_output(Connection& connection, int in[2], int out[2], std::string output) {
// 	(void)connection;
// 	(void)in;
// 	(void)out;
// 	(void)output;
// }

// stuffs to do
int CGI_handler(Connection& connection) {
	std::string					cgi_path;
	std::vector<std::string>	env_string;
	std::vector<char*>			env_pointer;
	char						**env;
	// pid_t						pid;
	int							in[2];
	int							out[2];
	// std::string					output;
	// int							status;
	
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
	(void)env;
	// pid = fork();
	// switch (pid) {
	// 	case -1:
	// 		error_response(connection, INTERNAL_ERROR);
	// 		return -1;
	// 	case 0:
	// 		child_launch_CGI(cgi_path, in, out, env);
	// 		break ;
	// 	default:
	// 		parent_reap_output(connection, in, out, output);
	// }
	// waitpid(pid, &status, 0);
	return 0;
}

	// somehow pass fd to fdpoll to keep track of blocking. Need to work this flow out
	// execve
	// if POST -> feed from stdin, content-length size
	// get result from stdout
	// parse result to check for headers
	// set headers and response body

// Pass fd to poll?
// Put CGI_FDin and CGI_FDout in Connection object.
// update poll fd table with those 2 fd.
// Pass poll fd table around?
// Put poll fd table in global?
// Hugo runs CGI? Create a new state, check after running handle_request, if set then run CGI handler with poll fd