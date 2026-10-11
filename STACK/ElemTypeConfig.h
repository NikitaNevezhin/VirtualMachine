#ifndef ELEM__TYPE_CONFIG_H

#define ELEM__TYPE_CONFIG_H

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "..\UTILS\FloatTools.h"

//------------------------USER!!!!! PLEASE, PLEASE WRITE type of elems in YOUR stack!!!!!----------------------------
// write "#define STACK_INT" if you want to use int as stack_elem_t
// write "#define STACK_DOUBLE" if you want to use double as stack_elem_t
#define STACK_INT
//-----------------------USER!!!!! DO NOT PASS PLEASE, CHECK YOUR CHOICE, PLEASE!!!!!!!!!----------------------------

#if defined(STACK_INT)
    typedef int stack_elem_t;

    #define DATA_LEFT_CANARY  0xB00B135
    #define DATA_RIGHT_CANARY 0xEBA75A0
    #define POIZON            -66666666

    #define IS_POIZON(value)  (value == POIZON)

    #define PRINT_STACK_ELEM(elem)  printf("%d", elem)

    #define PRINT_DATA_CANARIES(left_canary, right_canary)                                                    \
                printf("        Left:  %#X (expected %#X)\n", left_canary, DATA_LEFT_CANARY);                 \
                printf("        Right: %#X (expected %#X)\n", right_canary, DATA_RIGHT_CANARY);               \

    #define CHECK_LEFT_CANARY(left_canary)      (left_canary == DATA_LEFT_CANARY)

    #define CHECK_RIGHT_CANARY(right_canary)    (right_canary == DATA_RIGHT_CANARY)

#elif defined(STACK_DOUBLE)
    typedef double stack_elem_t;
    
    #define DATA_LEFT_CANARY  1776.2076
    #define DATA_RIGHT_CANARY -2012.1231

    #define POIZON             NAN
    
    #define IS_POIZON(value)          (isnan(value))  // conversion to float to escape [-Wconversion] warning (isnan() converts its argument to float)

    #define PRINT_STACK_ELEM(value)    printf("%lg", value)

    #define PRINT_DATA_CANARIES(left_canary, right_canary)                                                \
            printf("        Left:  %lg (expected %lg)\n", left_canary, DATA_LEFT_CANARY);                 \
            printf("        Right: %lg (expected %lg)\n", right_canary, DATA_RIGHT_CANARY);               \

    #define CHECK_LEFT_CANARY(left_canary)      (FloatEqual(left_canary, DATA_LEFT_CANARY))

    #define CHECK_RIGHT_CANARY(right_canary)    (FloatEqual(right_canary, DATA_RIGHT_CANARY))

// INTERESTING FACT: we can actually compare doubles in CHECK_CANARY by their bits 
// because left_canary and right_canary are declared in StackInit like this:
// stk->left_canary = DATA_LEFT_CANARY
// stk->right_canary = DATA_RIGHT_CANARY

#endif

#endif
