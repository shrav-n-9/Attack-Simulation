import http.server
import socketserver
import base64
import os
from datetime import datetime

# Define the port to listen on (80 or 8080, as used in poly_attack.c)
PORT = 8068
OUTPUT_DIR = "received_files"

# Ensure output directory exists
if not os.path.exists(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

class CustomHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_POST(self):
        # Check if the path is /upload
        if self.path == "/upload":
            try:
                # Get the content length
                content_length = int(self.headers.get("Content-Length", 0))
                if content_length == 0:
                    self.send_response(400)
                    self.end_headers()
                    self.wfile.write(b"No data received")
                    return

                # Read the Base64-encoded data
                b64_data = self.rfile.read(content_length).decode("utf-8")

                # Decode Base64
                zip_data = base64.b64decode(b64_data)

                # Generate a unique filename with timestamp
                timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
                output_file = os.path.join(OUTPUT_DIR, f"archive_{timestamp}.zip")

                # Save the decoded data as a zip file
                with open(output_file, "wb") as f:
                    f.write(zip_data)

                # Log success
                print(f"Received and saved file: {output_file} ({len(zip_data)} bytes)")

                # Send 200 OK response
                self.send_response(200)
                self.send_header("Content-Type", "text/plain")
                self.end_headers()
                self.wfile.write(b"Data received and saved successfully")

            except base64.binascii.Error:
                print("Error: Invalid Base64 data")
                self.send_response(400)
                self.end_headers()
                self.wfile.write(b"Invalid Base64 data")
            except Exception as e:
                print(f"Error processing request: {e}")
                self.send_response(500)
                self.end_headers()
                self.wfile.write(b"Server error")
        else:
            # Handle invalid paths
            self.send_response(404)
            self.end_headers()
            self.wfile.write(b"Not found")

# Set up the server
with socketserver.TCPServer(("", PORT), CustomHTTPRequestHandler) as httpd:
    print(f"HTTP server running on port {PORT}...")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nShutting down server...")
        httpd.server_close()