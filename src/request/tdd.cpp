/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tdd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:59:13 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/12 18:14:33 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "Request.hpp"

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
