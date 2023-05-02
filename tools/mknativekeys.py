#!/usr/bin/python3

from PyQt5.QtWidgets import QLabel, QApplication
from PyQt5.QtGui import QKeySequence
import sys

def ascify(s):
    ss = []
    for c in s:
        if ord(c)>=32 and ord(c)<127:
            ss.append(c)
        else:
            ss.append('?')
    return ''.join(ss)

keys = {}

class Label(QLabel):
    def __init__(self):
        super().__init__("text")
    def keyPressEvent(self, evt):
        print("text", evt.text())
        print("key", evt.key())
        print("seq", ascify(QKeySequence(evt.key()).toString()))
        print("native scan", evt.nativeScanCode())
        print("native virt", evt.nativeVirtualKey())
        keys[evt.nativeScanCode()] = (evt.key(), ascify(QKeySequence(evt.key()).toString()))
        print()

app = QApplication(sys.argv);
lbl = Label();
lbl.resize(500, 300)
lbl.show();
app.exec();
START = "{";
END = "}";
with open("../src/options/nativekeys.h", "w") as fd:
    fd.write('''// nativekeys.h - created by mknativekeys

#ifndef _nativekeys_h
#define _nativekeys_h

QMap<int, QString> scan2name{
''')
    for sc, (key, name) in keys.items():
        fd.write(f'  {START}{sc}, "{name}"{END},\n')
    fd.write('''};

#endif
''')
