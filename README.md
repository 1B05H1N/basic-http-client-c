### README: Advanced HTTP Client in C

---

#### Description
This is an advanced, interactive HTTP client written in C for educational purposes. The program allows users to:
- Choose between HTTP and HTTPS protocols.
- Enter a hostname (e.g., `example.com`).
- Specify a path (e.g., `/index.html`).
- Optionally define a port (default is `80` for HTTP, `443` for HTTPS).
- Add custom headers for advanced requests.

The client establishes a connection to the specified server, sends an HTTP GET request, and displays the server's response in the terminal or saves it to a file.

---

#### Disclaimer: **Use at Your Own Risk**
This program is intended for **educational purposes only**. It is a basic implementation and lacks the security and robustness required for production use. **Do not use this program for sensitive or mission-critical operations.**

Key limitations:
- Limited error handling for edge cases (e.g., malformed input or network issues).
- Not designed to handle large-scale or malicious requests.

---

#### Features
- **HTTP and HTTPS Support**: Establishes secure HTTPS connections using OpenSSL.
- **Custom Headers**: Users can add custom headers like `User-Agent` or `Authorization`.
- **Interactive Input**: Users can specify the protocol, hostname, path, port, and headers.
- **Dynamic Response Handling**: Can handle HTTP responses of any size by dynamically allocating memory.
- **Save to File Option**: Users can choose to save the response to a file (`response.txt`).
- **Verbose Logging**: Displays detailed logs about the connection and response.

---

#### Prerequisites
To compile and run this program, you need:
1. A C compiler, such as GCC.
2. **Windows** with Winsock (`Ws2_32.lib`).
3. **OpenSSL** installed on your system for HTTPS support.

---

#### How to Compile and Run
1. **Save the Program**:
   - Save the code in a file named `advanced_http_client.c`.

2. **Install OpenSSL**:
   - On **Windows**, download and install OpenSSL from [OpenSSL Windows binaries](https://slproweb.com/products/Win32OpenSSL.html).
   - Ensure `libssl` and `libcrypto` are accessible during compilation.

3. **Compile**:
   - Use GCC to compile the program with the necessary libraries:
     ```bash
     gcc advanced_http_client.c -o advanced_http_client -lws2_32 -lssl -lcrypto
     ```

4. **Run the Program**:
   - Execute the compiled binary:
     ```bash
     ./advanced_http_client
     ```

5. **Enter Inputs**:
   - Follow the prompts to specify the protocol, hostname, path, port, and custom headers.

---

#### Example Usage
**Input**:
```
Enter protocol (http or https): https
Enter hostname (e.g., example.com): example.com
Enter path (e.g., /index.html) [default is '/']: /
Enter custom headers (e.g., User-Agent: MyClient) [Press Enter to skip]: User-Agent: MyCustomClient
Save response to file? (1 for yes, 0 for no): 1
```

**Output**:
```
Connected using TLS_AES_256_GCM_SHA384 encryption
HTTP Response:
Response saved to response.txt
```

---

#### Important Notes
- **Use at Your Own Risk**: This program is not suitable for secure or production use.
- **Learning Focus**: Designed as an educational tool to demonstrate HTTP and HTTPS communication in C.
- **Dependencies**: Requires OpenSSL for HTTPS support.

---

#### License
This program is provided for educational purposes under the MIT License. You may use, modify, and distribute it at your own risk.

---