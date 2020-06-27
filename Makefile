EXE=FloPi
LIBS=wiringPi
SRC=main.cpp Drive.cpp

.PHONY: clean

$(EXE): $(SRC)
	g++ -o $(EXE) $(SRC) -l $(LIBS) -g

clean:
	rm -rf $(EXE)
