#ifndef  __MYSQL_UTIL_H_
#define  __MYSQL_UTIL_H_

#include <cstdint>
#include <cstdio>
#include <cfloat>

namespace mysql_replicator {

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned char my_bool;
typedef unsigned long long int ulonglong; /* ulong or unsigned long long */
typedef unsigned long int ulong;
typedef long long int   longlong;
typedef longlong int64;
typedef ulonglong uint64;

#define DECIMAL_BUFF_LENGTH 9
#define DECIMAL_MAX_POSSIBLE_PRECISION (DECIMAL_BUFF_LENGTH * 9)
#define DECIMAL_MAX_STR_LENGTH (DECIMAL_MAX_POSSIBLE_PRECISION + 2)

#define MY_TEST(a)      ((a) ? 1 : 0)

#define E_DEC_OK                0
#define E_DEC_TRUNCATED         1
#define E_DEC_OVERFLOW          2
#define E_DEC_BAD_NUM           8
#define E_DEC_OOM              16
#define E_DEC_BAD_PREC         32
#define E_DEC_BAD_SCALE        64
#define DIG_PER_DEC1 9
#define ROUND_UP(X)  (((X)+DIG_PER_DEC1-1)/DIG_PER_DEC1)

#define FIX_INTG_FRAC_ERROR(len, intg1, frac1, error)               \
    do                                                              \
    {                                                               \
        if (unlikely(intg1+frac1 > (len)))                          \
        {                                                           \
            if (unlikely(intg1 > (len)))                            \
            {                                                       \
                intg1=(len);                                        \
                frac1=0;                                            \
                error=E_DEC_OVERFLOW;                               \
            }                                                       \
            else                                                    \
            {                                                       \
                frac1=(len)-intg1;                                  \
                error=E_DEC_TRUNCATED;                              \
            }                                                       \
        }                                                           \
        else                                                        \
        error=E_DEC_OK;                                             \
    } while(0)

#define mi_sint1korr(A) ((int8_t)(*A))
#define mi_uint1korr(A) ((uint8_t)(*A))

#define mi_sint2korr(A) ((int16_t) (((int16_t) (((uchar*) (A))[1])) +\
            ((int16_t) ((int16_t) ((char*) (A))[0]) << 8)))
#define mi_sint3korr(A) ((int32_t) (((((uchar*) (A))[0]) & 128) ? \
            (((uint32_t) 255L << 24) | \
             (((uint32_t) ((uchar*) (A))[0]) << 16) |\
             (((uint32_t) ((uchar*) (A))[1]) << 8) | \
             ((uint32_t) ((uchar*) (A))[2])) : \
            (((uint32_t) ((uchar*) (A))[0]) << 16) |\
            (((uint32_t) ((uchar*) (A))[1]) << 8) | \
            ((uint32_t) ((uchar*) (A))[2])))
#define mi_sint4korr(A) ((int32_t) (((int32_t) (((uchar*) (A))[3])) +\
            ((int32_t) (((uchar*) (A))[2]) << 8) +\
            ((int32_t) (((uchar*) (A))[1]) << 16) +\
            ((int32_t) ((int16_t) ((char*) (A))[0]) << 24)))

#define mi_uint5korr(A) ((ulonglong)(((uint32_t) (((uchar*) (A))[4])) +\
            (((uint32_t) (((uchar*) (A))[3])) << 8) +\
            (((uint32_t) (((uchar*) (A))[2])) << 16) +\
            (((uint32_t) (((uchar*) (A))[1])) << 24)) +\
        (((ulonglong) (((uchar*) (A))[0])) << 32))

#define decimal_make_zero(dec)        do {                \
                                            (dec)->buf[0]=0;    \
                                            (dec)->intg=1;      \
                                            (dec)->frac=0;      \
                                            (dec)->sign=0;      \
                                          } while(0)
typedef struct st_decimal_t {
    int    intg, frac, len;
    char sign;
    int32_t *buf;
} decimal_t;

typedef int32_t dec1;

