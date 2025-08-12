/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tdd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:59:13 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/12 23:41:21 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CATCH_CONFIG_MAIN
#include <string>
#include "catch_amalgamated.hpp"
#include "Request.hpp"
#include "request_handler.hpp"

TEST_CASE( "Request class default constructor" ) {
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

TEST_CASE( "Parse Request method", "[Error]") {
	Request request;
	SECTION( "Empty request", "[General Error]") {
		std::string buffer = "";
		int code = parse_request_line(request, buffer);
		REQUIRE(code == 400);
	}
	SECTION( "No method", "[Method Error]") {
		std::string buffer = " /test/demo_form.php HTTP/1.1";
		int code = parse_request_line(request, buffer);
		REQUIRE(request.getMethod() == "");
		REQUIRE(code == 501);
	}
}
