import java.util.*;
import java.io.IOException;
import java.net.URLDecoder;
import java.nio.charset.StandardCharsets;

public class CGI {
	public static void main(String[] args) throws IOException {
		try {
			String method = System.getenv("REQUEST_METHOD");
			String input = "";
			if (method.equals("GET")) {
				String query = System.getenv("QUERY_STRING");
				try {
					input = URLDecoder.decode(query, StandardCharsets.UTF_8.name());
					input = input.substring(input.indexOf("=") + 1);
				} catch (Exception e) {
					System.out.print("Content-Length: 9\r\n");
					System.out.print("Content-Type: text/plain\r\n\r\n");
					System.out.println("CGI Error");
				}
			}
			if (method.equals("POST")) {
				int contentLength = Integer.valueOf(System.getenv("CONTENT_LENGTH"));
				int count = 0;
				while (count < contentLength) {
					input += (char)System.in.read();
					count++;
				}
			}
			String result = input.toUpperCase().replace('2', '4');

			StringBuilder html = new StringBuilder();
			html.append("<!DOCTYPE html>");
			html.append("<html><head><title>Result</title></head><body>");
			html.append("<h1>Result</h1>");
			html.append("<p>Before: ").append(input).append("</p>");
			html.append("<p>After: ").append(result).append("</p>");
			html.append("</body></html>");

			byte[] htmlBytes = html.toString().getBytes(StandardCharsets.UTF_8);

			System.out.print("Content-Type: text/html\r\n");
			System.out.print("Content-Length: " + htmlBytes.length + "\r\n\r\n");
			System.out.write(htmlBytes);
			System.out.flush();
		} catch(NullPointerException e) {
			System.out.print("Content-Length: 9\r\n");
			System.out.print("Content-Type: text/plain\r\n\r\n");
			System.out.println("CGI Error");
		}
	}
}
