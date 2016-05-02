#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <hangul.h>
#include <iconv.h>

void utf8_to_ucs4(char* inbuf, size_t inbufbytes, ucschar* outbuf, size_t outbufbytes)
{
    char* inbuftemp = inbuf;
    char* outbuftemp = (char*)outbuf;
    size_t inbufbytesleft = inbufbytes;
    size_t outbufbytesleft = outbufbytes;

    iconv_t cd = iconv_open("UCS-4LE", "UTF-8");
    if (cd == (iconv_t) - 1) fprintf(stderr, "iconv_open failed with %d\n", errno);

    int rc = iconv(cd, &inbuftemp, &inbufbytesleft, &outbuftemp, &outbufbytesleft);
    if (rc == -1) fprintf(stderr, "LINE %d: iconv failed with -1. errno is %d: %s\n", __LINE__, errno, strerror(errno));

    rc = iconv_close(cd);
    if (rc != 0) fprintf(stderr, "iconv_close failed with %d\n", errno);
}

void ucs4_to_utf8(ucschar* inbuf, size_t inbufbytes, char* outbuf, size_t outbufbytes)
{
    char* inbuftemp = (char*)inbuf;
    char* outbuftemp = outbuf;
    size_t inbufbytesleft = inbufbytes;
    size_t outbufbytesleft = outbufbytes;

    iconv_t cd = iconv_open("UTF-8", "UCS-4LE");
    if (cd == (iconv_t) - 1) fprintf(stderr, "iconv_open failed with %d\n", errno);

    int rc = iconv(cd, &inbuftemp, &inbufbytesleft, &outbuftemp, &outbufbytesleft);
    if (rc == (size_t) - 1) fprintf(stderr, "LINE %d: iconv failed with -1. errno is %d: %s\n", __LINE__, errno, strerror(errno));

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

    char in[4];
    printf("input: ");
    scanf("%s", in);

    size_t inlen = strlen(in);

    ucschar* out;
    size_t outlen = mbstowcs(NULL, in, 0);
    out = calloc(outlen, sizeof(ucschar));

    utf8_to_ucs4(in, inlen * sizeof(char), out, outlen * sizeof(ucschar));

    ucschar choseong = 0;
    ucschar jungseong = 0;
    ucschar jongseong = 0;

    hangul_syllable_to_jamo(out[0], &choseong, &jungseong, &jongseong);

    char utf8choseong[sizeof(ucschar)];
    memset(utf8choseong, 0, sizeof(utf8choseong));
    ucs4_to_utf8(&choseong, sizeof(ucschar), utf8choseong, sizeof(ucschar));

    char utf8jungseong[sizeof(ucschar)];
    memset(utf8jungseong, 0, sizeof(utf8jungseong));
    ucs4_to_utf8(&jungseong, sizeof(ucschar), utf8jungseong, sizeof(ucschar));

    char utf8jongseong[sizeof(ucschar)];
    memset(utf8jongseong, 0, sizeof(utf8jongseong));
    ucs4_to_utf8(&jongseong, sizeof(ucschar), utf8jongseong, sizeof(ucschar));

    printf("초성:%s (%x %x %x %x)\n", utf8choseong, utf8choseong[0], utf8choseong[1], utf8choseong[2], utf8choseong[3]);
    printf("중성:%s (%x %x %x %x)\n", utf8jungseong, utf8jungseong[0], utf8jungseong[1], utf8jungseong[2], utf8jungseong[3]);
    printf("종성:%s (%x %x %x %x)\n", utf8jongseong, utf8jongseong[0], utf8jongseong[1], utf8jongseong[2], utf8jongseong[3]);

    free(out);

    return 0;
}
