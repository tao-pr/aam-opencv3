# Compiler
CC :=g++ -std=c++11

# Where and what to include dependencies
LDFLAGS      := -L${OPENCV_DIR}/lib -lm
INCLUDE      := -I${OPENCV_DIR}/include -I headers
INCLUDE_TEST := -I${OPENCV_DIR}/include -I headers test
LIB          := -lopencv_core -lopencv_imgproc -lopencv_video \
                -lopencv_videoio -lopencv_tracking -lopencv_optflow \
                -lopencv_features2d -lopencv_ml -lopencv_highgui \
                -lopencv_imgcodecs -lopencv_objdetect \
                -lopencv_xfeatures2d

# Take all CPP source files
BUILD_DIR     := build
BUILDTEST_DIR := buildtest
SRC_DIR       := src
TEST_DIR      := test
SOURCES       := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS          := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.cpp=.o))
TARGET        := bin/aam-opencv3

#$(CC) $(INCLUDE) $(LDFLAGS) $(LIB) -o $(TARGET) $(OBJS)

all:$(TARGET)

$(TARGET): $(OBJS)
	@echo "••••••••••••••••••••••••••••••"
	@echo "Link Target : $(OBJS)"
	@echo "••••••••••••••••••••••••••••••"
	@mkdir -p bin/
	$(CC) $(LDFLAGS) $(LIB) $^ -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "••••••••••••••••••••••••••••••"
	@echo "Build : $@"
	@echo "••••••••••••••••••••••••••••••"
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCLUDE) -c -o $@ $<

clean:
	rm -r $(BUILD_DIR)/*.o $(TARGET)
	rm -r $(BUILDTEST_DIR)/*.o $(TEST)
	@make
	@make test