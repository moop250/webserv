/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tdd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:59:13 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/25 14:19:35 by hoannguy         ###   ########.fr       */
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
	REQUIRE(connection.request.getKeepAlive() == "");
	REQUIRE(connection.request.getKeepAliveTimeout() == -1);
	REQUIRE(connection.request.getKeepAliveMax() == -1);
	REQUIRE(connection.request.getHost() == "");
	REQUIRE(connection.request.getPort() == -1);
}

TEST_CASE("Parse Unit", "[Success]") {
	int				fd_client = 10;
	char			**env;
	Debug			dfile;
	Config			config("../configFiles/goodConfigs/default.config", dfile);
	Connection		connection;
	connection.chunked_size = -1;
	connection.state = READING_METHOD;
	connection.buffer = "POST /cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
						"Host: localhost:8002\r\n"
						"Connection: Keep-Alive\r\n"
						"Keep-Alive: timeout=5, max=200\r\n"
						"Content-Length: 12\r\n"
						"\r\n"
						"Hello World!";
	int code = parse_method(connection);
	REQUIRE(code == READING_PATH);
	REQUIRE(connection.buffer == "/cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
								 "Host: localhost:8002\r\n"
								 "Connection: Keep-Alive\r\n"
								 "Keep-Alive: timeout=5, max=200\r\n"
								 "Content-Length: 12\r\n"
								 "\r\n"
								 "Hello World!");
	REQUIRE(connection.request.getMethod() == "POST");
	code = parse_URL(connection, config);
	REQUIRE(code == READING_HTTPVERSION);
	REQUIRE(connection.buffer == "HTTP/1.1\r\n"
								 "Host: localhost:8002\r\n"
								 "Connection: Keep-Alive\r\n"
								 "Keep-Alive: timeout=5, max=200\r\n"
								 "Content-Length: 12\r\n"
								 "\r\n"
								 "Hello World!");
	REQUIRE(connection.request.getPath() == "/cgi/test.java");
	REQUIRE(connection.request.getQuery() == "user=Nguyen&school=42");
	code = parse_http_ver(connection);
	REQUIRE(code == READING_HEADERS);
	REQUIRE(connection.buffer == "Host: localhost:8002\r\n"
								 "Connection: Keep-Alive\r\n"
								 "Keep-Alive: timeout=5, max=200\r\n"
								 "Content-Length: 12\r\n"
								 "\r\n"
								 "Hello World!");
	REQUIRE(connection.request.getHttpVersion() == "HTTP/1.1");
	code = parse_headers(connection, config);
	REQUIRE(code == READING_BODY);
	REQUIRE(connection.buffer == "Hello World!");
	REQUIRE(connection.request.getHeader("host") == "localhost:8002");
	REQUIRE(connection.request.getHost() == "localhost");
	REQUIRE(connection.request.getPort() == 8002);
	REQUIRE(connection.request.getKeepAlive() == "keep-alive");
	REQUIRE(connection.request.getKeepAliveTimeout() == 5);
	REQUIRE(connection.request.getKeepAliveMax() == 200);
	REQUIRE(connection.request.getContentLength() == 12);
	code = parse_body(connection);
	REQUIRE(code == READING_COMPLETE);
	REQUIRE(connection.buffer == "");
	REQUIRE(connection.request.getBody() == "Hello World!");
	// REQUIRE(connection.request.getRequestType() == CGI);
	// REQUIRE(connection.request.getCgiType() == ".java");
}

