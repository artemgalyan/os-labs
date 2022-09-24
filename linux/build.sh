NUMBER_DIR="number"
VECTOR_DIR="vector"
DEMO_DIR="demo_project"
CPP_STD="c++20"

INCLUDE_NUMBER_HEADERS="-I"$NUMBER_DIR
INCLUDE_VECTOR_HEADERS="-I"$VECTOR_DIR

BUILD_DIR="build-output"
TEMP_DIR=$BUILD_DIR"/temp"

c++ -c $NUMBER_DIR/number.cpp -std=$CPP_STD -fpic -o $TEMP_DIR/libnumber.o
ar rvs $TEMP_DIR/libnumber.a $TEMP_DIR/libnumber.o

echo ">>>> libnumber.a is built"

c++ -c $VECTOR_DIR/vector.cpp $INCLUDE_NUMBER_HEADERS -fpic -std=$CPP_STD -o $TEMP_DIR/libvector.o
c++ -shared $TEMP_DIR/libvector.o $TEMP_DIR/libnumber.a -o $TEMP_DIR/libvector.so

echo ">>>> libvector.so is built"

c++ -c $DEMO_DIR/main.cpp $INCLUDE_NUMBER_HEADERS $INCLUDE_VECTOR_HEADERS -std=$CPP_STD -o $TEMP_DIR/demo_proj.o

export LD_PRELOAD=$TEMP_DIR/libvector.so

c++ -o $BUILD_DIR/demo_proj $TEMP_DIR/demo_proj.o $TEMP_DIR/libnumber.a $TEMP_DIR/libvector.so

echo ">>>> running demo_proj"
./$BUILD_DIR/demo_proj