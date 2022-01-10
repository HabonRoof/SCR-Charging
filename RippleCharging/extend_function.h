/*
 * extend_function.h
 *
 *  Created on: 2022¦~1¤ë4¤é
 *      Author: johnson
 */

#ifndef EXTEND_FUNCTION_H_
#define EXTEND_FUNCTION_H_
#include <math.h>

// defines for gcvt
#define PSH(X) (*(buf++)=(X))
#define PSH1(X) (*(buf--)=(X))
#define PEEK() buf[-1]
#define POP() *(--buf) = '\0'
#define PLUS 1
#define SPACE 2

// function to convert from float to string
char * gcvt(float f, uint16_t ndigit, char * buf)
{
 int i;
 uint32_t z,k;
 //int exp = 0;
 char *c = buf;
 float f2,t,scal;
 int   sign = 0;

 if((int)ndigit == -1)
   ndigit = 5;

 /* Unsigned long long only allows for 20 digits of precision
  * which is already more than double supports, so we limit the
  * digits to this.  long double might require an increase if it is ever
  * implemented.
  */
 if (ndigit > 20)
     ndigit = 20;

 if (f < 0.0) {
   sign = 1;
   f = -f;
    buf++;
 }

 scal = 1;
 for (i=ndigit; i>0; i--)
     scal *= 10;
 k = f + 0.1 / scal;
 f2 = f - k;
 if (!f) {
   PSH('0');
   if(ndigit > 0)
     PSH('.');
   for (i=0;i<ndigit;i++)
     PSH('0');
      PSH(0);
    return c;
 }

 i = 1;
 while (f >= 10.0) {
   f /= 10.0;
   i++;
 }

 buf += i + ndigit + 1;

 PSH1(0);

 if(ndigit > 0) {
     t = f2 * scal;
    z = t + 0.5;
   for (i = 0;i < ndigit;i++)
   {
     PSH1('0'+ (z % 10));
      z /= 10;
   }
   PSH1('.');
 }
 else
   PSH1(0);

 do {
   PSH1('0'+ (k % 10));
   k /= 10;
 }while (k);

 if (sign)
   PSH1('-');
 return c;
}



#define precision 2  //precision for decimal digits

// function to convert float to string
char* ftos (float num, char *buff)
{
    float f = num;
    char *str = buff;
    int a,b,c,k,l=0,m,i=0;

    // check for negative float
    if(f<0.0)
    {

        str[i++]='-';
        f*=-1;
    }

    a=f;    // extracting whole number
    f-=a;   // extracting decimal part
    k = precision;

    // number of digits in whole number
    while(k>-1)
    {
        l = pow(10,k);
        m = a/l;
        if(m>0)
        {
            break;
        }
    k--;
    }

    // number of digits in whole number are k+1

    /*
    extracting most significant digit i.e. right most digit , and concatenating to string
    obtained as quotient by dividing number by 10^k where k = (number of digit -1)
    */

    for(l=k+1;l>0;l--)
    {
        b = pow(10,l-1);
        c = a/b;
        str[i++]=c+48;
        a%=b;
    }
    str[i++] = '.';

    /* extracting decimal digits till precision */

    for(l=0;l<precision;l++)
    {
        f*=10.0;
        b = f;
        str[i++]=b+48;
        f-=b;
    }

    str[i]='\0';

    return str;
}


#endif /* EXTEND_FUNCTION_H_ */