TEST_CASE("Parse complete, content-length", "[Success]") {
	int				fd_client = 10;
	char			**env;
	Debug			dfile;
	Config			config("../configFiles/goodConfigs/default.config", dfile);
	Connection		connection;
	connection.chunked_size = -1;
	connection.state = READING_METHOD;
	connection.buffer = "POST /cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
						"Host: localhost:8002\r\n"
						"Connection: Keep-Alive\r\n"
						"Keep-Alive: timeout=5, max=200\r\n"
						"Content-Length: 12\r\n"
						"\r\n"
						"Hello World!";
	int code = 	parse_request(connection, config, fd_client, env);
	REQUIRE(code == READING_COMPLETE);
	REQUIRE(connection.request.getMethod() == "POST");
	REQUIRE(connection.request.getPath() == "/cgi/test.java");
	REQUIRE(connection.request.getQuery() == "user=Nguyen&school=42");
	REQUIRE(connection.request.getHttpVersion() == "HTTP/1.1");
	REQUIRE(connection.request.getHeader("host") == "localhost:8002");
	REQUIRE(connection.request.getHeader("connection") == "Keep-Alive");
	REQUIRE(connection.request.getHeader("keep-alive") == "timeout=5, max=200");
	REQUIRE(connection.request.getHeader("content-length") == "12");
	REQUIRE(connection.request.getBody() == "Hello World!");
	REQUIRE(connection.request.getHost() == "localhost");
	REQUIRE(connection.request.getPort() == 8002);
	REQUIRE(connection.request.getContentLength() == 12);
	REQUIRE(connection.request.getKeepAlive() == "keep-alive");
	REQUIRE(connection.request.getKeepAliveTimeout() == 5);
	REQUIRE(connection.request.getKeepAliveMax() == 200);
}

TEST_CASE("Parse complete, chunked", "[Success]") {
	int				fd_client = 10;
	char			**env;
	Debug			dfile;
	Config			config("../configFiles/goodConfigs/default.config", dfile);
	Connection		connection;
	connection.chunked_size = -1;
	connection.state = READING_METHOD;
	connection.buffer = "POST /cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
						"Host: localhost:8002\r\n"
						"Connection: Keep-Alive\r\n"
						"Keep-Alive: timeout=5, max=200\r\n"
						"Transfer-Encoding: chunked\r\n"
						"\r\n"
						"D\r\n"
						"Hello World!\n\r\n"
						"20\r\n"
						"This is a greeting from Lausanne\r\n"
						"0\r\n"
						"\r\n";
	int code = 	parse_request(connection, config, fd_client, env);
	REQUIRE(code == READING_COMPLETE);
	REQUIRE(connection.request.getMethod() == "POST");
	REQUIRE(connection.request.getPath() == "/cgi/test.java");
	REQUIRE(connection.request.getQuery() == "user=Nguyen&school=42");
	REQUIRE(connection.request.getHttpVersion() == "HTTP/1.1");
	REQUIRE(connection.request.getHeader("host") == "localhost:8002");
	REQUIRE(connection.request.getHeader("connection") == "Keep-Alive");
	REQUIRE(connection.request.getHeader("keep-alive") == "timeout=5, max=200");
	REQUIRE(connection.request.getHeader("transfer-encoding") == "chunked");
	REQUIRE(connection.request.getBody() == "Hello World!\nThis is a greeting from Lausanne");
	REQUIRE(connection.request.getHost() == "localhost");
	REQUIRE(connection.request.getPort() == 8002);
	REQUIRE(connection.request.getKeepAlive() == "keep-alive");
	REQUIRE(connection.request.getKeepAliveTimeout() == 5);
	REQUIRE(connection.request.getKeepAliveMax() == 200);
}

