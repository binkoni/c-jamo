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
while(true)
{
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
    free(out);

    char utf8_choseong[sizeof(ucschar)];
    memset(utf8_choseong, 0, sizeof(utf8_choseong));
    ucs4_to_utf8(&choseong, sizeof(ucschar), utf8_choseong, sizeof(ucschar));

    char utf8_jungseong[sizeof(ucschar)];
    memset(utf8_jungseong, 0, sizeof(utf8_jungseong));
    ucs4_to_utf8(&jungseong, sizeof(ucschar), utf8_jungseong, sizeof(ucschar));

    char utf8_jongseong[sizeof(ucschar)];
    memset(utf8_jongseong, 0, sizeof(utf8_jongseong));
    ucs4_to_utf8(&jongseong, sizeof(ucschar), utf8_jongseong, sizeof(ucschar));
    
    ucschar choseong_syllable = hangul_jamo_to_cjamo(choseong);
    ucschar jungseong_syllable = hangul_jamo_to_cjamo(jungseong);
    ucschar jongseong_syllable = hangul_jamo_to_cjamo(jongseong);

    char utf8_choseong_syllable[sizeof(ucschar)];
    memset(utf8_choseong_syllable, 0, sizeof(utf8_choseong_syllable));
    ucs4_to_utf8(&choseong_syllable, sizeof(ucschar), utf8_choseong_syllable, sizeof(ucschar));

    char utf8_jungseong_syllable[sizeof(ucschar)];
    memset(utf8_jungseong_syllable, 0, sizeof(utf8_jungseong_syllable));
    ucs4_to_utf8(&jungseong_syllable, sizeof(ucschar), utf8_jungseong_syllable, sizeof(ucschar)); 

    char utf8_jongseong_syllable[sizeof(ucschar)];
    memset(utf8_jongseong_syllable, 0, sizeof(utf8_jongseong_syllable));
    ucs4_to_utf8(&jongseong_syllable, sizeof(ucschar), utf8_jongseong_syllable, sizeof(ucschar)); 

    printf("초성:%s (%x %x %x %x)\n", utf8_choseong_syllable, utf8_choseong_syllable[0], utf8_choseong_syllable[1], utf8_choseong_syllable[2], utf8_choseong_syllable[3]);
    printf("중성:%s (%x %x %x %x)\n", utf8_jungseong_syllable, utf8_jungseong_syllable[0], utf8_jungseong_syllable[1], utf8_jungseong_syllable[2], utf8_jungseong_syllable[3]);
    printf("종성:%s (%x %x %x %x)\n", utf8_jongseong_syllable, utf8_jongseong_syllable[0], utf8_jongseong_syllable[1], utf8_jongseong_syllable[2], utf8_jongseong_syllable[3]);
}

    return 0;
}
