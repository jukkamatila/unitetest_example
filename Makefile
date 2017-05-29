APP=modbus_ascii

ifndef GTEST_DIR
GTEST_DIR=${HOME}/googletest/googletest
else
GTEST_DIR=${GTEST_DIR}
endif

tests: libgtest.a modbus_test

$(APP): main.o $(APP).o
	gcc -o $(APP) main.c $(APP).o

main.o:	main.c
	gcc -c main.c
$(APP).o: $(APP).c
	gcc -c $(APP).c

libgtest.a:
	g++ -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
modbus_test: GTest_case.c $(APP).c modbus.h
	g++ -isystem $(GTEST_DIR)/include -pthread GTest_case.c $(APP).c Gtest_main.c libgtest.a -o modbus_test

clean:
	rm -f *.o $(APP) modbus_test
