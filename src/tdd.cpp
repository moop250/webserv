/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tdd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:59:13 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/21 21:26:50 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CATCH_CONFIG_MAIN
#include <string>
#include "catch_amalgamated.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "request_handler.hpp"
#include "support_file.hpp"
#include "Config.hpp"

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
	Config			config("../configFiles/goodConfigs/simple.config", dfile);
	Connection		connection;
	connection.body_bytes_read = 0;
	connection.state = READING_METHOD;
	connection.buffer = "GET /wtfwtf?user=Nguyen&school=42 HTTP/1.1\r\nHost: localhost:8002\r\n\r\n";
	int code = parse_method(connection);
	REQUIRE(code == READING_PATH);
	REQUIRE(connection.buffer == "/wtfwtf?user=Nguyen&school=42 HTTP/1.1\r\nHost: localhost:8002\r\n\r\n");
	REQUIRE(connection.request.getMethod() == "GET");
	code = parse_URL(connection, config);
	REQUIRE(code == READING_HTTPVERSION);
	REQUIRE(connection.buffer == "HTTP/1.1\r\nHost: localhost:8002\r\n\r\n");
	REQUIRE(connection.request.getPath() == "/wtfwtf");
	REQUIRE(connection.request.getQuery() == "user=Nguyen&school=42");
	code = parse_http_ver(connection);
	REQUIRE(code == READING_HEADERS);
	REQUIRE(connection.buffer == "Host: localhost:8002\r\n\r\n");
	REQUIRE(connection.request.getHttpVersion() == "HTTP/1.1");
	
}

TEST_CASE("Parse method", "[Error]") {
	int				fd_client = 10;
	char			**env;
	Debug			dfile;
	Config			config("../configFiles/goodConfigs/simple.config", dfile);
	Connection		connection;
	connection.body_bytes_read = 0;
	connection.state = READING_METHOD;
	SECTION("Method not complete", "[Error]") {
		connection.buffer = "GE";
		int code = parse_method(connection);
		REQUIRE(code == READING_METHOD);
		REQUIRE(connection.buffer == "GE");
		REQUIRE(connection.request.getMethod() == "");
	}
	SECTION("Not implemented method", "[Error]") {
		connection.buffer = "CONNECT / HTTP/1.1\r\n";
		int code = parse_method(connection);
		REQUIRE(code == NOT_IMPLEMENTED);
		REQUIRE(connection.buffer == "CONNECT / HTTP/1.1\r\n");
		REQUIRE(connection.request.getMethod() == "");
	}
	SECTION("Random string", "[Error]") {
		connection.buffer = "FASDHFGJK / HTTP/1.1\r\n";
		int code = parse_method(connection);
		REQUIRE(code == BAD_REQUEST);
		REQUIRE(connection.buffer == "FASDHFGJK / HTTP/1.1\r\n");
		REQUIRE(connection.request.getMethod() == "");
	}
}

TEST_CASE("Parse path", "[Error]") {
	int				fd_client = 10;
	char			**env;
	Debug			dfile;
	Config			config("../configFiles/goodConfigs/simple.config", dfile);
	Connection		connection;
	connection.body_bytes_read = 0;
	connection.state = READING_METHOD;
	SECTION("Path not complete", "[Error]") {
		connection.buffer = "GET /wtfw";
		parse_method(connection);
		int code = parse_URL(connection, config);
		REQUIRE(code == READING_PATH);
		REQUIRE(connection.buffer == "/wtfw");
		REQUIRE(connection.request.getPath() == "");
	}
}

TEST_CASE("Parse http version", "[Error]") {
	int				fd_client = 10;
	char			**env;
	Debug			dfile;
	Config			config("../configFiles/goodConfigs/simple.config", dfile);
	Connection		connection;
	connection.body_bytes_read = 0;
	connection.state = READING_METHOD;
	SECTION("HTTP version not complete", "[Error]") {
		connection.buffer = "GET /wtfwtf?user=Nguyen&school=42 HTTP/1.";
		parse_method(connection);
		parse_URL(connection, config);
		int code = parse_http_ver(connection);
		REQUIRE(code == READING_HTTPVERSION);
		REQUIRE(connection.buffer == "HTTP/1.");
		REQUIRE(connection.request.getHttpVersion() == "");
	}
	SECTION("HTTP version mismatch", "[Error]") {
		connection.buffer = "GET /wtfwtf?user=Nguyen&school=42 HTTP/1.0\r\n";
		parse_method(connection);
		parse_URL(connection, config);
		int code = parse_http_ver(connection);
		REQUIRE(code == HTTP_VERSION_MISMATCH);
		REQUIRE(connection.buffer == "HTTP/1.0\r\n");
		REQUIRE(connection.request.getHttpVersion() == "");
	}
	
}