#define DIG_PER_DEC1 9
#define DIG_MASK     100000000
#define DIG_BASE     1000000000
#define DIG_MAX      (DIG_BASE-1)
#define DIG_BASE2    ((dec2)DIG_BASE * (dec2)DIG_BASE)
#define ROUND_UP(X)  (((X)+DIG_PER_DEC1-1)/DIG_PER_DEC1)
static const dec1 powers10[DIG_PER_DEC1+1]={
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
static const int dig2bytes[DIG_PER_DEC1+1]={0, 1, 1, 2, 2, 3, 3, 4, 4, 4};
static const dec1 frac_max[DIG_PER_DEC1-1]={
    900000000, 990000000, 999000000,
    999900000, 999990000, 999999000,
    999999900, 999999990 };

int decimal_bin_size(int precision, int scale);

int bin2decimal(const uchar *from, decimal_t *to, int precision, int scale);

inline
int my_decimal_get_binary_size(uint precision, uint scale)
{
    return decimal_bin_size((int)precision, (int)scale);
}


static inline int count_leading_zeroes(int i, dec1 val)
{
    int ret= 0;
    switch (i)
    {
        case 9: if (val >= 1000000000) break; ++ret;
        case 8: if (val >= 100000000) break; ++ret;
        case 7: if (val >= 10000000) break; ++ret;
        case 6: if (val >= 1000000) break; ++ret;
        case 5: if (val >= 100000) break; ++ret;
        case 4: if (val >= 10000) break; ++ret;
        case 3: if (val >= 1000) break; ++ret;
        case 2: if (val >= 100) break; ++ret;
        case 1: if (val >= 10) break; ++ret;
        case 0: if (val >= 1) break; ++ret;
        default: break;
    }
    return ret;
}


dec1 *remove_leading_zeroes(const decimal_t *from, int *intg_result);

int decimal2string(const decimal_t *from, char *to, int *to_len,
        int fixed_precision, int fixed_decimals,
        char filler);

#define DATETIME_MAX_DECIMALS 6
#define DATETIMEF_INT_OFS 0x8000000000LL
#define MAX_DATE_STRING_REP_LENGTH 30

#define MY_PACKED_TIME_GET_INT_PART(x)     ((x) >> 24)
#define MY_PACKED_TIME_GET_FRAC_PART(x)    ((x) % (1LL << 24))
#define MY_PACKED_TIME_MAKE(i, f)          ((((longlong) (i)) << 24) + (f))
#define MY_PACKED_TIME_MAKE_INT(i)         ((((longlong) (i)) << 24))

enum enum_mysql_timestamp_type
{
    MYSQL_TIMESTAMP_NONE= -2, MYSQL_TIMESTAMP_ERROR= -1,
    MYSQL_TIMESTAMP_DATE= 0, MYSQL_TIMESTAMP_DATETIME= 1, MYSQL_TIMESTAMP_TIME= 2
};

typedef struct st_mysql_time
{
    unsigned int  year, month, day, hour, minute, second;
    unsigned long second_part;  /**< microseconds */
    my_bool       neg;
    enum enum_mysql_timestamp_type time_type;
} MYSQL_TIME;


longlong my_datetime_packed_from_binary(const uchar *ptr, uint dec);

void TIME_from_longlong_datetime_packed(MYSQL_TIME *ltime, longlong tmp);


static inline int
TIME_to_datetime_str(char *to, const MYSQL_TIME *ltime)
{
    uint32_t temp, temp2;
    /* Year */
    temp= ltime->year / 100;
    *to++= (char) ('0' + temp / 10);
    *to++= (char) ('0' + temp % 10);
    temp= ltime->year % 100;
    *to++= (char) ('0' + temp / 10);
    *to++= (char) ('0' + temp % 10);
    *to++= '-';
    /* Month */
    temp= ltime->month;
    temp2= temp / 10;
    temp= temp-temp2 * 10;
    *to++= (char) ('0' + (char) (temp2));
    *to++= (char) ('0' + (char) (temp));
    *to++= '-';
    /* Day */ 
    temp= ltime->day;
    temp2= temp / 10;
    temp= temp - temp2 * 10;
    *to++= (char) ('0' + (char) (temp2));
    *to++= (char) ('0' + (char) (temp));
    *to++= ' ';
    /* Hour */
    temp= ltime->hour;
    temp2= temp / 10;
    temp= temp - temp2 * 10;
    *to++= (char) ('0' + (char) (temp2));
    *to++= (char) ('0' + (char) (temp));
    *to++= ':';
    /* Minute */
    temp= ltime->minute;
    temp2= temp / 10;
    temp= temp - temp2 * 10;
    *to++= (char) ('0' + (char) (temp2));
    *to++= (char) ('0' + (char) (temp));
    *to++= ':';
    /* Second */
    temp= ltime->second;
    temp2=temp / 10;
    temp= temp - temp2 * 10;
    *to++= (char) ('0' + (char) (temp2));
    *to++= (char) ('0' + (char) (temp));
    return 19;
}


int my_datetime_to_str(const MYSQL_TIME *l_time, char *to, uint dec);

}
#endif  //__MYSQL_UTIL_H_
