CC=clang
LD=clang
CFLAGS=-g 
LDFLAGS=-g
TARGET=matrix-mul
OBJS=main.o matrix_operation.o average_image_vector.o pgm.o svd.o principal_component.o filelist.o

.PHONY: build

build: $(OBJS) $(TARGET)

clean:
	rm $(TARGET) ./*.o
%.o:%c
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) 
