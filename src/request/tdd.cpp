/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tdd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:59:13 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/13 23:19:15 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CATCH_CONFIG_MAIN
#include <string>
#include "catch_amalgamated.hpp"
#include "Request.hpp"
#include "request_handler.hpp"

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

TEST_CASE("Parse Request", "[Error]") {
	Request request;
	SECTION( "Empty request", "[General Error]") {
		std::string buffer = "";
		int code = parse_request(buffer);
		REQUIRE(code == 400);
	}
	SECTION("Bad format", "General Error") {
		std::string buffer = "GET/test/demo_form.phpHTTP/1.1";
		int code = parse_method(request, buffer);
		REQUIRE(request.getMethod() == "");
		REQUIRE(code == 400);
	}
	SECTION("No method", "[Method Error]") {
		std::string buffer = " /test/demo_form.php HTTP/1.1";
		int code = parse_method(request, buffer);
		REQUIRE(request.getMethod() == "");
		REQUIRE(code == 400);
	}
	SECTION("Not supported method", "[Method Error]") {
		std::string buffer = "TRACE /test/demo_form.php HTTP/1.1";
		int code = parse_method(request, buffer);
		REQUIRE(request.getMethod() == "");
		REQUIRE(code == 501);
	}
	SECTION("Not recognised method", "[Method Error]") {
		std::string buffer = "FJAKSDHGF /test/demo_form.php HTTP/1.1";
		int code = parse_method(request, buffer);
		REQUIRE(request.getMethod() == "");
		REQUIRE(code == 400);
	}
	SECTION("No URL", "[URL Error]") {
		std::string buffer = "GET HTTP/1.1";
		parse_method(request, buffer);
		int code = parse_URL(request, buffer);
		REQUIRE(request.getPath() == "");
		REQUIRE(code == 400);
	}
	SECTION("Bad URL format", "[URL Error]") {
		std::string buffer = "GET www.google.com HTTP/1.1";
		parse_method(request, buffer);
		int code = parse_URL(request, buffer);
		REQUIRE(request.getPath() == "");
		REQUIRE(code == 400);
	}
	SECTION("No HTTP version", "[HTTP Error]") {
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
	SECTION("Wrong HTTP version", "[HTTP Error]") {
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
}

TEST_CASE("Parse request line", "[Success]") {
	Request request;
	SECTION("GET method", "[Success]") {
		std::string buffer = "GET /test/demo_form.php HTTP/1.1";
		int code = parse_method(request, buffer);
		REQUIRE(request.getMethod() == "GET");
		REQUIRE(buffer == "/test/demo_form.php HTTP/1.1");
		REQUIRE(code == 0);
	}
	SECTION("POST method", "[Success]") {
		std::string buffer = "POST /test/demo_form.php HTTP/1.1";
		int code = parse_method(request, buffer);
		REQUIRE(request.getMethod() == "POST");
		REQUIRE(buffer == "/test/demo_form.php HTTP/1.1");
		REQUIRE(code == 0);
	}
	SECTION("DELETE method", "[Success]") {
		std::string buffer = "DELETE /test/demo_form.php HTTP/1.1";
		int code = parse_method(request, buffer);
		REQUIRE(request.getMethod() == "DELETE");
		REQUIRE(buffer == "/test/demo_form.php HTTP/1.1");
		REQUIRE(code == 0);
	}
	SECTION("URL absolute path http no query", "[Success]") {
		std::string buffer = "GET http://www.google.com/test HTTP/1.1";
		parse_method(request, buffer);
		int code = parse_URL(request, buffer);
		REQUIRE(request.getPath() == "/test");
		REQUIRE(buffer == "HTTP/1.1");
		REQUIRE(code == 0);
	}
	SECTION("URL absolute path https no query", "[Success]") {
		std::string buffer = "DELETE https://www.google.com/test HTTP/1.1";
		parse_method(request, buffer);
		int code = parse_URL(request, buffer);
		REQUIRE(request.getPath() == "/test");
		REQUIRE(buffer == "HTTP/1.1");
		REQUIRE(code == 0);
	}
	SECTION("URL relative path no query", "[Success]") {
		std::string buffer = "DELETE / HTTP/1.1";
		parse_method(request, buffer);
		int code = parse_URL(request, buffer);
		REQUIRE(request.getPath() == "/");
		REQUIRE(buffer == "HTTP/1.1");
		REQUIRE(code == 0);
	}
	SECTION("URL absolute path http with query", "[Success]") {
		std::string buffer = "POST https://www.figma.com/board/5K7iAxd6BVPNkAG1kahXpk/Webserv-diagram?node-id=0-1&p=f&t=PEPXxciFALE3JwX7-0 HTTP/1.1";
		parse_method(request, buffer);
		int code = parse_URL(request, buffer);
		REQUIRE(request.getPath() == "/board/5K7iAxd6BVPNkAG1kahXpk/Webserv-diagram");
		REQUIRE(request.getQuery() == "node-id=0-1&p=f&t=PEPXxciFALE3JwX7-0");
		REQUIRE(buffer == "HTTP/1.1");
		REQUIRE(code == 0);
	}
	SECTION("HTTP version with headers", "[Success]") {
		std::string buffer = 
			"GET /test/demo_form.php?user=random&pass=1234 HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: CustomClient/1.0\r\nAccept: */*\r\nConnection: close\r\n\r\n";
		parse_method(request, buffer);
		parse_URL(request, buffer);
		int code = parse_http_ver(request, buffer);
		REQUIRE(request.getHttpVersion() == "HTTP/1.1");
		REQUIRE(buffer == "Host: www.example.com\r\nUser-Agent: CustomClient/1.0\r\nAccept: */*\r\nConnection: close\r\n\r\n");
		REQUIRE(code == 0);
	}
	SECTION("HTTP version without headers", "[Success]") {
		std::string buffer = "GET /test/demo_form.php?user=random&pass=1234 HTTP/1.1\r\n\r\n";
		parse_method(request, buffer);
		parse_URL(request, buffer);
		int code = parse_http_ver(request, buffer);
		REQUIRE(request.getHttpVersion() == "HTTP/1.1");
		REQUIRE(buffer == "\r\n");
		REQUIRE(code == 0);
	}
	SECTION("HTTP version with headers", "[Success]") {
		std::string buffer = 
			"GET /test/demo_form.php?user=random&pass=1234 HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: CustomClient/1.0\r\nAccept: */*\r\nConnection: close\r\n\r\n";
		parse_method(request, buffer);
		parse_URL(request, buffer);
		int code = parse_http_ver(request, buffer);
		REQUIRE(request.getHttpVersion() == "HTTP/1.1");
		REQUIRE(buffer == "Host: www.example.com\r\nUser-Agent: CustomClient/1.0\r\nAccept: */*\r\nConnection: close\r\n\r\n");
		REQUIRE(code == 0);
	}
}
