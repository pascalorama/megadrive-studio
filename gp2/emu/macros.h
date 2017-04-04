
#ifndef _MACROS_H_
#define _MACROS_H_


#define READ_BYTE(BASE, ADDR) (BASE)[(ADDR)^1]
#define READ_WORD(BASE, ADDR) (((BASE)[(ADDR)+1]<<8) |          \
                              (BASE)[(ADDR)])
#define READ_LONG(BASE, ADDR) (((BASE)[(ADDR)+1]<<24) |         \
                               ((BASE)[(ADDR)+0]<<16) |      \
                               ((BASE)[(ADDR)+3]<<8) |       \
                                (BASE)[(ADDR)+2])

#define WRITE_BYTE(BASE, ADDR, VAL) (BASE)[(ADDR)^1] = (VAL)&0xff
#define WRITE_WORD(BASE, ADDR, VAL) (BASE)[(ADDR)+1] = ((VAL)>>8) & 0xff;       \
                                    (BASE)[ADDR] = (VAL)&0xff
#define WRITE_LONG(BASE, ADDR, VAL) (BASE)[(ADDR)+1] = ((VAL)>>24) & 0xff;      \
                                    (BASE)[(ADDR)+0] = ((VAL)>>16)&0xff;    \
                                    (BASE)[(ADDR)+3] = ((VAL)>>8)&0xff;     \
                                    (BASE)[(ADDR)+2] = (VAL)&0xff



#if 0

#define READ_BYTE(BASE, ADDR) (BASE)[ADDR]
#define READ_WORD(BASE, ADDR) (((BASE)[ADDR]<<8) |			\
							  (BASE)[(ADDR)+1])
#define READ_LONG(BASE, ADDR) (((BASE)[ADDR]<<24) |			\
							  ((BASE)[(ADDR)+1]<<16) |		\
							  ((BASE)[(ADDR)+2]<<8) |		\
							  (BASE)[(ADDR)+3])

#define WRITE_BYTE(BASE, ADDR, VAL) (BASE)[ADDR] = (VAL)&0xff
#define WRITE_WORD(BASE, ADDR, VAL) (BASE)[ADDR] = ((VAL)>>8) & 0xff;		\
									(BASE)[(ADDR)+1] = (VAL)&0xff
#define WRITE_LONG(BASE, ADDR, VAL) (BASE)[ADDR] = ((VAL)>>24) & 0xff;		\
									(BASE)[(ADDR)+1] = ((VAL)>>16)&0xff;	\
									(BASE)[(ADDR)+2] = ((VAL)>>8)&0xff;		\
									(BASE)[(ADDR)+3] = (VAL)&0xff
#endif


#endif /* _MACROS_H_ */

