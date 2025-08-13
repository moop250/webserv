/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tdd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:59:13 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/13 18:22:36 by hoannguy         ###   ########.fr       */
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
	REQUIRE(requestDefault.getURL() == "");
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
		REQUIRE(request.getURL() == "");
		REQUIRE(code == 400);
	}
	SECTION("Bad URL format", "[URL Error]") {
		std::string buffer = "GET www.google.com HTTP/1.1";
		parse_method(request, buffer);
		int code = parse_URL(request, buffer);
		REQUIRE(request.getURL() == "");
		REQUIRE(code == 400);
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
		REQUIRE(request.getMethod() == "GET");
		int code = parse_URL(request, buffer);
		REQUIRE(request.getURL() == "http://www.google.com/test");
		REQUIRE(buffer == "HTTP/1.1");
		REQUIRE(code == 0);
	}
	SECTION("URL absolute path https no query", "[Success]") {
		std::string buffer = "DELETE https://www.google.com/test HTTP/1.1";
		parse_method(request, buffer);
		REQUIRE(request.getMethod() == "DELETE");
		int code = parse_URL(request, buffer);
		REQUIRE(request.getURL() == "https://www.google.com/test");
		REQUIRE(buffer == "HTTP/1.1");
		REQUIRE(code == 0);
	}
	SECTION("Whole line no query", "[Success]") {
		std::string buffer = "GET /test/demo_form.php HTTP/1.1";
		int code = parse_method(request, buffer);
		REQUIRE(request.getMethod() == "GET");
		// REQUIRE(buffer ==  "");
		// REQUIRE(request.getURL() == "/test/demo_form.php");
		// REQUIRE(request.getHttpVersion() == "HTTP/1.1");
		REQUIRE(code == 0);
	}
}
