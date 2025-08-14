/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tdd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:59:13 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/14 17:59:22 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CATCH_CONFIG_MAIN
#include <string>
#include "catch_amalgamated.hpp"
#include "Request.hpp"
#include "request_handler.hpp"

// ------------------- Request class Default ---------------------

TEST_CASE("Request class default constructor") {
	Request requestDefault;

    REQUIRE(requestDefault.getMethod() == "");
	REQUIRE(requestDefault.getPath() == "");
	REQUIRE(requestDefault.getQuery() == "");
	REQUIRE(requestDefault.getHttpVersion() == "");
	REQUIRE(requestDefault.getHeader("nothing") == "");
	REQUIRE(requestDefault.getCookie("Cookie1") == "");
	REQUIRE(requestDefault.getFileType() == "");
	REQUIRE(requestDefault.getRequestType() == None);
	REQUIRE(requestDefault.getBody() == "");
	REQUIRE(requestDefault.getContentLength() == 0);
}

// ------------------- Request Parsing Error ---------------------

TEST_CASE( "Empty request", "[General Error]") {
	Request request;
	std::string buffer = "";
	int code = parse_request(buffer);
	REQUIRE(code == 400);
}
TEST_CASE("Bad format", "[General Error]") {
	Request request;
	std::string buffer = "GET/test/demo_form.phpHTTP/1.1";
	int code = parse_method(request, buffer);
	REQUIRE(request.getMethod() == "");
	REQUIRE(code == 400);
}
TEST_CASE("No method", "[Method Error]") {
	Request request;
	std::string buffer = " /test/demo_form.php HTTP/1.1";
	int code = parse_method(request, buffer);
	REQUIRE(request.getMethod() == "");
	REQUIRE(code == 400);
}
TEST_CASE("Not supported method", "[Method Error]") {
	Request request;
	std::string buffer = "TRACE /test/demo_form.php HTTP/1.1";
	int code = parse_method(request, buffer);
	REQUIRE(request.getMethod() == "");
	REQUIRE(code == 501);
}
TEST_CASE("Not recognised method", "[Method Error]") {
	Request request;
	std::string buffer = "FJAKSDHGF /test/demo_form.php HTTP/1.1";
	int code = parse_method(request, buffer);
	REQUIRE(request.getMethod() == "");
	REQUIRE(code == 400);
}
TEST_CASE("No URL", "[URL Error]") {
	Request request;
	std::string buffer = "GET HTTP/1.1";
	parse_method(request, buffer);
	int code = parse_URL(request, buffer);
	REQUIRE(request.getPath() == "");
	REQUIRE(code == 400);
}
TEST_CASE("Bad URL format", "[URL Error]") {
	Request request;
	std::string buffer = "GET www.google.com HTTP/1.1";
	parse_method(request, buffer);
	int code = parse_URL(request, buffer);
	REQUIRE(request.getPath() == "");
	REQUIRE(code == 400);
}
TEST_CASE("No HTTP version", "[HTTP Error]") {
	Request request;
	std::string buffer = "GET /test/demo_form.php?user=random&pass=1234 \r\n\r\n";
	parse_method(request, buffer);
	REQUIRE(request.getMethod() == "GET");
	parse_URL(request, buffer);
	REQUIRE(request.getPath() == "/test/demo_form.php");
	REQUIRE(request.getQuery() == "user=random&pass=1234");
	int code = parse_http_ver(request, buffer);
	REQUIRE(request.getHttpVersion() == "");
	REQUIRE(code == 400);
}
TEST_CASE("Wrong HTTP version", "[HTTP Error]") {
	Request request;
	std::string buffer = "GET /test/demo_form.php?user=random&pass=1234 HTTP/1.0\r\n\r\n";
	parse_method(request, buffer);
	REQUIRE(request.getMethod() == "GET");
	parse_URL(request, buffer);
	REQUIRE(request.getPath() == "/test/demo_form.php");
	REQUIRE(request.getQuery() == "user=random&pass=1234");
	int code = parse_http_ver(request, buffer);
	REQUIRE(request.getHttpVersion() == "");
	REQUIRE(code == 505);
}
TEST_CASE("Header section doesn't have Content-length with POST", "[Header Error]") {
	Request request;
	std::string buffer = 
		"POST /test/demo_form.php?user=random&pass=1234 HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: CustomClient/1.0\r\nAccept: */*\r\nConnection: close\r\n\r\n{\n\"username\": \"user123\",\n\"password\": \"securepassword\"\n}\r\n";
	parse_method(request, buffer);
	REQUIRE(request.getMethod() == "POST");
	parse_URL(request, buffer);
	parse_http_ver(request, buffer);
	int code = parse_headers(request, buffer);
	REQUIRE(buffer == "\r\n{\n\"username\": \"user123\",\n\"password\": \"securepassword\"\n}\r\n");
	REQUIRE(code == 411);
}
TEST_CASE("Body with smaller size then Content-length", "[Body Error]") {
	Request request;
	std::string buffer = 
			"POST /test/demo_form.php?user=random&pass=1234 HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: CustomClient/1.0\r\nAccept: */*\r\nConnection: close\r\nContent-length: 150\r\n\r\n{\n\"username\": \"user123\",\n\"password\": \"securepassword\"\n}\r\n";
	parse_method(request, buffer);
	parse_URL(request, buffer);
	parse_http_ver(request, buffer);
	parse_headers(request, buffer);
	REQUIRE(request.getContentLength() == 150);
	int code = parse_body(request, buffer);
	REQUIRE(code == 400);
}

