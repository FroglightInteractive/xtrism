release: prep-release
	+cmake --build build --config Release

prep-release:
	+cmake -S . -B build -DCMAKE_BUILD_TYPE=Release  

debug: prep-debug
	+cmake --build build-debug --config Debug

prep-debug:
	+cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug 

clean:; rm -rf build build-debug
