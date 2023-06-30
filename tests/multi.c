#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

int main(int argc, char **argv)
{
    CURL **handles;
    CURLM *multi_handle;
    int addr_count = argc - 2;
    int ret = 0;
    int still_running = 1;
    int i;

    CURLMsg *msg;
    int msgs_left;

    handles = (CURL **)malloc(sizeof(handles[0]) * addr_count);
    multi_handle = curl_multi_init();

    for (i = 0; i < addr_count; i++)
    {
        handles[i] = curl_easy_init();
        curl_easy_setopt(handles[i], CURLOPT_URL, argv[2 + i]);
        curl_easy_setopt(handles[i], CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(handles[i], CURLOPT_CAPATH, argv[1]);
        curl_easy_setopt(handles[i], CURLOPT_VERBOSE, 1L);
        curl_multi_add_handle(multi_handle, handles[i]);
    }

    while (still_running)
    {
        CURLMcode mc = curl_multi_perform(multi_handle, &still_running);

        if (still_running)
            mc = curl_multi_poll(multi_handle, NULL, 0, 1000, NULL);

        if (mc)
            break;
    }

    while ((msg = curl_multi_info_read(multi_handle, &msgs_left)))
    {
        if (msg->msg == CURLMSG_DONE)
        {
            int idx;

            for (idx = 0; idx < addr_count; idx++)
            {
                int found = (msg->easy_handle == handles[idx]);
                if (found)
                    break;
            }
            printf("%d transfer completed with status %d\n", idx, msg->data.result);
            if (msg->data.result != 0)
            {
                ret = 1;
            }
        }
    }

    for (i = 0; i < addr_count; i++)
    {
        curl_multi_remove_handle(multi_handle, handles[i]);
        curl_easy_cleanup(handles[i]);
    }

    curl_multi_cleanup(multi_handle);

    return ret;
}