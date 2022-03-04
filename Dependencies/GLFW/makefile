CRSC=$(notdir src/er.c) op.c
TEST=$(CRSC:.c=.o )
build: src/er.o
	$(info $(TEST))

%.o: %.cpp
	g++ -c $< -o out/$(notdir $@)
	$(info here)