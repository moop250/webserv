/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tdd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:59:13 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/02 22:49:17 by hoannguy         ###   ########.fr       */
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

    REQUIRE(connection.getRequest().getMethod() == "");
	REQUIRE(connection.getRequest().getPath() == "");
	REQUIRE(connection.getRequest().getQuery() == "");
	REQUIRE(connection.getRequest().getHttpVersion() == "");
	REQUIRE(connection.getRequest().getHeader("nothing") == "");
	REQUIRE(connection.getRequest().getCookie("Cookie1") == "");
	REQUIRE(connection.getRequest().getFileType() == "");
	REQUIRE(connection.getRequest().getRequestType() == None);
	REQUIRE(connection.getRequest().getBody() == "");
	REQUIRE(connection.getRequest().getContentLength() == 0);
	REQUIRE(connection.getRequest().getContentType() == "");
	REQUIRE(connection.getRequest().getKeepAlive() == "");
	REQUIRE(connection.getRequest().getKeepAliveTimeout() == -1);
	REQUIRE(connection.getRequest().getKeepAliveMax() == -1);
	REQUIRE(connection.getRequest().getHost() == "");
	REQUIRE(connection.getRequest().getPort() == "");
}

TEST_CASE("Parse Unit", "[Success]") {
	char			**env;
	Debug			dfile;
	Config	*config = new Config("../configFiles/goodConfigs/default.config", dfile);
	config->parseContent();
	config->sanitize();
	Connection		connection;
	connection.buffer = "POST /cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
						"Host: localhost1:8001\r\n"
						"Connection: Keep-Alive\r\n"
						"Keep-Alive: timeout=5, max=200\r\n"
						"Content-Length: 12\r\n"
						"\r\n"
						"Hello World!";
	int code = parse_method(connection);
	REQUIRE(code == READING_PATH);
	REQUIRE(connection.buffer == "/cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
								 "Host: localhost1:8001\r\n"
								 "Connection: Keep-Alive\r\n"
								 "Keep-Alive: timeout=5, max=200\r\n"
								 "Content-Length: 12\r\n"
								 "\r\n"
								 "Hello World!");
	REQUIRE(connection.getRequest().getMethod() == "POST");
	code = parse_URL(connection, *config);
	REQUIRE(code == READING_HTTPVERSION);
	REQUIRE(connection.buffer == "HTTP/1.1\r\n"
								 "Host: localhost1:8001\r\n"
								 "Connection: Keep-Alive\r\n"
								 "Keep-Alive: timeout=5, max=200\r\n"
								 "Content-Length: 12\r\n"
								 "\r\n"
								 "Hello World!");
	REQUIRE(connection.getRequest().getPath() == "/cgi/test.java");
	REQUIRE(connection.getRequest().getQuery() == "user=Nguyen&school=42");
	code = parse_http_ver(connection);
	REQUIRE(code == READING_HEADERS);
	REQUIRE(connection.buffer == "Host: localhost1:8001\r\n"
								 "Connection: Keep-Alive\r\n"
								 "Keep-Alive: timeout=5, max=200\r\n"
								 "Content-Length: 12\r\n"
								 "\r\n"
								 "Hello World!");
	REQUIRE(connection.getRequest().getHttpVersion() == "HTTP/1.1");
	code = parse_headers(connection, *config);
	REQUIRE(code == READING_BODY);
	REQUIRE(connection.buffer == "Hello World!");
	REQUIRE(connection.getRequest().getHeader("host") == "localhost1:8001");
	REQUIRE(connection.getRequest().getHost() == "localhost1");
	REQUIRE(connection.getRequest().getPort() == "8001");
	REQUIRE(connection.getRequest().getKeepAlive() == "keep-alive");
	REQUIRE(connection.getRequest().getKeepAliveTimeout() == 5);
	REQUIRE(connection.getRequest().getKeepAliveMax() == 200);
	REQUIRE(connection.getRequest().getContentLength() == 12);
	code = parse_body(connection);
	REQUIRE(code == MAKING_RESPONSE);
	REQUIRE(connection.buffer == "");
	REQUIRE(connection.getRequest().getBody() == "Hello World!");
	// REQUIRE(connection.getRequest().getRequestType() == CGI);
	// REQUIRE(connection.getRequest().getCgiType() == ".java");
}

