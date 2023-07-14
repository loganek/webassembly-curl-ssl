#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

const char *EXPECTED_ENCODINGS[] = {"gzip", "deflate"};
const int EXPECTED_ENCODINGS_COUNT = sizeof(EXPECTED_ENCODINGS) / sizeof(EXPECTED_ENCODINGS[0]);

// Private CURL method
char *Curl_all_content_encodings();

typedef struct
{
    const char *name;
    int seen;
} encoding_t;

encoding_t *make_encoding_list()
{
    encoding_t *encoding_list = malloc(sizeof(encoding_list[0]) * EXPECTED_ENCODINGS_COUNT);
    for (int i = 0; i < EXPECTED_ENCODINGS_COUNT; i++)
    {
        encoding_list[i].name = EXPECTED_ENCODINGS[i];
        encoding_list[i].seen = 0;
    }

    return encoding_list;
}

int main(int argc, char **argv)
{
    const char *separator = ", ";
    char *encodings = Curl_all_content_encodings();
    char *encoding;
    int i = 0;
    int ret = 0;
    int total_encodings_found = 0, valid_encodings_found = 0;

    printf("Supported encodings: %s\n", encodings);

    encoding_t *encoding_list = make_encoding_list();

    encoding= strtok(encodings, separator);
    while (encoding != NULL)
    {
        total_encodings_found++;
        for (int j = 0; j < EXPECTED_ENCODINGS_COUNT; j++)
        {

            if (strcmp(encoding, encoding_list[j].name) == 0)
            {
                if (encoding_list[j].seen == 0)
                {
                    encoding_list[j].seen = 1;
                    valid_encodings_found++;
                }
                else
                {
                    fprintf(stderr, "Duplicated encoding found: %s\n", encoding);
                }
                break;
            }
        }
        encoding = strtok(NULL, separator);
    }

    if (valid_encodings_found != total_encodings_found || valid_encodings_found != EXPECTED_ENCODINGS_COUNT)
    {
        fprintf(stderr, "Not all encodings found. Total encodings: %d\n", total_encodings_found);
        ret = 1;
    }

    free(encoding_list);
    free(encodings);

    return ret;
}