// ------------------- Request Parsing Success ---------------------

TEST_CASE("GET method", "[Success]") {
	Request request;
	std::string buffer = "GET /test/demo_form.php HTTP/1.1";
	int code = parse_method(request, buffer);
	REQUIRE(request.getMethod() == "GET");
	REQUIRE(buffer == "/test/demo_form.php HTTP/1.1");
	REQUIRE(code == 0);
}
TEST_CASE("POST method", "[Success]") {
	Request request;
	std::string buffer = "POST /test/demo_form.php HTTP/1.1";
	int code = parse_method(request, buffer);
	REQUIRE(request.getMethod() == "POST");
	REQUIRE(buffer == "/test/demo_form.php HTTP/1.1");
	REQUIRE(code == 0);
}
TEST_CASE("DELETE method", "[Success]") {
	Request request;
	std::string buffer = "DELETE /test/demo_form.php HTTP/1.1";
	int code = parse_method(request, buffer);
	REQUIRE(request.getMethod() == "DELETE");
	REQUIRE(buffer == "/test/demo_form.php HTTP/1.1");
	REQUIRE(code == 0);
}
TEST_CASE("URL absolute path http no query", "[Success]") {
	Request request;
	std::string buffer = "GET http://www.google.com/test HTTP/1.1";
	parse_method(request, buffer);
	int code = parse_URL(request, buffer);
	REQUIRE(request.getPath() == "/test");
	REQUIRE(buffer == "HTTP/1.1");
	REQUIRE(code == 0);
}
TEST_CASE("URL absolute path https no query", "[Success]") {
	Request request;
	std::string buffer = "DELETE https://www.google.com/test HTTP/1.1";
	parse_method(request, buffer);
	int code = parse_URL(request, buffer);
	REQUIRE(request.getPath() == "/test");
	REQUIRE(buffer == "HTTP/1.1");
	REQUIRE(code == 0);
}
TEST_CASE("URL relative path no query", "[Success]") {
	Request request;
	std::string buffer = "DELETE / HTTP/1.1";
	parse_method(request, buffer);
	int code = parse_URL(request, buffer);
	REQUIRE(request.getPath() == "/");
	REQUIRE(buffer == "HTTP/1.1");
	REQUIRE(code == 0);
}
TEST_CASE("URL absolute path http with query", "[Success]") {
	Request request;
	std::string buffer = "POST https://www.figma.com/board/5K7iAxd6BVPNkAG1kahXpk/Webserv-diagram?node-id=0-1&p=f&t=PEPXxciFALE3JwX7-0 HTTP/1.1";
	parse_method(request, buffer);
	int code = parse_URL(request, buffer);
	REQUIRE(request.getPath() == "/board/5K7iAxd6BVPNkAG1kahXpk/Webserv-diagram");
	REQUIRE(request.getQuery() == "node-id=0-1&p=f&t=PEPXxciFALE3JwX7-0");
	REQUIRE(buffer == "HTTP/1.1");
	REQUIRE(code == 0);
}
TEST_CASE("HTTP version with headers", "[Success]") {
	Request request;
	std::string buffer = 
		"GET /test/demo_form.php?user=random&pass=1234 HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: CustomClient/1.0\r\nAccept: */*\r\nConnection: close\r\n\r\n";
	parse_method(request, buffer);
	parse_URL(request, buffer);
	int code = parse_http_ver(request, buffer);
	REQUIRE(request.getHttpVersion() == "HTTP/1.1");
	REQUIRE(buffer == "Host: www.example.com\r\nUser-Agent: CustomClient/1.0\r\nAccept: */*\r\nConnection: close\r\n\r\n");
	REQUIRE(code == 0);
}
TEST_CASE("HTTP version without headers", "[Success]") {
	Request request;
	std::string buffer = "GET /test/demo_form.php?user=random&pass=1234 HTTP/1.1\r\n\r\n";
	parse_method(request, buffer);
	parse_URL(request, buffer);
	int code = parse_http_ver(request, buffer);
	REQUIRE(request.getHttpVersion() == "HTTP/1.1");
	REQUIRE(buffer == "\r\n");
	REQUIRE(code == 0);
}
TEST_CASE("Header no body", "[Success]") {
	Request request;
	std::string buffer = 
			"GET /test/demo_form.php?user=random&pass=1234 HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: CustomClient/1.0\r\nAccept: */*\r\nConnection: close\r\n\r\n";
	parse_method(request, buffer);
	parse_URL(request, buffer);
	parse_http_ver(request, buffer);
	int code = parse_headers(request, buffer);
	REQUIRE(request.getHeader("Host") == "www.example.com");
	REQUIRE(request.getHeader("User-Agent") == "CustomClient/1.0");
	REQUIRE(request.getHeader("Connection") == "close");
	REQUIRE(buffer == "\r\n");
	REQUIRE(code == 0);
}
TEST_CASE("Header with body", "[Success]") {
	Request request;
	std::string buffer = 
			"POST /test/demo_form.php?user=random&pass=1234 HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: CustomClient/1.0\r\nAccept: */*\r\nConnection: close\r\nContent-length: 49\r\n\r\n{ username: user123,\npassword: securepassword }\r\n";
	parse_method(request, buffer);
	REQUIRE(request.getMethod() == "POST");
	parse_URL(request, buffer);
	parse_http_ver(request, buffer);
	int code = parse_headers(request, buffer);
	REQUIRE(request.getHeader("Host") == "www.example.com");
	REQUIRE(request.getHeader("User-Agent") == "CustomClient/1.0");
	REQUIRE(request.getHeader("Connection") == "close");
	REQUIRE(request.getHeader("Content-length") == "49");
	REQUIRE(buffer == "\r\n{ username: user123,\npassword: securepassword }\r\n");
	REQUIRE(request.getContentLength() == 49);
	REQUIRE(code == 0);
	code = parse_body(request, buffer);
	REQUIRE(request.getBody() == "{ username: user123,\npassword: securepassword }\r\n");
	REQUIRE(buffer == "");
	REQUIRE(code == 0);
}
TEST_CASE("Request type is Directory", "[Success]") {
	Request request;
	std::string buffer = 
			"GET /test/ HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: CustomClient/1.0\r\nAccept: */*\r\nConnection: close\r\nContent-length: 49\r\n\r\n{ username: user123,\npassword: securepassword }\r\n";
	parse_method(request, buffer);
	parse_URL(request, buffer);
	REQUIRE(request.getPath() == "/test/");
	int code = parse_request_type(request);
	REQUIRE(code == 0);
	REQUIRE(request.getRequestType() == Directory);
}
// TEST_CASE("Request type is CGI", "[Success]") {
// 	Request request;
// 	std::string buffer = 
// 			"GET /test/demo_form.php?user=random&pass=1234 HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: CustomClient/1.0\r\nAccept: */*\r\nConnection: close\r\nContent-length: 49\r\n\r\n{ username: user123,\npassword: securepassword }\r\n";
// 	parse_method(request, buffer);
// 	parse_URL(request, buffer);
// 	REQUIRE(request.getPath() == "/test/demo_form.php");
// 	int code = parse_request_type(request);
// 	REQUIRE(code == 0);
// 	REQUIRE(request.getRequestType() == CGI);
// }