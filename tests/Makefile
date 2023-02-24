LIB=kut

CFLAGS = -Wall -rdynamic
C_TESTS = $(wildcard src/*.c)
H_TESTS = $(wildcard include/*.h)
TESTS = $(C_TESTS) $(H_TESTS)

OBJECTS = $(subst src/$(LIB),obj,$(patsubst %.c,%.o,$(wildcard ../src/$(LIB)/*.c)))

bin/tests : $(TESTS) ../lib/lib$(LIB).a
	gcc $(CFLAGS) $(C_TESTS) -o bin/tests -Iinclude \
		-I../include -L../lib -l$(LIB) \
		-lgc -lpthread -lm

../lib/lib$(LIB).a : $(OBJECTS)
	if [ ! -e ../lib ];then mkdir ../lib;fi
	ar rcs ../lib/lib$(LIB).a $(OBJECTS)

../obj/%.o : ../src/$(LIB)/%.c ../include/$(LIB)/*.h
	if [ ! -e ../obj ];then mkdir ../obj;fi
	gcc $(CFLAGS) -c $< -o $@ -I../include
