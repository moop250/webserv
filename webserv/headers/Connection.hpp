/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 04:52:38 by hoannguy          #+#    #+#             */
/*   Updated: 2025/11/12 18:06:35 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"
#include "RequestServer.hpp"

#define FILE_CHUNK_SIZE 65536
#define FILE_WRITE_SIZE 8192
#define TIMEOUT_SECONDS 5

enum State {
	READING_METHOD = 1,
	READING_PATH = 2,
	READING_HTTPVERSION = 3,
	READING_HEADERS = 4,
	READING_BODY = 5,
	READING_CHUNKED = 6,
	CONTINUE_READ = 7,

	MAKING_RESPONSE = 8,
	IO_OPERATION = 9,
	SENDING_RESPONSE = 10,
	
	WAITING_REQUEST = 11
};

enum Operation {
	In,
	Out,
	No
};

class Connection {
	private:
		State			state;
		Request			request;
		Response		response;
		RequestServer	server;
		long			chunked_size;
		long			offset;
		bool			reconnect;
		bool			close;
		std::string		port;
		std::string		ip;
		int				fdin;
		int				fdout;
		int				first;
		int				requestType;
		Operation		operation;
		pid_t			pid;
		std::string		cgi_output;
		std::time_t		cgi_time;

	public:
		std::string		buffer;
		std::string		iobuffer;

		Connection();
		Connection(const Connection& copy);
		Connection& operator =(const Connection& assign);
		~Connection();

		State			getState() const;
		Request&		getRequest();
		Response&		getResponse();
		RequestServer	getServer() const;
		RequestServer	getDefaultServer() const;
		long			getChunkedSize() const;
		long			getOffset() const;
		bool			getReconnect() const;
		bool			getClose() const;
		std::string		getPort() const;
		std::string		getIP() const;
		int				getFDIN() const;
		int				getFDOUT() const;
		int				getFirst() const;
		int				getRequestType() const;
		Operation		getOperation() const;
		pid_t			getPid() const;
		std::string		get_CgiOutput() const;
		std::time_t		get_CgiTime() const;

		Connection&		setState(const State state);
		Connection&		setServer(const RequestServer& server);
		Connection&		setChunkedSize(const long size);
		Connection&		setOffset(const long size);
		Connection&		setReconnect(const bool reconnect);
		Connection&		setClose(const bool close);
		Connection&		setPort(const std::string port);
		Connection&		setIP(const std::string ip);
		Connection&		setFDIN(const int fdin);
		Connection&		setFDOUT(const int fdout);
		Connection&		setFirst(const int first);
		Connection&		setRequestType(const int requestType);
		Connection&		setOperation(const Operation operation);
		Connection&		setPid(const pid_t pid);
		Connection&		appendCGIoutput(const char *buffer, long size);
		Connection&		setCGItime(const std::time_t time);

		Connection&		setCGIoutput(const std::string cgioutput);
		Connection&		minusOffset(long size);
		Connection&		plusOffset(long size);
		Connection&		clear();
};

#endif