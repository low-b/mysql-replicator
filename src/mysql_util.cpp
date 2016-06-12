#include "mysql_util.h"
#include <cstring>
#include <alloca.h>
#include <algorithm>
inline bool likely(bool expr)
{
      return __builtin_expect(expr, true);
}
inline bool unlikely(bool expr)
{
      return __builtin_expect(expr, false);
}


namespace mysql_replicator {

int decimal_bin_size(int precision, int scale)
{
    int intg=precision-scale,
        intg0=intg/DIG_PER_DEC1, frac0=scale/DIG_PER_DEC1,
        intg0x=intg-intg0*DIG_PER_DEC1, frac0x=scale-frac0*DIG_PER_DEC1;

    return intg0*sizeof(dec1)+dig2bytes[intg0x]+
        frac0*sizeof(dec1)+dig2bytes[frac0x];
}

int bin2decimal(const uchar *from, decimal_t *to, int precision, int scale)
{

    int error=E_DEC_OK, intg=precision-scale,
        intg0=intg/DIG_PER_DEC1, frac0=scale/DIG_PER_DEC1,
        intg0x=intg-intg0*DIG_PER_DEC1, frac0x=scale-frac0*DIG_PER_DEC1,
        intg1=intg0+(intg0x>0), frac1=frac0+(frac0x>0);
    dec1 *buf=to->buf, mask=(*from & 0x80) ? 0 : -1;
    const uchar *stop;
    uchar *d_copy;
    int bin_size= decimal_bin_size(precision, scale);

    d_copy= (uchar*) alloca(bin_size);
    memcpy(d_copy, from, bin_size);
    d_copy[0]^= 0x80;
    from= d_copy;

    FIX_INTG_FRAC_ERROR(to->len, intg1, frac1, error);
    if (unlikely(error))
    {
        if (intg1 < intg0+(intg0x>0))
        {
            from+=dig2bytes[intg0x]+sizeof(dec1)*(intg0-intg1);
            frac0=frac0x=intg0x=0;
            intg0=intg1;
        }
        else
        {
            frac0x=0;
            frac0=frac1;
        }
    }

    to->sign=(mask != 0);
    to->intg=intg0*DIG_PER_DEC1+intg0x;
    to->frac=frac0*DIG_PER_DEC1+frac0x;

    if (intg0x)
    {
        int i=dig2bytes[intg0x];
        dec1 x= 0;
        switch (i)
        {
            case 1: x=mi_sint1korr(from); break;
            case 2: x=mi_sint2korr(from); break;
            case 3: x=mi_sint3korr(from); break;
            case 4: x=mi_sint4korr(from); break;
            default: break;
        }
        from+=i;
        *buf=x ^ mask;
        if (((ulonglong)*buf) >= (ulonglong) powers10[intg0x+1])
            goto err;
        if (buf > to->buf || *buf != 0)
            buf++;
        else 
            to->intg-=intg0x;
    }
    for (stop=from+intg0*sizeof(dec1); from < stop; from+=sizeof(dec1))
    {
        *buf=mi_sint4korr(from) ^ mask;
        if (((uint32_t)*buf) > DIG_MAX)
            goto err;
        if (buf > to->buf || *buf != 0)
            buf++;
        else
            to->intg-=DIG_PER_DEC1;
    }
    for (stop=from+frac0*sizeof(dec1); from < stop; from+=sizeof(dec1))
    {
        *buf=mi_sint4korr(from) ^ mask;
        if (((uint32_t)*buf) > DIG_MAX)
            goto err;
        buf++;
    }
    if (frac0x)
    {
        int i=dig2bytes[frac0x];
        dec1 x= 0;
        switch (i)
        {
            case 1: x=mi_sint1korr(from); break;
            case 2: x=mi_sint2korr(from); break;
            case 3: x=mi_sint3korr(from); break;
            case 4: x=mi_sint4korr(from); break;
            default: break;
        }
        *buf=(x ^ mask) * powers10[DIG_PER_DEC1 - frac0x];
        if (((uint32_t)*buf) > DIG_MAX)
            goto err;
        buf++;
    }

    if (to->intg == 0 && to->frac == 0)
        decimal_make_zero(to);
    return error;

err:
    decimal_make_zero(to);
    return(E_DEC_BAD_NUM);
}

dec1 *remove_leading_zeroes(const decimal_t *from, int *intg_result)
{
    int intg= from->intg, i;
    dec1 *buf0= from->buf;
    i= ((intg - 1) % DIG_PER_DEC1) + 1;
    while (intg > 0 && *buf0 == 0)
    {
        intg-= i;
        i= DIG_PER_DEC1;
        buf0++;
    }
    if (intg > 0)
    {
        intg-= count_leading_zeroes((intg - 1) % DIG_PER_DEC1, *buf0);
    }
    else
        intg=0;
    *intg_result= intg;
    return buf0;
}

int decimal2string(const decimal_t *from, char *to, int *to_len,
        int fixed_precision, int fixed_decimals,
        char filler)
{
    int len, intg, frac= from->frac, i, intg_len, frac_len, fill;
    int fixed_intg= (fixed_precision ?
            (fixed_precision - fixed_decimals) : 0);
    int error=E_DEC_OK;
    char *s=to;
    dec1 *buf, *buf0=from->buf, tmp;

    buf0= remove_leading_zeroes(from, &intg);
    if (unlikely(intg+frac==0))
    {
        intg=1;
        tmp=0;
        buf0=&tmp;
    }

    if (!(intg_len= fixed_precision ? fixed_intg : intg))
        intg_len= 1;
    frac_len= fixed_precision ? fixed_decimals : frac;
    len= from->sign + intg_len + MY_TEST(frac) + frac_len;
    if (fixed_precision)
    {
        if (frac > fixed_decimals)
        {
            error= E_DEC_TRUNCATED;
            frac= fixed_decimals;
        }
        if (intg > fixed_intg)
        {
            error= E_DEC_OVERFLOW;
            intg= fixed_intg;
        }
    }
    else if (unlikely(len > --*to_len)) /* reserve one byte for \0 */
    {
        int j= len - *to_len;             /* excess printable chars */
        error= (frac && j <= frac + 1) ? E_DEC_TRUNCATED : E_DEC_OVERFLOW;

        if (frac && j >= frac + 1)
            j--;

        if (j > frac)
        {
            intg_len= intg-= j-frac;
            frac= 0;
        }
        else
            frac-=j;
        frac_len= frac;
        len= from->sign + intg_len + MY_TEST(frac) + frac_len;
    }
    *to_len= len;
    s[len]= 0;

    if (from->sign)
        *s++='-';

    if (frac)
    {
        char *s1= s + intg_len;
        fill= frac_len - frac;
        buf=buf0+ROUND_UP(intg);
        *s1++='.';
        for (; frac>0; frac-=DIG_PER_DEC1)
        {
            dec1 x=*buf++;
            for (i= std::min(frac, DIG_PER_DEC1); i; i--)
            {
                dec1 y=x/DIG_MASK;
                *s1++='0'+(uchar)y;
                x-=y*DIG_MASK;
                x*=10;
            }
        }
        for(; fill > 0; fill--)
            *s1++=filler;
    }

    fill= intg_len - intg;
    if (intg == 0)
        fill--; /* symbol 0 before digital point */
    for(; fill > 0; fill--)
        *s++=filler;
    if (intg)
    {
        s+=intg;
        for (buf=buf0+ROUND_UP(intg); intg>0; intg-=DIG_PER_DEC1)
        {
            dec1 x=*--buf;
            for (i= std::min(intg, DIG_PER_DEC1); i; i--)
            {
                dec1 y=x/10;
                *--s='0'+(uchar)(x-y*10);
                x=y;
            }
        }
    }
    else
        *s= '0';

    return error;
}

longlong my_datetime_packed_from_binary(const uchar *ptr, uint dec)
{
    longlong intpart= mi_uint5korr(ptr) - DATETIMEF_INT_OFS;
    int frac;
    switch (dec)
    {
        case 0:
        default:
            return MY_PACKED_TIME_MAKE_INT(intpart);
        case 1:
        case 2:
            frac= ((int) (signed char) ptr[5]) * 10000;
            break;
        case 3:
        case 4:
            frac= mi_sint2korr(ptr + 5) * 100;
            break;
        case 5:
        case 6:
            frac= mi_sint3korr(ptr + 5);
            break;
    }
    return MY_PACKED_TIME_MAKE(intpart, frac);
}

void TIME_from_longlong_datetime_packed(MYSQL_TIME *ltime, longlong tmp)
{
    longlong ymd, hms;
    longlong ymdhms, ym;
    if ((ltime->neg= (tmp < 0)))
        tmp= -tmp;

    ltime->second_part= MY_PACKED_TIME_GET_FRAC_PART(tmp);
    ymdhms= MY_PACKED_TIME_GET_INT_PART(tmp);

    ymd= ymdhms >> 17;
    ym= ymd >> 5;
    hms= ymdhms % (1 << 17);

    ltime->day= ymd % (1 << 5);
    ltime->month= ym % 13;
    ltime->year= (uint)(ym / 13);

    ltime->second= hms % (1 << 6);
    ltime->minute= (hms >> 6) % (1 << 6);
    ltime->hour= (uint)(hms >> 12);

    ltime->time_type= MYSQL_TIMESTAMP_DATETIME;
}

ulonglong log_10_int[20]=
{
    1, 10, 100, 1000, 10000UL, 100000UL, 1000000UL, 10000000UL,
    100000000ULL, 1000000000ULL, 10000000000ULL, 100000000000ULL,
    1000000000000ULL, 10000000000000ULL, 100000000000000ULL,
    1000000000000000ULL, 10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL
};

static inline int
my_useconds_to_str(char *to, ulong useconds, uint dec)
{
    return sprintf(to, ".%0*lu", (int) dec,
            useconds / (ulong) log_10_int[DATETIME_MAX_DECIMALS - dec]);
}

int my_datetime_to_str(const MYSQL_TIME *l_time, char *to, uint dec)
{
    int len= TIME_to_datetime_str(to, l_time);
    if (dec)
        len+= my_useconds_to_str(to + len, l_time->second_part, dec);
    else
        to[len]= '\0';
    return len;
}

}
