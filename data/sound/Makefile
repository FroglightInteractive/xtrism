SOUNDS=applause.raw drop.raw explode.raw shoosh.raw speedup.raw turn.raw warning.raw

ALL: $(SOUNDS)

%.raw: %.wav
	sox $< -r44100 -L -c1 -b16 -esigned-integer -traw $@

.phony: ALL
