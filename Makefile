CC=gcc
CFLAGS=-lpthread
INCLUDE_DIR=include
SOURCE_DIR=src
BUILD_DIR=bin
SERVER_TARGET=$(BUILD_DIR)/server
CLIENT_TARGET=$(BUILD_DIR)/client
SERVER_OBJS=$(SOURCE_DIR)/server.o $(INCLUDE_DIR)/server.o $(INCLUDE_DIR)/chat.o
CLIENT_OBJS=$(SOURCE_DIR)/client.o $(INCLUDE_DIR)/client.o $(INCLUDE_DIR)/chat.o

$(SERVER_TARGET): $(SERVER_OBJS) $(CLIENT_TARGET)
	$(CC) -o $@ $(SERVER_OBJS) $(CFLAGS)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) -o $@ $(CLIENT_OBJS) $(CFLAGS)

$(SOURCE_DIR)/server.o: $(SOURCE_DIR)/server.c
	$(CC) -c -o $@ $(SOURCE_DIR)/server.c

$(INCLUDE_DIR)/server.o: $(INCLUDE_DIR)/server.c
	$(CC) -c -o $@ $(INCLUDE_DIR)/server.c

$(SOURCE_DIR)/client.o: $(SOURCE_DIR)/client.c
	$(CC) -c -o $@ $(SOURCE_DIR)/client.c

$(INCLUDE_DIR)/client.o: $(INCLUDE_DIR)/client.c
	$(CC) -c -o $@ $(INCLUDE_DIR)/client.c

$(INCLUDE_DIR)/chat.o: $(INCLUDE_DIR)/chat.c
	$(CC) -c -o $@ $(INCLUDE_DIR)/chat.c

clean: 
	rm -rf $(BUILD_DIR)/* $(SOURCE_DIR)/*.o $(INCLUDE_DIR)/*.o
