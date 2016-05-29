/* $Id: run68.h,v 1.1.1.1 2001-05-23 11:22:08 masamic Exp $ */

/*
 * $Log: not supported by cvs2svn $
 * Revision 1.14  1999/12/07  12:47:54  yfujii
 * *** empty log message ***
 *
 * Revision 1.14  1999/11/29  06:24:55  yfujii
 * Some functions' prototypes are added.
 *
 * Revision 1.13  1999/11/08  10:29:30  yfujii
 * Calling convention to eaaccess.c is changed.
 *
 * Revision 1.12  1999/11/08  03:09:41  yfujii
 * Debugger command "wathchc" is added.
 *
 * Revision 1.11  1999/11/01  10:36:33  masamichi
 * Reduced move[a].l routine. and Create functions about accessing effective address.
 *
 * Revision 1.10  1999/11/01  06:23:33  yfujii
 * Some debugging functions are introduced.
 *
 * Revision 1.9  1999/10/29  13:44:04  yfujii
 * Debugging facilities are introduced.
 *
 * Revision 1.8  1999/10/27  03:44:01  yfujii
 * Macro RUN68VERSION is defined.
 *
 * Revision 1.7  1999/10/26  12:26:08  yfujii
 * Environment variable function is drasticaly modified.
 *
 * Revision 1.6  1999/10/26  01:31:54  yfujii
 * Execution history and address trap is added.
 *
 * Revision 1.5  1999/10/25  03:26:27  yfujii
 * Declarations for some flags are added.
 *
 * Revision 1.4  1999/10/20  12:52:10  yfujii
 * Add an #if directive.
 *
 * Revision 1.3  1999/10/20  06:31:09  yfujii
 * Made a little modification for Cygnus GCC.
 *
 * Revision 1.2  1999/10/18  03:24:40  yfujii
 * Added RCS keywords and modified for WIN32 a little.
 *
 */

#define RUN68VERSION "0.08"
#if !defined(_RUN68_H_)
#define _RUN68_H_

#if defined(__GNUC__)
#define __int64 long long
#endif

#if defined(_WIN32) /* for Cygnus GCC */
#if !defined(WIN32)
#define WIN32
#endif
#endif

/*
#undef	TRACE
#undef	FNC_TRACE
*/

#if defined(WIN32)              /* Win32 API��DOS�R�[�����G�~�����[�g����B*/
  #undef DOSX
#endif

#if defined(WIN32)
#include <windows.h>
#endif

#include <stdio.h>
#include <setjmp.h>
#if !defined(WIN32)              /* Win32 API��DOS�R�[�����G�~�����[�g����B*/
#define	TRUE		-1
#define	FALSE		0
#endif
#define	XHEAD_SIZE	0x40		/* X�t�@�C���̃w�b�_�T�C�Y */
#define	HUMAN_HEAD	0x6800		/* Human�̃������Ǘ��u���b�N�ʒu */
#define	FCB_WORK	0x20F00		/* DOSCALL GETFCB�p���[�N�̈� */
#define	HUMAN_WORK	0x21000		/* ���荞�ݏ����擙�̃��[�N�̈� */
#define	ENV_TOP		0x21C00
#define	ENV_SIZE	0x2000
#define	STACK_TOP	ENV_TOP + ENV_SIZE
#define	STACK_SIZE	0x10000		/* 64KB */
#define	MB_SIZE		16
#define	PSP_SIZE	MB_SIZE + 240
#define	PROG_TOP	(STACK_TOP + STACK_SIZE + PSP_SIZE)
#define	NEST_MAX	20
#define	FILE_MAX	20

#define	RAS_INTERVAL	10000	/* ���X�^���荞�݂̊Ԋu */

#define	S_BYTE	0	/* BYTE�T�C�Y */
#define	S_WORD	1	/* WORD�T�C�Y */
#define	S_LONG	2	/* LONG�T�C�Y */

#define	MD_DD	0	/* �f�[�^���W�X�^���� */
#define	MD_AD	1	/* �A�h���X���W�X�^���� */
#define	MD_AI	2	/* �A�h���X���W�X�^�Ԑ� */
#define	MD_AIPI	3	/* �|�X�g�C���N�������g�E�A�h���X���W�X�^�Ԑ� */
#define	MD_AIPD	4	/* �v���f�N�������g�E�A�h���X���W�X�^�Ԑ� */
#define	MD_AID	5	/* �f�B�X�v���[�X�����g�t���A�h���X���W�X�^�Ԑ� */
#define	MD_AIX	6	/* �C���f�b�N�X�t���A�h���X���W�X�^�Ԑ� */
#define	MD_OTH	7	/* ���̑� */

