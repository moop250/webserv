/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 04:52:38 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/01 17:21:45 by hoannguy         ###   ########.fr       */
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

	public:
		std::string		buffer;

		Connection();
		~Connection();

		State			getState() const;
		Request&		getRequest();
		Response&		getResponse();
		RequestServer	getServer() const;
		long			getChunkedSize() const;
		long			getOffset() const;

		Connection&		setState(State state);
		Connection&		setServer(RequestServer& server);
		Connection&		setChunkedSize(long size);
		Connection&		setOffset(long size);

		Connection&		minusOffset(long size);
};

#endif