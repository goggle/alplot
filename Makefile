CC = gcc
CFLAGS = -c -Wall -Wextra -g
LDFLAGS = -lm `pkg-config --cflags --libs cairo`

test3: color.o alGraph.o alFigure.o alCairo.o alCore.o test3.o
	$(CC) $(LDFLAGS)  color.o alCore.o alFigure.o alDraw.o alCairo.o alGraph.o test3.o -o test3

color.o: src/color.c
	$(CC) $(CFLAGS) src/color.c
alCore.o: src/alCore.c
	$(CC) $(CFLAGS) src/alCore.c
#alLine.o: src/alLine.c color.o alCore.o
#	$(CC) $(CFLAGS) src/alLine.c
alGraph.o: src/alGraph.c alCore.o color.o
	$(CC) $(CFLAGS) src/alGraph.c
alFigure.o: src/alFigure.c alGraph.o alCore.o
	$(CC) $(CFLAGS) src/alFigure.c
alDraw.o: src/alDraw.c alFigure.o alCore.o
	$(CC) $(CFLAGS) src/alDraw.c
alCairo.o: src/alCairo.c alDraw.o alCore.o alGraph.o alFigure.o color.o
	$(CC) $(CFLAGS) $(LDFLAGS) src/alCairo.c
test3.o: src/test3.c alGraph.o alFigure.o color.o
	$(CC) $(CFLAGS) $(LDFLAGS) src/test3.c

clean:
	rm -f color.o alCore.o alLine.o alGraph.o alFigure.o alDraw.o alCairo.o test3.o test3 plot.pdf