#define	MR_SRT	0	/* ��΃V���[�g */
#define	MR_LNG	1	/* ��΃����O */
#define	MR_PC	2	/* �v���O�����J�E���^���� */
#define	MR_PCX	3	/* �C���f�b�N�X�t���v���O�����J�E���^���� */
#define	MR_IM	4	/* �C�~�f�B�G�C�g�f�[�^ */

/* Replace from MD_xx, MR_xx */
#define	EA_DD	0	/* �f�[�^���W�X�^���� */
#define	EA_AD	1	/* �A�h���X���W�X�^���� */
#define	EA_AI	2	/* �A�h���X���W�X�^�Ԑ� */
#define	EA_AIPI	3	/* �|�X�g�C���N�������g�E�A�h���X���W�X�^�Ԑ� */
#define	EA_AIPD	4	/* �v���f�N�������g�E�A�h���X���W�X�^�Ԑ� */
#define	EA_AID	5	/* �f�B�X�v���[�X�����g�t���A�h���X���W�X�^�Ԑ� */
#define	EA_AIX	6	/* �C���f�b�N�X�t���A�h���X���W�X�^�Ԑ� */
#define	EA_SRT	7	/* ��΃V���[�g */
#define	EA_LNG	8	/* ��΃����O */
#define	EA_PC	9	/* �v���O�����J�E���^���� */
#define	EA_PCX	10	/* �C���f�b�N�X�t���v���O�����J�E���^���� */
#define	EA_IM	11	/* �C�~�f�B�G�C�g�f�[�^ */

/* �I���\�����A�h���X�g�ݍ��킹          fedc ba98 7654 3210 */
#define EA_All			0x0fff	/* 0000 1111 1111 1111 */
#define EA_Control		0x07e4	/* 0000 0111 1110 0100 */
#define EA_Data			0x0ffd	/* 0000 1111 1111 1101 */
#define EA_PreDecriment		0x01f4	/* 0000 0001 1111 0100 */
#define EA_PostIncrement	0x07ec	/* 0000 0111 1110 1100 */
#define EA_VariableData		0x00fd	/* 0000 0000 1111 1101 */
#define EA_Variable		0x01ff	/* 0000 0001 1111 1111 */
#define EA_VariableMemory	0x01fc	/* 0000 0001 1111 1100 */



#include <stdbool.h>
typedef bool *BOOL;


/* EaAccess.c */
BOOL get_data_at_ea(int AceptAdrMode, int mode, int reg, int size, long *data) ;
BOOL set_data_at_ea(int AceptAdrMode, int mode, int reg, int size, long data) ;
BOOL get_ea(long save_pc, int AceptAdrMode, int mode, int reg, long *data) ;

#define	CCR_X_ON()	sr |= 0x0010
#define	CCR_X_OFF()	sr &= 0xFFEF
#define	CCR_X_REF()	(sr & 0x0010)
#define	CCR_N_ON()	sr |= 0x0008
#define	CCR_N_OFF()	sr &= 0xFFF7
#define	CCR_N_REF()	(sr & 0x0008)
#define	CCR_Z_ON()	sr |= 0x0004
#define	CCR_Z_OFF()	sr &= 0xFFFB
#define	CCR_Z_REF()	(sr & 0x0004)
#define	CCR_V_ON()	sr |= 0x0002
#define	CCR_V_OFF()	sr &= 0xFFFD
#define	CCR_V_REF()	(sr & 0x0002)
#define	CCR_C_ON()	sr |= 0x0001
#define	CCR_C_OFF()	sr &= 0xFFFE
#define	CCR_C_REF()	(sr & 0x0001)
#define	SR_S_ON()	sr |= 0x2000
#define	SR_S_OFF()	sr &= 0xDFFF
#define	SR_S_REF()	(sr & 0x2000)
#define	SR_T_REF()	(sr & 0x8000)

typedef	unsigned char	UChar ;
typedef	unsigned short	UShort ;
typedef	unsigned long	ULong ;


typedef struct	{
#if defined(WIN32)
    HANDLE   fh ;
#else
	FILE     *fh ;
#endif
	unsigned date ;
	unsigned time ;
	short    mode ;
	char     nest ;
	char     name [ 89 ] ;
} FILEINFO ;

