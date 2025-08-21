/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tdd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:59:13 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/21 18:19:12 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CATCH_CONFIG_MAIN
#include <string>
#include "catch_amalgamated.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "request_handler.hpp"
#include "support_file.hpp"

// ------------------- Request class Default ---------------------

TEST_CASE("Request class default constructor") {
	Connection	connection;

    REQUIRE(connection.request.getMethod() == "");
	REQUIRE(connection.request.getPath() == "");
	REQUIRE(connection.request.getQuery() == "");
	REQUIRE(connection.request.getHttpVersion() == "");
	REQUIRE(connection.request.getHeader("nothing") == "");
	REQUIRE(connection.request.getCookie("Cookie1") == "");
	REQUIRE(connection.request.getCgiType() == "");
	REQUIRE(connection.request.getRequestType() == None);
	REQUIRE(connection.request.getBody() == "");
	REQUIRE(connection.request.getContentLength() == 0);
	REQUIRE(connection.request.getContentType() == "");
	REQUIRE(connection.request.getKeepAlive() == false);
}

TEST_CASE("Parse", "[Success]") {
	int				fd_client = 10;
	char			**env;
	Debug			dfile;
	Config			config("../../configFiles/goodConfigs/simple.config", dfile);
	Connection		connection;
	connection.body_bytes_read = 0;
	SECTION("Parse method", "[Success]") {
		connection.state = READING_METHOD;
		connection.buffer = "GET /wtfwtf HTTP/1.1\r\nHost: localhost:8002\r\n\r\n";
		int code = parse_method(connection);
	}
}