TEST_CASE("Parse complete, content-length", "[Success]") {
	char			**env;
	Debug			dfile;
	Config	*config = new Config("../configFiles/goodConfigs/default.config", dfile);
	config->parseContent();
	config->sanitize();
	Connection		connection;
	connection.buffer = "POST http://www.test.com/cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
						"Host: localhost1:8001\r\n"
						"Connection: Keep-Alive\r\n"
						"Keep-Alive: timeout=5, max=200\r\n"
						"Content-Length: 12\r\n"
						"\r\n"
						"Hello World!";
	int code = 	parse_request(connection, *config, env);
	REQUIRE(code == MAKING_RESPONSE);
	REQUIRE(connection.getRequest().getMethod() == "POST");
	REQUIRE(connection.getRequest().getPath() == "/cgi/test.java");
	REQUIRE(connection.getRequest().getQuery() == "user=Nguyen&school=42");
	REQUIRE(connection.getRequest().getHttpVersion() == "HTTP/1.1");
	REQUIRE(connection.getRequest().getHeader("host") == "localhost1:8001");
	REQUIRE(connection.getRequest().getHeader("connection") == "Keep-Alive");
	REQUIRE(connection.getRequest().getHeader("keep-alive") == "timeout=5, max=200");
	REQUIRE(connection.getRequest().getHeader("content-length") == "12");
	REQUIRE(connection.getRequest().getBody() == "Hello World!");
	REQUIRE(connection.getRequest().getHost() == "localhost1");
	REQUIRE(connection.getRequest().getPort() == "8001");
	REQUIRE(connection.getRequest().getContentLength() == 12);
	REQUIRE(connection.getRequest().getKeepAlive() == "keep-alive");
	REQUIRE(connection.getRequest().getKeepAliveTimeout() == 5);
	REQUIRE(connection.getRequest().getKeepAliveMax() == 200);
}

TEST_CASE("Parse complete, chunked", "[Success]") {
	char			**env;
	Debug			dfile;
	Config	*config = new Config("../configFiles/goodConfigs/default.config", dfile);
	config->parseContent();
	config->sanitize();
	Connection		connection;
	connection.buffer = "POST /cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
						"Host: localhost1:8001\r\n"
						"Transfer-Encoding: chunked\r\n"
						"\r\n"
						"D\r\n"
						"Hello World!\n\r\n"
						"20\r\n"
						"This is a greeting from Lausanne\r\n"
						"0\r\n"
						"\r\n";
	int code = 	parse_request(connection, *config, env);
	REQUIRE(code == MAKING_RESPONSE);
	REQUIRE(connection.getRequest().getMethod() == "POST");
	REQUIRE(connection.getRequest().getPath() == "/cgi/test.java");
	REQUIRE(connection.getRequest().getQuery() == "user=Nguyen&school=42");
	REQUIRE(connection.getRequest().getHttpVersion() == "HTTP/1.1");
	REQUIRE(connection.getRequest().getHeader("host") == "localhost1:8001");
	REQUIRE(connection.getRequest().getHeader("transfer-encoding") == "chunked");
	REQUIRE(connection.getRequest().getBody() == "Hello World!\nThis is a greeting from Lausanne");
	REQUIRE(connection.getRequest().getHost() == "localhost1");
	REQUIRE(connection.getRequest().getPort() == "8001");
}

TEST_CASE("Connection state request section", "[Success]") {
	char			**env;
	Debug			dfile;
	Config	*config = new Config("../configFiles/goodConfigs/default.config", dfile);
	config->parseContent();
	config->sanitize();
	Connection		connection;
	connection.buffer = "POST /cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
						"Host: localhost1:8001\r\n"
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
	REQUIRE(connection.getState() == READING_METHOD);
	parse_method(connection);
	REQUIRE(connection.getState() == READING_PATH);
	parse_URL(connection, *config);
	REQUIRE(connection.getState() == READING_HTTPVERSION);
	parse_http_ver(connection);
	REQUIRE(connection.getState() == READING_HEADERS);
	parse_headers(connection, *config);
	REQUIRE(connection.getState() == READING_CHUNKED);
	parse_body_chunked(connection);
	REQUIRE(connection.getState() == MAKING_RESPONSE);
}

TEST_CASE("Parse method", "[Error]") {
	char			**env;
	Debug			dfile;
	Config	*config = new Config("../configFiles/goodConfigs/default.config", dfile);
	config->parseContent();
	config->sanitize();
	Connection		connection;
	SECTION("Method not complete", "[Error]") {
		connection.buffer = "GE";
		int code = parse_method(connection);
		REQUIRE(code == CONTINUE_READ);
		REQUIRE(connection.buffer == "GE");
		REQUIRE(connection.getRequest().getMethod() == "");
	}
	SECTION("Not implemented method", "[Error]") {
		connection.buffer = "CONNECT / HTTP/1.1\r\n";
		int code = parse_method(connection);
		REQUIRE(code == NOT_IMPLEMENTED);
		REQUIRE(connection.buffer == "CONNECT / HTTP/1.1\r\n");
		REQUIRE(connection.getRequest().getMethod() == "");
	}
	SECTION("Random string", "[Error]") {
		connection.buffer = "FASDHFGJK / HTTP/1.1\r\n";
		int code = parse_method(connection);
		REQUIRE(code == BAD_REQUEST);
		REQUIRE(connection.buffer == "FASDHFGJK / HTTP/1.1\r\n");
		REQUIRE(connection.getRequest().getMethod() == "");
	}
}

TEST_CASE("Parse path", "[Error]") {
	char			**env;
	Debug			dfile;
	Config	*config = new Config("../configFiles/goodConfigs/default.config", dfile);
	config->parseContent();
	config->sanitize();
	Connection		connection;
	SECTION("Path not complete", "[Error]") {
		connection.buffer = "GET /wtfw";
		parse_method(connection);
		int code = parse_URL(connection, *config);
		REQUIRE(code == CONTINUE_READ);
		REQUIRE(connection.buffer == "/wtfw");
		REQUIRE(connection.getRequest().getPath() == "");
	}
}

