# Countdown conundrum

This program is meant for solving puzzles from Countdown game, Channel 4 (8 out of 10 cats version).

This solves both letters and numbers puzzles, however there are some parts that can be improved.

Letters puzzle solver relies on a dictionary from [github.com/dwyl/english-words](https://github.com/dwyl/english-words), which is not the same as the official dictionary used on the programme. Proper solution to this discrepancy is to use list of words from  Oxford dictionary, but as far as I'm aware such a list is not available for download (it of course also needs to be licensed). When in doubt I would suggest consulting OED's official site.

## Usage

To solve letter puzzle just type the letters in lower case with no spaces and hit return. It will return all words that it can find, top 4 letter lengths -- usually there are too many solutions of lengths 4, 3, and they are not that interesting anyway.

Example:
```
Enter puzzle: ewabejsru
Found 223 solutions


7
abjures bewares bajrees

6
abjure jabers urease busera bursae beware beares reseau baseej bajree webers abuser

5
braws sawer bears urase ursae sujee beers rebus beres wares besaw jeers sweer easer braes beaus brees rajes brews warbs sware swear jubas buras bursa burse jubes abuse resaw saber weber sabre aures wears ewers ureas waurs sewer sewar seare bajus rubes bares beare erase bawrs resew suber saree baser baurs reuse

4
jeer seer jube jabs jars jaws juba urbs jews swee jees rees sabe ruse rusa rues suba rubs rube rews sere suer jura rebs sera sear sura raws sure rase swab jure wase brae ware bere bees beer wars beau bear bawr baur arbs base bras bars bare waur baju awes awee wear webs ares arse arew wabs urea ewes ewer ures eres ursa eras were user eaus wees ease braw ears ajee waes weer burs bura warb brus brew bree
Enter puzzle:
```

To solve numbers puzzle type all numbers with spaces in between, with the goal as the last number. Solver will print first 10 solutions, if found.
```
Enter puzzle: 25 100 1 7 9 5 729
Found at least 10 solutions
 1) 729 = 5+7*100-1+25
 2) 729 = 5+100*7-1+25
 3) 729 = 25+7*100-1+5
 4) 729 = 25+100*7-1+5
 5) 729 = 9+7*100-5+25
 6) 729 = 9+100*7-5+25
 7) 729 = 25+7*100-5+9
 8) 729 = 25+100*7-5+9
 9) 729 = 7*(9-5+100)+1
10) 729 = 7*(100-5+9)+1
Enter puzzle:
```

This solver needs some work -- solutions are often repeated (well, they are not identical from computers stand point, but they are same for a human), and the whole thing might be faster.

## Compilation

Requires [{fmt}](https://github.com/fmtlib/fmt) for compilation. [vcpkg](https://github.com/Microsoft/vcpkg) is good for installing libraries.

## Acknowledgements
* [{fmt}](https://github.com/fmtlib/fmt) is great
* [combinations library](https://github.com/HowardHinnant/combinations) from @HowardHinnant is amazing
* [word list](https://github.com/atebits/Words) from @atebits is indispensable
