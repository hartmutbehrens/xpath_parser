CC=gcc
 
CFLAGS= -c -w
LDFLAGS= -lm

all :  parser 

parser: autoPilot.o arrayList.o binaryExpr.o bookMark.o unaryExpr.o funcExpr.o filterExpr.o literalExpr.o unionExpr.o pathExpr.o locationPathExpr.o numberExpr.o variableExpr.o elementFragmentNs.o helper.o intHash.o lex.yy.o l8.tab.o vtdGen.o vtdNav.o textIter.o transcoder.o decoder.o contextBuffer.o indexHandler.o fastIntBuffer.o fastLongBuffer.o XMLChar.o parser.o
	${CC} ${LDFLAGS} autoPilot.o arrayList.o binaryExpr.o bookMark.o unaryExpr.o funcExpr.o filterExpr.o literalExpr.o unionExpr.o pathExpr.o locationPathExpr.o numberExpr.o variableExpr.o elementFragmentNs.o helper.o intHash.o lex.yy.o l8.tab.o vtdGen.o vtdNav.o textIter.o transcoder.o decoder.o contextBuffer.o indexHandler.o fastIntBuffer.o fastLongBuffer.o XMLChar.o parser.o -o parser

clean:
	-rm *.o
	
parser.o : parser.c
	${CC} ${CFLAGS} parser.c
	
arrayList.o : arrayList.c
	${CC} ${CFLAGS} arrayList.c
	
autoPilot.o : autoPilot.c
	${CC} ${CFLAGS} autoPilot.c
	
binaryExpr.o : binaryExpr.c
	${CC} ${CFLAGS} binaryExpr.c
	
bookMark.o : bookMark.c
	${CC} ${CFLAGS} bookMark.c
	
unaryExpr.o : unaryExpr.c
	${CC} ${CFLAGS} unaryExpr.c
	
unionExpr.o : unionExpr.c
	${CC} ${CFLAGS} unionExpr.c
	
elementFragmentNs.o  : elementFragmentNs.c
	${CC} ${CFLAGS} elementFragmentNs.c
   
fastIntBuffer.o : fastIntBuffer.c
	${CC} ${CFLAGS} fastIntBuffer.c

fastLongBuffer.o : fastLongBuffer.c
	${CC} ${CFLAGS} fastLongBuffer.c

filterExpr.o : filterExpr.c
	${CC} ${CFLAGS} filterExpr.c
	
funcExpr.o : funcExpr.c
	${CC} ${CFLAGS} funcExpr.c
	
literalExpr.o : literalExpr.c
	${CC} ${CFLAGS} literalExpr.c
	
locationPathExpr.o : locationPathExpr.c
	${CC} ${CFLAGS} locationPathExpr.c
	
numberExpr.o : numberExpr.c
	${CC} ${CFLAGS} numberExpr.c

variableExpr.o : variableExpr.c
	${CC} ${CFLAGS} variableExpr.c
	
pathExpr.o : pathExpr.c
	${CC} ${CFLAGS} pathExpr.c
	
intHash.o : intHash.c
	${CC} ${CFLAGS} intHash.c

vtdGen.o : vtdGen.c
	${CC} ${CFLAGS} vtdGen.c
	
vtdNav.o : vtdNav.c
	${CC} ${CFLAGS} vtdNav.c

contextBuffer.o : contextBuffer.c
	${CC} ${CFLAGS} contextBuffer.c 

decoder.o : decoder.c
	${CC} ${CFLAGS} decoder.c
	
helper.o : helper.c
	${CC} ${CFLAGS} helper.c
	
indexHandler.o	: indexHandler.c
	${CC} ${CFLAGS} indexHandler.c
	
lex.yy.o : lex.yy.c
	${CC} ${CFLAGS} lex.yy.c
	
l8.tab.o : l8.tab.c
	${CC} ${CFLAGS} l8.tab.c

textIter.o : textIter.c
	${CC} ${CFLAGS} textIter.c
	
transcoder.o : transcoder.c
	${CC} ${CFLAGS} transcoder.c	

XMLChar.o : XMLChar.c
	${CC} ${CFLAGS} XMLChar.c	