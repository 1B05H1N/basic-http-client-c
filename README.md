### Updated README: Basic HTTP Client in C

---

#### Description
This is a simple, basic HTTP client written in C for educational purposes. The program allows users to:
- Enter a hostname (e.g., `example.com`).
- Specify a path (e.g., `/index.html`).
- Optionally define a port (default is `80`).

The client establishes a connection to the specified server, sends an HTTP GET request, and displays the server's response in the terminal.

---

#### Disclaimer: **Use at Your Own Risk**
This program is intended for **educational purposes only**. It is a basic implementation and lacks the security and robustness required for production use. **Do not use this program for sensitive or mission-critical operations.**

Key limitations:
- No support for HTTPS (data sent is unencrypted).
- Limited error handling for edge cases (e.g., malformed input or network issues).
- Not designed to handle large-scale or malicious requests.

---

#### Features
- **Interactive Input**: Users can specify the hostname, path, and port.
- **Dynamic Response Handling**: The program can handle HTTP responses of any size by dynamically allocating memory.
- **Basic Error Handling**: Detects and reports common errors, such as invalid hostnames or connection failures.

---

#### Prerequisites
To compile and run this program, you need:
1. A C compiler, such as GCC.
2. Windows with Winsock (`Ws2_32.lib` is required for networking).

---

#### How to Compile and Run
1. **Save the Program**:
   - Save the code in a file named `basic_http_client.c`.

2. **Compile**:
   - Use GCC to compile the program:
     ```bash
     gcc basic_http_client.c -o basic_http_client -lws2_32
     ```

3. **Run the Program**:
   - Execute the compiled binary:
     ```bash
     ./basic_http_client
     ```

4. **Enter Inputs**:
   - Follow the prompts to specify the hostname, path, and port.

---

#### Example Usage
**Input**:
```
Enter hostname (e.g., example.com): example.com
Enter path (e.g., /index.html) [default is '/']: /about
Enter port [default is 80]: 80
```

**Output**:
```
HTTP Response:
HTTP/1.1 200 OK
Date: Mon, 13 Nov 2023 12:00:00 GMT
Content-Type: text/html; charset=UTF-8
Content-Length: 1256
Connection: close

<!doctype html>
<html>
<head>
<title>About Us</title>
...
</html>
```

---

#### Important Notes
- **Use at Your Own Risk**: This program is not suitable for secure or production use.
- **No HTTPS Support**: Requests and responses are sent unencrypted.
- **Basic Implementation**: It is designed as a learning exercise and may not handle all edge cases.

---

#### License
This program is provided for educational purposes under the MIT License. You may use, modify, and distribute it at your own risk.

---