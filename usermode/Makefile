NAME        := vision
CC          := g++
SRCS_DIR    := ./srcs
OBJS_DIR    := .objs
MAIN        := main.cpp
LIB_PATH    := libs/dma-library/dma-library
LIB_NAME    := dma-library.a

CFLAGS  :=                          \
    -DLINUX                         \
    -I includes                     \
    -I $(LIB_PATH)/libs/includes    \
    -I $(LIB_PATH)/includes         \

LDFLAGS :=                              \
    -L$(LIB_PATH)/libs/binary         	\
    -Wl,-rpath=$(LIB_PATH)/libs/binary  \
    -l:leechcore.so                     \
    -l:vmm.so                           \
    -lfmt                               \

SRCS :=                     \
    Socket.cpp              \
    WebRadar.cpp            \
    Game.cpp    \
    Player.cpp              \

SRCS_OBJS := $(patsubst %.cpp,$(OBJS_DIR)/%.o,$(SRCS))

$(OBJS_DIR)/%.o:$(SRCS_DIR)/%.cpp
	@mkdir -vp $(dir $@)
	$(CC) \
		$(CFLAGS) \
		-MMD \
		-MP \
		-o $@ \
		-c $< \

all: $(NAME)

-include  $(SRCS_OBJS:.o=.d)

$(NAME): $(SRCS_OBJS)
	$(CC) \
		$^ \
		$(MAIN) \
		$(LIB_PATH)/$(LIB_NAME) \
		$(LDFLAGS) \
		$(CFLAGS) \
		-o $(NAME) \

clean:
	rm -rf *.dSYM
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
