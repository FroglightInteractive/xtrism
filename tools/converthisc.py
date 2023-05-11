#!/usr/bin/python3

#ifn = "/home/wagenaar/progs/xtrism/xtrism/data/hisc"
ifn = "/home/wagenaar/.local/share/trism/scores"

import re

months = { "Jan": 1,
           "Feb": 2,
           "Mar": 3,
           "Apr": 4,
           "May": 5,
           "Jun": 6,
           "Jul": 7,
           "Aug": 8,
           "Sep": 9,
           "Oct": 10,
           "Nov": 11,
           "Dec": 12,
           }

def getdate(date):
    m = re.match("([A-Za-z]+) +(\d+) +(\d+) +(\d+):(\d+)", date)
    if m:
        mon = months[m.group(1)]
        day = int(m.group(2))
        year = int(m.group(3))
        hr = int(m.group(4))
        mn = int(m.group(5))
        return mon, day, year, hr, mn
    m = re.match("(\d+) +([A-Za-z]+) +(\d+) +(\d+):(\d+)", date)
    if m:
        day = int(m.group(1))
        mon = months[m.group(2)]
        year = int(m.group(3))
        hr = int(m.group(4))
        mn = int(m.group(5))
        return mon, day, year, hr, mn
    return None

def nicedate(year, mon, day, hr, mn):
    if year<100:
        year += 2000
    return f"{year:04}{mon:02}{day:02}.{hr:02}{mn:02}00"

def convert(line):
    m = re.match("(.*): (\d+) in (\d+) lines \(([0-9.]+) ppb\); bset (\d) -? ?(.*)", line.strip())
    if m:
        name = m.group(1)
        score = int(m.group(2))
        lines = int(m.group(3))
        ppb = float(m.group(4))
        bs = int(m.group(5))
        date = getdate(m.group(6))
        if date is not None:
            mon, day, year, hr, mn = date
            bits = [ nicedate(year,mon,day,hr,mn),
                     bs,
                     score,
                     lines,
                     int(score/ppb + .5),
                     name
                     ]
            #print(line, date, mon, day, year, hr, mn, score, lines, ppb)
            print("\t".join(f"{bit}" for bit in bits))
            #print()
    

with open(ifn) as fd:
    for l in fd.readlines():
        convert(l)
        
