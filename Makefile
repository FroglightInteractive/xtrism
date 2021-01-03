ALL: PREP
	+make -C build

PREP:
	mkdir -p build
	( cd build; qmake ../src/xtrism.pro )

clean:; rm -rf build
