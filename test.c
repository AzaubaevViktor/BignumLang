#include "test.h"

#define INIT printf("\n-------------Test #%.3d---------------\n\n", ++testC)
#define _ERR_VAR_CH if (_err) printf("Returned error `%d`\n", _err)
#define _ERR_CH(func) _err = (func); _ERR_VAR_CH


#define reset_ \
  printf("Reset state...");\
  _ERR_CH(bnNul(a));_ERR_CH(bnNul(b));_ERR_CH(bnNul(c));_ERR_CH(bnNul(n));var = 0;\
  printf("!\n");

int test_bignum(TestContext *cnt) {
  printf("======== TEST BIGNUM LIBRARY ========\n");
  printf("Quiet mode: %s\n", cnt->quiet ? "ON" : "OFF");

  int _err = 0;
  Bignum *a = NULL, *b = NULL, *c = NULL, *n = NULL;
  int64_t var = 0;
  int testC = 0;

  _ERR_CH(bnInit(&a,0));
  _ERR_CH(bnInit(&b,0));
  _ERR_CH(bnInit(&c,0));
  _ERR_CH(bnInit(&n,0));

  // bnChLen
  {INIT;
  reset_;
  printf("Change len to 0b with BNCHUNK=`%d`: `%"PRIu64"`\n", BNCHUNK, a->len);
  _ERR_CH(bnChLen(a,100));
  printf("Change len to 100b with BNCHUNK=`%d`: `%"PRIu64"`\n", BNCHUNK, a->len);
  printf("Set 99th byte is `0xFF`\n");
  a->num[99] = 0xFF;
  _ERR_CH(bnChLen(a,10));
  printf("Change len to 10b with BNCHUNK=`%d`: `%"PRIu64"`\n", BNCHUNK, a->len);
  _ERR_CH(bnChLen(a,100));
  printf("Change len to 100b with BNCHUNK=`%d`: `%"PRIu64"`\n", BNCHUNK, a->len);
  printf("99th byte: `0x%.2X` (must be 0x00) \n", a->num[99]);
  _ERR_CH(bnChLen(a,100000000));
  printf("Change len to 100000000b with BNCHUNK=`%d`: `%"PRIu64"`\n", BNCHUNK, a->len);
  }
  // bnSetInt
  {INIT;
  reset_;
  _ERR_CH(bnSetInt(a,0));
  printf("Set int `0`, num:`"); bnPrintHex(a);printf("`\n");
  _ERR_CH(bnSetInt(a,0x00FF));
  printf("Set int `0x00FF`, num:`"); bnPrintHex(a); printf("`\n");
  _ERR_CH(bnSetInt(a,0x1234567890ABCDEF));
  printf("Set int `0x1234567890ABCDEF`, num:`"); bnPrintHex(a); printf("`\n");
  _ERR_CH(bnSetInt(a,0xFEDCBA0987654321));
  printf("Set int `0xFEDCBA0987654321`, num:`"); bnPrintHex(a); printf("`\n");
  }
  // bnSetStr
  {INIT;
    reset_;
    _ERR_CH(bnSetStr(a, "X"));
    printf("Set str 'X', result:"); bnPrintHex(a); printf("\n");

    _ERR_CH(bnSetStr(a, "abc1234privet"));
    printf("Set str 'abc1234privet', result:"); bnPrintHex(a); printf("\n");

    _ERR_CH(bnSetStr(a, "bI"));
    printf("Set str 'bI', result:"); bnPrintHex(a); printf("\n");

    _ERR_CH(bnSetStr(a, ""));
    printf("Set str '', result:"); bnPrintHex(a); printf("\n");
  }
  // bnCmp
  {INIT;
    reset_;
    bnSetInt(a, 10);
    bnSetInt(b, -10);
    bnPrintHex(a); switch (bnCmp(a,b)) {case -1:printf(" < ");break;case 1:printf(" > ");break;default:printf(" = ");break;} bnPrintHex(b); printf("\n");

    bnSetInt(a, 0);
    bnSetInt(b, -1);
    bnPrintHex(a); switch (bnCmp(a,b)) {case -1:printf(" < ");break;case 1:printf(" > ");break;default:printf(" = ");break;} bnPrintHex(b); printf("\n");

    bnSetInt(a, -12);
    bnSetInt(b, -10);
    bnPrintHex(a); switch (bnCmp(a,b)) {case -1:printf(" < ");break;case 1:printf(" > ");break;default:printf(" = ");break;} bnPrintHex(b); printf("\n");

    bnSetInt(a, -75757);
    bnSetInt(b, 98756755);
    bnPrintHex(a); switch (bnCmp(a,b)) {case -1:printf(" < ");break;case 1:printf(" > ");break;default:printf(" = ");break;} bnPrintHex(b); printf("\n");

    bnSetInt(a, 10);
    bnSetInt(b, 10);
    bnChLen(b,3*BNCHUNK);
    bnPrintHex(a); switch (bnCmp(a,b)) {case -1:printf(" < ");break;case 1:printf(" > ");break;default:printf(" = ");break;} bnPrintHex(b); printf("\n");

    bnSetInt(a, 233);
    bnSetInt(b, 343);
    bnPrintHex(a); switch (bnCmp(a,b)) {case -1:printf(" < ");break;case 1:printf(" > ");break;default:printf(" = ");break;} bnPrintHex(b); printf("\n");

    bnSetInt(a, 299993);
    bnChLen(a, 100);
    bnSetInt(b, 343);
    bnPrintHex(a); switch (bnCmp(a,b)) {case -1:printf(" < ");break;case 1:printf(" > ");break;default:printf(" = ");break;} bnPrintHex(b); printf("\n");

    bnSetInt(a, 0x1000000000000000);
    bnSetInt(b, 0x1100000000000000);
    bnPrintHex(a); switch (bnCmp(a,b)) {case -1:printf(" < ");break;case 1:printf(" > ");break;default:printf(" = ");break;} bnPrintHex(b); printf("\n");
  }
  // bnSum
  {INIT;
    reset_;
    bnSetInt(a, 0x0);
    bnSetInt(b, 0x1);
    bnChLen(b,3*BNCHUNK);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0x0);
    bnSetInt(b, -0x1);
    bnChLen(b,3*BNCHUNK);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0xFFFF);
    bnSetInt(b, 0x0001);
    bnChLen(b,3*BNCHUNK);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0x1234);
    bnSetInt(b, -0x1234);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0x1010);
    bnSetInt(b, 0x0F0F);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0x1234);
    bnSetInt(b, -0x2345);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0x1234);
    bnSetInt(b, -0x1235);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0xFFFF);
    bnSetInt(b, -0x10000);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, -0xFFFFFFFFFFFFFF);
    bnSetInt(b, -0x1);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0x12);
    bnSetInt(b, -0x2);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");


    printf("-----------------------\n");

    bnSetInt(a, 0x134);
    bnSetInt(b, 0xF45);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0x134);
    bnSetInt(b, 0x66);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0xFFFF);
    bnSetInt(b, -0x10000);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0xAA0);
    bnSetInt(b, -0x999);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, -0xFFF1);
    bnSetInt(b, -0x111F);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, -0xFFF1);
    bnChLen(a, 100);
    bnSetInt(b, -0xAAAAA111F);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, -0x55FF);
    bnSetInt(b, 0x46A0);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, -0xD0D);
    bnSetInt(b, -0xF1F);
    bnSum(a,b,n);
    bnPrintHex(a);printf("+");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");
  }
  // bnSet1Deg
  {INIT;
    reset_;
    bnSet1Deg(a, 10);
    printf("Set 10th:"); bnPrintHex(a); printf("\n");

    bnSet1Deg(a, 0);
    printf("Set 0th:"); bnPrintHex(a); printf("\n");

    bnSet1Deg(a, 1);
    printf("Set 1th:"); bnPrintHex(a); printf("\n");

    bnSet1Deg(a, 2);
    printf("Set 2th:"); bnPrintHex(a); printf("\n");

    bnSet1Deg(a, 99);
    printf("Set 99th:"); bnPrintHex(a); printf("\n");

    bnSet1Deg(a, 25);
    printf("Set 25th:"); bnPrintHex(a); printf("\n");
  }
  // bnMul
  {INIT;
    reset_;
    bnSetInt(a, 0x12);
    bnSetInt(b, 0x2);
    bnChLen(b,3*BNCHUNK);
    bnMul(a,b,n);
    bnPrintHex(a);printf("*");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0xFF);
    bnSetInt(b, 0xFF);
    bnChLen(b,3*BNCHUNK);
    bnMul(a,b,n);
    bnPrintHex(a);printf("*");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, -0x1F);
    bnSetInt(b, 0x11);
    bnChLen(b,3*BNCHUNK);
    bnMul(a,b,n);
    bnPrintHex(a);printf("*");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0x100000);
    bnSetInt(b, 0x0);
    bnChLen(b,3*BNCHUNK);
    bnMul(a,b,n);
    bnPrintHex(a);printf("*");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0x444);
    bnChLen(a, 100);
    bnSetInt(b, 0xF);
    bnChLen(b,3*BNCHUNK);
    bnMul(a,b,n);
    bnPrintHex(a);printf("*");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0xFFFFFF);
    bnSetInt(b, 0xFFFFFF);
    bnChLen(b,3*BNCHUNK);
    bnMul(a,b,n);
    bnPrintHex(a);printf("*");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");
  }
  // bnDiv
  {INIT;
    reset_;
    bnSetInt(a, 0x12);
    bnSetInt(b, 0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|9");printf("\n");

    bnSetInt(a, -0x12);
    bnSetInt(b, 0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|-9");printf("\n");

    bnSetInt(a, 0x12);
    bnSetInt(b, -0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|-9");printf("\n");

    bnSetInt(a, -0x12);
    bnSetInt(b, -0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|9");printf("\n");

    bnSetInt(a, 0x13);
    bnSetInt(b, 0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|9");printf("\n");

    bnSetInt(a, -0x13);
    bnSetInt(b, 0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|-10");printf("\n");

    bnSetInt(a, 0x13);
    bnSetInt(b, -0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|-10");printf("\n");

    bnSetInt(a, -0x13);
    bnSetInt(b, -0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|9");printf("\n");

    bnSetInt(a, 0x12222);
    bnSetInt(b, 0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|9");printf("\n");

    bnSetInt(a, -0x12222);
    bnSetInt(b, 0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|-9");printf("\n");

    bnSetInt(a, 0x12222);
    bnSetInt(b, -0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|-9");printf("\n");

    bnSetInt(a, -0x12222);
    bnSetInt(b, -0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|9");printf("\n");

    bnSetInt(a, 0x12223);
    bnSetInt(b, 0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|9");printf("\n");

    bnSetInt(a, -0x12223);
    bnSetInt(b, 0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|-10");printf("\n");

    bnSetInt(a, 0x12223);
    bnSetInt(b, -0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|-10");printf("\n");

    bnSetInt(a, -0x12223);
    bnSetInt(b, -0x2);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|9");printf("\n");

    printf("---------------------\n");

    bnSetInt(a, 0xF73E);
    bnSetInt(b, 0x233);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, -0x12223);
    bnSetInt(b, -0x23333);
    bnChLen(b,3*BNCHUNK);
    bnDiv(a,b,n);
    bnPrintHex(a);printf("/");bnPrintHex(b);printf("=");bnPrintHex(n);printf("|9");printf("\n");
  }
  // bnMod
  {INIT;
    reset_;
    bnSetInt(a, 0x12);
    bnSetInt(b, 0x5);
    bnChLen(b,3*BNCHUNK);
    bnMod(a,b,n);
    bnPrintHex(a);printf(" %% ");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, 0x12);
    bnSetInt(b, 0x100);
    bnChLen(b,3*BNCHUNK);
    bnMod(a,b,n);
    bnPrintHex(a);printf(" %% ");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");

    bnSetInt(a, -0x12);
    bnSetInt(b, 0x5);
    bnChLen(b,3*BNCHUNK);
    bnMod(a,b,n);
    bnPrintHex(a);printf(" %% ");bnPrintHex(b);printf("=");bnPrintHex(n);printf("\n");
  }

  bnFree(a);
  bnFree(b);
  bnFree(c);
  bnFree(n);

  printf("=====================================\n");
  return 0;
}

#undef INIT
#undef _ERR_VAR_CH
#undef _ERR_CH
