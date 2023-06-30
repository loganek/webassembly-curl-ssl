#include <stdio.h>
#include <curl/curl.h>

int main(int argc, char **argv)
{
    CURL *curl;
    CURLcode res;
    const char *url;
    const char *ca_path;
    int ret = 0;

    if (argc < 3)
    {
        fprintf(stderr, "usage: %s CA_PATH URL", argv[0]);
        return 1;
    }

    ca_path = argv[1];
    url = argv[2];

    curl = curl_easy_init();
    if (!curl)
    {
        fprintf(stderr, "Failed to initialize CURL\n");
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_CAPATH, ca_path);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        ret = 1;
    }

    curl_easy_cleanup(curl);

    return ret;
}