import java.util.*;
import java.io.IOException;

// Doesnt parse the QUERY_STRING nor input correctly
// To be change to html form request
public class CGI {
	public static void main(String[] args) throws IOException {
		String method = System.getenv("REQUEST_METHOD");
		String input = "";
		if (method.equals("GET"))
			input = System.getenv("QUERY_STRING");
			// parse query here
		if (method.equals("POST")) {
			int contentLength = Integer.valueOf(System.getenv("CONTENT_LENGTH"));
			int count = 0;
			while (count < contentLength) {
				input += (char)System.in.read();
				count++;
			}
			// parse post body here
		}
		String result = input.toUpperCase().replace('2', '4');

		System.out.print("Content-Type: text/html\r\n");
		System.out.print("Content-Length: " + input.length() + "\r\n\r\n");

		System.out.println("<!DOCTYPE html>");
        System.out.println("<html><head><title>Result</title></head><body>");
        System.out.println("<h1>Résultat:</h1>");
        System.out.println("<p>Avant: " + input + "</p>");
        System.out.println("<p>Après: " + result + "</p>");
        System.out.println("</body></html>");
	}
}