typedef struct	{
	char	env_lower ;
	char	trap_emulate ;
	char	pc98_key ;
	char	io_through ;
} INI_INFO ;

/* �f�o�b�O�p�Ɏ��s�������߂̏���ۑ����Ă����\���� */
typedef struct {
    long    pc;
    /* �{���͑S���W�X�^��ۑ����Ă��������B*/
    unsigned short code; /* OP�R�[�h */
    long    rmem;        /* READ���������� */
    char    rsize;       /* B/W/L or N(READ�Ȃ�) movem�̏ꍇ�͍Ō�̈�� */
    long    wmem;        /* WRITE���������� */
    char    wsize;       /* B/W/L or N(WRITE�Ȃ�) movem�̏ꍇ�͍Ō�̈�� */
    char    mnemonic[64]; /* �j�[���j�b�N(�ł����) */
} EXEC_INSTRUCTION_INFO;

/* run68.c */
 /* �t���O */
extern BOOL func_trace_f;
extern BOOL trace_f;
extern long trap_pc;
extern jmp_buf jmp_when_abort;
extern unsigned short cwatchpoint;
/* �W�����͂̃n���h�� */


typedef void *HANDLE;


extern HANDLE stdin_handle;

/* ���ߎ��s��� */
extern EXEC_INSTRUCTION_INFO OP_info;
void	term( int ) ;

/* getini.c */
void	read_ini( char *path, char *prog ) ;
void	readenv_from_ini(char *path);

/* load.c */
FILE	*prog_open(char *, int ) ;
long	prog_read( FILE *, char *, long, long *, long *, int ) ;
int	make_psp( char *, long, long, long, long ) ;

/* exec.c */
int	prog_exec( void ) ;
int	get_cond( char ) ;
void	err68( char * ) ;
void    err68a(char *mes, char *file, int line);
void    err68b(char *mes, long pc, long ppc);
void	inc_ra( char, char ) ;
void	dec_ra( char, char ) ;
void	text_color( short ) ;
long	get_locate( void ) ;
void    OPBuf_insert(const EXEC_INSTRUCTION_INFO *op);
void    OPBuf_clear();
int     OPBuf_numentries();
const   EXEC_INSTRUCTION_INFO *OPBuf_getentry(int no);
void    OPBuf_display(int n);

/* calc.c */
long add_long(long src, long dest, int size);
long sub_long(long src, long dest, int size);

/* mem.c */
long	idx_get( void ) ;
long	imi_get ( char ) ;
long	mem_get ( long, char ) ;
void	mem_set ( long, long, char ) ;

/* doscall.c */
int	dos_call( UChar ) ;

/* iocscall.c */
int	iocs_call( void ) ;

/* key.c */
void	get_fnckey( int, char * ) ;
void	put_fnckey( int, char * ) ;
UChar	cnv_key98( UChar ) ;

/* line?.c */
int	line0( char * ) ;
int	line2( char * ) ;
int	line4( char * ) ;
int	line5( char * ) ;
int	line6( char * ) ;
int	line7( char * ) ;
int	line8( char * ) ;
int	line9( char * ) ;
int	lineb( char * ) ;
int	linec( char * ) ;
int	lined( char * ) ;
int	linee( char * ) ;
int	linef( char * ) ;

/* eaaccess.c */
BOOL get_data_at_ea_noinc(int AceptAdrMode, int mode, int reg, int size, long *data) ;

/* debugger.c */
typedef enum {
    RUN68_COMMAND_BREAK,  /* �u���[�N�|�C���g�̐ݒ� */
    RUN68_COMMAND_CLEAR,  /* �u���[�N�|�C���g�̃N���A */
    RUN68_COMMAND_CONT,   /* ���s�̌p�� */
    RUN68_COMMAND_DUMP,   /* ���������_���v���� */
    RUN68_COMMAND_HELP,   /* �f�o�b�K�̃w���v */
    RUN68_COMMAND_HISTORY, /* ���߂̎��s���� */
    RUN68_COMMAND_LIST,   /* �f�B�X�A�Z���u�� */
    RUN68_COMMAND_NEXT,   /* STEP�Ɠ����B�������A�T�u���[�`���ďo���̓X�L�b�v */
    RUN68_COMMAND_QUIT,   /* run68���I������ */
    RUN68_COMMAND_REG,    /* ���W�X�^�̓��e��\������ */
    RUN68_COMMAND_RUN,    /* �������������ăv���O�������s */
    RUN68_COMMAND_SET,    /* �������ɒl���Z�b�g���� */
    RUN68_COMMAND_STEP,   /* �ꖽ�ߕ��X�e�b�v���s */
    RUN68_COMMAND_WATCHC, /* ���߃E�H�b�` */
    RUN68_COMMAND_NULL,   /* �R�}���h�ł͂Ȃ�(�ړ��֎~) */
    RUN68_COMMAND_ERROR   /* �R�}���h�G���[(�ړ��֎~) */
} RUN68_COMMAND;

