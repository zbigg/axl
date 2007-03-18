/*
* Header:	arch.h
*
* Id:           $Id: arch.h 1.2 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	header for 'simple' architecture
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/
#ifndef __arch_h_
#define __arch_h_

/**@name Processor instructions
    Instructions that are understand to processor function.
*/
/*@{ */
/** No operation.
 */
#define INSTR_NOP		0

/** Exit thread.
 */
#define INSTR_EXIT		1

/** Load a register value.

    Copies value from specified register into the stack.

    Stack: +1

	OUTPUT <value>

    Complexity: 1
*/
#define INSTR_LOAD_REG		41

/** Load a immediate value.

    Loads a immediate value into the stack.
    Value from instruction is copied into the stack.
    It can be : integer, floating point or string.

    Paramater: register number
    Stack: +1

	OUTPUT <value>

    Complexity: 1
*/
#define INSTR_LOAD_VAL		2

/** Load reference to variable.

    Puts a reference to named variable visible
    in curent context.

    Paramater:
	- name of variable(string)
	- builtin function name : builtin:<module_name>:<symbol_name>

    Stack: +1

	OUTPUT <reference to variable>


    Complexity: 1
*/
#define INSTR_LOAD_VAR		3

/** Return reference of array element.

    Stack: -1

	INPUT <table object> <index>

	OUTPUT <reference to variable>

    Complexity:
    Should be constant, but in
    future this operation can be overloaded
    with user function.
*/
#define INSTR_INDEX		4

/** Return reference of object field.

    Paramater: name of field

    Stack: 0

	INPUT <record object>

	OUTPUT <reference to variable>

    Complexity: 1
*/
#define INSTR_FIELD		5

/** Store value into var name.

    Stack: -1

	INPUT <reference to variable> <value>

	OUTPUT <reference to variable>

    Complexity: 1
*/
#define INSTR_STORE		6

/** Store value into register.

    Parameter: register number

    Stack: -1
	INPUT <value>

	OUTPUT

    Complexity: 1
*/
#define INSTR_STORE_REG 	33

/** Store tith type cast(UNUSED)(NOT IMPLEMENTED).

    Stores value in place where the reference points
    without changing its [variable] type.

    Stack: -1

	INPUT <reference to variable> <value>

	OUTPUT <reference to variable>

    Complexity: 1
*/
#define INSTR_STORE_CAST	7

/* see proc.c */
#define INSTR_STD_OPER_BI	8
#define INSTR_STD_OPER_UN	9

#define INSTR_EQUAL		21	/* if condition returns true */
#define INSTR_NOT_EQUAL 	22
#define INSTR_LESS		23
#define INSTR_GREATER		24
#define INSTR_LESS_EQUAL	25
#define INSTR_GREATER_EQUAL	26
#define INSTR_CMP		27	/* compare values; what is it for ? */
#define INSTR_JMP		28	/* jump with no condition */
#define INSTR_TRUE		29	/* jump if true ( nonzero ) */
#define INSTR_FALSE		30	/* jump if false ( zero ) */
#define INSTR_DUP		31	/* push a duplicate of value */
					/* from top of stack */
#define INSTR_DROP		32	/* pop value from stack -> NULL */
/* 33 used */
#define INSTR_CALL		34
#define INSTR_RET		35
/** Store state of stack(UNUSED)(NOT IMPLEMENTED).
*/
#define INSTR_MARK		36
/** Restore state of stack(UNUSED)(NOT IMPLEMENTED).
*/
#define INSTR_RELEASE		37


/** Echo expression.

    Echo value from stack in human readable format.

    Stack: -1

	INPUT <value>

    Complexity: 1
*/
#define INSTR_ECHO		39
/** Get value from body of stack.
    Parameter : n: int index of requested value:
    1 - duplicate top of stack
    n>1 - duplicate value from -n

    Or value if n==0 value on stack.
*/
#define INSTR_GET		40
/** Load value of function parameter.
    parameter: n
	0 based number of requested parameter
*/
#define INSTR_PARAM		42
#define INSTR_COPY		43
#define INSTR_DEREF		44
/** Load context.
    Integer value is parameter X.
    if X == 0 it enters a context created from object on stack.
    if X == 1 it makes object on stack a global (under global context)
*/
#define INSTR_ENTER_CTX 	45
#define INSTR_LEAVE_CTX 	46


/*@} */
#define INSTR_FREE_VAR		47
#define INSTR_VALID		48
#define INSTR_BOOL		49
#define INSTR_LAST		49

#define REG_CASE		40
#define REG_RETURN		41
#define REG_STACK		42
#define REG_THIS		43
#define REG_PARAM_PTR		44
#define REG_PARAM_COUNT 	45
#define REG_IP_PTR		46
#define REG_CONTEXT_PTR		47
#define REG_SP		REG_STACK
#define REG_PP 		REG_PARAM_PTR
#define REG_CX		REG_CONTEXT_PTR
#define REG_PC		REG_PARAM_COUNT


/* C Extern Identifiers. */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* force single include of file */
