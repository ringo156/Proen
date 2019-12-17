CC :=g++
# インクルードファイル等
CFLAGS :=`pkg-config opencv --cflags` `pkg-config opencv --libs`
LDFLAGS :=
# ディレクトリ内の全てのC++ファイルをコンパイル
SOURCES :=$(wildcard *.cpp)
# C++ファイルの.cppをとったものを実行ファイルの名前とする
EXECUTABLE :=$(SOURCES:.cpp=)

all:$(EXECUTABLE)

$(EXECUTABLE):$(SOURCES)
	$(CC) -O3 -mtune=native -march=native -mfpmath=both $< $(LDFLAGS) $(CFLAGS) -o $@
clean:
	rm -rf $(EXECUTABLE)
