#include <microhttpd.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h> // for sockaddr_in and inet_pton

#define PORT 20600
#define IP "0.0.0.0"

static int route_home(struct MHD_Connection *connection) {
    const char *response_str = "<html><body><h1>Welcome to the Home Page!</h1></body></html>";
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(response_str),
                                                                     (void *)response_str,
                                                                     MHD_RESPMEM_PERSISTENT);
    if (!response)
        return MHD_NO;
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

static int route_about(struct MHD_Connection *connection) {
    const char *response_str = "<html><body><h1>About Us</h1><p>This is the about page.</p></body></html>";
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(response_str),
                                                                     (void *)response_str,
                                                                     MHD_RESPMEM_PERSISTENT);
    if (!response)
        return MHD_NO;
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

static int route_404(struct MHD_Connection *connection) {
    const char *response_str = "<html><body><h1>404 Not Found</h1></body></html>";
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(response_str),
                                                                     (void *)response_str,
                                                                     MHD_RESPMEM_PERSISTENT);
    if (!response)
        return MHD_NO;
    int ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    MHD_destroy_response(response);
    return ret;
}

static int handle_request(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
                          const char *version, const char *upload_data,
                          size_t *upload_data_size, void **con_cls) {
    printf("Request for %s with method %s\n", url, method);
    if (strcmp(url, "/") == 0)
        return route_home(connection);
    else if (strcmp(url, "/about") == 0)
        return route_about(connection);
    else
        return route_404(connection);
}

int main() {
    struct MHD_Daemon *server;
    struct sockaddr_in addr;

    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &addr.sin_addr); 


    server = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
                              &handle_request, NULL, MHD_OPTION_SOCK_ADDR, (struct sockaddr *)&addr, MHD_OPTION_END);
    if (!server) {
        fprintf(stderr, "Failed to start server\n");
        return 1;
    }

    printf("Server running on http:/\/%s:%d...\n", IP, PORT);
    getchar();
    MHD_stop_daemon(server);
    return 0;
}
