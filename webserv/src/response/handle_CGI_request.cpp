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

template <typename x>
static x    min(const x &a, const x &b)
{
    return a < b ? a : b;
}

void set_env(Connection& connection, std::vector<std::string>& env) {\
	std::string	body;

	env.push_back("REQUEST_METHOD=" + connection.getRequest().getMethod());
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	env.push_back("SERVER_NAME=" + connection.getRequest().getHost());
	env.push_back("SERVER_PORT=" + connection.getRequest().getPort());
	env.push_back("HTTPS=off");
	env.push_back("QUERY_STRING=" + connection.getRequest().getQuery());
	if (connection.getRequest().getMethod() == "POST") {
		body = connection.getRequest().getBody();
		if (body.substr(0, 8) == "content=")
			body.erase(0, 8);
		connection.getRequest().setBody(body);
		env.push_back("CONTENT_LENGTH=" + size_to_string(body.size()));
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
	std::string			fileType;

	path = connection.getRequest().getPath();
	fileType = connection.getRequest().getFileType();
	if (fileType == ".java") {
		cgi_path = path;
		av.push_back(const_cast<char*>("/bin/java"));
		av.push_back(const_cast<char*>("-cp"));
		av.push_back(const_cast<char*>(cgi_path.c_str()));
		av.push_back(const_cast<char*>("CGI"));
	} else if (fileType == ".cpp") {
		cgi_path = path + "RPN";
		av.push_back(const_cast<char*>(cgi_path.c_str()));
	} else if (fileType == ".py") {
		if (path.find(".py") == std::string::npos) {
			cgi_path = path + "main.py";
		} else {
			cgi_path = path;
		}
		av.push_back(const_cast<char*>("/usr/bin/python3"));
		av.push_back(const_cast<char*>(cgi_path.c_str()));
	} else if (fileType == ".c") {
		cgi_path = path + "calculator";
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

int parse_cgi_output_remake(Connection& connection) {
	std::string::size_type	end_pos;
	std::string::size_type	colon_pos;
	std::string				header;
	std::string				key;
	std::string				value;
	std::string::size_type	space_pos;
	int						code;
	std::string				output;

	output = connection.get_CgiOutput();
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
	// if (connection.getRequest().getKeepAlive() == "keep-alive")
	connection.getResponse().setHeader("Connection", "close");
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	return 0;
}

int parent_reap_output_remake(Connection& connection) {
	std::string	body;
	int			fdin;
	int			fdout;
	long		written;
	size_t		to_write;
	char		buffer[FILE_CHUNK_SIZE];
	long		n;
	int			status;

	status = 0;
	fdin = connection.getFDIN();
	if (connection.getOperation() == Out) {
		body = connection.getRequest().getBody();
		fdout = connection.getFDOUT();
		to_write = min((size_t)FILE_WRITE_SIZE, body.size());
		written = write(fdout, body.c_str(), to_write);
		if (written > 0) {
			connection.getRequest().removeBody(0, written);
			return 0;
		}
		if (connection.getRequest().getBody().empty()) {
			close(fdout);
			connection.setFDOUT(-1);
			connection.setOperation(In);
			return 0;
		}
		if (written < 0) {
			close(fdout);
			close(fdin);
			kill(connection.getPid(), SIGTERM); 
			connection.setFDOUT(-1);
			connection.setFDIN(-1);
			connection.setOperation(No);
			error_response(connection, INTERNAL_ERROR);
			return -1;
		}
	}
	if (connection.getOperation() == In) {
		if (connection.getOperation() != In)
			return 0;
		n = read(fdin, buffer, sizeof(buffer));
		if (n > 0) {
			connection.appendCGIoutput(buffer, n);
			return 0;
		}
		if (n == 0) {
			close(fdin);
			connection.setFDIN(-1);
			connection.setState(MAKING_RESPONSE);
			connection.setOperation(No);
			waitpid(connection.getPid(), &status, WNOHANG);
			return parse_cgi_output_remake(connection);
		}
		if (n < 0) {
			pid_t result = waitpid(connection.getPid(), &status, WNOHANG);
			if (result > 0) {
				close(fdin);
				connection.setFDIN(-1);
				connection.setState(MAKING_RESPONSE);
				connection.setOperation(No);
				return parse_cgi_output_remake(connection);
			}
		}
	}
	return 0;
}

int CGI_timeout(Connection& connection) {
	std::time_t	now;
	pid_t		pid;
	
	now = std::time(NULL);
	pid = connection.getPid();
	if (pid == -1)
		return 0;
	else if (now - connection.get_CgiTime() > TIMEOUT_SECONDS) {
		kill(pid, SIGKILL);
		waitpid(pid, NULL, 0);
		connection.setPid(-1);
		close(connection.getFDIN());
		close(connection.getFDOUT());
		connection.setOperation(No);
		error_response(connection, GATEWAY_TIMEOUT);
		// error_response(connection, FORBIDDEN);
		return -1;
	}
	return 0;
}

int CGI_handler_remake(Connection& connection) {
	if (connection.getState() == IO_OPERATION) {
		parent_reap_output_remake(connection);
	}
	if (connection.getState() == MAKING_RESPONSE) {
		return parse_cgi_output_remake(connection);
	}
	return 0;
}