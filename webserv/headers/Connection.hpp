/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 04:52:38 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/23 11:48:47 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"
#include "RequestServer.hpp"

enum State {
	READING_METHOD = 1,
	READING_PATH = 2,
	READING_HTTPVERSION = 3,
	READING_HEADERS = 4,
	READING_BODY = 5,
	READING_CHUNKED = 6,
	CONTINUE_READ = 7,

	MAKING_RESPONSE = 8,
	SENDING_RESPONSE = 9,
	
	WAITING_REQUEST = 10
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

	public:
		std::string		buffer;

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

		Connection&		setState(const State state);
		Connection&		setServer(const RequestServer& server);
		Connection&		setChunkedSize(const long size);
		Connection&		setOffset(const long size);
		Connection&		setReconnect(const bool reconnect);
		Connection&		setClose(const bool close);
		Connection&		setPort(const std::string port);
		Connection&		setIP(const std::string ip);

		Connection&		minusOffset(long size);
		Connection&		plusOffset(long size);
		Connection&		clear();
};

#endif