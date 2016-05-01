#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <hangul.h>
#include <iconv.h>

void utf8_to_ucs4(char** inbuf, size_t* inbufbytesleft, ucschar** outbuf, size_t* outbufbytesleft)
{
    iconv_t cd = iconv_open("UCS-4", "UTF-8");
    if (cd == (iconv_t) - 1) fprintf(stderr, "iconv_open failed with %d\n", errno);

    int rc = iconv(cd, inbuf, inbufbytesleft, (char**)outbuf, outbufbytesleft);
    if (rc == -1) fprintf(stderr, "iconv failed with -1. errno is %d: %s\n", errno, strerror(errno));

    rc = iconv_close(cd);
    if (rc != 0) fprintf(stderr, "iconv_close failed with %d\n", errno);
}

void ucs4_to_utf8(ucschar** inbuf, size_t* inbufbytesleft, char** outbuf, size_t* outbufbytesleft)
{
    iconv_t cd = iconv_open("UTF-8", "UCS-4");
    if (cd == (iconv_t) - 1) fprintf(stderr, "iconv_open failed with %d\n", errno);

    int rc = iconv(cd, (char**)inbuf, inbufbytesleft, outbuf, outbufbytesleft);
    if (rc == -1) fprintf(stderr, "iconv failed with -1. errno is %d: %s\n", errno, strerror(errno));

    rc = iconv_close(cd);
    if (rc != 0) fprintf(stderr, "iconv_close failed with %d\n", errno);
}

size_t ucs4_array_len(ucschar* array)
{
    size_t len = 0;
    while(array[len] != 0) len++;
    return len; 
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "en_US.utf8");

    char* in = "yoㅎㅎㅎ";
    printf("mbstowcs(in): %d\n", mbstowcs(NULL, in, 0));

    size_t inbuflen = strlen(in);

    ucschar* out;
    size_t outbuflen = mbstowcs(NULL, in, 0);
    out = calloc(outbuflen, sizeof(ucschar));

    size_t inbufbytesleft = inbuflen;
    size_t outbufbytesleft = outbuflen * sizeof(ucschar);

    char* inbuf = in;
    ucschar* outbuf = out;
    
    utf8_to_ucs4(&inbuf, &inbufbytesleft, &outbuf, &outbufbytesleft);

    printf("ucs4_array_len(out): %d\n", ucs4_array_len(out));



    ucschar* in2;
    size_t inbuflen2 = ucs4_array_len(out);
    in2 = malloc(inbuflen2 * sizeof(ucschar));

    memcpy(in2, out, inbuflen2 * sizeof(ucschar));

    char out2[100];
    size_t outbuflen2 = sizeof(out2);

    size_t inbufbytesleft2 = inbuflen2 * sizeof(ucschar);
    size_t outbufbytesleft2 = outbuflen2;

    ucschar* inbuf2 = in2;
    char* outbuf2 = out2;

    ucs4_to_utf8(&inbuf2, &inbufbytesleft2, &outbuf2, &outbufbytesleft2);

    printf("strlen(out2): %d\n", strlen(out2));
    printf("out2: %s\n", out2);

    free(out);
    free(in2);
    return 0;
}
