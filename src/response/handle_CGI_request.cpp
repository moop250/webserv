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

// stuffs to do
void set_env(Connection& connection, std::vector<std::string>& env /*, std::string cgi_path*/) {
	env.push_back("REQUEST_METHOD=" + connection.getRequest().getMethod());
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	// env.push_back("SCRIPT_NAME=" + cgi_path);
	env.push_back("SERVER_NAME=" + connection.getRequest().getHost());
	env.push_back("SERVER_PORT=" + connection.getRequest().getPort());
	env.push_back("HTTPS=off");
	env.push_back("QUERY_STRING=" + connection.getRequest().getQuery());
	if (connection.getRequest().getMethod() == "POST") {
		env.push_back("CONTENT_LENGTH=" + size_to_string(connection.getRequest().getContentLength()));
		env.push_back("CONTENT_TYPE=" + connection.getRequest().getContentType());
	}
	// HTTP_COOKIE
	// PATH_INFO			??? This was mentioned in the subjet
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
int CGI_handler(Connection& connection) {
	std::vector<std::string>	env_string;
	std::vector<char*>			env_pointer;
	char						**env;

	// std::cout << connection.getRequest().getFileType() << std::endl;
	// get path to cgi for type connection.getRequest().getFileType()
	set_env(connection, env_string);
	env = build_env(env_string, env_pointer);
	print_env(env);

	// set signal to wait for fork children. SIGCHLD/waitpid
	// fork
	// pipe
	// somehow pass fd to fdpoll to keep track of blocking. Need to work this flow out
	// execve
	// if POST -> feed from stdin, content-length size
	// get result from stdout
	// parse result to check for headers
	// set headers and response body
	return 0;
}