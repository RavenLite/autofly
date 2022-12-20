# compiler:
CC = gcc
# source files:
VPATH += src/
# compiler flags: 
CFLAGS += -fcommon
# object files to build:
#PROJ_OBJ += test.o
 PROJ_OBJ += octoMap.o
 PROJ_OBJ += octoTree.o
 PROJ_OBJ += octoNode.o
 PROJ_OBJ += octoNodeSet.o
# header files
INCLUDES += -I./interface
# the build target executable:
TARGET = octoMap

all: $(PROJ_OBJ)
	echo $(INCLUDES)
	$(CC) $(CFLAGS) -o $(TARGET) -I /Users/raven/workspace/autofly/interface/ $(PROJ_OBJ)
