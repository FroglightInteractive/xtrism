ALL: PREP
	+make -C build

PREP:
	mkdir -p build
	tools/updatesources.sh
	( cd build; qmake ../src/xtrism.pro )

clean:; rm -rf build
