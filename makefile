MacroProcessing : MacroProcessing.o
	gcc -g MacroProcessing.o -o MacroProcessing
MacroProcessing.o : MacroProcessing.c
	gcc -c MacroProcessing.c -o MacroProcessing.o