RUN68_COMMAND debugger(BOOL running);

/* conditions.c */
void general_conditions(long dest, int size);
void add_conditions(long src , long dest, long result, int size, BOOL zero_flag);
void cmp_conditions(long src , long dest, long result, int size);
void sub_conditions(long src , long dest, long result, int size, BOOL zero_flag);
void neg_conditions(long dest, long result, int size, BOOL zero_flag);
void check(char *mode, long src, long dest, long result, int size, short before);

#ifdef	MAIN
	FILEINFO finfo [ FILE_MAX ] ;	/* �t�@�C���Ǘ��e�[�u�� */
	INI_INFO ini_info ;		/* ini�t�@�C���̓��e */
	char	size_char [ 3 ] = { 'b', 'w', 'l' } ;
	long	ra [ 8 ] ;	/* �A�h���X���W�X�^ */
	long	rd [ 8 + 1 ] ;	/* �f�[�^���W�X�^ */
	long	usp ;		/* USP */
	long	pc ;		/* �v���O�����J�E���^ */
	short	sr ;		/* �X�e�[�^�X���W�X�^ */
	char	*prog_ptr ;	/* �v���O���������[�h�����������ւ̃|�C���^ */
	int	trap_count ;	/* ���荞�ݏ������Ȃ�O */
	long	superjsr_ret ;	/* DOSCALL SUPER_JSR�̖߂�A�h���X */
	long	psp [ NEST_MAX ] ;	/* PSP */
	long	nest_pc [ NEST_MAX ] ;	/* �e�v���Z�X�ւ̖߂�A�h���X��ۑ� */
	long	nest_sp [ NEST_MAX ] ;	/* �e�v���Z�X�̃X�^�b�N�|�C���^��ۑ� */
	char	nest_cnt ;	/* �q�v���Z�X���N�����邽�тɁ{�P */
	long	mem_aloc ;	/* ���C���������̑傫�� */
#else
	extern	FILEINFO finfo [ FILE_MAX ] ;
	extern	INI_INFO ini_info ;
	extern	char	size_char [ 3 ] ;
	extern	long	ra [ 8 ] ;
	extern	long	rd [ 8 + 1 ] ;
	extern	long	usp ;
	extern	long	pc ;
	extern	short	sr ;
	extern	char	*prog_ptr ;
	extern	int	trap_count ;
	extern	long	superjsr_ret ;
	extern	long	psp [ NEST_MAX ] ;
	extern	long	nest_pc [ NEST_MAX ] ;
	extern	long	nest_sp [ NEST_MAX ] ;
	extern	char	nest_cnt ;
	extern	long	mem_aloc ;
#endif

/*
�O���C�����߁Fmovep, addi, subi, cmpi, andi, eori, ori, btst, bset, bclr, bchg
�P���C�����߁Fmove.b
�Q���C�����߁Fmove.l, movea.l
�R���C�����߁Fmove.w, movea.w
�S���C�����߁Fmoveccr, movesr, moveusp, movem, swap, lea, pea, link, unlk,
�@�@�@�@�@�@�@clr, ext, neg, negx, tst, tas, not, nbcd, jmp, jsr, rtr, rts,
�@�@�@�@�@�@�@trap, trapv, chk, rte, reset, stop, nop
�T���C�����߁Faddq, subq, dbcc, scc
�U���C�����߁Fbcc, bra, bsr
�V���C�����߁Fmoveq
�W���C�����߁Fdivs, divu, or, sbcd
�X���C�����߁Fsub, suba, subx
�a���C�����߁Fcmp, cmpa, cmpm, eor
�b���C�����߁Fexg, muls, mulu, and, abcd
�c���C�����߁Fadd, adda, addx
�d���C�����߁Fasl, asr, lsl, lsr, rol, ror, roxl, roxr
*/

#endif /* !defined(_RUN68_H_) */
