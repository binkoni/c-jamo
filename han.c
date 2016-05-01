#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <hangul.h>
#include <iconv.h>

void utf8_to_ucs4(char** inbuf, size_t* inbufbytesleft, char** outbuf, size_t* outbufbytesleft)
{
    iconv_t cd = iconv_open("UCS-4", "UTF-8");
    if (cd == (iconv_t) - 1) fprintf(stderr, "iconv_open failed with %d\n", errno);

    int rc = iconv(cd, inbuf, inbufbytesleft, outbuf, outbufbytesleft);
    if (rc == -1) fprintf(stderr, "iconv failed with -1. errno is %d: %s\n", errno, strerror(errno));

    rc = iconv_close(cd);
    if (rc != 0) fprintf(stderr, "iconv_close failed with %d\n", errno);
}

void ucs4_to_utf8(char** inbuf, size_t* inbufbytesleft, char** outbuf, size_t* outbufbytesleft)
{
    iconv_t cd = iconv_open("UTF-8", "UCS-4");
    if (cd == (iconv_t) - 1) fprintf(stderr, "iconv_open failed with %d\n", errno);

    int rc = iconv(cd, inbuf, inbufbytesleft, outbuf, outbufbytesleft);
    if (rc == -1) fprintf(stderr, "iconv failed with -1. errno is %d: %s\n", errno, strerror(errno));

    rc = iconv_close(cd);
    if (rc != 0) fprintf(stderr, "iconv_close failed with %d\n", errno);
}

size_t ucs4_array_length(int* array)
{
    size_t length = 0;
    while(array[length] != 0) length++;
    return length; 
}


int main(int argc, char** argv)
{
    setlocale(LC_ALL, "en_US.utf8");

    char* in = "yoㅎㅎㅎ";
    printf("%d\n", mbstowcs(NULL, in, 0));

    size_t inbufbytes = strlen(in);

    char out[100];
    memset(out, 0, sizeof(out));
    size_t outbufbytes = sizeof(out);

    size_t inbufbytesleft = inbufbytes;
    size_t outbufbytesleft = outbufbytes;

    char* inbuf = in;
    char* outbuf = out;
    
    utf8_to_ucs4(&inbuf, &inbufbytesleft, &outbuf, &outbufbytesleft);

    printf("ucs4_array_length(out): %d\n", ucs4_array_length((int*)out));
    printf("out: %s\n", out);
    printf("out[0](%%d): %d\n", out[0]);
    printf("out[1](%%d): %d\n", out[1]);
    printf("out[2](%%d): %d\n", out[2]);
    printf("out[3](%%d): %d\n", out[3]);


    char in2[100];
    in2[0] = out[0];
    in2[1] = out[1];
    in2[2] = out[2];
    in2[3] = out[3];

    size_t inbufbytes2 = 4;

    char out2[100];
    size_t outbufbytes2 = sizeof(out2);

    size_t inbufbytesleft2 = inbufbytes2;
    size_t outbufbytesleft2 = outbufbytes2;

    char* inbuf2 = in2;
    char* outbuf2 = out2;

    ucs4_to_utf8(&inbuf2, &inbufbytesleft2, &outbuf2, &outbufbytesleft2);

    printf("strlen(out2): %d\n", strlen(out2));
    printf("out2: %s\n", out2);
    printf("out2[0](%%d): %d\n", out2[0]);
    printf("out2[1](%%d): %d\n", out2[1]);
    printf("out2[2](%%d): %d\n", out2[2]);
    printf("out2[3](%%d): %d\n", out2[3]);

    return 0;
}
