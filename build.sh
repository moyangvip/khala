#!/bin/sh
set -x

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-./build}
BUILD_TYPE=${BUILD_TYPE:-Debug}
BUILD_NO_EXAMPLES=${BUILD_NO_EXAMPLES:-0}
if [ $BUILD_NO_EXAMPLES -eq 0 ];then
  echo $SOURCE_DIR
  cp -rf $SOURCE_DIR/Khala/include/ $SOURCE_DIR/examples/HelloKhala/
fi
mkdir -p $BUILD_DIR/$BUILD_TYPE \
  && cd $BUILD_DIR/$BUILD_TYPE \
  && cmake \
           -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
           -DCMAKE_BUILD_NO_EXAMPLES=$BUILD_NO_EXAMPLES \
           $SOURCE_DIR \
  && make $*
if [ $? -eq 0 ] && [ -e ./lib/libKhala.a ];then
  if [ $BUILD_NO_EXAMPLES -eq 0 ];then  
    if [ -L $SOURCE_DIR/examples/HelloKhala/libs/libKhala.a ] \
  	|| [ -e $SOURCE_DIR/examples/HelloKhala/libs/libKhala.a ];then
      rm -f $SOURCE_DIR/examples/HelloKhala/libs/libKhala.a
    fi
    ln -s ./lib/libKhala.a $SOURCE_DIR/examples/HelloKhala/libs/
    if [ $? -eq 0 ];then
      echo "Build success!"
    fi
  else
    echo "Build success!"
  fi
fi