TEST_CASE("Parse http version", "[Error]") {
	char			**env;
	Debug			dfile;
	Config	*config = new Config("../configFiles/goodConfigs/default.config", dfile);
	config->parseContent();
	config->sanitize();
	Connection		connection;
	SECTION("HTTP version not complete", "[Error]") {
		connection.buffer = "GET /cgi/test.java?user=Nguyen&school=42 HTTP/1.";
		parse_method(connection);
		parse_URL(connection, *config);
		int code = parse_http_ver(connection);
		REQUIRE(code == CONTINUE_READ);
		REQUIRE(connection.buffer == "HTTP/1.");
		REQUIRE(connection.getRequest().getHttpVersion() == "");
	}
	SECTION("HTTP version mismatch", "[Error]") {
		connection.buffer = "GET /cgi/test.java?user=Nguyen&school=42 HTTP/1.0\r\n";
		parse_method(connection);
		parse_URL(connection, *config);
		int code = parse_http_ver(connection);
		REQUIRE(code == HTTP_VERSION_MISMATCH);
		REQUIRE(connection.buffer == "HTTP/1.0\r\n");
		REQUIRE(connection.getRequest().getHttpVersion() == "");
	}
	
}

TEST_CASE("Parse headers", "[Error]") {
	char			**env;
	Debug			dfile;
	Config	*config = new Config("../configFiles/goodConfigs/default.config", dfile);
	config->parseContent();
	config->sanitize();
	Connection		connection;
	SECTION("Dupplicate host", "[Error]") {
		connection.buffer = "GET /wtfwtf?user=Nguyen&school=42 HTTP/1.1\r\n"
							"Host: localhost:8002\r\n"
							"Host: localhost:3000\r\n"
							"\r\n";
		parse_method(connection);
		parse_URL(connection, *config);
		parse_http_ver(connection);
		int code = parse_headers(connection, *config);
		REQUIRE(code == BAD_REQUEST);
	}
	SECTION("No host", "[Error]") {
		connection.buffer = "GET /wtfwtf?user=Nguyen&school=42 HTTP/1.1\r\n"
							"Date: today\r\n"
							"Server: 42 Lausanne webserver\r\n"
							"\r\n";
		parse_method(connection);
		parse_URL(connection, *config);
		parse_http_ver(connection);
		int code = parse_headers(connection, *config);
		REQUIRE(code == BAD_REQUEST);
	}
	SECTION("Both Content-Length and Transfer-Encoding", "[Error]") {
		connection.buffer = "POST /cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
							"Host: localhost1:8001\r\n"
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
		int code = 	parse_request(connection, *config, env);
		REQUIRE(code == -1);
	}
	// SECTION("Method not allowed", "[Error]") {
	// 	connection.buffer = "DELETE /wtfwtf?user=Nguyen&school=42 HTTP/1.1\r\n"
	// 						"Host:localhost:3000\r\n"
	// 						"\r\n";
	// 	parse_method(connection);
	// 	parse_URL(connection, config);
	// 	parse_http_ver(connection);
	// 	int code = parse_headers(connection, config);
	// 	REQUIRE(connection.getRequest().getHost() == "localhost");
	// 	REQUIRE(connection.getRequest().getPort() == "3000");
	// 	REQUIRE(code == METHOD_NOT_ALLOWED);
	// }
}

TEST_CASE("File GET", "[Success]") {
	Connection		connection;
	connection.getRequest().setPath("../html/error/400.html");
	connection.getRequest().setFileType(".html");
	int code = get_file(connection);
	REQUIRE(code == 0);
	REQUIRE(connection.getResponse().getBody() == "<!DOCTYPE html>\n"
												"\n"
												"<html lang=\"fr\">\n"
												"  <head>\n"
												"    <meta charset=\"UTF-8\" />\n"
												"  </head>\n"
												"  <body>\n"
												"    <div style=\"margin-top: 50px; margin-bottom:200px; text-align: center; text-justify: center;\">\n"
												"      <h1 style=\"font-size: 5rem;\">400</h1>\n"
												"      <h3 style=\"font-size: 2rem;\">Bad Request</h3>\n"
												"      <br />\n"
												"      <p>Erreur de la requête client!</p>\n"
												"    </div>\n"
												"  </body>\n"
												"</html>\n"
												);
	REQUIRE(connection.getResponse().getContentLength() == 371);
}

// TEST_CASE("File DELETE", "[Success]") {
// 	Connection		connection;
// 	connection.getRequest().setPath("../test.txt");
// 	int code = delete_file(connection);
// 	REQUIRE(code == 0);
// }

TEST_CASE("File POST", "[Success]") {
	Connection		connection;
	connection.getRequest().setPath("../test.txt");
	connection.getRequest().setBody("THIS PART SHOULD APPEND TO THE FILE!\n");
	int code = post_file(connection);
	REQUIRE(code == 0);
}
