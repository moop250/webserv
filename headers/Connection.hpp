/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 04:52:38 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/27 06:38:47 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"

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
		const int		fd_client;
		State			state;
		Request			request;
		Response		response;
		t_ServerData	server;
		long			chunked_size;

	public:
		std::string		buffer;

		Connection(int fd_client);
		~Connection();

		int				getFdClient() const;
		State			getState() const;
		Request&		getRequest();
		Response&		getResponse();
		t_ServerData	getServer() const;
		long			getChunkedSize() const;

		Connection&		setBuffer(std::string& buffer);
		Connection&		setState(State state);
		Connection&		setServer(t_ServerData& server);
		Connection&		setChunkedSize(long size);
};

#endif