TEST_CASE("Parse method", "[Error]") {
	int				fd_client = 10;
	char			**env;
	Debug			dfile;
	Config			config("../configFiles/goodConfigs/default.config", dfile);
	Connection		connection;
	connection.chunked_size = -1;
	connection.state = READING_METHOD;
	SECTION("Method not complete", "[Error]") {
		connection.buffer = "GE";
		int code = parse_method(connection);
		REQUIRE(code == CONTINUE_READ);
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
	Config			config("../configFiles/goodConfigs/default.config", dfile);
	Connection		connection;
	connection.chunked_size = -1;
	connection.state = READING_METHOD;
	SECTION("Path not complete", "[Error]") {
		connection.buffer = "GET /wtfw";
		parse_method(connection);
		int code = parse_URL(connection, config);
		REQUIRE(code == CONTINUE_READ);
		REQUIRE(connection.buffer == "/wtfw");
		REQUIRE(connection.request.getPath() == "");
	}
}

TEST_CASE("Parse http version", "[Error]") {
	int				fd_client = 10;
	char			**env;
	Debug			dfile;
	Config			config("../configFiles/goodConfigs/default.config", dfile);
	Connection		connection;
	connection.chunked_size = -1;
	connection.state = READING_METHOD;
	SECTION("HTTP version not complete", "[Error]") {
		connection.buffer = "GET /cgi/test.java?user=Nguyen&school=42 HTTP/1.";
		parse_method(connection);
		parse_URL(connection, config);
		int code = parse_http_ver(connection);
		REQUIRE(code == CONTINUE_READ);
		REQUIRE(connection.buffer == "HTTP/1.");
		REQUIRE(connection.request.getHttpVersion() == "");
	}
	SECTION("HTTP version mismatch", "[Error]") {
		connection.buffer = "GET /cgi/test.java?user=Nguyen&school=42 HTTP/1.0\r\n";
		parse_method(connection);
		parse_URL(connection, config);
		int code = parse_http_ver(connection);
		REQUIRE(code == HTTP_VERSION_MISMATCH);
		REQUIRE(connection.buffer == "HTTP/1.0\r\n");
		REQUIRE(connection.request.getHttpVersion() == "");
	}
	
}

TEST_CASE("Parse headers", "[Error]") {
	int				fd_client = 10;
	char			**env;
	Debug			dfile;
	Config			config("../configFiles/goodConfigs/default.config", dfile);
	Connection		connection;
	connection.chunked_size = -1;
	connection.state = READING_METHOD;
	SECTION("Dupplicate host", "[Error]") {
		connection.buffer = "GET /wtfwtf?user=Nguyen&school=42 HTTP/1.1\r\n"
							"Host: localhost:8002\r\n"
							"Host: localhost:3000\r\n"
							"\r\n";
		parse_method(connection);
		parse_URL(connection, config);
		parse_http_ver(connection);
		int code = parse_headers(connection, config);
		REQUIRE(code == BAD_REQUEST);
	}
	SECTION("No host", "[Error]") {
		connection.buffer = "GET /wtfwtf?user=Nguyen&school=42 HTTP/1.1\r\n"
							"Date: today\r\n"
							"Server: 42 Lausanne webserver\r\n"
							"\r\n";
		parse_method(connection);
		parse_URL(connection, config);
		parse_http_ver(connection);
		int code = parse_headers(connection, config);
		REQUIRE(code == BAD_REQUEST);
	}
	SECTION("Both Content-Length and Transfer-Encoding", "[Error]") {
		connection.buffer = "POST /cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
							"Host: localhost:8002\r\n"
							"Connection: Keep-Alive\r\n"
							"Keep-Alive: timeout=5, max=200\r\n"
							"Transfer-Encoding: chunked\r\n"
							"Content-Length: 48\r\n"
							"\r\n"
							"D\r\n"
							"Hello World!\n\r\n"
							"20\r\n"
							"This is a greeting from Lausanne\r\n"
							"0\r\n"
							"\r\n";
		int code = 	parse_request(connection, config, fd_client, env);
		REQUIRE(code == BAD_REQUEST);
	}
	// SECTION("Method not allowed", "[Error]") {
	// 	connection.buffer = "DELETE /wtfwtf?user=Nguyen&school=42 HTTP/1.1\r\n"
	// 						"Host:localhost:3000\r\n"
	// 						"\r\n";
	// 	parse_method(connection);
	// 	parse_URL(connection, config);
	// 	parse_http_ver(connection);
	// 	int code = parse_headers(connection, config);
	// 	REQUIRE(connection.request.getHost() == "localhost");
	// 	REQUIRE(connection.request.getPort() == 3000);
	// 	REQUIRE(code == METHOD_NOT_ALLOWED);
	// }
}
