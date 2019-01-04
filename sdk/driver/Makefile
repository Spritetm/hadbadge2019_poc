TARGET := libdriver.a
OBJS := gfxdriver.o buttons.o loader.o
CFLAGS += -I..

$(TARGET): $(OBJS)
	$(PREFIX)ar rc $(TARGET) $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)