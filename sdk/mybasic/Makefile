TARGET := libmybasic.a
OBJS := my_basic.o extensions.o ext_btn.o ext_gfx.o ext_intliteral_strtol.o
CFLAGS += -I..

$(TARGET): $(OBJS)
	$(PREFIX)ar rc $(TARGET) $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)