#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

/* close the connection */
static void websocket_close(CURL *curl)
{
    size_t sent;
    (void)curl_ws_send(curl, "", 0, &sent, 0, CURLWS_CLOSE);
}

int main(int argc, char **argv)
{
    CURL *curl;
    int ret;
    const char *url;
    const char *ca_path;
    int is_echo = 0;

    if (argc < 3)
    {
        fprintf(stderr, "usage: %s CA_PATH URL [IS_ECHO]", argv[0]);
        return 1;
    }

    ca_path = argv[1];
    url = argv[2];
    if (argc > 3)
    {
        is_echo = 1;
    }

    curl = curl_easy_init();
    if (!curl)
    {
        fprintf(stderr, "Failed to initialize CURL\n");
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    curl_easy_setopt(curl, CURLOPT_CAPATH, ca_path);

    if (curl_easy_perform(curl) != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed\n");
        ret = 1;
    }
    else
    {
        size_t sent;
        const char *message = "hello";
        size_t message_len = strlen(message);

        if (curl_ws_send(curl, message, message_len, &sent, 0, CURLWS_PING) != CURLE_OK)
        {
            fprintf(stderr, "Failed to send a message\n");
            ret = 1;
        }
        else
        {
            char buffer[256] = {0};
            size_t response_len;
            const struct curl_ws_frame *meta;
            CURLcode code = CURLE_OK;
            do
            {
                code = curl_ws_recv(curl, buffer, sizeof(buffer), &response_len, &meta);
            } while (code == CURLE_AGAIN);

            if (code != CURLE_OK)
            {
                fprintf(stderr, "Failed to receive a message: %d\n", code);
                ret = 1;
            }
            else if (is_echo && strncmp(message, buffer, message_len) != 0)
            {
                fprintf(stderr, "Received unexpected message; expected %s, got: %sX\n", message, buffer);
                ret = 1;
            }
            else
            {
                printf("Received a message %s\n", buffer);
            }
        }
        curl_ws_send(curl, "", 0, &sent, 0, CURLWS_CLOSE);
    }

    curl_easy_cleanup(curl);

    return ret;
}
